/***************************************************************************
 * mobprog.c -- Mobile/Object/Room programs for GodWars_Modern            *
 *                                                                         *
 * Based on the Merc MobProg snippet (public domain).                      *
 * Supports mob triggers, object triggers, and room triggers.             *
 *                                                                         *
 * Program syntax:                                                         *
 *   if <condition>                                                        *
 *     <commands>                                                          *
 *   else                                                                  *
 *     <commands>                                                          *
 *   endif                                                                 *
 *   break    -- stop executing this prog                                  *
 *                                                                         *
 * Conditions:  ispc, isnpc, isgood, isevil, isneutral, isfight,         *
 *              isimmort, ischarmed, isfollow, isaffected <bit>,          *
 *              hitprcnt <pct>, inroom <vnum>, wears <vnum>,              *
 *              carries <vnum>, class <class>, name <str>, pos <pos>,     *
 *              rand <pct>                                                 *
 *                                                                         *
 * Commands:    All standard do_* commands plus:                           *
 *   mpmload <vnum>     -- load a mobile                                  *
 *   mpoload <vnum>     -- load an object                                 *
 *   mpkill             -- kill the actor                                  *
 *   mppurge [target]   -- purge mob or target                            *
 *   mpgoto <vnum>      -- teleport mob to room                           *
 *   mptransfer <who>   -- transfer a player to mob's room                *
 *   mpforce <who> <cmd>-- force someone to do a command                  *
 *   mpecho <msg>       -- echo to room                                   *
 *   mpechoat <who> <m> -- echo to one person                            *
 *   mpdelay <pulses>   -- delay execution                                *
 *   mpcancel           -- cancel pending delay                           *
 ***************************************************************************/

#if !defined(_WIN32)
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "merc.h"
#include "mobprog.h"

/*---------------------------------------------------------------------------
 * Internal: skip whitespace
 *--------------------------------------------------------------------------*/
static const char *skip_ws( const char *p )
{
    while ( *p == ' ' || *p == '\t' )
        p++;
    return p;
}

/*---------------------------------------------------------------------------
 * Internal: get a word (modifies buf, returns next parse pos)
 *--------------------------------------------------------------------------*/
static const char *get_word( const char *p, char *buf, int bufsz )
{
    int i = 0;
    p = skip_ws( p );
    while ( *p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r'
            && i < bufsz - 1 )
        buf[i++] = *p++;
    buf[i] = '\0';
    return p;
}

/*---------------------------------------------------------------------------
 * Internal: evaluate a single if-condition
 *--------------------------------------------------------------------------*/
static bool eval_condition( const char *line, PROG_CTX *ctx )
{
    char word[64];
    const char *p = skip_ws( line );
    CHAR_DATA *ch  = ctx->actor;
    CHAR_DATA *mob = ctx->mob;
    bool negate = false;

    /* Support "!" prefix for negation */
    if ( *p == '!' ) { negate = true; p++; }

    p = get_word( p, word, sizeof(word) );
    bool result = false;

    if ( !str_cmp( word, "ispc" ) )
        result = ( ch && !IS_NPC(ch) );
    else if ( !str_cmp( word, "isnpc" ) )
        result = ( ch && IS_NPC(ch) );
    else if ( !str_cmp( word, "isgood" ) )
        result = ( ch && ch->alignment >= 350 );
    else if ( !str_cmp( word, "isevil" ) )
        result = ( ch && ch->alignment <= -350 );
    else if ( !str_cmp( word, "isneutral" ) )
        result = ( ch && ch->alignment > -350 && ch->alignment < 350 );
    else if ( !str_cmp( word, "isfight" ) )
        result = ( ch && ch->fighting != NULL );
    else if ( !str_cmp( word, "isimmort" ) )
        result = ( ch && IS_IMMORTAL(ch) );
    else if ( !str_cmp( word, "ischarmed" ) )
        result = ( ch && IS_AFFECTED(ch, AFF_CHARM) );
    else if ( !str_cmp( word, "isfollow" ) )
        result = ( ch && ch->master != NULL );
    else if ( !str_cmp( word, "rand" ) )
    {
        char pct[16];
        get_word( p, pct, sizeof(pct) );
        result = ( number_percent() <= atoi(pct) );
    }
    else if ( !str_cmp( word, "hitprcnt" ) )
    {
        char pct[16];
        get_word( p, pct, sizeof(pct) );
        result = ( mob && mob->max_hit > 0 &&
                   ( mob->hit * 100 / mob->max_hit ) <= atoi(pct) );
    }
    else if ( !str_cmp( word, "inroom" ) )
    {
        char vnum[16];
        get_word( p, vnum, sizeof(vnum) );
        result = ( mob && mob->in_room &&
                   mob->in_room->vnum == atoi(vnum) );
    }
    else if ( !str_cmp( word, "carries" ) )
    {
        char vnum[16];
        OBJ_DATA *obj;
        get_word( p, vnum, sizeof(vnum) );
        result = false;
        if ( ch )
            for ( obj = ch->carrying; obj; obj = obj->next_content )
                if ( obj->pIndexData->vnum == atoi(vnum) )
                    { result = true; break; }
    }
    else if ( !str_cmp( word, "wears" ) )
    {
        char vnum[16];
        int  slot;
        get_word( p, vnum, sizeof(vnum) );
        result = false;
        if ( ch )
            for ( slot = 0; slot < MAX_WEAR; slot++ )
            {
                OBJ_DATA *obj = get_eq_char( ch, slot );
                if ( obj && obj->pIndexData->vnum == atoi(vnum) )
                    { result = true; break; }
            }
    }
    else if ( !str_cmp( word, "name" ) )
    {
        char name[64];
        get_word( p, name, sizeof(name) );
        result = ( ch && is_name( name, ch->name ) );
    }
    else if ( !str_cmp( word, "class" ) )
    {
        char cls[32];
        get_word( p, cls, sizeof(cls) );
        if ( ch && !IS_NPC(ch) )
        {
            if ( !str_cmp(cls,"vampire") )  result = IS_VAMPIRE(ch);
            else if ( !str_cmp(cls,"werewolf") ) result = IS_WEREWOLF(ch);
            else if ( !str_cmp(cls,"mage") )     result = IS_MAGE(ch);
            else if ( !str_cmp(cls,"lich") )     result = IS_LICH(ch);
            else if ( !str_cmp(cls,"demon") )    result = IS_DEMON(ch);
        }
    }
    else if ( !str_cmp( word, "pos" ) )
    {
        char pos[32];
        get_word( p, pos, sizeof(pos) );
        if ( ch )
        {
            if ( !str_cmp(pos,"dead") )     result = (ch->position == POS_DEAD);
            else if ( !str_cmp(pos,"sleep")) result = (ch->position == POS_SLEEPING);
            else if ( !str_cmp(pos,"rest")) result = (ch->position == POS_RESTING);
            else if ( !str_cmp(pos,"stand")) result = (ch->position == POS_STANDING);
            else if ( !str_cmp(pos,"fight")) result = (ch->position == POS_FIGHTING);
        }
    }

    return negate ? !result : result;
}

/*---------------------------------------------------------------------------
 * mprog_driver
 * Main interpreter loop for a program block.
 * Supports if/else/endif/break nesting up to 10 levels deep.
 *--------------------------------------------------------------------------*/
#define MPROG_MAX_DEPTH 10

void mprog_driver( const char *prog, PROG_CTX *ctx )
{
    char         line[MAX_STRING_LENGTH];
    char         word[256];
    const char  *p;
    bool         cond_stack[MPROG_MAX_DEPTH];
    bool         exec_stack[MPROG_MAX_DEPTH]; /* are we executing? */
    int          depth = 0;
    bool         do_break = false;

    if ( ctx->depth >= MPROG_MAX_DEPTH )
    {
        bug( "mprog_driver: MAX recursion depth reached.", 0 );
        return;
    }
    ctx->depth++;

    cond_stack[0] = true;
    exec_stack[0] = true;

    p = prog;
    while ( *p )
    {
        /* Read one line */
        int li = 0;
        while ( *p && *p != '\n' && *p != '\r' && li < (int)sizeof(line)-1 )
            line[li++] = *p++;
        line[li] = '\0';
        while ( *p == '\n' || *p == '\r' ) p++;

        /* Skip blank / comment lines */
        const char *lp = skip_ws( line );
        if ( !*lp || *lp == '*' || *lp == '#' )
            continue;

        /* Get first keyword */
        lp = get_word( lp, word, sizeof(word) );

        if ( !str_cmp( word, "if" ) )
        {
            bool cond = false;
            if ( exec_stack[depth] )
                cond = eval_condition( lp, ctx );

            depth++;
            if ( depth >= MPROG_MAX_DEPTH ) { ctx->depth--; return; }
            cond_stack[depth] = cond;
            exec_stack[depth] = exec_stack[depth-1] && cond;
        }
        else if ( !str_cmp( word, "else" ) )
        {
            if ( depth > 0 )
            {
                cond_stack[depth] = !cond_stack[depth];
                exec_stack[depth] = exec_stack[depth-1] && cond_stack[depth];
            }
        }
        else if ( !str_cmp( word, "endif" ) )
        {
            if ( depth > 0 ) depth--;
        }
        else if ( !str_cmp( word, "break" ) )
        {
            do_break = true;
            break;
        }
        else if ( exec_stack[depth] )
        {
            /* Execute a command */
            CHAR_DATA *mob  = ctx->mob;
            CHAR_DATA *actor= ctx->actor;

            /* Build full command from word + rest-of-line */
            char fullcmd[MAX_STRING_LENGTH];
            snprintf( fullcmd, sizeof(fullcmd), "%s %s", word,
                      skip_ws(lp) );

            /* --- MobProg special commands --- */
            if ( !str_cmp( word, "mpmload" ) )
            {
                /* mpmload <vnum> */
                int vnum = atoi( skip_ws(lp) );
                MOB_INDEX_DATA *pMob = get_mob_index( vnum );
                if ( pMob && mob && mob->in_room )
                {
                    CHAR_DATA *newmob = create_mobile( pMob );
                    char_to_room( newmob, mob->in_room );
                }
            }
            else if ( !str_cmp( word, "mpoload" ) )
            {
                /* mpoload <vnum> */
                int vnum = atoi( skip_ws(lp) );
                OBJ_INDEX_DATA *pObj = get_obj_index( vnum );
                if ( pObj && mob )
                {
                    OBJ_DATA *newobj = create_object( pObj, mob->level );
                    obj_to_char( newobj, mob );
                }
            }
            else if ( !str_cmp( word, "mpkill" ) )
            {
                if ( mob && actor && !IS_NPC(actor) )
                {
                    if ( mob->fighting == NULL )
                        set_fighting( mob, actor );
                }
            }
            else if ( !str_cmp( word, "mppurge" ) )
            {
                char who[64];
                get_word( skip_ws(lp), who, sizeof(who) );
                if ( who[0] == '\0' )
                {
                    /* Purge self */
                    if ( mob && IS_NPC(mob) )
                    {
                        extract_char( mob, true );
                        ctx->depth--;
                        return; /* mob is gone */
                    }
                }
                else
                {
                    /* Purge named target in room */
                    CHAR_DATA *victim;
                    if ( mob && (victim = get_char_room(mob, who)) != NULL
                         && IS_NPC(victim) )
                        extract_char( victim, true );
                }
            }
            else if ( !str_cmp( word, "mpgoto" ) )
            {
                int vnum = atoi( skip_ws(lp) );
                ROOM_INDEX_DATA *room = get_room_index( vnum );
                if ( mob && room )
                {
                    char_from_room( mob );
                    char_to_room( mob, room );
                }
            }
            else if ( !str_cmp( word, "mptransfer" ) )
            {
                char who[64];
                get_word( skip_ws(lp), who, sizeof(who) );
                CHAR_DATA *victim = NULL;
                if ( !str_cmp( who, "all" ) )
                {
                    /* Transfer all players in room to mob's room */
                    if ( actor && actor->in_room && mob && mob->in_room )
                    {
                        CHAR_DATA *vch, *vch_next;
                        ROOM_INDEX_DATA *dest = mob->in_room;
                        for ( vch = actor->in_room->people; vch; vch = vch_next )
                        {
                            vch_next = vch->next_in_room;
                            if ( !IS_NPC(vch) )
                            {
                                char_from_room( vch );
                                char_to_room( vch, dest );
                                do_look( vch, "auto" );
                            }
                        }
                    }
                }
                else if ( (victim = get_char_world(mob, who)) != NULL
                          && mob && mob->in_room )
                {
                    char_from_room( victim );
                    char_to_room( victim, mob->in_room );
                    do_look( victim, "auto" );
                }
            }
            else if ( !str_cmp( word, "mpforce" ) )
            {
                char who[64];
                const char *rest = get_word( skip_ws(lp), who, sizeof(who) );
                rest = skip_ws(rest);
                CHAR_DATA *victim = NULL;
                if ( !str_cmp( who, "all" ) && mob && mob->in_room )
                {
                    CHAR_DATA *vch, *vch_next;
                    for ( vch = mob->in_room->people; vch; vch = vch_next )
                    {
                        vch_next = vch->next_in_room;
                        if ( !IS_NPC(vch) )
                            interpret( vch, (char *)rest );
                    }
                }
                else if ( (victim = get_char_world(mob, who)) != NULL )
                {
                    interpret( victim, (char *)rest );
                }
            }
            else if ( !str_cmp( word, "mpecho" ) )
            {
                if ( mob && mob->in_room )
                    act( (char *)skip_ws(lp), mob, NULL, NULL, TO_ROOM );
            }
            else if ( !str_cmp( word, "mpechoat" ) )
            {
                char who[64];
                const char *rest = get_word( skip_ws(lp), who, sizeof(who) );
                rest = skip_ws(rest);
                CHAR_DATA *victim = mob ? get_char_room(mob, who) : NULL;
                if ( victim )
                    act( (char *)rest, mob, NULL, victim, TO_VICT );
            }
            else if ( !str_cmp( word, "mpdelay" ) )
            {
                int pulses = atoi( skip_ws(lp) );
                if ( mob )
                {
                    mob->mprog_delay  = UMAX(1, pulses);
                    mob->mprog_target = actor;
                }
            }
            else if ( !str_cmp( word, "mpcancel" ) )
            {
                if ( mob ) mob->mprog_delay = -1;
            }
            else if ( !str_cmp( word, "mpsay" ) )
            {
                /* mpsay - mob says without "You say" prefix */
                if ( mob )
                {
                    char buf[MAX_STRING_LENGTH];
                    sprintf( buf, "%s says '%s'", mob->short_descr,
                             skip_ws(lp) );
                    act( buf, mob, NULL, NULL, TO_ROOM );
                    act( buf, mob, NULL, NULL, TO_CHAR );
                }
            }
            else
            {
                /* Standard game command — mob performs it */
                if ( mob )
                    interpret( mob, fullcmd );
            }
        }
    }

    ctx->depth--;
}

/*---------------------------------------------------------------------------
 * Internal: run all progs of a given trigger type for a mob
 *--------------------------------------------------------------------------*/
static void run_mob_progs( CHAR_DATA *mob, int trig_type,
                           CHAR_DATA *actor, OBJ_DATA *obj,
                           void *vo, const char *arg )
{
    if ( mob == NULL || !IS_NPC(mob) ) return;
    if ( mob->pIndexData == NULL ) return;
    if ( !IS_SET(mob->pIndexData->progtypes, trig_type) ) return;

    PROG_LIST *prg;
    for ( prg = mob->pIndexData->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != trig_type ) continue;

        PROG_CTX ctx;
        ctx.mob         = mob;
        ctx.actor       = actor;
        ctx.obj         = obj;
        ctx.vo          = vo;
        ctx.trigger_arg = (char *)arg;
        ctx.depth       = 0;

        mprog_driver( prg->prog_text, &ctx );

        /* Mob may have been purged during prog execution */
        if ( mob->in_room == NULL ) break;
    }
}

/*===========================================================================
 * Trigger dispatch functions
 *==========================================================================*/

void mprog_act_trigger( const char *buf, CHAR_DATA *mob,
                        CHAR_DATA *ch, OBJ_DATA *obj, void *vo )
{
    if ( !IS_NPC(mob) ) return;
    if ( !IS_SET(mob->pIndexData->progtypes, TRIG_ACT) ) return;

    PROG_LIST *prg;
    for ( prg = mob->pIndexData->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != TRIG_ACT ) continue;
        if ( prg->trig_phrase &&
             !strstr( buf, prg->trig_phrase ) ) continue;

        PROG_CTX ctx = { mob, ch, obj, vo, (char *)buf, 0 };
        mprog_driver( prg->prog_text, &ctx );
        if ( mob->in_room == NULL ) break;
    }
}

void mprog_bribe_trigger( CHAR_DATA *mob, CHAR_DATA *ch, int amount )
{
    if ( !IS_NPC(mob) ) return;
    if ( !IS_SET(mob->pIndexData->progtypes, TRIG_BRIBE) ) return;

    PROG_LIST *prg;
    for ( prg = mob->pIndexData->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != TRIG_BRIBE ) continue;
        if ( amount < atoi(prg->trig_phrase) ) continue;

        PROG_CTX ctx = { mob, ch, NULL, NULL, NULL, 0 };
        mprog_driver( prg->prog_text, &ctx );
        if ( mob->in_room == NULL ) break;
    }
}

void mprog_death_trigger( CHAR_DATA *mob, CHAR_DATA *killer )
{
    run_mob_progs( mob, TRIG_DEATH, killer, NULL, NULL, NULL );
}

void mprog_entry_trigger( CHAR_DATA *mob )
{
    run_mob_progs( mob, TRIG_ENTRY, NULL, NULL, NULL, NULL );
}

void mprog_fight_trigger( CHAR_DATA *mob, CHAR_DATA *ch )
{
    run_mob_progs( mob, TRIG_FIGHT, ch, NULL, NULL, NULL );
}

void mprog_give_trigger( CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_NPC(mob) ) return;
    if ( !IS_SET(mob->pIndexData->progtypes, TRIG_GIVE) ) return;

    PROG_LIST *prg;
    for ( prg = mob->pIndexData->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != TRIG_GIVE ) continue;
        if ( prg->trig_phrase &&
             str_cmp(prg->trig_phrase,"all") &&
             !is_name(prg->trig_phrase, obj->name) ) continue;

        PROG_CTX ctx = { mob, ch, obj, NULL, NULL, 0 };
        mprog_driver( prg->prog_text, &ctx );
        if ( mob->in_room == NULL ) break;
    }
}

void mprog_greet_trigger( CHAR_DATA *ch )
{
    CHAR_DATA *mob;
    if ( ch->in_room == NULL ) return;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( !IS_NPC(mob) ) continue;
        if ( mob->fighting ) continue;
        if ( !IS_SET(mob->pIndexData->progtypes, TRIG_GREET) ) continue;
        if ( number_percent() > 75 ) continue; /* throttle slightly */

        run_mob_progs( mob, TRIG_GREET, ch, NULL, NULL, NULL );
    }
}

void mprog_kill_trigger( CHAR_DATA *mob, CHAR_DATA *ch )
{
    run_mob_progs( mob, TRIG_KILL, ch, NULL, NULL, NULL );
}

void mprog_hpfall_trigger( CHAR_DATA *mob, CHAR_DATA *ch )
{
    run_mob_progs( mob, TRIG_HPCNT, ch, NULL, NULL, NULL );
}

void mprog_random_trigger( CHAR_DATA *mob )
{
    if ( !IS_NPC(mob) ) return;
    if ( !IS_SET(mob->pIndexData->progtypes, TRIG_RANDOM) ) return;

    PROG_LIST *prg;
    for ( prg = mob->pIndexData->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != TRIG_RANDOM ) continue;
        int chance = prg->trig_phrase ? atoi(prg->trig_phrase) : 5;
        if ( number_percent() > chance ) continue;

        PROG_CTX ctx = { mob, NULL, NULL, NULL, NULL, 0 };
        mprog_driver( prg->prog_text, &ctx );
        if ( mob->in_room == NULL ) break;
    }
}

void mprog_speech_trigger( const char *txt, CHAR_DATA *ch )
{
    CHAR_DATA *mob;
    if ( !ch || ch->in_room == NULL ) return;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( !IS_NPC(mob) ) continue;
        if ( !IS_SET(mob->pIndexData->progtypes, TRIG_SPEECH) ) continue;

        PROG_LIST *prg;
        for ( prg = mob->pIndexData->progs; prg; prg = prg->next )
        {
            if ( prg->trig_type != TRIG_SPEECH ) continue;
            if ( prg->trig_phrase &&
                 !strstr( txt, prg->trig_phrase ) ) continue;

            PROG_CTX ctx = { mob, ch, NULL, NULL, (char *)txt, 0 };
            mprog_driver( prg->prog_text, &ctx );
            if ( mob->in_room == NULL ) break;
        }
    }
}

void mprog_exit_trigger( CHAR_DATA *ch, int door )
{
    CHAR_DATA *mob;
    if ( !ch || ch->in_room == NULL ) return;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( !IS_NPC(mob) ) continue;
        if ( !IS_SET(mob->pIndexData->progtypes, TRIG_EXIT) ) continue;
        run_mob_progs( mob, TRIG_EXIT, ch, NULL, NULL, NULL );
    }
}

void mprog_delay_trigger( CHAR_DATA *mob )
{
    run_mob_progs( mob, TRIG_DELAY, mob->mprog_target, NULL, NULL, NULL );
    mob->mprog_delay  = -1;
    mob->mprog_target = NULL;
}

/*---------------------------------------------------------------------------
 * ObjProg triggers
 *--------------------------------------------------------------------------*/
static void run_obj_progs( OBJ_DATA *obj, int trig_type, CHAR_DATA *ch )
{
    if ( !obj || !obj->pIndexData ) return;
    if ( !IS_SET(obj->pIndexData->progtypes, trig_type) ) return;

    PROG_LIST *prg;
    for ( prg = obj->pIndexData->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != trig_type ) continue;
        PROG_CTX ctx = { NULL, ch, obj, NULL, NULL, 0 };
        mprog_driver( prg->prog_text, &ctx );
    }
}

void oprog_get_trigger   ( CHAR_DATA *ch, OBJ_DATA *obj )
    { run_obj_progs( obj, TRIG_OBJ_GET,    ch ); }
void oprog_drop_trigger  ( CHAR_DATA *ch, OBJ_DATA *obj )
    { run_obj_progs( obj, TRIG_OBJ_DROP,   ch ); }
void oprog_wear_trigger  ( CHAR_DATA *ch, OBJ_DATA *obj )
    { run_obj_progs( obj, TRIG_OBJ_WEAR,   ch ); }
void oprog_remove_trigger( CHAR_DATA *ch, OBJ_DATA *obj )
    { run_obj_progs( obj, TRIG_OBJ_REMOVE, ch ); }

/*---------------------------------------------------------------------------
 * RoomProg triggers
 *--------------------------------------------------------------------------*/
void rprog_enter_trigger( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *room;
    if ( !ch || (room = ch->in_room) == NULL ) return;
    if ( !IS_SET(room->progtypes, TRIG_ROOM_ENTER) ) return;

    PROG_LIST *prg;
    for ( prg = room->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != TRIG_ROOM_ENTER ) continue;
        PROG_CTX ctx = { NULL, ch, NULL, NULL, NULL, 0 };
        mprog_driver( prg->prog_text, &ctx );
    }
}

void rprog_exit_trigger( CHAR_DATA *ch, int door )
{
    ROOM_INDEX_DATA *room;
    if ( !ch || (room = ch->in_room) == NULL ) return;
    if ( !IS_SET(room->progtypes, TRIG_ROOM_EXIT) ) return;

    PROG_LIST *prg;
    for ( prg = room->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != TRIG_ROOM_EXIT ) continue;
        PROG_CTX ctx = { NULL, ch, NULL, NULL, NULL, 0 };
        mprog_driver( prg->prog_text, &ctx );
    }
}

void rprog_random_trigger( ROOM_INDEX_DATA *room )
{
    if ( !room ) return;
    if ( !IS_SET(room->progtypes, TRIG_ROOM_RAND) ) return;

    PROG_LIST *prg;
    for ( prg = room->progs; prg; prg = prg->next )
    {
        if ( prg->trig_type != TRIG_ROOM_RAND ) continue;
        int chance = prg->trig_phrase ? atoi(prg->trig_phrase) : 5;
        if ( number_percent() > chance ) continue;
        PROG_CTX ctx = { NULL, NULL, NULL, NULL, NULL, 0 };
        mprog_driver( prg->prog_text, &ctx );
    }
}

/*---------------------------------------------------------------------------
 * Memory management
 *--------------------------------------------------------------------------*/
PROG_LIST *new_prog( void )
{
    PROG_LIST *prg = alloc_perm( sizeof(*prg) );
    prg->next        = NULL;
    prg->trig_type   = 0;
    prg->trig_phrase = str_dup( "" );
    prg->prog_text   = str_dup( "" );
    return prg;
}

void free_prog( PROG_LIST *prg )
{
    if ( !prg ) return;
    free_string( prg->trig_phrase );
    free_string( prg->prog_text );
    /* Note: we use alloc_perm so we don't actually free the struct —
       just clear it for safety */
    prg->next = NULL;
}
