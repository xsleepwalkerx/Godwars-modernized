/***************************************************************************
 * protocol.h -- Telnet protocol negotiation for GodWars_Modern            *
 *                                                                         *
 * Based on KaVir's Protocol Snippet (public domain, Feb 2011).            *
 * Adapted and simplified for Merc/GodWars by the GodWars_Modern project. *
 *                                                                         *
 * Supports: TTYPE, NAWS, MSDP, MSSP, ATCP, MSP, MXP, ECHO               *
 ***************************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* ---- Telnet option codes not always in <arpa/telnet.h> ---- */
#ifndef TELOPT_MSDP
#define TELOPT_MSDP         69
#endif
#ifndef TELOPT_MSSP
#define TELOPT_MSSP         70
#endif
#ifndef TELOPT_MCCP2
#define TELOPT_MCCP2        86
#endif
#ifndef TELOPT_MSP
#define TELOPT_MSP          90
#endif
#ifndef TELOPT_MXP
#define TELOPT_MXP          91
#endif
#ifndef TELOPT_ATCP
#define TELOPT_ATCP         200
#endif
#ifndef TELOPT_CHARSET
#define TELOPT_CHARSET      42
#endif

/* ---- MSDP special bytes ---- */
#define MSDP_VAR            1
#define MSDP_VAL            2
#define MSDP_TABLE_OPEN     3
#define MSDP_TABLE_CLOSE    4
#define MSDP_ARRAY_OPEN     5
#define MSDP_ARRAY_CLOSE    6

/* ---- MSSP special bytes ---- */
#define MSSP_VAR            1
#define MSSP_VAL            2

/* ---- MUD name for MSSP ---- */
#define MUD_NAME            "GodWars Modern"
#define MUD_PORT            "4000"
#define MUD_WEBSITE         "http://localhost"

/* ---- MSDP variable indices ---- */
typedef enum
{
    MSDP_NONE = 0,
    MSDP_CHARACTER_NAME,
    MSDP_SERVER_ID,
    MSDP_SERVER_TIME,
    MSDP_HEALTH,
    MSDP_HEALTH_MAX,
    MSDP_MANA,
    MSDP_MANA_MAX,
    MSDP_MOVES,
    MSDP_MOVES_MAX,
    MSDP_AC,
    MSDP_HITROLL,
    MSDP_DAMROLL,
    MSDP_EXPERIENCE,
    MSDP_ROOM_NAME,
    MSDP_ROOM_EXITS,
    MSDP_WORLD_TIME,
    MSDP_OPPONENT_HEALTH,
    MSDP_OPPONENT_HEALTH_MAX,
    MSDP_OPPONENT_NAME,
    MSDP_MAX
} msdp_var_t;

/* ---- Function prototypes ---- */

/* Called in new_descriptor() — sends initial telnet negotiations */
void    protocol_negotiate    ( DESCRIPTOR_DATA *d );

/* Called in read_from_descriptor() — processes IAC sequences from client */
int     protocol_input        ( DESCRIPTOR_DATA *d, char *src, int srclen,
                                char *dst, int dstmax );

/* Send MSDP variable/value pair to a descriptor */
void    msdp_send_pair        ( DESCRIPTOR_DATA *d, const char *var,
                                const char *val );

/* Update all MSDP variables for a character — call from update loop */
void    msdp_update           ( CHAR_DATA *ch );

/* Send MSSP response to a descriptor (called when client negotiates MSSP) */
void    mssp_send             ( DESCRIPTOR_DATA *d );

/* Wrap a string with MXP tags — returns static buffer, not re-entrant */
const char *mxp_tag           ( const char *tag, const char *text );

/* Returns a printable string with the client's terminal type */
const char *protocol_terminal_type ( DESCRIPTOR_DATA *d );

/* MSSP helper — call once at startup with mud start time */
void        MSSPSetPlayers   ( int n );

/* Integer to string — simple static buffer utility */
const char *itoa_static      ( int n );

#endif /* PROTOCOL_H */
