/***************************************************************************
 * kingdom.c — Kingdom / Clan Political System for GodWars Modernized      *
 *                                                                         *
 * Kingdoms are persistent player-run factions with a hierarchy:          *
 *   King/Queen  — sole leader; can promote, demote, declare war, set tax  *
 *   Warlord     — can recruit and manage members, lead war parties         *
 *   Member      — rank-and-file citizen                                   *
 *                                                                         *
 * Each kingdom has:                                                       *
 *   - A name and description                                              *
 *   - A gold treasury (tax contributions from members)                   *
 *   - An alignment (good / neutral / evil)                                *
 *   - A reputation score (rises from battles won, quests completed)      *
 *   - War declarations against other kingdoms                            *
 *                                                                         *
 * Commands (do_kingdom):                                                  *
 *   kingdom                       — show your kingdom info               *
 *   kingdom list                  — list all kingdoms                    *
 *   kingdom create <name>         — found a new kingdom (imm only)       *
 *   kingdom join <name>           — petition to join a kingdom           *
 *   kingdom leave                 — leave your kingdom                   *
 *   kingdom info <name>           — detailed info on a kingdom           *
 *   kingdom recruit <player>      — accept a petitioner (king/warlord)   *
 *   kingdom expel <player>        — remove a member (king/warlord)       *
 *   kingdom promote <player>      — promote member to warlord (king)     *
 *   kingdom demote <player>       — demote warlord to member (king)      *
 *   kingdom tax <amount>          — contribute gold to treasury (member)  *
 *   kingdom war <name>            — declare war on another kingdom (king) *
 *   kingdom peace <name>          — end a war (king)                     *
 *   kingdom rename <new name>     — rename your kingdom (king)           *
 *   kingdom desc <text>           — set kingdom description (king)       *
 *   kingdom disband               — disband kingdom (imm only)           *
 *                                                                         *
 * Persistence: kingdoms are saved to ../area/kingdoms.dat                *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "merc.h"

/* =========================================================================
 * Constants
 * ========================================================================= */

#define KINGDOM_FILE        "../area/kingdoms.dat"
#define MAX_KINGDOMS        20
#define MAX_KD_NAME         40
#define MAX_KD_DESC         256
#define MAX_KD_MEMBERS      50
#define MAX_KD_WARS         10

/* Kingdom rank values (stored in pcdata->kd_rank). */
#define KD_RANK_NONE        0
#define KD_RANK_MEMBER      1
#define KD_RANK_WARLORD     2
#define KD_RANK_KING        3

/* Kingdom alignment. */
#define KD_ALIGN_NEUTRAL    0
#define KD_ALIGN_GOOD       1
#define KD_ALIGN_EVIL       2

/* Kingdom creation gold cost (taken from king's personal gold). */
#define KD_CREATE_COST      0       /* free — imm-gated anyway */

/* Tax gain to reputation per gold donated. */
#define KD_REP_PER_TAX      100     /* 1 rep per 100 gold    */
#define KD_WIN_REP          5       /* rep gain per war kill */

/* =========================================================================
 * Data structures
 * ========================================================================= */

typedef struct kingdom_data KINGDOM_DATA;
struct kingdom_data
{
    KINGDOM_DATA   *next;
    char            name[MAX_KD_NAME];
    char            desc[MAX_KD_DESC];
    char            leader[MAX_INPUT_LENGTH];     /* name of the King/Queen   */
    char            members[MAX_KD_MEMBERS][MAX_INPUT_LENGTH];
    int             member_count;
    char            wars[MAX_KD_WARS][MAX_KD_NAME]; /* names of enemy kingdoms */
    int             war_count;
    long            treasury;   /* gold in the kingdom bank               */
    int             reputation; /* fame/power score                       */
    int             alignment;  /* KD_ALIGN_*                             */
};

/* =========================================================================
 * Globals
 * ========================================================================= */

static KINGDOM_DATA *kingdom_list = NULL;
static int           kingdom_count = 0;

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

static KINGDOM_DATA *find_kingdom( const char *name )
{
    KINGDOM_DATA *kd;
    for ( kd = kingdom_list; kd != NULL; kd = kd->next )
        if ( !str_cmp( kd->name, name ) )
            return kd;
    return NULL;
}

static KINGDOM_DATA *kingdom_of( CHAR_DATA *ch )
{
    if ( IS_NPC(ch) || ch->pcdata->kd_name[0] == '\0' )
        return NULL;
    return find_kingdom( ch->pcdata->kd_name );
}

static bool kingdom_add_member( KINGDOM_DATA *kd, const char *name )
{
    if ( kd->member_count >= MAX_KD_MEMBERS )
        return FALSE;
    strncpy( kd->members[kd->member_count++], name, MAX_INPUT_LENGTH - 1 );
    return TRUE;
}

static void kingdom_remove_member( KINGDOM_DATA *kd, const char *name )
{
    int i, j;
    for ( i = 0; i < kd->member_count; i++ )
    {
        if ( !str_cmp( kd->members[i], name ) )
        {
            for ( j = i; j < kd->member_count - 1; j++ )
                strcpy( kd->members[j], kd->members[j+1] );
            kd->member_count--;
            return;
        }
    }
}

static bool kingdoms_at_war( KINGDOM_DATA *kd1, KINGDOM_DATA *kd2 )
{
    int i;
    for ( i = 0; i < kd1->war_count; i++ )
        if ( !str_cmp( kd1->wars[i], kd2->name ) )
            return TRUE;
    return FALSE;
}

static const char *rank_name( int rank )
{
    switch ( rank )
    {
    case KD_RANK_KING:    return "King/Queen";
    case KD_RANK_WARLORD: return "Warlord";
    case KD_RANK_MEMBER:  return "Member";
    default:              return "None";
    }
}

static const char *align_name( int al )
{
    switch ( al )
    {
    case KD_ALIGN_GOOD:    return "Good";
    case KD_ALIGN_EVIL:    return "Evil";
    default:               return "Neutral";
    }
}

/* =========================================================================
 * Persistence
 * ========================================================================= */

void kingdom_save( void )
{
    FILE        *fp;
    KINGDOM_DATA *kd;
    int          i;

    if ( ( fp = fopen( KINGDOM_FILE, "w" ) ) == NULL )
    {
        bug( "kingdom_save: cannot open %s for writing", 0 );
        return;
    }

    for ( kd = kingdom_list; kd != NULL; kd = kd->next )
    {
        fprintf( fp, "KINGDOM\n" );
        fprintf( fp, "Name        %s~\n", kd->name );
        fprintf( fp, "Desc        %s~\n", kd->desc );
        fprintf( fp, "Leader      %s~\n", kd->leader );
        fprintf( fp, "Treasury    %ld\n", kd->treasury );
        fprintf( fp, "Reputation  %d\n",  kd->reputation );
        fprintf( fp, "Alignment   %d\n",  kd->alignment );

        for ( i = 0; i < kd->member_count; i++ )
            fprintf( fp, "Member      %s~\n", kd->members[i] );

        for ( i = 0; i < kd->war_count; i++ )
            fprintf( fp, "War         %s~\n", kd->wars[i] );

        fprintf( fp, "End\n\n" );
    }

    fprintf( fp, "EOF\n" );
    fclose( fp );
}

void kingdom_load( void )
{
    FILE        *fp;
    KINGDOM_DATA *kd = NULL;
    char         word[MAX_INPUT_LENGTH];
    char         buf[MAX_KD_DESC];
    bool         done = FALSE;

    kingdom_list  = NULL;
    kingdom_count = 0;

    if ( ( fp = fopen( KINGDOM_FILE, "r" ) ) == NULL )
        return;  /* first boot — no file yet */

    while ( !done )
    {
        if ( fscanf( fp, " %s", word ) != 1 ) break;

        if ( !strcmp( word, "EOF" ) )
        {
            done = TRUE;
            break;
        }

        if ( !strcmp( word, "KINGDOM" ) )
        {
            kd = calloc( 1, sizeof(KINGDOM_DATA) );
            kd->next = kingdom_list;
            kingdom_list = kd;
            kingdom_count++;
            continue;
        }

        if ( kd == NULL ) continue;

        if ( !strcmp( word, "End" ) )
        {
            kd = NULL;
            continue;
        }

#define READ_STR(field, size) \
    do { \
        if ( fscanf(fp, " ") == EOF ) {} \
        fgets(buf, sizeof(buf), fp); \
        { int len = (int)strlen(buf); \
          while (len > 0 && (buf[len-1]=='\n'||buf[len-1]=='\r'||buf[len-1]=='~')) \
              buf[--len] = '\0'; } \
        strncpy(field, buf, (size)-1); \
    } while(0)

        if ( !strcmp( word, "Name" ) )
        {
            READ_STR( kd->name, MAX_KD_NAME );
        }
        else if ( !strcmp( word, "Desc" ) )
        {
            READ_STR( kd->desc, MAX_KD_DESC );
        }
        else if ( !strcmp( word, "Leader" ) )
        {
            READ_STR( kd->leader, MAX_INPUT_LENGTH );
        }
        else if ( !strcmp( word, "Treasury" ) )
        {
            if ( fscanf( fp, " %ld", &kd->treasury ) != 1 ) kd->treasury = 0;
        }
        else if ( !strcmp( word, "Reputation" ) )
        {
            if ( fscanf( fp, " %d", &kd->reputation ) != 1 ) kd->reputation = 0;
        }
        else if ( !strcmp( word, "Alignment" ) )
        {
            if ( fscanf( fp, " %d", &kd->alignment ) != 1 ) kd->alignment = 0;
        }
        else if ( !strcmp( word, "Member" ) )
        {
            if ( kd->member_count < MAX_KD_MEMBERS )
            {
                READ_STR( kd->members[kd->member_count], MAX_INPUT_LENGTH );
                kd->member_count++;
            }
        }
        else if ( !strcmp( word, "War" ) )
        {
            if ( kd->war_count < MAX_KD_WARS )
            {
                READ_STR( kd->wars[kd->war_count], MAX_KD_NAME );
                kd->war_count++;
            }
        }

#undef READ_STR
    }

    fclose( fp );
}

/* =========================================================================
 * Notify all online members of a kingdom.
 * ========================================================================= */

static void kingdom_echo( KINGDOM_DATA *kd, const char *msg )
{
    CHAR_DATA *ch;
    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
        if ( IS_NPC(ch) ) continue;
        if ( !str_cmp( ch->pcdata->kd_name, kd->name ) )
            send_to_char( msg, ch );
    }
}

/* =========================================================================
 * Called from fight.c when a player kills another player.
 * Awards war reputation if the two are from kingdoms at war.
 * ========================================================================= */

void kingdom_check_kill( CHAR_DATA *killer, CHAR_DATA *victim )
{
    KINGDOM_DATA *kk, *kv;

    if ( IS_NPC(killer) || IS_NPC(victim) ) return;
    if ( killer->pcdata->kd_name[0] == '\0' ) return;
    if ( victim->pcdata->kd_name[0] == '\0' ) return;
    if ( !str_cmp( killer->pcdata->kd_name, victim->pcdata->kd_name ) ) return;

    kk = find_kingdom( killer->pcdata->kd_name );
    kv = find_kingdom( victim->pcdata->kd_name );
    if ( kk == NULL || kv == NULL ) return;

    if ( kingdoms_at_war( kk, kv ) )
    {
        char buf[MAX_STRING_LENGTH];
        kk->reputation += KD_WIN_REP;
        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s has slain %s in the name of %s! (+%d rep)\n\r",
            killer->name, victim->name, kk->name, KD_WIN_REP );
        kingdom_echo( kk, buf );
        kingdom_save();
    }
}

/* =========================================================================
 * Command: do_kingdom
 * ========================================================================= */

void do_kingdom( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH * 2];

    if ( IS_NPC(ch) ) return;

    argument = one_argument( argument, arg );

    /* ------------------------------------------------------------------ */
    /* kingdom — show own info */
    if ( arg[0] == '\0' )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        if ( kd == NULL )
        {
            send_to_char(
                "You do not belong to any kingdom.\n\r"
                "Type 'kingdom list' to see available kingdoms.\n\r",
                ch );
            return;
        }

        snprintf( buf, sizeof(buf),
            "#Y========== Kingdom: %s ==========#n\n\r"
            "  Description:  %s\n\r"
            "  Leader:       %s\n\r"
            "  Your Rank:    %s\n\r"
            "  Members:      %d\n\r"
            "  Treasury:     %ld gold\n\r"
            "  Reputation:   %d\n\r"
            "  Alignment:    %s\n\r"
            "  At War With:  ",
            kd->name,
            kd->desc[0] ? kd->desc : "(none)",
            kd->leader,
            rank_name( ch->pcdata->kd_rank ),
            kd->member_count,
            kd->treasury,
            kd->reputation,
            align_name( kd->alignment ) );
        send_to_char( buf, ch );

        if ( kd->war_count == 0 )
            send_to_char( "Nobody\n\r", ch );
        else
        {
            int i;
            for ( i = 0; i < kd->war_count; i++ )
            {
                snprintf( buf, sizeof(buf), "%s%s",
                    kd->wars[i], (i < kd->war_count - 1) ? ", " : "\n\r" );
                send_to_char( buf, ch );
            }
        }

        send_to_char( "#Y===================================#n\n\r", ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom list */
    if ( !str_cmp( arg, "list" ) )
    {
        KINGDOM_DATA *kd;
        int n = 0;

        send_to_char(
            "#Y===== Kingdoms of the Realm =====#n\n\r"
            "Name                    Leader              Rep    Members\n\r"
            "----------------------  ------------------  -----  -------\n\r",
            ch );

        for ( kd = kingdom_list; kd != NULL; kd = kd->next )
        {
            snprintf( buf, sizeof(buf),
                "%-22s  %-18s  #Y%5d#n  %d\n\r",
                kd->name, kd->leader, kd->reputation, kd->member_count );
            send_to_char( buf, ch );
            n++;
        }

        if ( n == 0 )
            send_to_char( "  No kingdoms exist yet.\n\r", ch );

        send_to_char( "#Y==================================#n\n\r", ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom info <name> */
    if ( !str_cmp( arg, "info" ) )
    {
        KINGDOM_DATA *kd;
        char name[MAX_INPUT_LENGTH];
        one_argument( argument, name );

        if ( name[0] == '\0' )
        {
            send_to_char( "Info on which kingdom?\n\r", ch );
            return;
        }

        if ( ( kd = find_kingdom( name ) ) == NULL )
        {
            send_to_char( "No such kingdom.\n\r", ch );
            return;
        }

        snprintf( buf, sizeof(buf),
            "#Y========== Kingdom: %s ==========#n\n\r"
            "  Description: %s\n\r"
            "  Leader:      %s\n\r"
            "  Members:     %d / %d\n\r"
            "  Treasury:    %ld gold\n\r"
            "  Reputation:  %d\n\r"
            "  Alignment:   %s\n\r"
            "  Wars:        %d active\n\r"
            "#Y===================================#n\n\r",
            kd->name,
            kd->desc[0] ? kd->desc : "(none)",
            kd->leader,
            kd->member_count, MAX_KD_MEMBERS,
            kd->treasury,
            kd->reputation,
            align_name( kd->alignment ),
            kd->war_count );
        send_to_char( buf, ch );
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom create <name>  — immortal only */
    if ( !str_cmp( arg, "create" ) )
    {
        char name[MAX_KD_NAME];
        KINGDOM_DATA *kd;

        if ( !IS_IMMORTAL(ch) )
        {
            send_to_char( "Only immortals may found kingdoms.\n\r", ch );
            return;
        }

        one_argument( argument, name );
        if ( name[0] == '\0' )
        {
            send_to_char( "Usage: kingdom create <name>\n\r", ch );
            return;
        }
        if ( (int)strlen(name) > MAX_KD_NAME - 1 )
        {
            snprintf( buf, sizeof(buf), "Kingdom name too long (max %d chars).\n\r",
                MAX_KD_NAME - 1 );
            send_to_char( buf, ch );
            return;
        }
        if ( find_kingdom( name ) != NULL )
        {
            send_to_char( "A kingdom with that name already exists.\n\r", ch );
            return;
        }
        if ( kingdom_count >= MAX_KINGDOMS )
        {
            send_to_char( "The maximum number of kingdoms has been reached.\n\r", ch );
            return;
        }

        kd = calloc( 1, sizeof(KINGDOM_DATA) );
        strncpy( kd->name, name, MAX_KD_NAME - 1 );
        strncpy( kd->desc, "A newly founded kingdom.", MAX_KD_DESC - 1 );
        strncpy( kd->leader, "(none)", MAX_INPUT_LENGTH - 1 );
        kd->next = kingdom_list;
        kingdom_list = kd;
        kingdom_count++;

        snprintf( buf, sizeof(buf), "Kingdom '%s' has been founded!\n\r", name );
        send_to_char( buf, ch );
        send_to_all_char( buf );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom join <name> */
    if ( !str_cmp( arg, "join" ) )
    {
        KINGDOM_DATA *kd;
        char name[MAX_KD_NAME];
        one_argument( argument, name );

        if ( ch->pcdata->kd_name[0] != '\0' )
        {
            send_to_char( "You already belong to a kingdom.  Leave first.\n\r", ch );
            return;
        }
        if ( name[0] == '\0' )
        {
            send_to_char( "Join which kingdom?\n\r", ch );
            return;
        }
        if ( ( kd = find_kingdom( name ) ) == NULL )
        {
            send_to_char( "No such kingdom.\n\r", ch );
            return;
        }
        if ( kd->member_count >= MAX_KD_MEMBERS )
        {
            send_to_char( "That kingdom is full.\n\r", ch );
            return;
        }

        /* Petitions require a king/warlord to accept via 'kingdom recruit'. */
        /* Mark pending by setting kd_petition. */
        strncpy( ch->pcdata->kd_petition, kd->name,
                 sizeof(ch->pcdata->kd_petition) - 1 );

        snprintf( buf, sizeof(buf),
            "You petition to join the Kingdom of %s.\n\r"
            "A warlord or the king must accept you with 'kingdom recruit %s'.\n\r",
            kd->name, ch->name );
        send_to_char( buf, ch );

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s petitions to join %s.\n\r", ch->name, kd->name );
        kingdom_echo( kd, buf );
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom leave */
    if ( !str_cmp( arg, "leave" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );

        if ( kd == NULL )
        {
            send_to_char( "You do not belong to a kingdom.\n\r", ch );
            return;
        }
        if ( ch->pcdata->kd_rank == KD_RANK_KING && kd->member_count > 1 )
        {
            send_to_char(
                "You cannot leave while you are king and others remain.\n\r"
                "Promote a warlord to king first, or disband.\n\r", ch );
            return;
        }

        kingdom_remove_member( kd, ch->name );

        /* If last member leaves, the kingdom dissolves. */
        if ( kd->member_count == 0 )
        {
            KINGDOM_DATA **prev = &kingdom_list;
            while ( *prev && *prev != kd )
                prev = &(*prev)->next;
            if ( *prev )
                *prev = kd->next;
            snprintf( buf, sizeof(buf),
                "#Y[KINGDOM]#n The Kingdom of %s has disbanded.\n\r", kd->name );
            send_to_all_char( buf );
            free( kd );
            kingdom_count--;
        }
        else
        {
            snprintf( buf, sizeof(buf),
                "#Y[KINGDOM]#n %s has left %s.\n\r", ch->name, kd->name );
            kingdom_echo( kd, buf );
        }

        ch->pcdata->kd_name[0]  = '\0';
        ch->pcdata->kd_rank     = KD_RANK_NONE;
        send_to_char( "You have left your kingdom.\n\r", ch );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom recruit <player> — king or warlord only */
    if ( !str_cmp( arg, "recruit" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        CHAR_DATA    *victim;
        char          target[MAX_INPUT_LENGTH];
        one_argument( argument, target );

        if ( kd == NULL )
        {
            send_to_char( "You do not belong to a kingdom.\n\r", ch );
            return;
        }
        if ( ch->pcdata->kd_rank < KD_RANK_WARLORD )
        {
            send_to_char( "Only warlords and the king can recruit.\n\r", ch );
            return;
        }
        if ( target[0] == '\0' )
        {
            send_to_char( "Recruit whom?\n\r", ch );
            return;
        }
        if ( ( victim = get_char_world( ch, target ) ) == NULL || IS_NPC(victim) )
        {
            send_to_char( "That player is not online.\n\r", ch );
            return;
        }
        if ( str_cmp( victim->pcdata->kd_petition, kd->name ) )
        {
            snprintf( buf, sizeof(buf),
                "%s has not petitioned to join %s.\n\r", victim->name, kd->name );
            send_to_char( buf, ch );
            return;
        }
        if ( kd->member_count >= MAX_KD_MEMBERS )
        {
            send_to_char( "The kingdom is full.\n\r", ch );
            return;
        }

        kingdom_add_member( kd, victim->name );
        strncpy( victim->pcdata->kd_name, kd->name,
                 sizeof(victim->pcdata->kd_name) - 1 );
        victim->pcdata->kd_rank      = KD_RANK_MEMBER;
        victim->pcdata->kd_petition[0] = '\0';

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s has joined the Kingdom of %s!\n\r",
            victim->name, kd->name );
        kingdom_echo( kd, buf );
        send_to_char( buf, victim );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom expel <player> — king or warlord */
    if ( !str_cmp( arg, "expel" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        CHAR_DATA    *victim;
        char          target[MAX_INPUT_LENGTH];
        one_argument( argument, target );

        if ( kd == NULL )
        {
            send_to_char( "You do not belong to a kingdom.\n\r", ch );
            return;
        }
        if ( ch->pcdata->kd_rank < KD_RANK_WARLORD )
        {
            send_to_char( "Only warlords and the king can expel members.\n\r", ch );
            return;
        }
        if ( target[0] == '\0' )
        {
            send_to_char( "Expel whom?\n\r", ch );
            return;
        }
        if ( ( victim = get_char_world( ch, target ) ) == NULL || IS_NPC(victim) )
        {
            send_to_char( "That player is not online.\n\r", ch );
            return;
        }
        if ( str_cmp( victim->pcdata->kd_name, kd->name ) )
        {
            send_to_char( "They are not in your kingdom.\n\r", ch );
            return;
        }
        if ( victim->pcdata->kd_rank >= ch->pcdata->kd_rank )
        {
            send_to_char( "You cannot expel someone of equal or higher rank.\n\r", ch );
            return;
        }

        kingdom_remove_member( kd, victim->name );
        victim->pcdata->kd_name[0] = '\0';
        victim->pcdata->kd_rank    = KD_RANK_NONE;

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s has been expelled from %s!\n\r",
            victim->name, kd->name );
        kingdom_echo( kd, buf );

        snprintf( buf, sizeof(buf),
            "You have been expelled from the Kingdom of %s.\n\r", kd->name );
        send_to_char( buf, victim );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom promote <player> — king only */
    if ( !str_cmp( arg, "promote" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        CHAR_DATA    *victim;
        char          target[MAX_INPUT_LENGTH];
        one_argument( argument, target );

        if ( kd == NULL ) { send_to_char("You have no kingdom.\n\r",ch); return; }
        if ( ch->pcdata->kd_rank != KD_RANK_KING )
        {
            send_to_char( "Only the king can promote.\n\r", ch );
            return;
        }
        if ( target[0] == '\0' ) { send_to_char("Promote whom?\n\r",ch); return; }
        if ( ( victim = get_char_world( ch, target ) ) == NULL || IS_NPC(victim) )
        {
            send_to_char( "That player is not online.\n\r", ch );
            return;
        }
        if ( str_cmp( victim->pcdata->kd_name, kd->name ) )
        {
            send_to_char( "They are not in your kingdom.\n\r", ch );
            return;
        }
        if ( victim->pcdata->kd_rank >= KD_RANK_KING )
        {
            send_to_char( "They are already king.\n\r", ch );
            return;
        }

        victim->pcdata->kd_rank++;
        if ( victim->pcdata->kd_rank == KD_RANK_KING )
        {
            /* Transfer leadership. */
            strncpy( kd->leader, victim->name, MAX_INPUT_LENGTH - 1 );
            ch->pcdata->kd_rank = KD_RANK_WARLORD;
        }

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s has been promoted to %s in %s!\n\r",
            victim->name, rank_name(victim->pcdata->kd_rank), kd->name );
        kingdom_echo( kd, buf );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom demote <player> — king only */
    if ( !str_cmp( arg, "demote" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        CHAR_DATA    *victim;
        char          target[MAX_INPUT_LENGTH];
        one_argument( argument, target );

        if ( kd == NULL ) { send_to_char("You have no kingdom.\n\r",ch); return; }
        if ( ch->pcdata->kd_rank != KD_RANK_KING )
        {
            send_to_char( "Only the king can demote.\n\r", ch );
            return;
        }
        if ( target[0] == '\0' ) { send_to_char("Demote whom?\n\r",ch); return; }
        if ( ( victim = get_char_world( ch, target ) ) == NULL || IS_NPC(victim) )
        {
            send_to_char( "That player is not online.\n\r", ch );
            return;
        }
        if ( str_cmp( victim->pcdata->kd_name, kd->name ) )
        {
            send_to_char( "They are not in your kingdom.\n\r", ch );
            return;
        }
        if ( victim->pcdata->kd_rank <= KD_RANK_MEMBER )
        {
            send_to_char( "They are already a regular member.\n\r", ch );
            return;
        }

        victim->pcdata->kd_rank--;

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s has been demoted to %s in %s.\n\r",
            victim->name, rank_name(victim->pcdata->kd_rank), kd->name );
        kingdom_echo( kd, buf );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom tax <amount> */
    if ( !str_cmp( arg, "tax" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        long          amount;
        char          amtbuf[MAX_INPUT_LENGTH];
        one_argument( argument, amtbuf );

        if ( kd == NULL ) { send_to_char("You have no kingdom.\n\r",ch); return; }
        if ( amtbuf[0] == '\0' )
        {
            send_to_char( "Contribute how much gold?\n\r", ch );
            return;
        }
        amount = atol( amtbuf );
        if ( amount <= 0 )
        {
            send_to_char( "Amount must be positive.\n\r", ch );
            return;
        }
        if ( ch->gold < amount )
        {
            send_to_char( "You don't have that much gold.\n\r", ch );
            return;
        }

        ch->gold      -= amount;
        kd->treasury  += amount;
        kd->reputation += (int)(amount / KD_REP_PER_TAX);

        snprintf( buf, sizeof(buf),
            "You contribute %ld gold to the Kingdom of %s.\n\r",
            amount, kd->name );
        send_to_char( buf, ch );

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s contributed %ld gold to the treasury.\n\r",
            ch->name, amount );
        kingdom_echo( kd, buf );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom war <name> — king only */
    if ( !str_cmp( arg, "war" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        KINGDOM_DATA *enemy;
        char          ename[MAX_KD_NAME];
        one_argument( argument, ename );

        if ( kd == NULL ) { send_to_char("You have no kingdom.\n\r",ch); return; }
        if ( ch->pcdata->kd_rank != KD_RANK_KING )
        {
            send_to_char( "Only the king can declare war.\n\r", ch );
            return;
        }
        if ( ename[0] == '\0' )
        {
            send_to_char( "Declare war on which kingdom?\n\r", ch );
            return;
        }
        if ( ( enemy = find_kingdom( ename ) ) == NULL )
        {
            send_to_char( "No such kingdom.\n\r", ch );
            return;
        }
        if ( enemy == kd )
        {
            send_to_char( "You cannot declare war on your own kingdom.\n\r", ch );
            return;
        }
        if ( kingdoms_at_war( kd, enemy ) )
        {
            send_to_char( "You are already at war with them.\n\r", ch );
            return;
        }
        if ( kd->war_count >= MAX_KD_WARS )
        {
            send_to_char( "You are already at war with too many kingdoms.\n\r", ch );
            return;
        }

        strncpy( kd->wars[kd->war_count++], enemy->name, MAX_KD_NAME - 1 );
        strncpy( enemy->wars[enemy->war_count++], kd->name, MAX_KD_NAME - 1 );

        snprintf( buf, sizeof(buf),
            "#R[WAR]#n The Kingdom of %s has declared WAR on %s!\n\r",
            kd->name, enemy->name );
        send_to_all_char( buf );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom peace <name> — king only */
    if ( !str_cmp( arg, "peace" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        KINGDOM_DATA *enemy;
        char          ename[MAX_KD_NAME];
        int           i, j;
        one_argument( argument, ename );

        if ( kd == NULL ) { send_to_char("You have no kingdom.\n\r",ch); return; }
        if ( ch->pcdata->kd_rank != KD_RANK_KING )
        {
            send_to_char( "Only the king can sue for peace.\n\r", ch );
            return;
        }
        if ( ename[0] == '\0' )
        {
            send_to_char( "Make peace with which kingdom?\n\r", ch );
            return;
        }
        if ( ( enemy = find_kingdom( ename ) ) == NULL )
        {
            send_to_char( "No such kingdom.\n\r", ch );
            return;
        }
        if ( !kingdoms_at_war( kd, enemy ) )
        {
            send_to_char( "You are not at war with them.\n\r", ch );
            return;
        }

        /* Remove from both war lists. */
        for ( i = 0; i < kd->war_count; i++ )
            if ( !str_cmp( kd->wars[i], enemy->name ) )
            {
                for ( j = i; j < kd->war_count - 1; j++ )
                    strcpy( kd->wars[j], kd->wars[j+1] );
                kd->war_count--;
                break;
            }

        for ( i = 0; i < enemy->war_count; i++ )
            if ( !str_cmp( enemy->wars[i], kd->name ) )
            {
                for ( j = i; j < enemy->war_count - 1; j++ )
                    strcpy( enemy->wars[j], enemy->wars[j+1] );
                enemy->war_count--;
                break;
            }

        snprintf( buf, sizeof(buf),
            "#G[PEACE]#n The Kingdom of %s and %s have ended their war.\n\r",
            kd->name, enemy->name );
        send_to_all_char( buf );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom rename <new name> — king only */
    if ( !str_cmp( arg, "rename" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );
        char          newname[MAX_KD_NAME];
        one_argument( argument, newname );

        if ( kd == NULL ) { send_to_char("You have no kingdom.\n\r",ch); return; }
        if ( ch->pcdata->kd_rank != KD_RANK_KING )
        {
            send_to_char( "Only the king can rename the kingdom.\n\r", ch );
            return;
        }
        if ( newname[0] == '\0' )
        {
            send_to_char( "Rename to what?\n\r", ch );
            return;
        }
        if ( (int)strlen(newname) > MAX_KD_NAME - 1 )
        {
            send_to_char( "New name too long.\n\r", ch );
            return;
        }
        if ( find_kingdom( newname ) != NULL )
        {
            send_to_char( "A kingdom with that name already exists.\n\r", ch );
            return;
        }

        /* Update all member records. */
        CHAR_DATA *scan;
        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( !str_cmp( scan->pcdata->kd_name, kd->name ) )
                strncpy( scan->pcdata->kd_name, newname,
                         sizeof(scan->pcdata->kd_name) - 1 );
        }

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n %s has been renamed to %s!\n\r",
            kd->name, newname );
        send_to_all_char( buf );

        strncpy( kd->name, newname, MAX_KD_NAME - 1 );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom desc <text> — king only */
    if ( !str_cmp( arg, "desc" ) )
    {
        KINGDOM_DATA *kd = kingdom_of( ch );

        if ( kd == NULL ) { send_to_char("You have no kingdom.\n\r",ch); return; }
        if ( ch->pcdata->kd_rank != KD_RANK_KING )
        {
            send_to_char( "Only the king can set the description.\n\r", ch );
            return;
        }
        if ( argument[0] == '\0' )
        {
            send_to_char( "Set description to what?\n\r", ch );
            return;
        }

        smash_tilde( argument );
        strncpy( kd->desc, argument, MAX_KD_DESC - 1 );
        send_to_char( "Kingdom description updated.\n\r", ch );
        kingdom_save();
        return;
    }

    /* ------------------------------------------------------------------ */
    /* kingdom disband — imm only */
    if ( !str_cmp( arg, "disband" ) )
    {
        KINGDOM_DATA  *kd;
        KINGDOM_DATA **prev;
        char           kname[MAX_KD_NAME];
        CHAR_DATA     *scan;
        one_argument( argument, kname );

        if ( !IS_IMMORTAL(ch) )
        {
            send_to_char( "Only immortals can disband kingdoms.\n\r", ch );
            return;
        }
        if ( kname[0] == '\0' )
        {
            send_to_char( "Disband which kingdom?\n\r", ch );
            return;
        }
        if ( ( kd = find_kingdom( kname ) ) == NULL )
        {
            send_to_char( "No such kingdom.\n\r", ch );
            return;
        }

        /* Clear all member records. */
        for ( scan = char_list; scan != NULL; scan = scan->next )
        {
            if ( IS_NPC(scan) ) continue;
            if ( !str_cmp( scan->pcdata->kd_name, kd->name ) )
            {
                scan->pcdata->kd_name[0] = '\0';
                scan->pcdata->kd_rank    = KD_RANK_NONE;
            }
        }

        prev = &kingdom_list;
        while ( *prev && *prev != kd )
            prev = &(*prev)->next;
        if ( *prev )
            *prev = kd->next;

        snprintf( buf, sizeof(buf),
            "#Y[KINGDOM]#n The Kingdom of %s has been disbanded by the gods.\n\r",
            kd->name );
        send_to_all_char( buf );
        free( kd );
        kingdom_count--;
        kingdom_save();
        return;
    }

    /* Fallthrough. */
    send_to_char(
        "Kingdom commands: list, info, create, join, leave, recruit, expel,\n\r"
        "                  promote, demote, tax, war, peace, rename, desc, disband\n\r",
        ch );
}
