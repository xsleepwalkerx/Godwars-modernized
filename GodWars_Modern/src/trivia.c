/***************************************************************************
 * trivia.c - Automated Trivia System for GodWars Modernized               *
 *                                                                         *
 * Ported/inspired from UMGW (Ultimate God Wars).                          *
 * Questions are loaded from ../area/trivia.dat at boot.                   *
 * Trivia sessions auto-start at a configurable interval.                  *
 * First player to type the correct answer via "answer <text>" wins QPs.   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#if defined(_WIN32)
#define strncasecmp _strnicmp
#endif

/*--------------------------------------------------------------------------
 * Constants
 *--------------------------------------------------------------------------*/
#define TRIVIA_FILE         "../area/trivia.dat"
#define TRIVIA_MAX_Q        512         /* max questions in database        */
#define TRIVIA_QUESTION_LEN 256         /* max question string length       */
#define TRIVIA_ANSWER_LEN   128         /* max answer string length         */

/* How many PULSE_TICK ticks between auto-trivia sessions (1 tick = 60 s). */
#define TRIVIA_AUTO_INTERVAL    30      /* 30 minutes between sessions      */

/* How many ticks before an unanswered question expires.                    */
#define TRIVIA_EXPIRE_TICKS     3       /* 3 minutes to answer              */

/* QP reward for a correct answer.                                          */
#define TRIVIA_QP_REWARD        10

/*--------------------------------------------------------------------------
 * State machine
 *--------------------------------------------------------------------------*/
typedef enum
{
    TRIVIA_IDLE = 0,    /* waiting for next session                        */
    TRIVIA_ACTIVE       /* question is live, waiting for answer            */
} TRIVIA_STATE;

/*--------------------------------------------------------------------------
 * Data structures
 *--------------------------------------------------------------------------*/
typedef struct trivia_question
{
    char question[TRIVIA_QUESTION_LEN];
    char answer  [TRIVIA_ANSWER_LEN];
} TRIVIA_QUESTION;

/* Module-level globals */
static TRIVIA_QUESTION  trivia_db[TRIVIA_MAX_Q];
static int              trivia_count        = 0;
static int              trivia_current      = -1;  /* index of live question */
static TRIVIA_STATE     trivia_state        = TRIVIA_IDLE;
static int              trivia_idle_ticks   = 0;   /* ticks since last session */
static int              trivia_active_ticks = 0;   /* ticks since question asked */

/*--------------------------------------------------------------------------
 * trivia_load
 * Read questions from TRIVIA_FILE.
 * File format (each pair must be contiguous):
 *   Question: <text>
 *   Answer: <text>
 *--------------------------------------------------------------------------*/
void trivia_load(void)
{
    FILE *fp;
    char  line[512];
    char  q[TRIVIA_QUESTION_LEN] = { '\0' };
    bool  have_q = FALSE;

    trivia_count = 0;

    fp = fopen(TRIVIA_FILE, "r");
    if (!fp)
    {
        log_string("trivia_load: could not open " TRIVIA_FILE);
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        /* Strip trailing newline */
        size_t len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (len > 0 && (line[len-1] == '\r'))
            line[--len] = '\0';

        /* Skip blank lines and comments */
        if (line[0] == '\0' || line[0] == '#') continue;

        if (!strncasecmp(line, "Question:", 9))
        {
            /* Trim leading space after colon */
            const char *val = line + 9;
            while (*val == ' ') val++;
            strncpy(q, val, TRIVIA_QUESTION_LEN - 1);
            have_q = TRUE;
        }
        else if (!strncasecmp(line, "Answer:", 7) && have_q)
        {
            const char *val = line + 7;
            while (*val == ' ') val++;

            if (trivia_count < TRIVIA_MAX_Q)
            {
                strncpy(trivia_db[trivia_count].question, q,   TRIVIA_QUESTION_LEN - 1);
                strncpy(trivia_db[trivia_count].answer,   val, TRIVIA_ANSWER_LEN   - 1);
                trivia_count++;
            }
            have_q = FALSE;
            q[0]   = '\0';
        }
    }

    fclose(fp);
    mud_logf("trivia_load: loaded %d question(s) from %s.", trivia_count, TRIVIA_FILE);
}

/*--------------------------------------------------------------------------
 * trivia_broadcast
 * Send a formatted trivia message to every connected player.
 *--------------------------------------------------------------------------*/
static void trivia_broadcast(const char *msg)
{
    DESCRIPTOR_DATA *d;
    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING && d->character)
            send_to_char(msg, d->character);
    }
}

/*--------------------------------------------------------------------------
 * trivia_ask
 * Select a random question and broadcast it.
 *--------------------------------------------------------------------------*/
static void trivia_ask(void)
{
    char buf[MAX_STRING_LENGTH];

    if (trivia_count <= 0)
    {
        log_string("trivia_ask: no questions loaded.");
        return;
    }

    trivia_current      = number_range(0, trivia_count - 1);
    trivia_state        = TRIVIA_ACTIVE;
    trivia_active_ticks = 0;

    sprintf(buf,
        "\n\r#Y[TRIVIA] #7Question: #W%s\n\r"
        "#Y[TRIVIA] #7Type '#Wanswer <your answer>#7' to win %d quest points!\n\r",
        trivia_db[trivia_current].question, TRIVIA_QP_REWARD);
    trivia_broadcast(buf);
}

/*--------------------------------------------------------------------------
 * trivia_expire
 * Called when nobody answered in time.
 *--------------------------------------------------------------------------*/
static void trivia_expire(void)
{
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,
        "\n\r#Y[TRIVIA] #7Time's up! The answer was: #W%s#n\n\r",
        trivia_db[trivia_current].answer);
    trivia_broadcast(buf);

    trivia_state        = TRIVIA_IDLE;
    trivia_current      = -1;
    trivia_idle_ticks   = 0;
}

/*--------------------------------------------------------------------------
 * trivia_update
 * Called once per PULSE_TICK from update_handler().
 * Manages the idle countdown and question expiry.
 *--------------------------------------------------------------------------*/
void trivia_update(void)
{
    if (trivia_count <= 0) return;

    switch (trivia_state)
    {
        case TRIVIA_IDLE:
            trivia_idle_ticks++;
            if (trivia_idle_ticks >= TRIVIA_AUTO_INTERVAL)
            {
                trivia_idle_ticks = 0;
                trivia_ask();
            }
            break;

        case TRIVIA_ACTIVE:
            trivia_active_ticks++;
            if (trivia_active_ticks == (TRIVIA_EXPIRE_TICKS - 1))
            {
                char buf[MAX_STRING_LENGTH];
                sprintf(buf, "\n\r#Y[TRIVIA] #7Hint: The answer starts with '#W%c#7'!\n\r",
                        trivia_db[trivia_current].answer[0]);
                trivia_broadcast(buf);
            }
            if (trivia_active_ticks >= TRIVIA_EXPIRE_TICKS)
                trivia_expire();
            break;
    }
}

/*--------------------------------------------------------------------------
 * do_answer
 * Player command: answer <text>
 *--------------------------------------------------------------------------*/
bool trivia_is_active( void )
{
    return trivia_state == TRIVIA_ACTIVE && trivia_current >= 0;
}

void do_trivia_answer(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;

    if (trivia_state != TRIVIA_ACTIVE || trivia_current < 0)
    {
        send_to_char("#Y[TRIVIA] #7No trivia question is active right now.\n\r", ch);
        return;
    }

    if (argument[0] == '\0')
    {
        send_to_char("#Y[TRIVIA] #7Answer what? Type 'answer <text>'.\n\r", ch);
        return;
    }

    /* Case-insensitive comparison */
    if (!str_cmp(argument, trivia_db[trivia_current].answer))
    {
        /* Correct! */
        ch->pcdata->quest += TRIVIA_QP_REWARD;

        sprintf(buf,
            "\n\r#Y[TRIVIA] #W%s #7got it right! The answer was '#W%s#7'.\n\r"
            "#Y[TRIVIA] #W%s #7wins #R%d #7quest points!\n\r",
            ch->name,
            trivia_db[trivia_current].answer,
            ch->name,
            TRIVIA_QP_REWARD);
        trivia_broadcast(buf);

        save_char_obj(ch);

        trivia_state        = TRIVIA_IDLE;
        trivia_current      = -1;
        trivia_idle_ticks   = 0;
    }
    else
    {
        sprintf(buf, "#Y[TRIVIA] #7Sorry, '#W%s#7' is not correct. Keep trying!\n\r", argument);
        send_to_char(buf, ch);
    }
}

/*--------------------------------------------------------------------------
 * do_trivia
 * Immortal command to manage the trivia system.
 * Syntax: trivia start | stop | reload | status | ask
 *--------------------------------------------------------------------------*/
void do_trivia(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg);

    if (!IS_IMMORTAL(ch))
    {
        /* Mortals can see status only */
        if (trivia_state == TRIVIA_ACTIVE && trivia_current >= 0)
        {
            sprintf(buf,
                "#Y[TRIVIA] #7Current question: #W%s\n\r",
                trivia_db[trivia_current].question);
            send_to_char(buf, ch);
        }
        else
        {
            send_to_char("#Y[TRIVIA] #7No trivia session is currently active.\n\r", ch);
        }
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char(
            "Syntax: trivia <start|stop|reload|status|ask>\n\r"
            "  start  - Start the auto-trivia timer.\n\r"
            "  stop   - Stop any active session and reset timer.\n\r"
            "  reload - Reload questions from disk.\n\r"
            "  status - Show trivia system status.\n\r"
            "  ask    - Ask a random question immediately.\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "start"))
    {
        if (trivia_state == TRIVIA_ACTIVE)
        {
            send_to_char("A trivia question is already active!\n\r", ch);
            return;
        }
        trivia_state      = TRIVIA_IDLE;
        trivia_idle_ticks = 0;
        send_to_char("Trivia auto-timer started.\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "stop"))
    {
        if (trivia_state == TRIVIA_ACTIVE)
        {
            sprintf(buf,
                "\n\r#Y[TRIVIA] #7Session ended by %s. The answer was '#W%s#7'.\n\r",
                ch->name, trivia_db[trivia_current].answer);
            trivia_broadcast(buf);
        }
        trivia_state        = TRIVIA_IDLE;
        trivia_current      = -1;
        trivia_idle_ticks   = 0;
        trivia_active_ticks = 0;
        send_to_char("Trivia stopped and timer reset.\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "reload"))
    {
        if (trivia_state == TRIVIA_ACTIVE)
        {
            send_to_char("Cannot reload while a question is active. Stop first.\n\r", ch);
            return;
        }
        trivia_load();
        sprintf(buf, "Trivia database reloaded: %d question(s).\n\r", trivia_count);
        send_to_char(buf, ch);
        return;
    }

    if (!str_cmp(arg, "status"))
    {
        sprintf(buf,
            "#Y[TRIVIA STATUS]\n\r"
            "  Questions loaded   : %d\n\r"
            "  State              : %s\n\r"
            "  Idle ticks         : %d / %d\n\r"
            "  Active ticks       : %d / %d\n\r"
            "  Current question   : %s\n\r",
            trivia_count,
            trivia_state == TRIVIA_ACTIVE ? "ACTIVE" : "IDLE",
            trivia_idle_ticks,   TRIVIA_AUTO_INTERVAL,
            trivia_active_ticks, TRIVIA_EXPIRE_TICKS,
            (trivia_current >= 0) ? trivia_db[trivia_current].question : "(none)");
        send_to_char(buf, ch);
        return;
    }

    if (!str_cmp(arg, "ask"))
    {
        if (trivia_state == TRIVIA_ACTIVE)
        {
            send_to_char("A question is already active!\n\r", ch);
            return;
        }
        trivia_ask();
        send_to_char("Trivia question asked.\n\r", ch);
        return;
    }

    send_to_char("Unknown trivia option. Type 'trivia' for usage.\n\r", ch);
}
