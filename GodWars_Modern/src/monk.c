/***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/
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
/*
#include "player.h"
*/


void do_guide( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int ch_age = years_old(ch);
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_MONK(ch) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Guide whom?\n\r", ch );
	return;
    }

    if ( ch_age < 50 )
    {
       send_to_char("They must be 50 to become a monk!\n\r", ch);
       return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortals's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot guide yourself.\n\r", ch );
	return;
    }

    if (IS_MONK(victim))
    {
	send_to_char( "They are already guided.\n\r", ch );
	return;
    }


    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) ) 
    {
	send_to_char( "You can only guide avatars.\n\r", ch );
	return;
    }

    if ( victim->class != 0 )
    {
      send_to_char("They are already classed.\n\r",ch);
      return;
    }


    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot guide an unwilling person.\n\r", ch );
	return;
    }

    if (ch->exp < 50000)
    {
	send_to_char("You cannot afford the 50000 exp required to guide them.\n\r",ch);
	return;
    }


    if (victim->exp < 50000)
    {
	send_to_char("They cannot afford the 50000 exp required to be guided from you.\n\r",ch);
	return;
    }

    ch->exp -= 50000;
    victim->exp -= 50000;

    act("You guide $N in the ways of god.", ch, NULL, victim, TO_CHAR);
    act("$n guide $N in the ways of god.", ch, NULL, victim, TO_NOTVICT);
    act("$n guide you in the way of god.", ch, NULL, victim, TO_VICT);
    send_to_char( "You are now a monk.\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->class    = CLASS_MONK;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}






void do_flaminghands( CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
     return;

    if (!IS_MONK(ch))
    {
	  send_to_char("Huh?\n\r",ch);
	  return;
    }

    if (ch->pcdata->powers[PMONK] <5)
    {
	  send_to_char("You do not have the mantra of Flaming Hands.\n\r",ch);
	  return;
    }

    if (IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	  REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
        send_to_char("Your hands are no longer engulfed by flames.\n\r",ch);
	  act("$n's hands are no longer engulfed by flames.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
    if (!IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	  SET_BIT(ch->newbits, NEW_MONKFLAME);
	  send_to_char("Your hands are engulfed by flames!\n\r",ch);
	  act("$n's hands are engulfed by flames!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
}

void do_adamantium( CHAR_DATA *ch, char *argument)
{
     if ( IS_NPC(ch) )
     return;

    if (!IS_MONK(ch))
    {
	  send_to_char("Huh?\n\r",ch);
	  return;
    }

    if (ch->monkab[BODY] < 1)
    {
	  send_to_char("You have not learned this ability yet.\n\r",ch);
	  return;
    }

    if (IS_SET(ch->newbits, NEW_MONKADAM))
    {
	  REMOVE_BIT(ch->newbits, NEW_MONKADAM);
        send_to_char("Your hands resume their normal toughness.\n\r",ch);
	  act("$n's hands resume their normal toughness.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }

    if (!IS_SET(ch->newbits, NEW_MONKADAM))
    {
	  SET_BIT(ch->newbits, NEW_MONKADAM);
	  send_to_char("Your hands turn as hard as adamantium!\n\r",ch);
	  act("$n's hands turn as hard as adamantium!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
}



void do_celestial( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if (!IS_MONK(ch))
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if (ch->monkab[SPIRIT] < 4)
	{
	   send_to_char("You must obtain level 4 in Spirit to use Celestial Path.\n\r",ch);
	   return;
	}

    if (ch->focus[CURRENT] < 10)
    {
	send_to_char("You require atleast 10 focus to use this power!\n\r", ch);
        return;
    }
    if (ch->move < 250)
	{
	    send_to_char("You do not have enough vitality to do that!\n\r",ch);
	    return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open a celestial path to who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
	if  (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
	{
	send_to_char("There summon is off!\n\r",ch);
	return;
}
   
 if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }

    if (victim->in_room->vnum == ch->in_room->vnum)
    {
	  send_to_char("But you're already there!\n\r",ch);
        return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    if (IS_NPC(victim))
    {
    sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->short_descr);
    send_to_char(buf, ch);
    }

    if (!IS_NPC(victim))
    {
    sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->name);
    send_to_char(buf, ch);
    }
    act("$n appears before you through a Celestial portal!\n\r",ch,NULL,NULL,TO_ROOM);
    do_look(ch, "auto");
    ch->focus[CURRENT] -= 10;
    ch->move = ch->move - 250;
    return;
}

void do_godsheal( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch))
	return;
    if (!IS_MONK(ch))
        {
         send_to_char("Huh?\n\r",ch);
           return;
        }


     if ( IS_MONK(ch) && ch->pcdata->powers[PMONK] < 12 )

    {
	send_to_char("You need to obtain heal to use heal.\n\r",ch);
	return;
    }

    if (ch->mana < 300)
    {
	send_to_char("You don't have enough energy to use heal.\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char("You are surrounded in a glowing halo of energy.\n\r",ch);
	act("$n is surrounded by a glowing halo, and looks very relaxed.\n\r",ch,NULL,NULL,TO_ROOM);
        ch->hit += 150;
        ch->mana -= 400;
	WAIT_STATE(ch,12);
	return;
    }

    if (ch->fighting == NULL)
    {
	send_to_char("You feel a mighty force lay his hands on you.\n\r",ch);
	act("$n is surrounded by a glowing halo, and looks very relaxed.\n\r",ch,NULL,NULL,TO_ROOM);
	ch->hit += 500;
	ch->mana -= 400;
	WAIT_STATE(ch,8);
	return;
    }
  }


