/***************************************************************************
 * questmaster.c — Randomised Quest Task System for GodWars Modernized     *
 *                                                                         *
 * Players find NPCs whose keyword list includes "questmaster" and type    *
 * "questmaster" to receive a task.  Two task types are supported:        *
 *   QTYPE_KILL  — hunt and slay a specific mob (by vnum)                 *
 *   QTYPE_FETCH — locate an object (by vnum) and return it here          *
 *                                                                         *
 * Commands:                                                               *
 *   questmaster  — request / view quest from a nearby questmaster NPC    *
 *   complete     — claim reward / hand in fetch item                     *
 *   questinfo    — display current active quest details                  *
 *                                                                         *
 * quest_update() is called every PULSE_TICK from update_handler().       *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

/* -------------------------------------------------------------------------
 * Internal helpers
 * ------------------------------------------------------------------------- */

/* Pick a random mob from the world suitable for a kill quest.
 * Requirements: NPC, not in the same area as the player, level >= ch->level. */
static MOB_INDEX_DATA *pick_quest_mob( CHAR_DATA *ch )
{
    MOB_INDEX_DATA *pMob;
    MOB_INDEX_DATA *candidates[64];
    int             ncand = 0;
    int             i;

    for ( i = 0; i < MAX_KEY_HASH; i++ )
    {
        for ( pMob = mob_index_hash[i]; pMob != NULL; pMob = pMob->next )
        {
            if ( pMob->level < 1 ) continue;
            /* Avoid trivially easy mobs */
            if ( pMob->level < (ch->level / 2) ) continue;
            /* Avoid mobs whose area is the same as where we are */
            if ( pMob->area == ch->in_room->area ) continue;
            candidates[ncand++] = pMob;
            if ( ncand >= 64 ) break;
        }
        if ( ncand >= 64 ) break;
    }

    if ( ncand == 0 ) return NULL;
    return candidates[ number_range(0, ncand - 1) ];
}

/* Pick a random object suitable for a fetch quest.
 * Requirements: not a quest item, not a corpse, accessible vnum. */
static OBJ_INDEX_DATA *pick_quest_obj( CHAR_DATA *ch )
{
    OBJ_INDEX_DATA *pObj;
    OBJ_INDEX_DATA *candidates[64];
    int             ncand = 0;
    int             i;

    for ( i = 0; i < MAX_KEY_HASH; i++ )
    {
        for ( pObj = obj_index_hash[i]; pObj != NULL; pObj = pObj->next )
        {
            if ( pObj->item_type == ITEM_CORPSE_NPC  ) continue;
            if ( pObj->item_type == ITEM_CORPSE_PC   ) continue;
            if ( IS_SET(pObj->extra_flags, ITEM_NODROP) ) continue;
            /* Skip quest-locked items */
            if ( pObj->quest != 0 ) continue;
            candidates[ncand++] = pObj;
            if ( ncand >= 64 ) break;
        }
        if ( ncand >= 64 ) break;
    }

    if ( ncand == 0 ) return NULL;
    return candidates[ number_range(0, ncand - 1) ];
}

/* Scale QP reward with mob/obj level and player level. */
static int quest_scale_reward( int base, int mob_level, int ch_level )
{
    int reward = base + (mob_level * 2);
    if ( ch_level > mob_level )
        reward = reward * mob_level / UMAX(ch_level, 1);
    return URANGE(10, reward, 200);
}

/* Find a questmaster NPC in the player's current room. */
static CHAR_DATA *find_questmaster( CHAR_DATA *ch )
{
    CHAR_DATA *qm;
    for ( qm = ch->in_room->people; qm != NULL; qm = qm->next_in_room )
    {
        if ( !IS_NPC(qm) ) continue;
        if ( is_name(QUEST_KEYWORD, qm->name) ) return qm;
    }
    return NULL;
}

/* -------------------------------------------------------------------------
 * do_questmaster
 * Request a quest from a nearby questmaster NPC.
 * ------------------------------------------------------------------------- */
void do_questmaster( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA      *qm;
    MOB_INDEX_DATA *pMob;
    OBJ_INDEX_DATA *pObj;
    char            buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) return;

    qm = find_questmaster(ch);
    if ( !qm )
    {
        send_to_char("There is no questmaster here.\n\r", ch);
        return;
    }

    /* Already on a quest? */
    if ( ch->pcdata->quest_type != QTYPE_NONE )
    {
        sprintf(buf,
            "%s says '#7You are already on a quest!  Type #Wquestinfo#7 or #Wcomplete#7 it.#n'\n\r",
            qm->short_descr);
        send_to_char(buf, ch);
        return;
    }

    /* Cooldown between quests */
    if ( ch->pcdata->quest_wait > 0 )
    {
        sprintf(buf,
            "%s says '#7You must rest %d more minute(s) before I have another task for you.#n'\n\r",
            qm->short_descr, ch->pcdata->quest_wait);
        send_to_char(buf, ch);
        return;
    }

    /* 50/50 kill vs fetch */
    if ( number_percent() < 50 )
    {
        /* --- KILL QUEST --- */
        pMob = pick_quest_mob(ch);
        if ( !pMob )
        {
            send_to_char("The questmaster has no suitable task for you right now.\n\r", ch);
            return;
        }

        ch->pcdata->quest_type   = QTYPE_KILL;
        ch->pcdata->quest_mob    = pMob->vnum;
        ch->pcdata->quest_timer  = QUEST_DURATION;
        ch->pcdata->quest_reward = quest_scale_reward(20, pMob->level, ch->level);

        sprintf(buf,
            "%s says '#7I need you to slay #W%s#7.  "
            "Return to me within #W%d#7 minutes and I will pay you #R%d#7 quest points.#n'\n\r",
            qm->short_descr, pMob->short_descr,
            ch->pcdata->quest_timer, ch->pcdata->quest_reward);
        send_to_char(buf, ch);
    }
    else
    {
        /* --- FETCH QUEST --- */
        pObj = pick_quest_obj(ch);
        if ( !pObj )
        {
            /* Fall back to kill quest if no objects available */
            pMob = pick_quest_mob(ch);
            if ( !pMob ) { send_to_char("The questmaster has no task for you now.\n\r", ch); return; }
            ch->pcdata->quest_type   = QTYPE_KILL;
            ch->pcdata->quest_mob    = pMob->vnum;
            ch->pcdata->quest_timer  = QUEST_DURATION;
            ch->pcdata->quest_reward = quest_scale_reward(20, pMob->level, ch->level);
            sprintf(buf,
                "%s says '#7Fetch a task was unavailable — slay #W%s#7 within #W%d#7 minutes "
                "for #R%d#7 quest points.#n'\n\r",
                qm->short_descr, pMob->short_descr,
                ch->pcdata->quest_timer, ch->pcdata->quest_reward);
            send_to_char(buf, ch);
            return;
        }

        ch->pcdata->quest_type   = QTYPE_FETCH;
        ch->pcdata->quest_obj    = pObj->vnum;
        ch->pcdata->quest_timer  = QUEST_DURATION;
        ch->pcdata->quest_reward = quest_scale_reward(15, 1, ch->level);

        sprintf(buf,
            "%s says '#7I require #W%s#7.  Bring it to me within #W%d#7 minutes "
            "and you shall receive #R%d#7 quest points.#n'\n\r",
            qm->short_descr, pObj->short_descr,
            ch->pcdata->quest_timer, ch->pcdata->quest_reward);
        send_to_char(buf, ch);
    }

    act("$N consults $n about a quest.", ch, NULL, qm, TO_ROOM);
}

/* -------------------------------------------------------------------------
 * do_complete
 * Claim the quest reward.  For kill quests the kill already set
 * quest_type = QTYPE_NONE — this just gives a friendly message.
 * For fetch quests the player must be carrying the object.
 * ------------------------------------------------------------------------- */
void do_complete( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA  *qm;
    OBJ_DATA   *obj;
    char        buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) return;

    qm = find_questmaster(ch);
    if ( !qm )
    {
        send_to_char("You must return to a questmaster to complete your quest.\n\r", ch);
        return;
    }

    if ( ch->pcdata->quest_type == QTYPE_NONE )
    {
        if ( ch->pcdata->quest_reward > 0 )
        {
            /* Kill quest was already auto-completed in fight.c; deliver reward now */
            int reward = ch->pcdata->quest_reward;
            ch->pcdata->quest += reward;
            ch->pcdata->quest_count++;
            ch->pcdata->quest_reward = 0;
            ch->pcdata->quest_wait   = QUEST_COOLDOWN;

            sprintf(buf,
                "%s says '#7Well done, %s!  Here are your #R%d#7 quest points.#n'\n\r",
                qm->short_descr, ch->name, reward);
            send_to_char(buf, ch);
            act("$N rewards $n for completing a quest.", ch, NULL, qm, TO_ROOM);
            save_char_obj(ch);
        }
        else
        {
            send_to_char("You have no active quest to complete.\n\r", ch);
        }
        return;
    }

    if ( ch->pcdata->quest_type == QTYPE_FETCH )
    {
        /* Look for the required object in the player's inventory */
        for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
        {
            if ( obj->pIndexData->vnum == ch->pcdata->quest_obj ) break;
        }

        if ( !obj )
        {
            sprintf(buf,
                "%s says '#7You don't seem to have what I need.  Keep looking!#n'\n\r",
                qm->short_descr);
            send_to_char(buf, ch);
            return;
        }

        int reward = ch->pcdata->quest_reward;
        ch->pcdata->quest       += reward;
        ch->pcdata->quest_count++;
        ch->pcdata->quest_type   = QTYPE_NONE;
        ch->pcdata->quest_obj    = 0;
        ch->pcdata->quest_timer  = 0;
        ch->pcdata->quest_reward = 0;
        ch->pcdata->quest_wait   = QUEST_COOLDOWN;

        obj_from_char(obj);
        extract_obj(obj);

        sprintf(buf,
            "%s says '#7Excellent work, %s!  Here are your #R%d#7 quest points.#n'\n\r",
            qm->short_descr, ch->name, reward);
        send_to_char(buf, ch);
        act("$N takes $p from $n and hands over a reward.", ch, obj, qm, TO_ROOM);
        save_char_obj(ch);
        return;
    }

    /* Kill quest still active — remind them */
    {
        MOB_INDEX_DATA *pMob = get_mob_index(ch->pcdata->quest_mob);
        sprintf(buf,
            "%s says '#7You haven't slain %s yet!  "
            "You have #W%d#7 minute(s) remaining.#n'\n\r",
            qm->short_descr,
            pMob ? pMob->short_descr : "your target",
            ch->pcdata->quest_timer);
        send_to_char(buf, ch);
    }
}

/* -------------------------------------------------------------------------
 * do_questinfo
 * Show current quest status.
 * ------------------------------------------------------------------------- */
void do_questinfo( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) return;

    if ( ch->pcdata->quest_wait > 0 )
    {
        sprintf(buf, "#7[Quest] Cooldown: #W%d#7 minute(s) until next quest.\n\r",
                ch->pcdata->quest_wait);
        send_to_char(buf, ch);
    }

    switch ( ch->pcdata->quest_type )
    {
        case QTYPE_NONE:
            if ( ch->pcdata->quest_reward > 0 )
                send_to_char("#7[Quest] #GKill task complete! Return to questmaster to claim reward.#n\n\r", ch);
            else
                send_to_char("#7[Quest] You have no active quest.  Find a questmaster to begin.\n\r", ch);
            break;

        case QTYPE_KILL:
        {
            MOB_INDEX_DATA *pMob = get_mob_index(ch->pcdata->quest_mob);
            sprintf(buf,
                "#7[Quest] #RKILL#7 task: Slay '#W%s#7'\n\r"
                "#7        Reward: #R%d#7 QP  |  Time remaining: #W%d#7 minute(s)\n\r",
                pMob ? pMob->short_descr : "unknown",
                ch->pcdata->quest_reward, ch->pcdata->quest_timer);
            send_to_char(buf, ch);
            break;
        }

        case QTYPE_FETCH:
        {
            OBJ_INDEX_DATA *pObj = get_obj_index(ch->pcdata->quest_obj);
            sprintf(buf,
                "#7[Quest] #YFETCH#7 task: Return '#W%s#7' to a questmaster\n\r"
                "#7        Reward: #R%d#7 QP  |  Time remaining: #W%d#7 minute(s)\n\r",
                pObj ? pObj->short_descr : "unknown",
                ch->pcdata->quest_reward, ch->pcdata->quest_timer);
            send_to_char(buf, ch);
            break;
        }
    }
}

/* -------------------------------------------------------------------------
 * quest_kill_check
 * Called from raw_kill() in fight.c when an NPC is killed.
 * ch = killer, victim = dead NPC.
 * ------------------------------------------------------------------------- */
void quest_kill_check( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( IS_NPC(ch) || !IS_NPC(victim) ) return;
    if ( ch->pcdata->quest_type != QTYPE_KILL ) return;
    if ( victim->pIndexData->vnum != ch->pcdata->quest_mob ) return;

    /* Match — mark quest as pending reward collection */
    ch->pcdata->quest_type = QTYPE_NONE;    /* done; reward waiting */

    send_to_char(
        "\n\r#G[Quest] Target slain!  Return to a questmaster to claim your reward.#n\n\r", ch);
}

/* -------------------------------------------------------------------------
 * quest_update
 * Called every PULSE_TICK.  Ticks down timers; expires overdue quests.
 * ------------------------------------------------------------------------- */
void quest_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *ch;
    char             buf[MAX_STRING_LENGTH];

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected != CON_PLAYING ) continue;
        ch = d->character;
        if ( !ch || IS_NPC(ch) ) continue;

        /* Tick down cooldown */
        if ( ch->pcdata->quest_wait > 0 )
            ch->pcdata->quest_wait--;

        /* Tick down active quest */
        if ( ch->pcdata->quest_type == QTYPE_NONE ) continue;

        ch->pcdata->quest_timer--;

        /* Warn at 3 and 1 minute marks */
        if ( ch->pcdata->quest_timer == 3 || ch->pcdata->quest_timer == 1 )
        {
            sprintf(buf,
                "#R[Quest] WARNING: Only %d minute(s) remain to complete your quest!#n\n\r",
                ch->pcdata->quest_timer);
            send_to_char(buf, ch);
        }

        if ( ch->pcdata->quest_timer <= 0 )
        {
            send_to_char(
                "#R[Quest] Your quest has expired.  You failed to complete it in time.#n\n\r", ch);
            ch->pcdata->quest_type   = QTYPE_NONE;
            ch->pcdata->quest_mob    = 0;
            ch->pcdata->quest_obj    = 0;
            ch->pcdata->quest_timer  = 0;
            ch->pcdata->quest_reward = 0;
            ch->pcdata->quest_failed++;
            ch->pcdata->quest_wait   = QUEST_COOLDOWN;
        }
    }
}
