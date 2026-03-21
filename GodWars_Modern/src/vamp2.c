/***************************************************************************
 *  vamp2.c — Additional vampire discipline powers.
 *
 *  Ported from GodWars-LOW/Cronic (original GW-LOW codebase) and adapted
 *  to the GodWars-Modernized discipline API (get_disc / ch->vampaff).
 *  All ch->power[DISC_VAMP_*] references have been replaced with
 *  get_disc(ch, DISC_*) reads and ch->pcdata->powers[DISC_*] writes.
 *
 *  New commands provided:
 *    do_vtwist        — Daimoinon  5: rename a carried item
 *    do_bloodwater    — Necromancy 5: turn victim's blood to water (damage)
 *    do_vampdarkness  — Quietus    6: fill room with supernatural darkness
 *    do_vamp_assassinate — Quietus 4: sneak-attack from hidden
 *    do_flamehands    — Protean    5: toggle flaming hands (NEW_MONKFLAME)
 *    do_cauldron      — Thaumaturgy 2: boil victim's blood for damage
 *    do_shadowgaze    — Obtenebration 10: drag an NPC into the Shadowplane
 *    do_scales        — Serpentis  5: armour-boosting scales (NEW_SKIN)
 *    do_bonemod       — Vicissitude 3: grow/retract body modifications
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"

/* -----------------------------------------------------------------------
 * do_vtwist — Daimoinon 5.
 *   Uses infernal will to reshape the name and short description of a
 *   carried object.  Useful for disguising items or roleplaying.
 * ----------------------------------------------------------------------- */
void do_vtwist( CHAR_DATA *ch, char *argument )
{
    char     arg1[MAX_STRING_LENGTH];
    char     arg2[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument(argument, arg1);
    strncpy(arg2, argument, sizeof(arg2) - 1);
    arg2[sizeof(arg2) - 1] = '\0';

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_DAIMOINON) < 5 )
    {
        stc("You must obtain level 5 Daimoinon to use Veil Twist.\n\r", ch);
        return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        stc("Syntax: vtwist <item> <new description>\n\r", ch);
        return;
    }

    if ( (obj = get_obj_carry(ch, arg1)) == NULL )
    {
        stc("You don't have that item.\n\r", ch);
        return;
    }

    if ( strlen(arg2) > 40 || strlen(arg2) < 3 )
    {
        stc("Description must be between 3 and 40 characters.\n\r", ch);
        return;
    }

    free_string(obj->name);
    obj->name = str_dup(arg2);
    free_string(obj->short_descr);
    obj->short_descr = str_dup(arg2);

    stc("The item shimmers as your will reshapes its essence.\n\r", ch);
    act("$n's eyes glow as $e reshapes the essence of an item.",
        ch, NULL, NULL, TO_ROOM);
    return;
}

/* -----------------------------------------------------------------------
 * do_bloodwater — Necromancy 5.
 *   Transmutes the target's blood into water, dealing 1000 damage to PCs
 *   (2000 to NPCs) and draining 150 blood from vampires instead.
 * ----------------------------------------------------------------------- */
void do_bloodwater( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char       arg[MAX_INPUT_LENGTH];
    int        dam;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_NECROMANCY) < 5 )
    {
        stc("You need at least level 5 Necromancy to use Blood Water.\n\r", ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        stc("Whose blood do you wish to turn to water?\n\r", ch);
        return;
    }

    if ( (victim = get_char_room(ch, arg)) == NULL )
    {
        stc("They aren't here.\n\r", ch);
        return;
    }

    if ( is_safe(ch, victim) ) return;

    dam = IS_NPC(victim) ? 2000 : 1000;

    act("$N screams in agony as you turn $S blood to water.",
        ch, NULL, victim, TO_CHAR);
    act("$N screams in agony as $n turns $S blood to water.",
        ch, NULL, victim, TO_NOTVICT);
    act("You scream in agony as $n turns your blood to water.",
        ch, NULL, victim, TO_VICT);

    if ( !IS_CLASS(victim, CLASS_VAMPIRE) )
    {
        victim->hit -= dam;
        update_pos(victim);
        WAIT_STATE(ch, 12);
    }
    else
    {
        victim->pcdata->condition[COND_THIRST] -= 150;
        if ( victim->pcdata->condition[COND_THIRST] < 0 )
            victim->pcdata->condition[COND_THIRST] = 0;
        WAIT_STATE(ch, 6);
    }

    if ( !IS_NPC(victim) && IS_MORE(victim, MORE_GUARDIAN) )
    {
        guardian_message(victim);
        act("$n just used Blood Water on you.", ch, NULL, victim, TO_VICT);
    }
    if ( IS_NPC(victim) && victim->hit < 1 ) raw_kill(victim, ch);
    return;
}

/* -----------------------------------------------------------------------
 * do_vampdarkness — Quietus 6.
 *   Expends 300 blood to flood the current room with supernatural darkness.
 *   The darkness persists until dispelled or the game resets the flag.
 * ----------------------------------------------------------------------- */
void do_vampdarkness( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_QUIETUS) < 6 )
    {
        stc("You require level 6 Quietus to use Darkness of Death.\n\r", ch);
        return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 300 )
    {
        stc("You have insufficient blood.\n\r", ch);
        return;
    }

    ch->pcdata->condition[COND_THIRST] -= 300;

    act("A look of concentration crosses $n's face.", ch, NULL, NULL, TO_ROOM);
    stc("A look of concentration crosses your face.\n\r", ch);

    SET_BIT(ch->in_room->room_flags, ROOM_DARK);
    stc("A supernatural darkness consumes the room.\n\r", ch);
    act("A supernatural darkness consumes the room.", ch, NULL, NULL, TO_ROOM);
    return;
}

/* -----------------------------------------------------------------------
 * do_vamp_assassinate — Quietus 4.
 *   A lethal first strike requiring the vampire to be hidden (AFF_HIDE).
 *   Deals Quietus×200 to PCs and Quietus×400 to NPCs; finishing headbutt
 *   if victim survives at 1 hp.
 * ----------------------------------------------------------------------- */
void do_vamp_assassinate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char       arg[MAX_INPUT_LENGTH];
    char       buf[MAX_STRING_LENGTH];
    int        dam;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_QUIETUS) < 4 )
    {
        stc("You need Quietus level 4 to Assassinate.\n\r", ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        stc("Assassinate whom?\n\r", ch);
        return;
    }

    if ( (victim = get_char_room(ch, arg)) == NULL )
    {
        stc("They aren't here.\n\r", ch);
        return;
    }

    if ( victim == ch )
    {
        stc("How can you assassinate yourself?\n\r", ch);
        return;
    }

    if ( is_safe(ch, victim) ) return;

    if ( victim->fighting != NULL )
    {
        stc("You can't assassinate a fighting person.\n\r", ch);
        return;
    }

    if ( victim->hit < victim->max_hit )
    {
        act("$N is hurt and suspicious — you can't sneak up.",
            ch, NULL, victim, TO_CHAR);
        return;
    }

    if ( !IS_AFFECTED(ch, AFF_HIDE) )
    {
        stc("You must be hidden to assassinate!\n\r", ch);
        return;
    }

    WAIT_STATE(ch, 15);

    dam = IS_NPC(victim)
          ? get_disc(ch, DISC_QUIETUS) * 400
          : get_disc(ch, DISC_QUIETUS) * 200;
    dam += number_range(1, 20);

    if ( dam > victim->hit ) dam = victim->hit - 1;

    sprintf(buf, "Your assassination strikes $N with deadly force! [%d]", dam);
    act(buf, ch, NULL, victim, TO_CHAR);
    sprintf(buf, "$n's assassination strikes you with deadly force! [%d]", dam);
    act(buf, ch, NULL, victim, TO_VICT);
    sprintf(buf, "$n's assassination strikes $N with deadly force! [%d]", dam);
    act(buf, ch, NULL, victim, TO_NOTVICT);

    victim->hit -= dam;
    ch->fighting  = victim;
    victim->fighting = ch;

    if ( victim->hit <= 1 )
    {
        victim->hit = 1;
        stc("You lunge forward with a finishing headbutt!\n\r", ch);
        damage(ch, victim, 11, gsn_headbutt);
    }
    return;
}

/* -----------------------------------------------------------------------
 * do_flamehands — Protean 5.
 *   Toggles NEW_MONKFLAME on the vampire — their hands burn with an unholy
 *   fire that adds to melee damage (handled in fight.c/more.c checks).
 * ----------------------------------------------------------------------- */
void do_flamehands( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_PROTEAN) < 5 )
    {
        stc("You need level 5 Protean to use Flaming Hands.\n\r", ch);
        return;
    }

    if ( IS_SET(ch->newbits, NEW_MONKFLAME) )
    {
        REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
        stc("Your hands are no longer wreathed in unholy flames.\n\r", ch);
        act("$n's hands are no longer wreathed in unholy flames.",
            ch, NULL, NULL, TO_ROOM);
        return;
    }

    SET_BIT(ch->newbits, NEW_MONKFLAME);
    stc("Your hands erupt in unholy flames!\n\r", ch);
    act("$n's hands erupt in unholy flames!", ch, NULL, NULL, TO_ROOM);
    return;
}

/* -----------------------------------------------------------------------
 * do_cauldron — Thaumaturgy 2.
 *   Spends <amount> of the caster's blood to boil the target's blood.
 *   NPCs take ×4 damage; PCs take ×1.5 and also lose blood pool.
 * ----------------------------------------------------------------------- */
void do_cauldron( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char       arg1[MAX_INPUT_LENGTH];
    char       arg2[MAX_INPUT_LENGTH];
    int        value;
    int        dam;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    value    = is_number(arg2) ? atoi(arg2) : -1;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_THAUMATURGY) < 2 )
    {
        stc("You need at least level 2 Thaumaturgy to use Cauldron of Blood.\n\r", ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        stc("Whose blood do you wish to boil?\n\r", ch);
        return;
    }

    if ( (victim = get_char_room(ch, arg1)) == NULL )
    {
        stc("They aren't here.\n\r", ch);
        return;
    }

    if ( arg2[0] == '\0' || value < 1 )
    {
        stc("How much blood do you wish to expend? (1-200)\n\r", ch);
        return;
    }

    if ( value > 200 )
    {
        stc("You can only expend up to 200 blood on this power.\n\r", ch);
        return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < value )
    {
        stc("You don't have that much blood!\n\r", ch);
        return;
    }

    if ( is_safe(ch, victim) ) return;

    ch->pcdata->condition[COND_THIRST] -= value;

    if ( IS_NPC(victim) )
    {
        dam = value * 4;
        victim->hit -= dam;
        act("$N screams in agony as $S blood begins to boil.",
            ch, NULL, victim, TO_CHAR);
        act("$N screams in agony as $n causes $S blood to boil.",
            ch, NULL, victim, TO_NOTVICT);
        act("You scream in agony as $n causes your blood to boil.",
            ch, NULL, victim, TO_VICT);
        WAIT_STATE(ch, 12);
        if ( victim->hit < 1 ) raw_kill(victim, ch);
        return;
    }

    /* PC target — costs both casters' and victim's blood */
    dam = (int)(value * 1.5);
    victim->hit -= dam;

    if ( victim->pcdata->condition[COND_THIRST] < value )
        victim->pcdata->condition[COND_THIRST] = 1;
    else
        victim->pcdata->condition[COND_THIRST] -= value;

    act("$N screams in agony as $S blood begins to boil.",
        ch, NULL, victim, TO_CHAR);
    act("$N screams in agony as $n causes $S blood to boil.",
        ch, NULL, victim, TO_NOTVICT);
    act("You scream in agony as $n causes your blood to boil.",
        ch, NULL, victim, TO_VICT);

    update_pos(victim);
    WAIT_STATE(ch, 12);

    if ( !IS_NPC(victim) && IS_MORE(victim, MORE_GUARDIAN) )
    {
        guardian_message(victim);
        act("$n just used Cauldron of Blood on you.", ch, NULL, victim, TO_VICT);
    }
    return;
}

/* -----------------------------------------------------------------------
 * do_shadowgaze — Obtenebration 10.
 *   While in the Shadowplane, yanks an NPC into it with a gaze, leaving
 *   them stranded there until the room is cleared.
 * ----------------------------------------------------------------------- */
void do_shadowgaze( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char       arg[MAX_INPUT_LENGTH];
    char       buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_OBTENEBRATION) < 10 )
    {
        stc("You need level 10 Obtenebration to use Shadow Gaze.\n\r", ch);
        return;
    }

    if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) )
    {
        stc("You need to be in the Shadowplane first!\n\r", ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        stc("Who do you wish to use Shadow Gaze on?\n\r", ch);
        return;
    }

    if ( (victim = get_char_room(ch, arg)) == NULL )
    {
        stc("They aren't here.\n\r", ch);
        return;
    }

    if ( IS_AFFECTED(victim, AFF_SHADOWPLANE) )
    {
        stc("They are already in the Shadowplane!\n\r", ch);
        return;
    }

    if ( !IS_NPC(victim) )
    {
        stc("Shadow Gaze only works on NPCs.\n\r", ch);
        return;
    }

    sprintf(buf, "%s gazes intently at %s.", ch->name, victim->short_descr);
    act(buf, ch, NULL, NULL, TO_ROOM);
    sprintf(buf, "You gaze deeply into %s's eyes.\n\r", victim->short_descr);
    stc(buf, ch);

    WAIT_STATE(ch, 8);

    sprintf(buf, "You pull %s into the Shadowplane!", victim->short_descr);
    act(buf, ch, NULL, NULL, TO_CHAR);
    sprintf(buf, "%s rips %s into the Shadowplane!", ch->name, victim->short_descr);
    act(buf, ch, NULL, NULL, TO_ROOM);

    SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
    return;
}

/* -----------------------------------------------------------------------
 * do_scales — Serpentis 5.
 *   Toggles chitinous scales that emerge from the body, granting -100 AC.
 *   Uses the NEW_SKIN newbit (same as amazon barkskin, separate classes).
 * ----------------------------------------------------------------------- */
void do_scales( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_SERPENTIS) < 5 )
    {
        stc("You need Serpentis 5 to manifest scales.\n\r", ch);
        return;
    }

    if ( !IS_SET(ch->newbits, NEW_SKIN) )
    {
        stc("Chitinous scales slide out and encase your body.\n\r", ch);
        act("Chitinous scales slide out and encase $n's body.",
            ch, NULL, NULL, TO_ROOM);
        ch->armor -= 100;
        SET_BIT(ch->newbits, NEW_SKIN);
        return;
    }

    stc("Your scales retract back beneath your flesh.\n\r", ch);
    act("$n's scales retract back beneath $s flesh.",
        ch, NULL, NULL, TO_ROOM);
    ch->armor += 100;
    REMOVE_BIT(ch->newbits, NEW_SKIN);
    return;
}

/* -----------------------------------------------------------------------
 * do_bonemod — Vicissitude 3.
 *   Allows a vampire to grow or retract body modifications (horns, wings,
 *   exoskeleton, tail, lion head) using ch->vampaff bitmasks.
 *
 *   Syntax: bonemod <horns|wings|exoskeleton|tail|head>
 * ----------------------------------------------------------------------- */
void do_bonemod( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];

    smash_tilde(argument);
    argument = one_argument(argument, arg1);

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( get_disc(ch, DISC_VICISSITUDE) < 3 )
    {
        stc("You need level 3 Vicissitude to use Bone Modification.\n\r", ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        stc("Syntax: bonemod <horns|wings|exoskeleton|tail|head>\n\r", ch);
        return;
    }

    if ( !str_cmp(arg1, "horns") )
    {
        if ( !IS_VAMPAFF(ch, VAM_BONEHORNS) )
        {
            SET_BIT(ch->vampaff, VAM_BONEHORNS);
            stc("You force a set of curved horns out through your skull.\n\r", ch);
            act("$n forces a set of curved horns out through $s skull!",
                ch, NULL, NULL, TO_ROOM);
        }
        else
        {
            REMOVE_BIT(ch->vampaff, VAM_BONEHORNS);
            stc("You retract your horns back into your skull.\n\r", ch);
            act("$n's horns retract back into $s skull.",
                ch, NULL, NULL, TO_ROOM);
        }
        return;
    }

    if ( !str_cmp(arg1, "wings") )
    {
        if ( !IS_VAMPAFF(ch, VAM_BONEWINGS) )
        {
            SET_BIT(ch->vampaff, VAM_BONEWINGS);
            stc("Leathery wings tear through the flesh of your back.\n\r", ch);
            act("Leathery wings tear through the flesh of $n's back!",
                ch, NULL, NULL, TO_ROOM);
        }
        else
        {
            REMOVE_BIT(ch->vampaff, VAM_BONEWINGS);
            stc("Your wings fold back and absorb into your flesh.\n\r", ch);
            act("$n's wings fold back and absorb into $s flesh.",
                ch, NULL, NULL, TO_ROOM);
        }
        return;
    }

    if ( !str_cmp(arg1, "exoskeleton") )
    {
        if ( !IS_VAMPAFF(ch, VAM_EXOSKELETON) )
        {
            SET_BIT(ch->vampaff, VAM_EXOSKELETON);
            stc("A hard chitin exoskeleton forces its way to the surface of your skin.\n\r", ch);
            act("A hard chitin exoskeleton forces its way to the surface of $n's skin!",
                ch, NULL, NULL, TO_ROOM);
        }
        else
        {
            REMOVE_BIT(ch->vampaff, VAM_EXOSKELETON);
            stc("Your exoskeleton recedes beneath your skin.\n\r", ch);
            act("$n's exoskeleton recedes beneath $s skin.",
                ch, NULL, NULL, TO_ROOM);
        }
        return;
    }

    if ( !str_cmp(arg1, "tail") )
    {
        if ( !IS_VAMPAFF(ch, VAM_BONETAIL) )
        {
            SET_BIT(ch->vampaff, VAM_BONETAIL);
            stc("Your spine elongates into a long, barbed tail.\n\r", ch);
            act("$n's spine elongates into a long, barbed tail!",
                ch, NULL, NULL, TO_ROOM);
        }
        else
        {
            REMOVE_BIT(ch->vampaff, VAM_BONETAIL);
            stc("Your tail retracts back into your spine.\n\r", ch);
            act("$n's tail retracts back into $s spine.",
                ch, NULL, NULL, TO_ROOM);
        }
        return;
    }

    if ( !str_cmp(arg1, "head") )
    {
        if ( !IS_VAMPAFF(ch, VAM_LIONHEAD) )
        {
            SET_BIT(ch->vampaff, VAM_LIONHEAD);
            stc("Your head reshapes into the fierce visage of a lion.\n\r", ch);
            act("$n's head reshapes into the fierce visage of a lion!",
                ch, NULL, NULL, TO_ROOM);
        }
        else
        {
            REMOVE_BIT(ch->vampaff, VAM_LIONHEAD);
            stc("Your head slowly resumes its normal form.\n\r", ch);
            act("$n's head slowly resumes its normal form.",
                ch, NULL, NULL, TO_ROOM);
        }
        return;
    }

    stc("Syntax: bonemod <horns|wings|exoskeleton|tail|head>\n\r", ch);
    return;
}
