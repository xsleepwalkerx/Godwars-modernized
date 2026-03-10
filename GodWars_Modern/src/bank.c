/*
 * bank.c
 *
 * deals with the bank.
 *
 * (C) September 14, 1996 Robert L. Peckham  (Rip)
 */


#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include "merc.h"


void do_balance( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) )
	return;

    if ( ch->bank <= 0 )
    {
	send_to_char("You ain't got no gold.\n\r", ch );
	return;
    }

    sprintf( buf, "You have %d coins in your account.\n\r", ch->bank );
    send_to_char( buf, ch );

    return;
}


void do_deposit( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    if ( IS_NPC( ch ) )
	return;
    if (ch->in_room != NULL && ch->in_room->vnum != ROOM_VNUM_BANK)
    {
	send_to_char("You aren't in a bank.\n\r", ch );
	return;
    }

    if ( !is_number( argument ) )
    {
	send_to_char("Deposit how many coins?\n\r", ch );
	return;
    }

    i = atoi( argument );

    if ( i <= 0 )
    {
	send_to_char("Your point is?\n\r", ch );
	return;
    }

    if ( i > ch->gold )
    {
	send_to_char("We don't take wooden nickels.\n\r", ch );
	return;
    }

    sprintf( buf, "You hand %d gold to the teller.\n\r", i );
    send_to_char( buf, ch );
    ch->gold -= i;
    ch->bank += i;

    return;
}

void do_withdraw( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    if ( IS_NPC( ch ) )
	return;

    if (ch->in_room != NULL && ch->in_room->vnum != ROOM_VNUM_BANK)
    {
	send_to_char("You aren't in a bank.\n\r", ch );
	return;
    }

    if ( !is_number( argument ) )
    {
	send_to_char("Withdraw how many coins?\n\r", ch );
	return;
    }

    i = atoi( argument );

    if ( i <= 0 )
    {
	send_to_char("Your point is?\n\r", ch );
	return;
    }

    if ( i > ch->bank )
    {
	send_to_char("What, you think we give away money?\n\r", ch );
	return;
    }

    sprintf( buf, "You get %d gold coins from the teller.\n\r", i );
    send_to_char( buf, ch );
    ch->gold += i;
    ch->bank -= i;

    return;
}


