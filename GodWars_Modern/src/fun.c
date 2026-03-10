#include <stdio.h>
#include <time.h>
#include "merc.h"


void do_fakekill( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char arg1 [MAX_INPUT_LENGTH];
   char buf [MAX_STRING_LENGTH];
   char *chsex;
   int fakemessage;

   argument = one_argument( argument, arg1 );
   fakemessage = number_range(1,4);
   chsex = ch->sex == SEX_FEMALE ? "her" : "his";
    if (arg1[0] == '\0')
    {
        send_to_char( "Syntax: FakeKill <target>\n\r", ch );
        return;
    }
    if (( victim = get_char_world( ch, arg1 ) ) == NULL)
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }
    if( fakemessage == 1 )
    {
        sprintf(buf,"#r%s#n has been decapitated by #r%s#n.",victim->name, ch->name);
        do_fatality(ch,buf);
        return;
    }
    if( fakemessage == 2 )
    {
        sprintf(buf,"#r%s#n has smashed in #r%s#n's chest!",ch->name,victim->name);
        do_fatality(ch,buf);
        return;
    }
    if( fakemessage == 3 )
    {
        sprintf(buf,"#r%s#n has shattered #r%s#n's spine!",ch->name,victim->name );
        do_fatality(ch,buf);
        return;
    }
    if( fakemessage == 4 )
    {
        sprintf(buf,"#r%s#n has swung %s weapon between #r%s#n's legs!", ch->name, chsex, victim->name);
        do_fatality(ch,buf);
        return;
    }
}

