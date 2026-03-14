#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(_WIN32)
#include <unistd.h> /* unlink() */
#else
#include <io.h>
#define unlink _unlink
#endif
#include "merc.h"

extern int port, control;

/*
 * Local Defines
 */
#define CH(descriptor)  ((descriptor)->original ? \
(descriptor)->original : (descriptor)->character)
#define COPYOVER_FILE "copyover.data"
#define EXE_FILE          "../src/merc"
int cptimer = 0;
bool    write_to_descriptor     args( ( int desc, char *txt, int length ) );
void call_all( CHAR_DATA *ch );


/*
 * cptimer_check - called every pulse from update_handler.
 * Counts up to MAXCPTIMER seconds, broadcasting warnings at
 * 60/30/10 seconds remaining, then triggers auto_copyover.
 */
void cptimer_check(void)
{
    int timeleft;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    timeleft = (MAXCPTIMER - cptimer);

    if (cptimer <= MAXCPTIMER)
    {
        cptimer++;

        if (   timeleft == 3600   /* 1 hour   */
            || timeleft == 1800   /* 30 min   */
            || timeleft == 600    /* 10 min   */
            || timeleft == 300    /* 5 min    */
            || timeleft == 60     /* 1 min    */
            || timeleft == 30
            || timeleft == 10)
        {
            /* Format time-remaining label */
            char timestr[32];
            if      (timeleft >= 3600) sprintf(timestr, "%d hour(s)",  timeleft / 3600);
            else if (timeleft >= 60)   sprintf(timestr, "%d minute(s)", timeleft / 60);
            else                       sprintf(timestr, "%d second(s)", timeleft);

            sprintf(buf, "\n\r#1*** #7Automatic copyover in %s #1***#n\n\r", timestr);

            for (d = descriptor_list; d != NULL; d = d->next)
            {
                if (d->connected == CON_PLAYING && d->character)
                    send_to_char(buf, d->character);
            }
        }
        return;
    }

    if (cptimer >= MAXCPTIMER)
    {
        auto_copyover();
        return;
    }
}

/*
 * auto_copyover - timed automatic copyover.
 * Saves all PCs, drops logging-on connections, writes the copyover file,
 * and exec()s the new binary with file descriptors inherited.
 */
void auto_copyover(void)
{
    FILE *fp;
    DESCRIPTOR_DATA *d, *d_next;
    CHAR_DATA *ch;
    char buf [100], buf2[100];

    cptimer = 0;
    fp = fopen (COPYOVER_FILE, "w");

    if (!fp)
    {
        /* ch is not set here — log to file only */
        mud_logf("auto_copyover: could not open %s for writing.", COPYOVER_FILE);
        perror ("auto_copyover:fopen");
        return;
    }

    /* Save all playing characters and call them to the void safely */
    for (ch = char_list; ch != NULL; ch = ch->next)
    {
        if (IS_NPC(ch)) continue;
        do_peace(ch, "");
        call_all(ch);
        do_save(ch, "");
    }

    sprintf(buf, "\n\r *** TIMED SYSTEM COPYOVER *** - please remain seated!\n\r");

    /* Write descriptor table and close any half-connected sockets */
    for (d = descriptor_list; d ; d = d_next)
    {
        CHAR_DATA *och = CH(d);
        d_next = d->next;

        if (!d->character || d->connected > CON_PLAYING)
        {
            write_to_descriptor(d->descriptor,
                "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0);
            close_socket(d);
        }
        else
        {
            fprintf(fp, "%d %s %s\n", d->descriptor, och->name, d->host);
            save_char_obj(och);
            write_to_descriptor(d->descriptor, buf, 0);
        }
    }

    fprintf(fp, "-1\n");
    fclose(fp);

    fclose(fpReserve);

    sprintf(buf,  "%d", port);
    sprintf(buf2, "%d", control);
    execl(EXE_FILE, "merc", buf, "copyover", buf2, (char *) NULL);

    /* If we reach here execl() failed */
    perror("auto_copyover: execl");
    mud_logf("auto_copyover: execl failed!");
    /* fpReserve is closed — nothing safe to send_to_char */
}

/*
 * do_timer_check - immortal command to inspect/set internal timers.
 * Syntax: timercheck status copyover
 *         timercheck set copyover <seconds>
 */
void do_timer_check(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int timeleft;
    int newtime;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    argument = one_argument(argument, arg3);

    if (!IS_IMMORTAL(ch))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if (arg1[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char(
            "\n\rSyntax: timercheck <status|set> <timer> [time]\n\r"
            "  Available timers: copyover\n\r", ch);
        return;
    }

    if (!str_cmp(arg1, "status"))
    {
        if (!str_cmp(arg2, "copyover"))
        {
            timeleft = MAXCPTIMER - cptimer;
            sprintf(buf, "Time remaining until next automatic copyover: %d seconds.\n\r", timeleft);
            send_to_char(buf, ch);
            return;
        }
    }

    if (!str_cmp(arg1, "set"))
    {
        if (arg3[0] == '\0')
        {
            send_to_char("You must specify a value to set the timer.\n\r", ch);
            return;
        }
        if (!str_cmp(arg2, "copyover"))
        {
            cptimer  = (MAXCPTIMER - atoi(arg3));
            newtime  = (MAXCPTIMER - cptimer);
            sprintf(buf, "Copyover timer set. Time remaining: %d seconds.\n\r", newtime);
            send_to_char(buf, ch);
            return;
        }
    }
}
