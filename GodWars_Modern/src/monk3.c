 /***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
/*#include "player.h"*/
#include "monk.h"

void do_blinding( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_MONK(ch))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if ( ch->monkab[SPIRIT] < 1 )
  {
    send_to_char("Your spirit is far too weak.\n\r",ch);
    send_to_char("You need a level one mastery of the Spirit to use Blinding Agony.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    send_to_char("Who do you wish to use Blinding Agony on?\n\r",ch);
    return;
  }

  if ( ch->move < 200 )
  {
    send_to_char("You are far too exhausted to use Blinding Agony.\n\r",ch);
    send_to_char("Try getting 200 movement points.\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char("They are not here.\n\r",ch);
    return;
  }

  if ( is_safe(ch,victim) ) return;

  if ( IS_SET(victim->flag2, AFF_TOTALBLIND) )
  {
    act("You hurl a ball of blinding light at $N.",ch,NULL,victim,TO_CHAR);
    act("$n hurls a ball of blinding light at you.",ch,NULL,victim,TO_VICT); 
    act("$n hurls a ball of blinding light at $N.",ch,NULL,victim,TO_ROOM);
    send_to_char("Nothing happens.\n\r",ch);
    ch->move -= 100;
    return;
  }

  else
  {
    act("You hurl a ball of blinding light at $N.",ch,NULL,victim,TO_CHAR);
    act("$n hurls a ball of blinding light at you.",ch,NULL,victim,TO_VICT);
    act("$n hurls a ball of blinding light at $N.",ch,NULL,victim,TO_ROOM);
    SET_BIT(victim->flag2, AFF_TOTALBLIND);
    ch->move -= 200;
    send_to_char("The searing heat of the ball blinds you.\n\r",victim);
    return;
  }

 return;

}
