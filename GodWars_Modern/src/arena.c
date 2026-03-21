/***************************************************************************
 * arena.c — Arena Combat System for GodWars Modernized                    *
 *                                                                         *
 * Provides safe PvP practice, ranked duels, and a spectator system.      *
 * Deaths in the arena do not count; losers are restored to 1 hp and      *
 * returned to their recall room.  Winners gain arena points.             *
 *                                                                         *
 * Commands:                                                               *
 *   arena                  — show arena status / help                    *
 *   arena join             — enter the arena queue                       *
 *   arena leave            — leave the queue / forfeit an ongoing match  *
 *   arena challenge <name> — issue a direct challenge to another player  *
 *   arena accept           — accept a pending challenge                  *
 *   arena decline          — decline a pending challenge                 *
 *   arena list             — list all queued / fighting players          *
 *   arena score            — show your arena wins/losses/rank            *
 *   arena watch            — teleport to the arena as a spectator        *
 *   arena top              — top-10 arena rankings                       *
 *                                                                         *
 * arena_check_death() is called from fight.c when a player hits 0 hp.   *
 * arena_update()     is called every PULSE_TICK from update.c.           *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

/* =========================================================================
 * Constants
 * ========================================================================= */

#define ARENA_VNUM_FIGHT    30050   /* Room where arena fights take place  */
#define ARENA_VNUM_LOBBY    30051   /* Lobby / waiting room                */
#define ARENA_VNUM_SPEC     30052   /* Spectator balcony                   */

#define ARENA_WIN_PTS       5       /* Points awarded to winner            */
#define ARENA_LOSS_PTS      1       /* Points deducted from loser          */
#define ARENA_CHALLENGE_TO  10      /* Ticks before challenge expires      */
#define ARENA_MAX_QUEUE     20      /* Maximum players in the queue        */

/* States for a player's arena_state field */
#define ARENA_NONE          0       /* Not involved                        */
#define ARENA_QUEUED        1       /* Waiting for a match                 */
#define ARENA_FIGHTING      2       /* Currently in a match                */
#define ARENA_SPECTATING    3       /* Watching as spectator               */

/* =========================================================================
 * Module-level state
 * ========================================================================= */

/* Queue of players waiting for a match (names only; we look up live). */
static char arena_queue[ARENA_MAX_QUEUE][MAX_INPUT_LENGTH];
static int  arena_queue_count = 0;

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

/* Return the arena fight room (may be NULL if not loaded). */
static ROOM_INDEX_DATA *arena_fight_room( void )
{
    ROOM_INDEX_DATA *r = get_room_index( ARENA_VNUM_FIGHT );
    if ( r == NULL )
        r = get_room_index( ROOM_VNUM_CHAT );  /* graceful fallback */
    return r;
}

static ROOM_INDEX_DATA *arena_lobby_room( void )
{
    ROOM_INDEX_DATA *r = get_room_index( ARENA_VNUM_LOBBY );
    if ( r == NULL )
        r = get_room_index( ROOM_VNUM_CHAT );
    return r;
}

static ROOM_INDEX_DATA *arena_spec_room( void )
{
    ROOM_INDEX_DATA *r = get_room_index( ARENA_VNUM_SPEC );
    if ( r == NULL )
        r = get_room_index( ROOM_VNUM_CHAT );
    return r;
}

/* Remove a name from the queue (case-insensitive). */
static void queue_remove( const char *name )
{
    int i, j;
    for ( i = 0; i < arena_queue_count; i++ )
    {
        if ( !str_cmp( arena_queue[i], name ) )
        {
            for ( j = i; j < arena_queue_count - 1; j++ )
                strcpy( arena_queue[j], arena_queue[j+1] );
            arena_queue_count--;
            return;
        }
    }
}

/* Restore a character to fighting trim (no death, just back to 1 hp if needed). */
static void arena_restore( CHAR_DATA *ch )
{
    if ( ch->hit < 1 )         ch->hit   = 1;
    if ( ch->mana < 0 )        ch->mana  = 0;
    if ( ch->move < 0 )        ch->move  = 0;
    update_pos( ch );
}

/* Recall ch to their home room (or temple). */
static void arena_recall( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *recall;

    if ( ch->in_room != NULL && IS_SET( ch->in_room->room_flags, ROOM_ARENA ) )
    {
        recall = get_room_index( ROOM_VNUM_TEMPLE );
        if ( recall == NULL )
            recall = get_room_index( ROOM_VNUM_LIMBO );
        char_from_room( ch );
        char_to_room( ch, recall );
        act( "$n is sent back to the arena lobby.", ch, NULL, NULL, TO_ROOM );
    }
}

/* Begin a match between two queued players. */
static void arena_start_match( CHAR_DATA *ch1, CHAR_DATA *ch2 )
{
    ROOM_INDEX_DATA *fight_room = arena_fight_room();
    char buf[MAX_STRING_LENGTH];

    if ( fight_room == NULL )
    {
        send_to_char( "Arena fight room not found — match cancelled.\n\r", ch1 );
        send_to_char( "Arena fight room not found — match cancelled.\n\r", ch2 );
        return;
    }

    /* Move both fighters in. */
    char_from_room( ch1 );
    char_to_room( ch1, fight_room );
    char_from_room( ch2 );
    char_to_room( ch2, fight_room );

    /* Restore to full for the fight. */
    ch1->hit  = ch1->max_hit;
    ch1->mana = ch1->max_mana;
    ch1->move = ch1->max_move;
    update_pos( ch1 );

    ch2->hit  = ch2->max_hit;
    ch2->mana = ch2->max_mana;
    ch2->move = ch2->max_move;
    update_pos( ch2 );

    /* Set arena state. */
    ch1->pcdata->arena_state   = ARENA_FIGHTING;
    ch1->pcdata->arena_opponent[0] = '\0';
    strncat( ch1->pcdata->arena_opponent, ch2->name, sizeof(ch1->pcdata->arena_opponent)-1 );

    ch2->pcdata->arena_state   = ARENA_FIGHTING;
    ch2->pcdata->arena_opponent[0] = '\0';
    strncat( ch2->pcdata->arena_opponent, ch1->name, sizeof(ch2->pcdata->arena_opponent)-1 );

    /* Remove from queue. */
    queue_remove( ch1->name );
    queue_remove( ch2->name );

    /* Announce. */
    snprintf( buf, sizeof(buf),
        "#Y[ARENA]#n %s and %s have entered the arena for a fight!\n\r",
        ch1->name, ch2->name );
    send_to_all_char( buf );

    act( "You face $N in the arena!", ch1, NULL, ch2, TO_CHAR );
    act( "You face $N in the arena!", ch2, NULL, ch1, TO_CHAR );
}

/* =========================================================================
 * Public API
 * ========================================================================= */

/*
 * arena_check_death() — called from fight.c when a player's hp <= 0 in an
 * arena room.  Returns TRUE if the death was arena-handled (suppress normal
 * death processing).
 */
bool arena_check_death( CHAR_DATA *victim, CHAR_DATA *killer )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *opponent;

    if ( IS_NPC(victim) )                               return FALSE;
    if ( victim->pcdata->arena_state != ARENA_FIGHTING ) return FALSE;
    if ( !IS_SET( victim->in_room->room_flags, ROOM_ARENA ) ) return FALSE;

    /* Find opponent in world. */
    opponent = NULL;
    if ( killer != NULL && !IS_NPC(killer) &&
         !str_cmp( killer->name, victim->pcdata->arena_opponent ) )
        opponent = killer;

    /* Announce winner/loser. */
    if ( opponent != NULL )
    {
        /* Award points. */
        opponent->pcdata->arena_wins++;
        opponent->pcdata->arena_pts += ARENA_WIN_PTS;
        victim->pcdata->arena_losses++;
        if ( victim->pcdata->arena_pts >= ARENA_LOSS_PTS )
            victim->pcdata->arena_pts -= ARENA_LOSS_PTS;

        snprintf( buf, sizeof(buf),
            "#Y[ARENA]#n %s has defeated %s! (%+d pts)\n\r",
            opponent->name, victim->name, ARENA_WIN_PTS );
        send_to_all_char( buf );

        /* Reward winner. */
        send_to_char( "#YYou win the arena match!#n\n\r", opponent );
        snprintf( buf, sizeof(buf), "You gain #Y%d#n arena points.\n\r", ARENA_WIN_PTS );
        send_to_char( buf, opponent );

        /* Clear winner state. */
        opponent->pcdata->arena_state = ARENA_NONE;
        opponent->pcdata->arena_opponent[0] = '\0';
        arena_recall( opponent );
    }
    else
    {
        snprintf( buf, sizeof(buf),
            "#Y[ARENA]#n %s has been defeated in the arena!\n\r", victim->name );
        send_to_all_char( buf );
    }

    /* Restore and recall loser. */
    send_to_char( "#RYou have been defeated!#n  You are returned to safety.\n\r", victim );
    arena_restore( victim );
    victim->pcdata->arena_state = ARENA_NONE;
    victim->pcdata->arena_opponent[0] = '\0';

    /* Clear any fight pointers. */
    stop_fighting( victim, TRUE );
    arena_recall( victim );

    return TRUE;    /* suppress normal death */
}

/*
 * arena_update() — called each PULSE_TICK.
 * Matches up pairs of queued players.
 */
void arena_update( void )
{
    CHAR_DATA  *ch1, *ch2;
    int i;

    if ( arena_queue_count < 2 )
        return;

    /* Pair the first two in queue who are still online and queued. */
    ch1 = ch2 = NULL;

    for ( i = 0; i < arena_queue_count && ch1 == NULL; i++ )
    {
        CHAR_DATA *scan;
        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( !str_cmp( scan->name, arena_queue[i] ) &&
                 scan->pcdata->arena_state == ARENA_QUEUED )
            {
                ch1 = scan;
                break;
            }
        }
        if ( ch1 == NULL )
            queue_remove( arena_queue[i] );   /* stale entry */
    }

    if ( ch1 == NULL ) return;

    for ( i = 0; i < arena_queue_count && ch2 == NULL; i++ )
    {
        CHAR_DATA *scan;
        if ( !str_cmp( arena_queue[i], ch1->name ) ) continue;
        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( !str_cmp( scan->name, arena_queue[i] ) &&
                 scan->pcdata->arena_state == ARENA_QUEUED )
            {
                ch2 = scan;
                break;
            }
        }
        if ( ch2 == NULL )
            queue_remove( arena_queue[i] );
    }

    if ( ch2 == NULL ) return;

    arena_start_match( ch1, ch2 );
}

/* =========================================================================
 * Command: do_arena
 * ========================================================================= */

void do_arena( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
        send_to_char( "NPCs cannot use the arena.\n\r", ch );
        return;
    }

    argument = one_argument( argument, arg );

    /* ------------------------------------------------------------------ */
    if ( arg[0] == '\0' || !str_cmp( arg, "help" ) )
    {
        send_to_char(
            "#Y========== Arena System ==========#n\n\r"
            "  arena join              - Enter the arena queue\n\r"
            "  arena leave             - Leave the queue / forfeit match\n\r"
            "  arena challenge <name>  - Challenge a specific player\n\r"
            "  arena accept            - Accept a challenge\n\r"
            "  arena decline           - Decline a challenge\n\r"
            "  arena list              - List queued / fighting players\n\r"
            "  arena score             - Show your arena record\n\r"
            "  arena watch             - Spectate from the balcony\n\r"
            "  arena top               - Top-10 arena rankings\n\r"
            "#Y===================================#n\n\r",
            ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "join" ) )
    {
        if ( ch->pcdata->arena_state == ARENA_QUEUED )
        {
            send_to_char( "You are already in the arena queue.\n\r", ch );
            return;
        }
        if ( ch->pcdata->arena_state == ARENA_FIGHTING )
        {
            send_to_char( "You are already in an arena match!\n\r", ch );
            return;
        }
        if ( arena_queue_count >= ARENA_MAX_QUEUE )
        {
            send_to_char( "The arena queue is full.  Try again later.\n\r", ch );
            return;
        }
        if ( ch->fighting != NULL )
        {
            send_to_char( "You cannot join the arena while in combat.\n\r", ch );
            return;
        }

        /* Move to lobby and queue. */
        ROOM_INDEX_DATA *lobby = arena_lobby_room();
        if ( lobby != NULL )
        {
            char_from_room( ch );
            char_to_room( ch, lobby );
        }

        ch->pcdata->arena_state = ARENA_QUEUED;
        strcpy( arena_queue[arena_queue_count++], ch->name );
        ch->pcdata->arena_opponent[0] = '\0';

        send_to_char( "You join the arena queue.  Waiting for an opponent...\n\r", ch );
        snprintf( buf, sizeof(buf),
            "#Y[ARENA]#n %s has entered the arena queue. (%d waiting)\n\r",
            ch->name, arena_queue_count );
        send_to_all_char( buf );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "leave" ) )
    {
        if ( ch->pcdata->arena_state == ARENA_NONE )
        {
            send_to_char( "You are not in the arena.\n\r", ch );
            return;
        }
        if ( ch->pcdata->arena_state == ARENA_FIGHTING )
        {
            /* Forfeit — opponent wins. */
            CHAR_DATA *opp = NULL;
            CHAR_DATA *scan;
            for ( scan = char_list; scan != NULL; scan = scan->next )
            {
                if ( !IS_NPC(scan) &&
                     !str_cmp( scan->name, ch->pcdata->arena_opponent ) )
                {
                    opp = scan;
                    break;
                }
            }
            if ( opp != NULL )
            {
                opp->pcdata->arena_wins++;
                opp->pcdata->arena_pts += ARENA_WIN_PTS;
                snprintf( buf, sizeof(buf),
                    "#Y[ARENA]#n %s has forfeited.  %s wins!\n\r",
                    ch->name, opp->name );
                send_to_all_char( buf );
                opp->pcdata->arena_state = ARENA_NONE;
                opp->pcdata->arena_opponent[0] = '\0';
                stop_fighting( opp, TRUE );
                arena_recall( opp );
            }
            ch->pcdata->arena_losses++;
        }

        queue_remove( ch->name );
        ch->pcdata->arena_state = ARENA_NONE;
        ch->pcdata->arena_opponent[0] = '\0';
        stop_fighting( ch, TRUE );
        arena_recall( ch );

        send_to_char( "You leave the arena.\n\r", ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "challenge" ) )
    {
        CHAR_DATA *victim;
        char target[MAX_INPUT_LENGTH];
        one_argument( argument, target );

        if ( target[0] == '\0' )
        {
            send_to_char( "Challenge whom?\n\r", ch );
            return;
        }
        if ( ch->pcdata->arena_state != ARENA_NONE )
        {
            send_to_char( "You are already in the arena or queue.\n\r", ch );
            return;
        }
        if ( ( victim = get_char_world( ch, target ) ) == NULL || IS_NPC(victim) )
        {
            send_to_char( "That player is not online.\n\r", ch );
            return;
        }
        if ( victim == ch )
        {
            send_to_char( "You cannot challenge yourself.\n\r", ch );
            return;
        }
        if ( victim->pcdata->arena_state != ARENA_NONE )
        {
            send_to_char( "They are already in the arena.\n\r", ch );
            return;
        }

        /* Record the challenge. */
        strncpy( ch->pcdata->arena_opponent, victim->name,
                 sizeof(ch->pcdata->arena_opponent) - 1 );
        ch->pcdata->arena_state = ARENA_QUEUED;
        ch->pcdata->arena_timer = ARENA_CHALLENGE_TO;

        snprintf( buf, sizeof(buf),
            "%s has challenged you to an arena duel! (arena accept / arena decline)\n\r",
            ch->name );
        send_to_char( buf, victim );

        snprintf( buf, sizeof(buf),
            "You challenge %s to an arena duel.\n\r", victim->name );
        send_to_char( buf, ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "accept" ) )
    {
        /* Find challenger. */
        CHAR_DATA *challenger = NULL;
        CHAR_DATA *scan;

        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( scan->pcdata->arena_state == ARENA_QUEUED &&
                 !str_cmp( scan->pcdata->arena_opponent, ch->name ) &&
                 scan->pcdata->arena_timer > 0 )
            {
                challenger = scan;
                break;
            }
        }

        if ( challenger == NULL )
        {
            send_to_char( "You have no pending arena challenge.\n\r", ch );
            return;
        }
        if ( ch->pcdata->arena_state != ARENA_NONE )
        {
            send_to_char( "You are already in the arena.\n\r", ch );
            return;
        }

        ch->pcdata->arena_state = ARENA_FIGHTING;
        arena_start_match( challenger, ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "decline" ) )
    {
        CHAR_DATA *challenger = NULL;
        CHAR_DATA *scan;

        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( scan->pcdata->arena_state == ARENA_QUEUED &&
                 !str_cmp( scan->pcdata->arena_opponent, ch->name ) )
            {
                challenger = scan;
                break;
            }
        }

        if ( challenger == NULL )
        {
            send_to_char( "You have no pending arena challenge.\n\r", ch );
            return;
        }

        challenger->pcdata->arena_state = ARENA_NONE;
        challenger->pcdata->arena_opponent[0] = '\0';
        queue_remove( challenger->name );

        snprintf( buf, sizeof(buf), "%s has declined your challenge.\n\r", ch->name );
        send_to_char( buf, challenger );
        send_to_char( "You decline the challenge.\n\r", ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "list" ) )
    {
        CHAR_DATA *scan;
        int n = 0;

        send_to_char(
            "#Y========== Arena Status ==========#n\n\r"
            "Name                     Status\n\r"
            "------------------------  --------\n\r",
            ch );

        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( scan->pcdata->arena_state == ARENA_NONE ) continue;

            const char *status_str = "Unknown";
            switch ( scan->pcdata->arena_state )
            {
            case ARENA_QUEUED:    status_str = "Queued";   break;
            case ARENA_FIGHTING:  status_str = "Fighting"; break;
            case ARENA_SPECTATING: status_str = "Watching"; break;
            }
            snprintf( buf, sizeof(buf), "%-25s %s\n\r", scan->name, status_str );
            send_to_char( buf, ch );
            n++;
        }

        if ( n == 0 )
            send_to_char( "  No players in the arena right now.\n\r", ch );

        send_to_char( "#Y===================================#n\n\r", ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "score" ) )
    {
        snprintf( buf, sizeof(buf),
            "#Y========== Arena Record ==========#n\n\r"
            "  Wins:         #G%d#n\n\r"
            "  Losses:       #R%d#n\n\r"
            "  Arena Points: #Y%d#n\n\r"
            "#Y===================================#n\n\r",
            ch->pcdata->arena_wins,
            ch->pcdata->arena_losses,
            ch->pcdata->arena_pts );
        send_to_char( buf, ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "watch" ) )
    {
        if ( ch->pcdata->arena_state == ARENA_FIGHTING )
        {
            send_to_char( "You are already fighting in the arena!\n\r", ch );
            return;
        }

        ROOM_INDEX_DATA *spec = arena_spec_room();
        if ( spec == NULL )
        {
            send_to_char( "The spectator area is unavailable.\n\r", ch );
            return;
        }

        if ( ch->pcdata->arena_state != ARENA_SPECTATING )
        {
            if ( ch->pcdata->arena_state == ARENA_QUEUED )
                queue_remove( ch->name );
            ch->pcdata->arena_state = ARENA_SPECTATING;
        }

        char_from_room( ch );
        char_to_room( ch, spec );
        act( "$n arrives to watch the arena matches.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "You take a seat in the spectator area.\n\r", ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    if ( !str_cmp( arg, "top" ) )
    {
        /* Simple top-10 scan — walks char_list (online only) sorted by pts. */
        CHAR_DATA *tops[10];
        int        count = 0;
        CHAR_DATA *scan;

        memset( tops, 0, sizeof(tops) );

        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( scan->pcdata->arena_pts <= 0 && scan->pcdata->arena_wins == 0 )
                continue;

            /* Simple insertion sort. */
            int i, j;
            for ( i = 0; i < count && i < 10; i++ )
            {
                if ( scan->pcdata->arena_pts > tops[i]->pcdata->arena_pts )
                {
                    for ( j = UMIN(count, 9); j > i; j-- )
                        tops[j] = tops[j-1];
                    tops[i] = scan;
                    if ( count < 10 ) count++;
                    goto inserted;
                }
            }
            if ( count < 10 )
                tops[count++] = scan;
            inserted:;
        }

        send_to_char(
            "#Y===== Top Arena Fighters (Online) =====#n\n\r"
            "Rank  Name                  Pts   W   L\n\r"
            "----  --------------------  ----  --  --\n\r",
            ch );

        if ( count == 0 )
        {
            send_to_char( "  No ranked fighters online.\n\r", ch );
        }
        else
        {
            int i;
            for ( i = 0; i < count; i++ )
            {
                if ( tops[i] == NULL ) break;
                snprintf( buf, sizeof(buf),
                    "#Y%4d#n  %-20s  #Y%4d#n  #G%2d#n  #R%2d#n\n\r",
                    i + 1,
                    tops[i]->name,
                    tops[i]->pcdata->arena_pts,
                    tops[i]->pcdata->arena_wins,
                    tops[i]->pcdata->arena_losses );
                send_to_char( buf, ch );
            }
        }

        send_to_char( "#Y========================================#n\n\r", ch );
        return;
    }

    /* Fallthrough. */
    send_to_char( "Unknown arena command.  Type 'arena' for help.\n\r", ch );
}
