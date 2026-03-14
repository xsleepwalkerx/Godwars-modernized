/*
 * stubs.c — Stub implementations and missing global definitions.
 *
 * Provides definitions for functions/variables referenced across the merged
 * EM+Genesis codebase that do not have a canonical implementation yet.
 * Replace stubs with real implementations incrementally.
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* =========================================================================
 * Global variables needed by multiple translation units.
 * ========================================================================= */

/* Leader-board global (declared extern in merc.h) */
LEADER_BOARD leader_board;

/* Crash-log diagnostic variables (declared extern in merc.h) */
bool   pfile           = FALSE;
char  *pfile_name      = NULL;
char  *last_user       = NULL;
bool   command_finished = FALSE;

/* =========================================================================
 * Trivia system stubs (trivia.c not yet implemented).
 * ========================================================================= */

void do_trivia( CHAR_DATA *ch, char *argument )
{
    send_to_char( "The trivia system is not yet implemented.\n\r", ch );
}

/* do_answer is defined in act_comm.c */

void trivia_load( void )
{
    /* Stub: no trivia data to load */
}

void trivia_update( void )
{
    /* Stub: no trivia to update */
}

/* =========================================================================
 * Quest task system stubs (questmaster.c not yet implemented).
 * ========================================================================= */

void do_questmaster( CHAR_DATA *ch, char *argument )
{
    send_to_char( "The quest system is not yet implemented.\n\r", ch );
}

void do_questinfo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "The quest system is not yet implemented.\n\r", ch );
}

void quest_update( void )
{
    /* Stub: no active quests to update */
}

void quest_kill_check( CHAR_DATA *ch, CHAR_DATA *victim )
{
    /* Stub: no quest kill tracking yet */
}

/* =========================================================================
 * Auction house stubs (auction.c not yet implemented).
 * ========================================================================= */

void do_auction( CHAR_DATA *ch, char *argument )
{
    send_to_char( "The auction house is not yet implemented.\n\r", ch );
}

void auction_load( void )
{
    /* Stub: no auction lots to load */
}

void auction_save( void )
{
    /* Stub: no auction lots to save */
}

void auction_update( void )
{
    /* Stub: no lots to tick */
}

void auction_deliver( CHAR_DATA *ch )
{
    /* Stub: no lots to deliver */
}

/* =========================================================================
 * Sorcerer color helper.
 * Maps from PURPLE/RED/BLUE/GREEN/YELLOW/BLACK _MAGIC constants to the
 * index (0-5) in pcdata->colors[6], then returns the stored value.
 * ========================================================================= */

long get_color( CHAR_DATA *ch, long color )
{
    int idx;

    if ( IS_NPC(ch) ) return 0;

    switch ( (int)color )
    {
        case PURPLE_MAGIC: idx = 0; break;
        case RED_MAGIC:    idx = 1; break;
        case BLUE_MAGIC:   idx = 2; break;
        case GREEN_MAGIC:  idx = 3; break;
        case YELLOW_MAGIC: idx = 4; break;
        case BLACK_MAGIC:  idx = 5; break;
        default:           return 0;
    }
    return ch->pcdata->colors[idx];
}

/* =========================================================================
 * Combat utility stubs.
 * ========================================================================= */

/*
 * hurt_fighting — validates that ch can fight victim.
 * Returns TRUE if combat can proceed, FALSE to skip.
 * Ensures victim is not already dead and ch is still alive.
 */
bool hurt_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( ch == NULL || victim == NULL ) return FALSE;
    if ( ch->position <= POS_STUNNED ) return FALSE;
    if ( victim->position <= POS_DEAD ) return FALSE;
    return TRUE;
}

/*
 * mod_damcap — return the effective damage cap for ch hitting victim.
 * Stub: returns victim's max_hit as a reasonable cap placeholder.
 */
long mod_damcap( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( victim == NULL ) return 1000;
    return (long)victim->max_hit;
}
