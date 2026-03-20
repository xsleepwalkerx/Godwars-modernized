/*
 * pretitle.c — Player pretitle command.
 * Sets a short title prefix displayed before the player's name.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

void do_pretitle( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int  value;

    if ( IS_NPC(ch) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if ( ch->pcdata->pretit == NULL || ch->pcdata->pretit[0] == '\0' )
        ch->pcdata->pretit = str_dup( "" );

    if ( argument[0] == '\0' )
    {
        sprintf( buf, "Your current pretitle is '%s'.\n\r", ch->pcdata->pretit );
        send_to_char( buf, ch );
        return;
    }

    if ( strlen(argument) > 45 )
    {
        argument[45] = '\0';
    }
    else
    {
        value = strlen(argument);
        argument[value] = '\0';
    }

    free_string( ch->pcdata->pretit );
    ch->pcdata->pretit = str_dup( argument );
    send_to_char( "Done.\n\r", ch );
    return;
}
