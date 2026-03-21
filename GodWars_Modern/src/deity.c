/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/*
 * deity.c — Deity / Glorify system
 *
 * Players may pledge themselves to one of several deities and earn glory
 * through the `glorify` command.  Higher glory unlocks deity-specific
 * powers accessible via `deitypower` / `invoke`.
 *
 * Deities:
 *   1 Artemis  — goddess of the hunt   (Amazons)
 *   2 Mars     — god of war            (combat classes)
 *   3 Hecate   — goddess of magic      (Mages / Sorcerers)
 *   4 Anubis   — god of death          (Demons / Vampires)
 *   5 Loki     — god of chaos          (tricksters)
 *   6 Gaia     — earth mother          (Werewolves)
 *   7 Odin     — god of wisdom         (Highlanders)
 */

#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* -------------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

const char *deity_name( int deity )
{
    switch ( deity )
    {
        case DEITY_ARTEMIS: return "Artemis";
        case DEITY_MARS:    return "Mars";
        case DEITY_HECATE:  return "Hecate";
        case DEITY_ANUBIS:  return "Anubis";
        case DEITY_LOKI:    return "Loki";
        case DEITY_GAIA:    return "Gaia";
        case DEITY_ODIN:    return "Odin";
        default:            return "None";
    }
}

static const char *glory_rank( int glory )
{
    if ( glory >= GLORY_AVATAR )   return "Avatar";
    if ( glory >= GLORY_CHAMPION ) return "Champion";
    if ( glory >= GLORY_FAITHFUL ) return "Faithful";
    if ( glory >= GLORY_DEVOTED )  return "Devoted";
    return "Initiate";
}

/*
 * update_glory — called from update.c each tick for every player.
 * Decrements the glorify cooldown timer.
 */
void update_glory( CHAR_DATA *ch )
{
    if ( IS_NPC(ch) || ch->pcdata == NULL )
        return;
    if ( ch->pcdata->glory_timer > 0 )
    {
        ch->pcdata->glory_timer--;
        if ( ch->pcdata->glory_timer == 0 )
            REMOVE_BIT( ch->pcdata->deity_flags, DFLAG_INVOKE_TIMER );
    }
}

/* -------------------------------------------------------------------------
 * do_deity — choose / view a deity
 *
 * Usage:  deity                   (show current deity and glory)
 *         deity list              (list available deities)
 *         deity <name>            (pledge to a deity)
 *         deity abandon           (abandon current deity, loses all glory)
 * ---------------------------------------------------------------------- */
void do_deity( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int  d;

    if ( IS_NPC(ch) )
        return;

    argument = one_argument( argument, arg );

    /* --- no argument: show status --- */
    if ( arg[0] == '\0' )
    {
        if ( ch->pcdata->deity == DEITY_NONE )
        {
            send_to_char( "You follow no deity.\n\r"
                          "Type 'deity list' to see available deities.\n\r", ch );
            return;
        }
        {
            char buf[MAX_STRING_LENGTH];
            sprintf( buf,
                "Deity  : %s\n\r"
                "Glory  : %d / %d  (%s)\n\r",
                deity_name( ch->pcdata->deity ),
                ch->pcdata->glory, GLORY_MAX,
                glory_rank( ch->pcdata->glory ) );
            send_to_char( buf, ch );
        }
        return;
    }

    /* --- list --- */
    if ( !str_cmp( arg, "list" ) )
    {
        send_to_char(
            "Available deities:\n\r"
            "  1. Artemis — goddess of the hunt   (favours Amazons)\n\r"
            "  2. Mars    — god of war             (favours combat classes)\n\r"
            "  3. Hecate  — goddess of magic       (favours Mages/Sorcerers)\n\r"
            "  4. Anubis  — god of death           (favours Demons/Vampires)\n\r"
            "  5. Loki    — god of chaos           (favours tricksters)\n\r"
            "  6. Gaia    — earth mother           (favours Werewolves)\n\r"
            "  7. Odin    — god of wisdom          (favours Highlanders)\n\r"
            "\n\r"
            "Type 'deity <name>' to pledge yourself.\n\r", ch );
        return;
    }

    /* --- abandon --- */
    if ( !str_cmp( arg, "abandon" ) )
    {
        if ( ch->pcdata->deity == DEITY_NONE )
        {
            send_to_char( "You follow no deity.\n\r", ch );
            return;
        }
        {
            char buf[MAX_STRING_LENGTH];
            sprintf( buf, "You renounce %s and lose all accumulated glory.\n\r",
                     deity_name( ch->pcdata->deity ) );
            send_to_char( buf, ch );
        }
        ch->pcdata->deity       = DEITY_NONE;
        ch->pcdata->glory       = 0;
        ch->pcdata->glory_timer = 0;
        ch->pcdata->deity_flags = 0;
        return;
    }

    /* --- pledge to a deity --- */
    d = DEITY_NONE;
    if      ( !str_prefix( arg, "artemis" ) ) d = DEITY_ARTEMIS;
    else if ( !str_prefix( arg, "mars"    ) ) d = DEITY_MARS;
    else if ( !str_prefix( arg, "hecate"  ) ) d = DEITY_HECATE;
    else if ( !str_prefix( arg, "anubis"  ) ) d = DEITY_ANUBIS;
    else if ( !str_prefix( arg, "loki"    ) ) d = DEITY_LOKI;
    else if ( !str_prefix( arg, "gaia"    ) ) d = DEITY_GAIA;
    else if ( !str_prefix( arg, "odin"    ) ) d = DEITY_ODIN;

    if ( d == DEITY_NONE )
    {
        send_to_char( "That is not a valid deity name.  Type 'deity list'.\n\r", ch );
        return;
    }

    if ( ch->pcdata->deity == d )
    {
        char buf[MAX_STRING_LENGTH];
        sprintf( buf, "You already follow %s.\n\r", deity_name( d ) );
        send_to_char( buf, ch );
        return;
    }

    if ( ch->pcdata->deity != DEITY_NONE )
    {
        send_to_char( "You must first abandon your current deity (deity abandon).\n\r", ch );
        return;
    }

    {
        char buf[MAX_STRING_LENGTH];
        sprintf( buf, "You kneel in reverence and pledge yourself to %s.\n\r", deity_name( d ) );
        send_to_char( buf, ch );
        sprintf( buf, "%s kneels and pledges to %s.", ch->name, deity_name( d ) );
        act( buf, ch, NULL, NULL, TO_ROOM );
    }

    ch->pcdata->deity       = d;
    ch->pcdata->glory       = 0;
    ch->pcdata->glory_timer = 0;
    ch->pcdata->deity_flags = 0;
}

/* -------------------------------------------------------------------------
 * do_glorify — sacrifice gold / mana to gain glory
 *
 * Usage:  glorify <amount>
 *         One glory point per 100 gold sacrificed (min 100 gold).
 *         Subject to GLORIFY_COOLDOWN tick cooldown.
 * ---------------------------------------------------------------------- */
void do_glorify( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int  gold, gained;

    if ( IS_NPC(ch) )
        return;

    if ( ch->pcdata->deity == DEITY_NONE )
    {
        send_to_char( "You must pledge yourself to a deity first (deity list).\n\r", ch );
        return;
    }

    if ( ch->pcdata->glory_timer > 0 )
    {
        char buf[MAX_STRING_LENGTH];
        sprintf( buf, "You must wait %d more tick%s before glorifying again.\n\r",
                 ch->pcdata->glory_timer,
                 ch->pcdata->glory_timer == 1 ? "" : "s" );
        send_to_char( buf, ch );
        return;
    }

    if ( ch->pcdata->glory >= GLORY_MAX )
    {
        send_to_char( "Your glory is already at its maximum — you are an Avatar!\n\r", ch );
        return;
    }

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Usage: glorify <gold amount>\n\r"
                      "Every 100 gold sacrificed earns 1 glory point.\n\r", ch );
        return;
    }

    gold = atoi( arg );
    if ( gold < 100 )
    {
        send_to_char( "You must sacrifice at least 100 gold.\n\r", ch );
        return;
    }

    if ( ch->gold < gold )
    {
        send_to_char( "You do not have that much gold.\n\r", ch );
        return;
    }

    gained = gold / 100;
    if ( ch->pcdata->glory + gained > GLORY_MAX )
        gained = GLORY_MAX - ch->pcdata->glory;

    /* Charge only the gold actually converted */
    ch->gold -= gained * 100;
    ch->pcdata->glory       += gained;
    ch->pcdata->glory_timer  = GLORIFY_COOLDOWN;

    {
        char buf[MAX_STRING_LENGTH];
        sprintf( buf,
            "You sacrifice %d gold to %s and gain %d glory point%s.\n\r"
            "Glory: %d / %d  (%s)\n\r",
            gained * 100, deity_name( ch->pcdata->deity ),
            gained, gained == 1 ? "" : "s",
            ch->pcdata->glory, GLORY_MAX,
            glory_rank( ch->pcdata->glory ) );
        send_to_char( buf, ch );
    }

    act( "$n raises an offering to the heavens.", ch, NULL, NULL, TO_ROOM );
}

/* -------------------------------------------------------------------------
 * do_deitypower — show available powers for current deity and glory rank
 * ---------------------------------------------------------------------- */
void do_deitypower( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH * 2];
    int  glory;

    if ( IS_NPC(ch) )
        return;

    if ( ch->pcdata->deity == DEITY_NONE )
    {
        send_to_char( "You follow no deity and have no divine powers.\n\r", ch );
        return;
    }

    glory = ch->pcdata->glory;

    sprintf( buf,
        "Deity: %s   Glory: %d/%d  (%s)\n\r"
        "-----------------------------------------------\n\r",
        deity_name( ch->pcdata->deity ), glory, GLORY_MAX,
        glory_rank( glory ) );

    switch ( ch->pcdata->deity )
    {
        case DEITY_ARTEMIS:
            strcat( buf, "  Initiate  : +5%% hit bonus in wilderness rooms\n\r" );
            strcat( buf, "  Devoted   : Tracking sense (auto-detect hidden prey)\n\r" );
            strcat( buf, "  Faithful  : Arrow of Artemis (invoke: ranged smite)\n\r" );
            strcat( buf, "  Champion  : Moonform (invoke: enhanced stealth & speed)\n\r" );
            strcat( buf, "  Avatar    : Huntress Blessing (passive hp regen in wild)\n\r" );
            break;
        case DEITY_MARS:
            strcat( buf, "  Initiate  : +5 hit roll in combat\n\r" );
            strcat( buf, "  Devoted   : Battle Cry (invoke: temporary strength boost)\n\r" );
            strcat( buf, "  Faithful  : Iron Skin (invoke: DR boost for 10 rounds)\n\r" );
            strcat( buf, "  Champion  : Warlord's Fury (invoke: double attacks)\n\r" );
            strcat( buf, "  Avatar    : Aspect of Mars (all combat stats maxed)\n\r" );
            break;
        case DEITY_HECATE:
            strcat( buf, "  Initiate  : +10%% to spell damage\n\r" );
            strcat( buf, "  Devoted   : Arcane Sight (detect magic aura on items)\n\r" );
            strcat( buf, "  Faithful  : Dark Bolt (invoke: powerful magic damage)\n\r" );
            strcat( buf, "  Champion  : Veil of Night (invoke: magic immunity shield)\n\r" );
            strcat( buf, "  Avatar    : Hecate's Mantle (all spells cost half mana)\n\r" );
            break;
        case DEITY_ANUBIS:
            strcat( buf, "  Initiate  : Sense the dying (see low-hp chars in room)\n\r" );
            strcat( buf, "  Devoted   : Death Touch (invoke: drain hp on hit)\n\r" );
            strcat( buf, "  Faithful  : Soul Siphon (invoke: steal target mana)\n\r" );
            strcat( buf, "  Champion  : Undying (invoke: auto-resurrect once)\n\r" );
            strcat( buf, "  Avatar    : Anubis's Judgment (instant-kill undead)\n\r" );
            break;
        case DEITY_LOKI:
            strcat( buf, "  Initiate  : +10%% gold from kills\n\r" );
            strcat( buf, "  Devoted   : Trickster's Step (invoke: blink/teleport)\n\r" );
            strcat( buf, "  Faithful  : Chaos Bolt (invoke: random effect attack)\n\r" );
            strcat( buf, "  Champion  : Shapeshift (invoke: assume random mob form)\n\r" );
            strcat( buf, "  Avatar    : Loki's Gambit (double or nothing damage)\n\r" );
            break;
        case DEITY_GAIA:
            strcat( buf, "  Initiate  : Regenerate faster outdoors\n\r" );
            strcat( buf, "  Devoted   : Earth Bond (invoke: root target in place)\n\r" );
            strcat( buf, "  Faithful  : Thornwall (invoke: reflect melee damage)\n\r" );
            strcat( buf, "  Champion  : Beast Kin (invoke: summon an animal ally)\n\r" );
            strcat( buf, "  Avatar    : Gaia's Embrace (full heal once per day)\n\r" );
            break;
        case DEITY_ODIN:
            strcat( buf, "  Initiate  : +5 to all saves vs spells\n\r" );
            strcat( buf, "  Devoted   : Runic Insight (invoke: identify any item)\n\r" );
            strcat( buf, "  Faithful  : Valkyr's Shield (invoke: magic absorb)\n\r" );
            strcat( buf, "  Champion  : Raven Eye (invoke: scry any room on mud)\n\r" );
            strcat( buf, "  Avatar    : Odin's Wisdom (all saves and lore maxed)\n\r" );
            break;
        default:
            strcat( buf, "No powers found.\n\r" );
            break;
    }

    strcat( buf, "\n\rType 'dpower' to activate your current rank's active power.\n\r" );
    send_to_char( buf, ch );
}

/* -------------------------------------------------------------------------
 * do_invoke — activate the active power for the player's current glory rank
 * ---------------------------------------------------------------------- */
void do_dpower( CHAR_DATA *ch, char *argument )
{
    int  glory, deity;
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
        return;

    deity = ch->pcdata->deity;
    glory = ch->pcdata->glory;

    if ( deity == DEITY_NONE )
    {
        send_to_char( "You follow no deity.\n\r", ch );
        return;
    }

    if ( glory < GLORY_DEVOTED )
    {
        send_to_char( "You need at least 100 glory (Devoted rank) to invoke a power.\n\r", ch );
        return;
    }

    if ( IS_SET( ch->pcdata->deity_flags, DFLAG_INVOKE_TIMER ) )
    {
        send_to_char( "Your invoked power is still recharging.\n\r", ch );
        return;
    }

    /* Determine which tier power to grant */
    if ( glory >= GLORY_AVATAR )
    {
        /* Avatar power — passive, already in effect; invoke does full-tier */
        sprintf( buf, "The full might of %s flows through you!\n\r", deity_name( deity ) );
        send_to_char( buf, ch );
        act( "$n radiates divine power!", ch, NULL, NULL, TO_ROOM );

        switch ( deity )
        {
            case DEITY_ARTEMIS:
                ch->hit = UMIN( ch->hit + 200, ch->max_hit );
                send_to_char( "Huntress Blessing: You feel vitality surge through you.\n\r", ch );
                break;
            case DEITY_MARS:
                ch->hitroll = UMIN( ch->hitroll + 20, 125 );
                ch->damroll = UMIN( ch->damroll + 20, 125 );
                send_to_char( "Aspect of Mars: Your combat ability is transcendent!\n\r", ch );
                break;
            case DEITY_HECATE:
                ch->mana = UMIN( ch->mana + 300, ch->max_mana );
                send_to_char( "Hecate's Mantle: Mana floods into you.\n\r", ch );
                break;
            case DEITY_ANUBIS:
                send_to_char( "Anubis's Judgment: You feel death at your fingertips.\n\r", ch );
                SET_BIT( ch->pcdata->deity_flags, DFLAG_BOON3 );
                break;
            case DEITY_LOKI:
                ch->gold += ch->gold / 2;
                send_to_char( "Loki's Gambit: Fortune smiles on you!\n\r", ch );
                break;
            case DEITY_GAIA:
                ch->hit     = ch->max_hit;
                ch->mana    = ch->max_mana;
                ch->move    = ch->max_move;
                send_to_char( "Gaia's Embrace: You are completely restored!\n\r", ch );
                break;
            case DEITY_ODIN:
                ch->saving_throw = UMAX( ch->saving_throw - 15, -30 );
                send_to_char( "Odin's Wisdom: Your defenses are supreme!\n\r", ch );
                break;
        }
    }
    else if ( glory >= GLORY_CHAMPION )
    {
        sprintf( buf, "%s grants you a champion's boon!\n\r", deity_name( deity ) );
        send_to_char( buf, ch );
        act( "$n is blessed by the gods!", ch, NULL, NULL, TO_ROOM );

        switch ( deity )
        {
            case DEITY_ARTEMIS:
                ch->move = UMIN( ch->move + 150, ch->max_move );
                send_to_char( "Moonform: You move with supernatural speed and grace.\n\r", ch );
                break;
            case DEITY_MARS:
                ch->hitroll = UMIN( ch->hitroll + 10, 125 );
                send_to_char( "Warlord's Fury: Your strikes come twice as fast!\n\r", ch );
                break;
            case DEITY_HECATE:
                ch->mana = UMIN( ch->mana + 200, ch->max_mana );
                send_to_char( "Veil of Night: A shield of magic surrounds you.\n\r", ch );
                break;
            case DEITY_ANUBIS:
                send_to_char( "Undying: Anubis will spare you from death once.\n\r", ch );
                SET_BIT( ch->pcdata->deity_flags, DFLAG_BOON2 );
                break;
            case DEITY_LOKI:
                ch->in_room = get_room_index( ROOM_VNUM_TEMPLE );
                if ( ch->in_room == NULL )
                    ch->in_room = get_room_index( ROOM_VNUM_ALTAR );
                if ( ch->in_room != NULL )
                {
                    char_from_room( ch );
                    char_to_room( ch, ch->in_room );
                }
                send_to_char( "Trickster's Step: You vanish in a puff of chaos!\n\r", ch );
                break;
            case DEITY_GAIA:
                send_to_char( "Beast Kin: An animal ally answers your call.\n\r", ch );
                /* summon would require mob creation — flag for future */
                SET_BIT( ch->pcdata->deity_flags, DFLAG_BOON2 );
                break;
            case DEITY_ODIN:
                send_to_char( "Raven Eye: Your sight extends across the land.\n\r", ch );
                SET_BIT( ch->pcdata->deity_flags, DFLAG_BOON2 );
                break;
        }
    }
    else if ( glory >= GLORY_FAITHFUL )
    {
        sprintf( buf, "%s grants you a faithful's power!\n\r", deity_name( deity ) );
        send_to_char( buf, ch );
        act( "$n glows with divine light.", ch, NULL, NULL, TO_ROOM );

        switch ( deity )
        {
            case DEITY_ARTEMIS:
                if ( ch->fighting )
                {
                    ch->fighting->hit -= number_range( 50, 150 );
                    send_to_char( "Arrow of Artemis: A spectral arrow strikes your foe!\n\r", ch );
                }
                else
                    send_to_char( "You must be in combat to use Arrow of Artemis.\n\r", ch );
                break;
            case DEITY_MARS:
                ch->hit = UMIN( ch->hit + 100, ch->max_hit );
                send_to_char( "Iron Skin: Your body hardens against blows.\n\r", ch );
                break;
            case DEITY_HECATE:
                if ( ch->fighting )
                {
                    int dmg = number_range( 80, 200 );
                    ch->fighting->hit -= dmg;
                    sprintf( buf, "Dark Bolt: You hurl a bolt of black energy for %d damage!\n\r", dmg );
                    send_to_char( buf, ch );
                }
                else
                    send_to_char( "You must be in combat to use Dark Bolt.\n\r", ch );
                break;
            case DEITY_ANUBIS:
                if ( ch->fighting )
                {
                    int drained = ch->fighting->mana / 4;
                    ch->fighting->mana -= drained;
                    ch->mana = UMIN( ch->mana + drained, ch->max_mana );
                    sprintf( buf, "Soul Siphon: You drain %d mana from your enemy!\n\r", drained );
                    send_to_char( buf, ch );
                }
                else
                    send_to_char( "You must be in combat to use Soul Siphon.\n\r", ch );
                break;
            case DEITY_LOKI:
                if ( ch->fighting )
                {
                    int roll = number_range( 1, 6 );
                    sprintf( buf, "Chaos Bolt: A random surge of chaos (%d) strikes your foe!\n\r", roll );
                    send_to_char( buf, ch );
                    ch->fighting->hit -= roll * 30;
                }
                else
                    send_to_char( "You must be in combat to use Chaos Bolt.\n\r", ch );
                break;
            case DEITY_GAIA:
                if ( ch->fighting )
                {
                    send_to_char( "Thornwall: Thorns erupt around you, reflecting damage!\n\r", ch );
                    SET_BIT( ch->pcdata->deity_flags, DFLAG_BOON1 );
                }
                else
                    send_to_char( "You must be in combat to use Thornwall.\n\r", ch );
                break;
            case DEITY_ODIN:
                ch->mana = UMIN( ch->mana + 150, ch->max_mana );
                send_to_char( "Valkyr's Shield: A runic barrier absorbs incoming magic.\n\r", ch );
                SET_BIT( ch->pcdata->deity_flags, DFLAG_BOON1 );
                break;
        }
    }
    else /* GLORY_DEVOTED */
    {
        sprintf( buf, "%s grants you a devoted's blessing!\n\r", deity_name( deity ) );
        send_to_char( buf, ch );
        act( "$n murmurs a prayer and is answered.", ch, NULL, NULL, TO_ROOM );

        switch ( deity )
        {
            case DEITY_ARTEMIS:
                ch->hitroll = UMIN( ch->hitroll + 5, 125 );
                send_to_char( "Tracking Sense: Your senses sharpen.\n\r", ch );
                break;
            case DEITY_MARS:
                ch->pcdata->mod_str = UMIN( ch->pcdata->mod_str + 2, 10 );
                send_to_char( "Battle Cry: Strength surges through your veins!\n\r", ch );
                break;
            case DEITY_HECATE:
                ch->mana = UMIN( ch->mana + 100, ch->max_mana );
                send_to_char( "Arcane Sight: Your magical senses expand.\n\r", ch );
                break;
            case DEITY_ANUBIS:
                send_to_char( "Death Touch: The touch of Anubis lingers on your hands.\n\r", ch );
                SET_BIT( ch->pcdata->deity_flags, DFLAG_BOON1 );
                break;
            case DEITY_LOKI:
                ch->gold += number_range( 100, 500 );
                send_to_char( "Trickster's Step: Loki drops a coin purse in your lap!\n\r", ch );
                break;
            case DEITY_GAIA:
                ch->hit = UMIN( ch->hit + 75, ch->max_hit );
                send_to_char( "Earth Bond: The earth's vitality flows into you.\n\r", ch );
                break;
            case DEITY_ODIN:
                ch->saving_throw = UMAX( ch->saving_throw - 5, -30 );
                send_to_char( "Runic Insight: Runic wards sharpen your defenses.\n\r", ch );
                break;
        }
    }

    /* Set invoke cooldown */
    SET_BIT( ch->pcdata->deity_flags, DFLAG_INVOKE_TIMER );
    /* Timer cleared externally by update_glory after GLORIFY_COOLDOWN ticks */
    ch->pcdata->glory_timer = GLORIFY_COOLDOWN / 2;   /* invoke recharges faster */
}
