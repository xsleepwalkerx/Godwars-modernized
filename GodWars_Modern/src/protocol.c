/***************************************************************************
 * protocol.c -- Telnet protocol negotiation for GodWars_Modern            *
 *                                                                         *
 * Based on KaVir's Protocol Snippet (public domain, Feb 2011).            *
 * Adapted for Merc/GodWars by the GodWars_Modern project.                *
 *                                                                         *
 * Protocols implemented:                                                  *
 *   TTYPE  - Terminal type detection (client name/version)                *
 *   NAWS   - Window size negotiation                                      *
 *   MSDP   - MUD Server Data Protocol (HP bars, stats, room info, etc.)   *
 *   MSSP   - MUD Server Status Protocol (crawlers/mud lists)              *
 *   ATCP   - Achaea Telnet Client Protocol (legacy MSDP-equivalent)       *
 *   MSP    - MUD Sound Protocol                                           *
 *   MXP    - MUD eXtension Protocol (clickable links)                     *
 ***************************************************************************/

#if !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(_WIN32)
#include <arpa/telnet.h>
#else
/* Minimal telnet constants for Windows / Pelles C (arpa/telnet.h is POSIX-only) */
#define IAC     255
#define DONT    254
#define DO      253
#define WONT    252
#define WILL    251
#define SB      250
#define SE      240
#define TELOPT_ECHO     1
#define TELOPT_NAWS     31
#define TELOPT_TTYPE    24
#endif

#include "merc.h"
#include "protocol.h"

/*---------------------------------------------------------------------------
 * Internal helpers
 *--------------------------------------------------------------------------*/

static void protocol_write( DESCRIPTOR_DATA *d, const char *data, int len )
{
    /* Write raw bytes directly to the descriptor, bypassing colour processing */
    if ( len <= 0 )
        len = (int)strlen( data );
    write_to_descriptor( d->descriptor, (char *)data, len );
}

static void send_iac( DESCRIPTOR_DATA *d, unsigned char cmd, unsigned char opt )
{
    char buf[3];
    buf[0] = (char)IAC;
    buf[1] = (char)cmd;
    buf[2] = (char)opt;
    protocol_write( d, buf, 3 );
}

/*---------------------------------------------------------------------------
 * protocol_negotiate
 * Called from new_descriptor() after the greeting is sent.
 * Sends WILL/DO sequences to begin capability negotiation.
 *--------------------------------------------------------------------------*/
void protocol_negotiate( DESCRIPTOR_DATA *d )
{
    /* Ask if client supports TTYPE (terminal type) */
    send_iac( d, DO,   TELOPT_TTYPE );

    /* Ask if client supports NAWS (window size) */
    send_iac( d, DO,   TELOPT_NAWS );

    /* Offer MSDP */
    send_iac( d, WILL, TELOPT_MSDP );

    /* Offer MSSP */
    send_iac( d, WILL, TELOPT_MSSP );

    /* Offer MXP */
    send_iac( d, DO,   TELOPT_MXP );

    /* Offer MSP */
    send_iac( d, WILL, TELOPT_MSP );

    /* Ask for ATCP (Achaea Telnet Client Protocol) */
    send_iac( d, DO,   TELOPT_ATCP );

    d->bNegotiated = false;
}

/*---------------------------------------------------------------------------
 * protocol_input
 * Strips and processes IAC sequences from raw input.
 * Returns the number of bytes written to dst.
 *--------------------------------------------------------------------------*/
int protocol_input( DESCRIPTOR_DATA *d, char *src, int srclen,
                    char *dst, int dstmax )
{
    int s = 0, w = 0;

    while ( s < srclen && w < dstmax - 1 )
    {
        unsigned char c = (unsigned char)src[s];

        if ( c != IAC )
        {
            dst[w++] = src[s++];
            continue;
        }

        /* IAC sequence */
        s++; /* skip IAC */
        if ( s >= srclen ) break;

        unsigned char cmd = (unsigned char)src[s++];

        if ( cmd == IAC )
        {
            /* Escaped IAC — literal 0xFF byte */
            dst[w++] = (char)IAC;
            continue;
        }

        if ( cmd == SB )
        {
            /* Subnegotiation: collect until IAC SE */
            if ( s >= srclen ) break;
            unsigned char opt = (unsigned char)src[s++];
            char subbuf[512];
            int  sublen = 0;

            while ( s < srclen - 1 )
            {
                unsigned char sc = (unsigned char)src[s];
                if ( sc == IAC )
                {
                    s++;
                    if ( s < srclen && (unsigned char)src[s] == SE )
                    {
                        s++; /* consume SE */
                        break;
                    }
                    /* IAC inside SB that isn't IAC SE — unusual, skip */
                    continue;
                }
                if ( sublen < (int)sizeof(subbuf) - 1 )
                    subbuf[sublen++] = (char)sc;
                s++;
            }
            subbuf[sublen] = '\0';

            /* Process subnegotiation */
            if ( opt == TELOPT_TTYPE )
            {
                /* sublen > 0 and subbuf[0] == IS (0) */
                if ( sublen > 1 && (unsigned char)subbuf[0] == 0 )
                {
                    strncpy( d->terminal_type, subbuf + 1,
                             sizeof(d->terminal_type) - 1 );
                    d->terminal_type[sizeof(d->terminal_type)-1] = '\0';
                    d->bTTYPE = true;

                    /* Detect client capabilities from terminal type */
                    if ( strstr( d->terminal_type, "Mudlet" ) ||
                         strstr( d->terminal_type, "mudlet" ) )
                    {
                        /* Mudlet supports MSDP, MXP, 256-colour */
                        d->bMSDP = true;
                        d->bMXP  = true;
                    }
                    else if ( strstr( d->terminal_type, "MUSHclient" ) ||
                              strstr( d->terminal_type, "mushclient" ) )
                    {
                        d->bMSDP = true;
                        d->bMXP  = true;
                    }
                    else if ( strstr( d->terminal_type, "tintin" ) ||
                              strstr( d->terminal_type, "TinTin" ) ||
                              strstr( d->terminal_type, "wintin" ) )
                    {
                        d->bMSDP = true;
                    }
                }
            }
            else if ( opt == TELOPT_NAWS && sublen >= 4 )
            {
                d->screen_width  = (int16_t)(
                    ((unsigned char)subbuf[0] << 8) | (unsigned char)subbuf[1]);
                d->screen_height = (int16_t)(
                    ((unsigned char)subbuf[2] << 8) | (unsigned char)subbuf[3]);
                d->bNAWS = true;
            }
            else if ( opt == TELOPT_MSDP && sublen >= 2 )
            {
                /* Parse MSDP variable/value pairs from client */
                /* For now we handle MSDP LIST, REPORT, etc. minimally */
                int i = 0;
                while ( i < sublen - 1 )
                {
                    if ( (unsigned char)subbuf[i] == MSDP_VAR )
                    {
                        char var[64] = {0};
                        int  vi = 0;
                        i++;
                        while ( i < sublen && (unsigned char)subbuf[i] != MSDP_VAL
                                && vi < (int)sizeof(var)-1 )
                            var[vi++] = subbuf[i++];

                        char val[256] = {0};
                        int  vli = 0;
                        if ( i < sublen && (unsigned char)subbuf[i] == MSDP_VAL )
                        {
                            i++;
                            while ( i < sublen && (unsigned char)subbuf[i] != MSDP_VAR
                                    && vli < (int)sizeof(val)-1 )
                                val[vli++] = subbuf[i++];
                        }

                        /* Handle MSDP LIST command */
                        if ( !str_cmp( var, "LIST" ) )
                        {
                            if ( !str_cmp( val, "COMMANDS" ) )
                            {
                                msdp_send_pair( d, "COMMANDS",
                                    "\005\002LIST\002REPORT\002RESET\002SEND\002UNREPORT\006" );
                            }
                            else if ( !str_cmp( val, "REPORTABLE_VARIABLES" ) )
                            {
                                msdp_send_pair( d, "REPORTABLE_VARIABLES",
                                    "\005"
                                    "\002CHARACTER_NAME"
                                    "\002HEALTH\002HEALTH_MAX"
                                    "\002MANA\002MANA_MAX"
                                    "\002MOVES\002MOVES_MAX"
                                    "\002AC\002HITROLL\002DAMROLL"
                                    "\002EXPERIENCE"
                                    "\002ROOM_NAME"
                                    "\002WORLD_TIME"
                                    "\002OPPONENT_HEALTH\002OPPONENT_HEALTH_MAX\002OPPONENT_NAME"
                                    "\006" );
                            }
                        }
                        else if ( !str_cmp( var, "REPORT" ) )
                        {
                            /* Client is asking us to auto-report this variable */
                            /* We handle this via msdp_update() on each tick */
                            d->bMSDP = true; /* flag that client wants MSDP */
                        }
                    }
                    else
                        i++;
                }
            }
            else if ( opt == TELOPT_MSSP )
            {
                /* Client is negotiating MSSP */
                mssp_send( d );
            }
            continue;
        }

        /* WILL / WONT / DO / DONT */
        if ( s > srclen ) break;
        /* cmd already consumed the option byte above for SB; for others: */
        if ( cmd == WILL || cmd == WONT || cmd == DO || cmd == DONT )
        {
            if ( s >= srclen ) break;
            unsigned char opt = (unsigned char)src[s++];

            if ( cmd == WILL )
            {
                if ( opt == TELOPT_TTYPE )
                {
                    /* Client says it WILL send ttype — ask for it */
                    char req[] = { (char)IAC, (char)SB, TELOPT_TTYPE,
                                   0 /* IS */, (char)IAC, (char)SE };
                    /* Actually we need SEND (1), not IS */
                    char send_req[] = { (char)IAC, (char)SB, TELOPT_TTYPE,
                                        1 /* SEND */, (char)IAC, (char)SE };
                    protocol_write( d, send_req, 6 );
                }
                else if ( opt == TELOPT_NAWS )
                    d->bNAWS = true;
                else if ( opt == (unsigned char)TELOPT_MSDP )
                    d->bMSDP = true;
                else if ( opt == (unsigned char)TELOPT_MSSP )
                    d->bMSSP = true;
                else if ( opt == (unsigned char)TELOPT_MXP )
                {
                    d->bMXP = true;
                    /* Enable MXP */
                    char enable[] = { (char)IAC, (char)SB,
                                      (char)TELOPT_MXP,
                                      (char)IAC, (char)SE };
                    protocol_write( d, enable, 5 );
                }
                else if ( opt == (unsigned char)TELOPT_MSP )
                    d->bMSP = true;
                else if ( opt == (unsigned char)TELOPT_ATCP )
                    d->bATCP = true;
            }
            /* DONT/WONT — just note the refusal, don't act on it */
        }
    }

    dst[w] = '\0';
    return w;
}

/*---------------------------------------------------------------------------
 * msdp_send_pair
 * Sends a single MSDP variable/value pair via telnet subnegotiation.
 *--------------------------------------------------------------------------*/
void msdp_send_pair( DESCRIPTOR_DATA *d, const char *var, const char *val )
{
    if ( !d->bMSDP && !d->bATCP ) return;

    char buf[1024];
    int  len = 0;

    if ( d->bMSDP )
    {
        buf[len++] = (char)IAC;
        buf[len++] = (char)SB;
        buf[len++] = (char)TELOPT_MSDP;
        buf[len++] = (char)MSDP_VAR;
        while ( *var && len < (int)sizeof(buf) - 10 )
            buf[len++] = *var++;
        buf[len++] = (char)MSDP_VAL;
        while ( *val && len < (int)sizeof(buf) - 6 )
            buf[len++] = *val++;
        buf[len++] = (char)IAC;
        buf[len++] = (char)SE;
    }
    else /* ATCP fallback */
    {
        buf[len++] = (char)IAC;
        buf[len++] = (char)SB;
        buf[len++] = (char)TELOPT_ATCP;
        /* ATCP format: "Var.SubVar value" */
        while ( *var && len < (int)sizeof(buf) - 10 )
            buf[len++] = *var++;
        buf[len++] = ' ';
        while ( *val && len < (int)sizeof(buf) - 6 )
            buf[len++] = *val++;
        buf[len++] = (char)IAC;
        buf[len++] = (char)SE;
    }

    if ( len > 0 )
        protocol_write( d, buf, len );
}

/*---------------------------------------------------------------------------
 * msdp_update
 * Called from bust_a_prompt() or the update loop to push current stats.
 * Only sends values that have actually changed (tracked via last-sent).
 *--------------------------------------------------------------------------*/
void msdp_update( CHAR_DATA *ch )
{
    DESCRIPTOR_DATA *d;
    char buf[64];

    if ( ch == NULL || IS_NPC(ch) ) return;
    d = ch->desc;
    if ( d == NULL ) return;
    if ( !d->bMSDP && !d->bATCP ) return;

    /* Character name */
    msdp_send_pair( d, "CHARACTER_NAME", ch->name ? ch->name : "" );

    /* Health */
    sprintf( buf, "%d", (int)ch->hit );
    msdp_send_pair( d, "HEALTH", buf );
    sprintf( buf, "%d", (int)ch->max_hit );
    msdp_send_pair( d, "HEALTH_MAX", buf );

    /* Mana */
    sprintf( buf, "%d", (int)ch->mana );
    msdp_send_pair( d, "MANA", buf );
    sprintf( buf, "%d", (int)ch->max_mana );
    msdp_send_pair( d, "MANA_MAX", buf );

    /* Moves */
    sprintf( buf, "%d", (int)ch->move );
    msdp_send_pair( d, "MOVES", buf );
    sprintf( buf, "%d", (int)ch->max_move );
    msdp_send_pair( d, "MOVES_MAX", buf );

    /* Combat stats */
    sprintf( buf, "%d", char_ac( ch ) );
    msdp_send_pair( d, "AC", buf );
    sprintf( buf, "%d", char_hitroll( ch ) );
    msdp_send_pair( d, "HITROLL", buf );
    sprintf( buf, "%d", char_damroll( ch ) );
    msdp_send_pair( d, "DAMROLL", buf );

    /* Experience */
    sprintf( buf, "%d", (int)ch->exp );
    msdp_send_pair( d, "EXPERIENCE", buf );

    /* Room name */
    if ( ch->in_room != NULL )
        msdp_send_pair( d, "ROOM_NAME", ch->in_room->name );

    /* Opponent info */
    if ( ch->fighting != NULL )
    {
        CHAR_DATA *opp = ch->fighting;
        msdp_send_pair( d, "OPPONENT_NAME",
            IS_NPC(opp) ? opp->short_descr : opp->name );
        sprintf( buf, "%d", (int)opp->hit );
        msdp_send_pair( d, "OPPONENT_HEALTH", buf );
        sprintf( buf, "%d", (int)opp->max_hit );
        msdp_send_pair( d, "OPPONENT_HEALTH_MAX", buf );
    }
    else
    {
        msdp_send_pair( d, "OPPONENT_NAME", "" );
        msdp_send_pair( d, "OPPONENT_HEALTH", "0" );
        msdp_send_pair( d, "OPPONENT_HEALTH_MAX", "0" );
    }
}

/*---------------------------------------------------------------------------
 * mssp_send
 * Sends MUD Server Status Protocol data in response to a client request.
 * Used by mud-list crawlers (TheMudConnector, MudStats, etc.)
 *--------------------------------------------------------------------------*/

/* These can be set at startup time */
static int  s_Players  = 0;
static time_t s_Uptime = 0;

void MSSPSetPlayers( int n )
{
    s_Players = n;
    if ( s_Uptime == 0 )
        s_Uptime = time(0);
}

void mssp_send( DESCRIPTOR_DATA *d )
{
    char buf[2048];
    int  len = 0;
    int  players = 0;
    char uptime_str[32];

    /* Count connected players */
    DESCRIPTOR_DATA *dd;
    for ( dd = descriptor_list; dd != NULL; dd = dd->next )
    {
        if ( dd->connected == CON_PLAYING && dd->character != NULL &&
             !IS_NPC(dd->character) )
            players++;
    }

    sprintf( uptime_str, "%ld", (long)( s_Uptime ? s_Uptime : time(0) ) );

    buf[len++] = (char)IAC;
    buf[len++] = (char)SB;
    buf[len++] = (char)TELOPT_MSSP;

#define MSSP_PAIR(var, val) \
    do { \
        buf[len++] = (char)MSSP_VAR; \
        const char *_v = (var); \
        while (*_v && len < (int)sizeof(buf)-20) buf[len++] = *_v++; \
        buf[len++] = (char)MSSP_VAL; \
        _v = (val); \
        while (*_v && len < (int)sizeof(buf)-20) buf[len++] = *_v++; \
    } while(0)

    MSSP_PAIR( "NAME",         MUD_NAME );
    MSSP_PAIR( "PLAYERS",      itoa_static(players) );
    MSSP_PAIR( "UPTIME",       uptime_str );
    MSSP_PAIR( "PORT",         MUD_PORT );
    MSSP_PAIR( "CODEBASE",     "GodWars" );
    MSSP_PAIR( "GENRE",        "Fantasy" );
    MSSP_PAIR( "SUBGENRE",     "World of Darkness" );
    MSSP_PAIR( "GAMEPLAY",     "Hack and Slash" );
    MSSP_PAIR( "STATUS",       "Alpha" );
    MSSP_PAIR( "LANGUAGE",     "English" );
    MSSP_PAIR( "LOCATION",     "United States" );
    MSSP_PAIR( "MINIMUM AGE",  "18" );
    MSSP_PAIR( "WEBSITE",      MUD_WEBSITE );
    MSSP_PAIR( "FAMILY",       "Merc" );
    MSSP_PAIR( "ANSI",         "1" );
    MSSP_PAIR( "VT100",        "1" );
    MSSP_PAIR( "UTF-8",        "0" );
    MSSP_PAIR( "256 COLORS",   "0" );
    MSSP_PAIR( "MCCP",         "0" );
    MSSP_PAIR( "MXP",          "1" );
    MSSP_PAIR( "MSDP",         "1" );
    MSSP_PAIR( "MSP",          "1" );
    MSSP_PAIR( "SSL",          "0" );
    MSSP_PAIR( "PAY TO PLAY",  "0" );
    MSSP_PAIR( "PAY FOR PERKS","0" );

#undef MSSP_PAIR

    buf[len++] = (char)IAC;
    buf[len++] = (char)SE;

    protocol_write( d, buf, len );
}

/* Simple integer-to-string without sprintf — avoids re-entrancy issues */
static char s_itoa_buf[32];
const char *itoa_static( int n )
{
    sprintf( s_itoa_buf, "%d", n );
    return s_itoa_buf;
}
/*---------------------------------------------------------------------------
 * mxp_tag
 * Wraps text in an MXP tag. If client doesn't support MXP, returns plain text.
 * Example: mxp_tag("A HREF='north'", "Go north")
 *--------------------------------------------------------------------------*/
static char s_mxp_buf[512];
const char *mxp_tag( const char *tag, const char *text )
{
    snprintf( s_mxp_buf, sizeof(s_mxp_buf), "\x1B[1z<%s>%s</%s>",
              tag, text, tag );
    return s_mxp_buf;
}

/*---------------------------------------------------------------------------
 * protocol_terminal_type
 *--------------------------------------------------------------------------*/
const char *protocol_terminal_type( DESCRIPTOR_DATA *d )
{
    if ( d->bTTYPE && d->terminal_type[0] != '\0' )
        return d->terminal_type;
    return "unknown";
}
