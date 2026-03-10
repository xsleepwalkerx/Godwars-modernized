#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> /* unlink() */
#include "merc.h"


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


void cptimer_check(void)
{
    int timeleft;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    
    timeleft = (MAXCPTIMER - cptimer);

	if (cptimer <= MAXCPTIMER)
         {
           cptimer++;

	   if (timeleft == 60
           ||  timeleft == 30
           ||  timeleft == 10)
           {
           for (d = descriptor_list; d != NULL; d = d->next)
            {
            sprintf(buf,"\n\r
#1****
****#7 Automatic copyover in %d seconds
#1****#n\n\r",timeleft);
            send_to_char(buf,d->character);
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
void auto_copyover(void)
{
        FILE *fp;
        DESCRIPTOR_DATA *d, *d_next;
        CHAR_DATA *ch;
        char buf [100], buf2[100];
        extern int port,control; /* db.c */

        cptimer = 0;
        fp = fopen (COPYOVER_FILE, "w");

        if (!fp)
        {
                send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
                logf ("Could not write to copyover file: %s", COPYOVER_FILE);
                perror ("do_copyover:fopen");
                return;
        }

        /* Consider changing all saved areas here, if you use OLC */

        /* do_asave (NULL, ""); - autosave changed areas */


        sprintf (buf, "\n\r *** TIMED SYSTEM COPYOVER *** - please remain seated!\n\r");


	for ( ch = char_list; ch != NULL;ch = ch->next )
	{
		do_peace(ch, "");
                do_restore(ch, "all");
		call_all(ch);
		do_save(ch, "");
	}		

        /* For each playing descriptor, save its state */
        for (d = descriptor_list; d ; d = d_next)
        {
                CHAR_DATA * och = CH (d);
                d_next = d->next; /* We delete from the list , so need to save this */

                if (!d->character || d->connected > CON_PLAYING) /* drop those logging on */
                {
                        write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0);
                        close_socket (d); /* throw'em out */
                }
                else
                {
                        fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
                        save_char_obj (och);
                        write_to_descriptor (d->descriptor, buf, 0);
                }
        }

        fprintf (fp, "-1\n");
        fclose (fp);

        /* Close reserve and other always-open files and release other resources */

        fclose (fpReserve);

        /* exec - descriptors are inherited */

        sprintf (buf, "%d", port);
        sprintf (buf2, "%d", control);
        execl (EXE_FILE, "merc", buf, "copyover", buf2, (char *) NULL);

        /* Failed - sucessful exec will not return */

        perror ("do_copyover: execl");
        send_to_char ("Copyover FAILED!\n\r",ch);

        /* Here you might want to reopen fpReserve */
}

void do_timer_check(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int timeleft;
    int newtime;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if (!IS_IMMORTAL(ch))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }

    if (arg1[0] == '\0'
    ||  arg2[0] == '\0')
    {
        sprintf(buf,"\n\rSyntax: timercheck <status/set> <timer> <time remaining>.\n\r
	****Available Timers****
	------------------------
	copyover\n\r");
	send_to_char(buf,ch);
        return;
    }
	
    if (!str_cmp(arg1,"status"))
    {
	if (!str_cmp(arg2,"copyover"))
        {
	 timeleft = MAXCPTIMER - cptimer;
	 sprintf(buf, "Time remaining untill next automatic copyover : %d seconds\n\r",timeleft);
	 send_to_char(buf,ch);
	 return;
        }
    }

    if (!str_cmp(arg1,"set"))
    {
	if (arg3[0] == '\0')
	{
	 sprintf(buf, "You must specify a value to set the timer manually.\n\r");
	 send_to_char(buf,ch);
	 return;
	}
	if (!str_cmp(arg2,"copyover"))
	{
	 cptimer = (MAXCPTIMER - atoi(arg3)); 
	 newtime = (MAXCPTIMER - cptimer);
	 sprintf(buf,"Timer set.  Remaining time for %s timer is %d.\n\r",arg2,newtime);
	 send_to_char(buf,ch);
	 return;
	}
    }
}
