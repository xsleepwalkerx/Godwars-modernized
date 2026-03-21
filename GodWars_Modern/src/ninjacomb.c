/***************************************************************************
 *  ninjacomb.c — Ninja Hamashaka combat techniques.
 *
 *  Ported from Midnight Realities (Smoo/David Brandon), based on:
 *    Midnight Dreams 1.0 → Rogue Edition → Lords of War → GodWars (KaVir)
 *    → Merc → Diku.
 *
 *  These four techniques unlock via the Hamashaka training track
 *  (pcdata->powers[NPOWER_HAMASHAKA]).  Combat integration (NBIT_SHIELD
 *  damage reflection and NBIT_REDIRECT damage boost) is handled in fight.c.
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"

/* -----------------------------------------------------------------------
 * do_yashakin — Mental energy shield (Hamashaka 4).
 *   Toggles NBIT_SHIELD.  While active there is a 1-in-13 chance per
 *   incoming hit to reflect the blow (handled in fight.c).
 * ----------------------------------------------------------------------- */
void do_yashakin( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( ch->pcdata->powers[NPOWER_HAMASHAKA] < 4 )
    {
        stc("You need Hamashaka level 4 to use Yashakin.\n\r", ch);
        return;
    }

    if ( ch->fighting && !IS_SET(nbit(ch), NBIT_SHIELD) )
    {
        stc("It is too late to raise that shield now.\n\r", ch);
        return;
    }

    nbit(ch) ^= NBIT_SHIELD;

    if ( !IS_SET(nbit(ch), NBIT_SHIELD) )
    {
        stc("&CYou loosen the mental energy field around yourself.#n\n\r", ch);
        act("&C$n loosens the mental energy field around $mself.#n",
            ch, NULL, NULL, TO_ROOM);
        return;
    }

    act("&M$n raises a shield of mental energy around $mself.#n",
        ch, NULL, NULL, TO_ROOM);
    stc("&MYou raise a shield of mental energy around yourself.#n\n\r", ch);
    return;
}

/* -----------------------------------------------------------------------
 * do_tsunaki — Gut-punch technique (Hamashaka 1).
 *   Deals 250-650 damage; 20% chance to stun the target.
 * ----------------------------------------------------------------------- */
void do_tsunaki( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vict = NULL;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( ch->pcdata->powers[NPOWER_HAMASHAKA] < 1 )
    {
        stc("You need Hamashaka level 1 to use Tsunaki.\n\r", ch);
        return;
    }

    if ( !*argument && !ch->fighting )
    {
        stc("Tsunaki whom?\n\r", ch);
        return;
    }

    if ( *argument )
        vict = get_char_room(ch, argument);

    if ( vict == NULL )
        vict = ch->fighting;

    if ( vict == NULL )
    {
        stc("Tsunaki whom?\n\r", ch);
        return;
    }

    act("&W$n shouts out, \"&ySUPER LABOYA MASHATA!&W\".#n",
        ch, NULL, NULL, TO_ROOM);
    stc("&WYou shout out, \"&ySUPER LABOYA MASHATA!&W\".#n\n\r", ch);

    damage(ch, vict, number_range(250, 650), gsn_tsunaki);
    WAIT_STATE(ch, 24);

    if ( vict->hit > 0 && number_range(1, 5) == 3 )
    {
        ch->fighting    = NULL;
        vict->fighting  = NULL;
        ch->position    = POS_STANDING;
        vict->position  = POS_STANDING;
        act("$n's punch hits you in the stomach and leaves you gasping.",
            ch, NULL, vict, TO_VICT);
        act("$n's punch hits $N in the stomach and $E is left gasping.",
            ch, NULL, vict, TO_NOTVICT);
        act("Your punch hits $N in the stomach and $E is left gasping.",
            ch, NULL, vict, TO_CHAR);
        vict->position = POS_STUNNED;
    }

    return;
}

/* -----------------------------------------------------------------------
 * do_jusitori — Mental energy globe (Hamashaka 2).
 *   Sets NBIT_REDIRECT; while active the next hit taken is doubled and
 *   then fired back at the attacker (handled in fight.c).
 * ----------------------------------------------------------------------- */
void do_jusitori( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( ch->pcdata->powers[NPOWER_HAMASHAKA] < 2 )
    {
        stc("You need Hamashaka level 2 to use Jusitori.\n\r", ch);
        return;
    }

    if ( !ch->fighting )
    {
        stc("You need to be fighting to use that.\n\r", ch);
        return;
    }

    if ( IS_SET(nbit(ch), NBIT_REDIRECT) )
    {
        stc("&CYour mind has already focused energy towards that.#n\n\r", ch);
        return;
    }

    act("&Y$n summons up a globe of mental energy.#n",
        ch, NULL, NULL, TO_ROOM);
    stc("&YYou summon up a globe of mental energy.#n\n\r", ch);

    SET_BIT(nbit(ch), NBIT_REDIRECT);
    WAIT_STATE(ch, PULSE_VIOLENCE / 2);
    return;
}

/* -----------------------------------------------------------------------
 * do_jeritsu — Three-death technique (Hamashaka 3).
 *   Heals the target to full, then hits them with three rapid strikes
 *   before multi-hitting.  Only works outside combat.
 * ----------------------------------------------------------------------- */
void do_jeritsu( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vict;
    bool       sac = FALSE;

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_NINJA) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( ch->pcdata->powers[NPOWER_HAMASHAKA] < 3 )
    {
        stc("You need Hamashaka level 3 to use Jeritsu.\n\r", ch);
        return;
    }

    if ( !*argument )
    {
        stc("&BSummon the jeritsu upon who?#n\n\r", ch);
        return;
    }

    if ( (vict = get_char_room(ch, argument)) == NULL )
    {
        stc("They aren't here!\n\r", ch);
        return;
    }

    if ( vict->fighting || ch->fighting )
    {
        stc("Stop the violence first!\n\r", ch);
        return;
    }

    if ( vict->hit < vict->max_hit / 2 )
    {
        stc("They are too hurt!\n\r", ch);
        return;
    }

    act("&Y$n &Gshouts out, \"&MHUMASHAKA TORI BLADJORA&G\" as $e attacks.#n",
        ch, NULL, NULL, TO_ROOM);
    stc("&GYou shout out, \"&RYOU SHALL DIE THE DEATH OF THREE&G\" as you attack.#n\n\r", ch);

    /* Heal victim to full so the technique counts — fair fight declaration */
    vict->hit = vict->max_hit;

    /* Preserve sacred-invis across the attack if set */
    if ( IS_SET(ch->act, PLR_SACREDINVIS) ) sac = TRUE;
    REMOVE_BIT(ch->act, PLR_SACREDINVIS);

    damage(ch, vict, number_range(4000, 5700), gsn_backstab);
    if ( ch->fighting )
        damage(ch, vict, number_range(2000, 4200), gsn_backstab);
    if ( ch->fighting )
        damage(ch, vict, number_range(1500, 3200), gsn_backstab);

    if ( sac && ch->fighting )
        SET_BIT(ch->act, PLR_SACREDINVIS);

    if ( ch->fighting )
        multi_hit(ch, vict, TYPE_HIT);
    if ( ch->fighting )
        multi_hit(ch, vict, TYPE_HIT);
    if ( ch->fighting )
        damage(ch, vict, number_range(1, 700), TYPE_HIT + 5);
    if ( ch->fighting )
        WAIT_STATE(ch, PULSE_VIOLENCE * 2);

    return;
}
