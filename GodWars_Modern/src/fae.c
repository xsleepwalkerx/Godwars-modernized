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

//operating in fight.c:
//Each point of elemental defence confers 2% damage resistance
//Earthshield confers an additional 10%

//operating in clan.c : 
//2% MR per point of Elemental Defence
//Earthshield gives another 10%

//channel: windwhisper
//have access to scry and readaura

//base code from dragon provided by Aspertame
//modifications and additions by Kaelas

void do_elementalIncrease( CHAR_DATA *ch, char *argument )
{ 
  
    char arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_FAE(ch))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    } 

    if (arg[0] == '\0')
    {
        send_to_char("Increase Attack or Defense?\n\r",ch);
        return;
    }
    if (!str_cmp(arg,"Attack"))
    {
        if (ch->pcdata->ELEMENTAL_ATTACK > 9)
        {
            send_to_char("You cannot increase it any more.\n\r",ch);
            return;
        }
    if (ch->exp < 100000 * (ch->pcdata->ELEMENTAL_ATTACK + 1)) 
    {
        send_to_char("Cost is 100 thousand exp per level.\n\r",ch);
        return;
    }
    ch->pcdata->ELEMENTAL_ATTACK += 1;
    ch->exp -= 100000 * ch->pcdata->ELEMENTAL_ATTACK;
    send_to_char("Ok.\n\r",ch);
    }

    else if (!str_cmp(arg,"Defence"))
    {
        if (ch->pcdata->ELEMENTAL_DEFENCE > 9)
        {
            send_to_char("You cannot increase it any more.\n\r",ch);
            return;
        }
        if (ch->exp < 100000 * (ch->pcdata->ELEMENTAL_DEFENCE + 1))
        {
            send_to_char("Cost is 100 thousand exp per level.\n\r",ch);
            return;
        }
    ch->pcdata->ELEMENTAL_DEFENCE += 1;
    ch->exp -= 100000 * ch->pcdata->ELEMENTAL_DEFENCE;
    send_to_char("Ok.\n\r",ch);
    }
    else send_to_char("Enter a valid choice please.\n\r",ch);
    return;
}

void do_bloodpact( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );

    if (!IS_FAE(ch))
    {
        send_to_char("You want to slit your palm and put your blood in them? Why?\n\r",ch);
        return;
    }

    if ((ch->pcdata->ELEMENTAL_DEFENCE < 9) || (ch->pcdata->ELEMENTAL_ATTACK < 9))
    {
        send_to_char( "You are not powerful enough for form a bloodpact.\n\r", ch);
        return;
    }
 
    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
        send_to_char( "Form a bloodpact with whom? whom?\n\r", ch );
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
        send_to_char( "You cannot use a bloodpact on yourself.\n\r", ch );
        return;
    }
    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
        send_to_char( "You can only teach avatars.\n\r", ch );
        return;
    }
    if ( IS_SET(victim->act,WAS_STAKED))
    {
        send_to_char( "The taint of a vampire does not go well with fae blood. Best find someone 
else.\n\r", ch );
        return;
    }
    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {   
        send_to_char( "You cannot share blood with an unwilling person.\n\r", ch );
        return;
    }
    
    if (ch->exp < 100000)
    {
        send_to_char("You cannot afford the 100000 exp required to form a bloodpact.\n\r",ch);
        return;
    }
    if (victim->exp <10000000)
    {
        send_to_char("They cannot afford the 10 million exp required for a bloodpact.\n\r",ch);
        return;
    }
        
    ch->exp -= 100000;
    victim->exp -= 10000000;
    
    act("You form a bloodpact with $N, turning them to fae.", ch, NULL, victim, TO_CHAR);
    act("$n forms a bloodpact with $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n forms a bloodpact with you, $e blood in your veins.", ch, NULL, victim, TO_VICT);
    victim->class    = CLASS_FAE;
    send_to_char( "You are now one of the fae.\n\r", victim );
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_trueform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_FAE(ch))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->ELEMENTAL_ATTACK < 1)
	{
        send_to_char("You need more elemental power to change to your true form.\n\r",ch);
        return;
    }

    if (IS_POLYAFF(ch, POLY_ZULO))
    {
        REMOVE_BIT(ch->polyaff, POLY_ZULO);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        REMOVE_BIT(ch->vampaff, VAM_NIGHTSIGHT); 
    	REMOVE_BIT(ch->pcdata->bodymods, MOD_UNFOLDED);
	REMOVE_BIT(ch->pcdata->bodymods, MOD_WINGS);
        REMOVE_BIT(ch->vampaff, VAM_CLAWS);
        REMOVE_BIT(ch->vampaff, VAM_FANGS);
        REMOVE_BIT(ch->pcdata->bodymods, MOD_HORNS);
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        act( "You form the glamour of humankind.", ch, NULL, NULL, TO_CHAR );
        act( "Your fangs slide back into your gums.", ch, NULL, NULL, TO_CHAR);
        act( "Your claws slide back into your fingers.", ch, NULL, NULL, TO_CHAR);
        act( "Your wings slide into your back.", ch, NULL, NULL, TO_CHAR);
        act( "Your horns fade back into your skin.", ch, NULL, NULL, TO_CHAR);
        act( "The red glow in your eyes fades.", ch, NULL, NULL, TO_CHAR);
        act( "$n shifts and fades, appearing human.", ch, NULL, NULL, TO_ROOM );
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 150;
        ch->hitroll = ch->hitroll - 150;
        ch->pcdata->mod_str = 0;
        ch->pcdata->mod_dex = 0;
        ch->pcdata->mod_con = 0;
        ch->armor   += 1000; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
        send_to_char("You can't release the glamour when changed.\n\r",ch);
	return;
    }

    if (ch->stance[0] != -1) do_stance(ch,"");
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    act( "You release the glamour.", ch, NULL, NULL, TO_CHAR );
    act( "Sharp fangs extend from your gums.", ch, NULL, NULL, TO_CHAR);
    act( "Sharp claws extend from your fingers.", ch, NULL, NULL, TO_CHAR);
    act( "Large translucent wings unfold from your back.", ch, NULL, NULL, TO_CHAR);
    act( "Glittering transparent horns curl around your head.", ch, NULL, NULL, TO_CHAR);
    act( "Your eyes glow red.", ch, NULL, NULL, TO_CHAR);
    act( "$n changes, becoming fae.", ch, NULL, NULL, TO_ROOM);
    ch->pcdata->mod_str = 5;
    ch->pcdata->mod_dex = 5;
    ch->pcdata->mod_con = 5;
    SET_BIT(ch->polyaff, POLY_ZULO);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->bodymods, MOD_WINGS);
    SET_BIT(ch->pcdata->bodymods, MOD_UNFOLDED);
    SET_BIT(ch->act, PLR_HOLYLIGHT);
    SET_BIT(ch->vampaff, VAM_NIGHTSIGHT);
    SET_BIT(ch->vampaff, VAM_CLAWS);
    SET_BIT(ch->vampaff, VAM_FANGS);
    SET_BIT(ch->pcdata->bodymods, MOD_HORNS);
    sprintf(buf, "%s", ch->name);
    free_string( ch->morph );
    ch->morph = str_dup( buf );
    ch->damroll = ch->damroll + 150;
    ch->hitroll = ch->hitroll + 150;
    ch->armor   -= 1000;
    return;
}

void do_wingslice( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
        char arg[MAX_INPUT_LENGTH];
	int stance;
	int max_dam;
	int dam;

    one_argument (argument, arg);

    if (IS_NPC(ch)) return;

    if (!IS_FAE(ch))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->ELEMENTAL_ATTACK < 2)
    {
        send_to_char("Your wings aren't sharp enough yet.\n\r",ch);
        return;
    }

    if (arg[0] == '\0' && ch->fighting == NULL)
    {
        send_to_char("Who do you wish to attack?\n\r",ch);
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
        {
            send_to_char( "They aren't here.\n\r", ch );
            return;
        }
    }

    if (IS_ITEMAFF(ch, ITEMA_PEACE)) do_majesty(ch,"");
    
    if (is_safe(ch,victim))
    {
        if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
        {
            guardian_message( victim );
            act("$n just failed to attack with their wings.",ch,NULL,victim,TO_VICT);
        }
        return;
    }

    dam = ch->pcdata->ELEMENTAL_ATTACK * 150;
    if (is_safe(ch,victim)== TRUE) return;
    dam += ch->damroll;
    stance = ch->stance[0];
    if ( IS_STANCE(ch, STANCE_NORMAL) ) dam *= 1.25;
    else dam = dambonus(ch,victim,dam,stance);

    max_dam = ch->damcap[DAM_CAP];
    if (dam > max_dam) dam = max_dam;
    if (dam < 0) dam = 0;

    if (IS_AFFECTED(victim, AFF_SANCTUARY)) dam *= 0.5;

    if(!(ch->fighting))    set_fighting(ch,victim);
    sprintf(buf,"Your wings slice into $N incredibly hard! [#g%d#n]",dam);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n's wings slice into you incredibly hard! [#r%d#n]",dam);
    act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"$n's wings slice into $N incredibly hard!");
    act(buf,ch,NULL,victim,TO_NOTVICT);
    hurt_person(ch,victim, dam, FALSE);
    WAIT_STATE( ch, 5 );
    return;
}

void do_faestorm (CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int dam = 0;

    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );
                                              

    if (IS_NPC(ch)) return;

    if(!IS_FAE(ch))
    {
        send_to_char("What?\n\r",ch);
        return;
    }

    if (ch->pcdata->ELEMENTAL_ATTACK < 3 )
    {
        send_to_char("You can't weild a faestorm yet.\n\r",ch);
        return;
    }

    if (!IS_POLYAFF(ch, POLY_ZULO))
    {
        send_to_char("You cannot weild the elements like that under the glamour.\n\r",ch);
        return;
    }


    if ( arg1[0] == '\0')
    {
        stc( "Syntax is faestorm <fire/ice/lightning>.\n\r", ch);
        return;
    }
  
    /*if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
         send_to_char( "They are not here.\n\r", ch );
         return;
    }*/

    /*if ( ch == victim )
    {
        send_to_char( "Suicide annoys the mother. Don't do it.\n\r", ch );
        return;
    }*/

    if (ch->position != POS_FIGHTING)
    {
	send_to_char("You need the power of battle to release the faestorm.\r\n", ch);
	return;
    }

    victim = ch->fighting;

    if (!str_cmp(arg1,"fire"))
    {
        dam = ch->damroll;
        dam *= ch->pcdata->ELEMENTAL_ATTACK/3;
        if(!(IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) || (number_range(1,5) == 1))
        {
            dam *=2;
            SET_BIT(victim->affected_by, AFF_FLAMING);
	    act("$n's faestorm ignites you!",ch,NULL, victim, TO_VICT);
	    act("The faestorm ignites $N!", ch,NULL, victim, TO_NOTVICT);
	    act("Your faestorm ignites $N!", ch,NULL,victim, TO_CHAR);
        }
        if (dam > 2000) dam = 2000;
	sprintf(buf,"Your faestorm consumes $N! [#g%d#n]",dam);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n's faestorm consumes you! [#r%d#n]",dam);
        act(buf,ch,NULL,victim,TO_VICT);
        sprintf(buf,"$n's faestorm consumes $N!");
        act(buf,ch,NULL,victim,TO_NOTVICT);
        hurt_person(ch, victim, dam, TRUE);
        WAIT_STATE(ch, 10);
        return;
    }

    else if(!str_cmp(arg1,"ice"))
    {
        if (ch->pcdata->ELEMENTAL_ATTACK < 4 )
        {
            send_to_char("You can't weild an icy faestorm yet.\n\r",ch);
            return;
        }
        dam = ch->damroll;
        dam *= ch->pcdata->ELEMENTAL_ATTACK/3;
        if(!(IS_ITEMAFF(victim, ITEMA_ICESHIELD)) || number_range(1,3) == 1)
        {
            dam *=2;
       	    WAIT_STATE(victim, 20);
            act("$n's faestorm freezes you!",ch,NULL, victim, TO_VICT);
            act("The faestorm freezes $N!", ch,NULL, victim, TO_NOTVICT);
	    act("Your faestorm freezes $N!", ch,NULL, victim, TO_CHAR);
        }
        if (dam > 2000) dam = 2000;
	sprintf(buf,"Your faestorm consumes $N! [#g%d#n]",dam);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n's faestorm consumes you! [#r%d#n]",dam);
        act(buf,ch,NULL,victim,TO_VICT);
        sprintf(buf,"$n's faestorm consumes $N!");
        act(buf,ch,NULL,victim,TO_NOTVICT);
        hurt_person(ch, victim, dam, TRUE);
        WAIT_STATE(ch, 15);
        return;
    }

    else if(!str_cmp(arg1,"lightning"))
    {
        if (ch->pcdata->ELEMENTAL_ATTACK < 5 )
        {
            send_to_char("You can't weild a faestorm of lightning yet.\n\r",ch);
            return;
        }
        dam = ch->damroll;
        dam *= ch->pcdata->ELEMENTAL_ATTACK/3;
        if(!(IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD)) || number_range(1,4)==1)
        {
            dam *=3;
        }
        if (dam > 3000) dam = 3000;
	sprintf(buf,"Your faestorm consumes $N! [#g%d#n]",dam);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n's faestorm consumes you! [#r%d#n]",dam);
        act(buf,ch,NULL,victim,TO_VICT);
        sprintf(buf,"$n's faestorm consumes $N!");
        act(buf,ch,NULL,victim,TO_NOTVICT);
        hurt_person(ch, victim, dam, TRUE);
        WAIT_STATE(ch, 10);
        return;
    }
}

void do_soar (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg1[MAX_INPUT_LENGTH];   
  char arg2[MAX_INPUT_LENGTH];
         
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
          
    if ( IS_NPC(ch) ) return;
        
    if ( !IS_FAE(ch) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
 
    if (ch->pcdata->ELEMENTAL_ATTACK < 7)
    {
        send_to_char("Your wings are not yet strong enough to carry you.\n\r", ch);
        return;
    }

    if (!IS_POLYAFF(ch, POLY_ZULO))
    {
        send_to_char("You cannot be under the glamour to use this.\n\r",ch);
        return;
    }

    if (ch->fight_timer > 0)
    {
        send_to_char("You need to recover from your fight before you can fly. Your wings hurt.\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char("Whom do you wish to fly to?\n\r",ch);
        return;
    }
 
    if (IS_SET(ch->tag_flags,TAG_PLAYING))
    {
        send_to_char("Not while playing freezetag!\n\r",ch);
        return;
    }
  
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char("You scry the foreign planes, but can find no such entity.\n\r",ch);
        return;
    }
  
    if ( ch == victim)
    {
        send_to_char("Why you wanna fly to your self?,\n\r", ch);
        return;
    }

    if ( victim->in_room == ch->in_room )
    {
        send_to_char( "But you are already there!\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) && victim->level > 500 )
    {
        send_to_char("You failed.\n\r", ch);
        return;
    }
     
    if ( IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
      ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
      ||   IS_SET(victim->extra, EXTRA_ILLUSION)
      ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) ) )
    {
        send_to_char( "You are unable to focus on their location.\n\r", ch );
        if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
        {  
            guardian_message( victim );
            act("$n just failed to use Summoning on you.",ch,NULL,victim,TO_VICT);
        }
        return;
    }
    
    act("$n untucks $s wings,  and leaps into the air.",ch,NULL,victim,TO_ROOM);
    act("You leap into the air.",ch,NULL,victim,TO_CHAR);
    char_from_room(ch);
    char_to_room( ch, victim->in_room );
    do_look(ch,"");
    act("You land gracefully.",ch,NULL,victim,TO_CHAR);
    act("$n drops from the sky into a soft crouch.",ch,NULL,victim,TO_ROOM);
   
    
    return;  
}
  
void do_wail( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int dam;
    argument = one_argument( argument, arg );

    if (!IS_FAE(ch))
    {
        send_to_char("You cry like a baby. Feel better?\n\r",ch);
        act("$n cries like a little baby who dropped $s wowwypop\n\r",ch,NULL,NULL,TO_ROOM);
	return;
    }

    if (ch->pcdata->ELEMENTAL_ATTACK < 8)
    {
        send_to_char("You are not skilled enough with the elements to use the wail.\r\n",ch);
	return;
    }

    if( arg[0] == '\0' )
    {
        send_to_char("Who do you wish to use the wail upon?\r\n", ch);
        return;
    }

    if( (victim = get_char_room(ch, arg)) == NULL )
    {
        send_to_char("How can you use the wail upon someone who is not here?\r\n", ch);
        return;
    }

    if (IS_ITEMAFF(ch, ITEMA_PEACE)) do_majesty(ch,"");
    
    if (is_safe(ch,victim))
    {
        if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
        {
            guardian_message( victim );
            act("$n's wail failed upon you.",ch,NULL,victim,TO_VICT);
        }
        return;
    }

    if(number_range(1,10) == 1)
    {
        if((IS_NINJA(victim) && victim->pcdata->wolf > 0))
        {
            victim->pcdata->wolf = 0;
            act("$n's wail drives you from the path of michi!\n\r",ch,NULL,victim,TO_VICT);
	    act("Your wail drives $N from michi!\n\r",ch,NULL,victim,TO_CHAR);
        }

        if((IS_VAMPIRE(victim)) && (IS_POLYAFF(ch,POLY_ZULO)))
        {
            do_zuloform(victim, "");
            act("$n's wail forces you back to your human form!\n\r",ch,NULL,victim,TO_VICT);
	    act("Your wail drives $N to a human form!\n\r",ch,NULL,victim,TO_CHAR);
        }

        if((IS_WEREWOLF(victim)) && (IS_SET(victim->act, PLR_WOLFMAN)))
        {
            do_unwerewolf(victim,"");
            act("$n's wail drives the wolf from your mind!\n\r",ch,NULL,victim,TO_VICT);
	    act("Your wail drives out $N's wolf!\n\r",ch,NULL,victim,TO_CHAR);
        }

        if((IS_ANGEL(victim)) && (IS_POLYAFF(ch,POLY_ZULO)))
        {
            do_angelform(victim, "");
            act("$n's wail drives you back to your human form!\n\r",ch,NULL,victim,TO_VICT);
	    act("Your wail drives $N back to a human form!\n\r",ch,NULL,victim,TO_CHAR);
        }

        if((IS_DEMON(victim)) && (IS_POLYAFF(ch,POLY_ZULO)))
        {
            do_demonform(victim, "");
            act("$n's wail drives you back to your human form!\n\r",ch,NULL,victim,TO_VICT);
	    act("Your wail drives $N back to a human form!\n\r",ch,NULL,victim,TO_CHAR);
        }
    }
    dam = char_damroll(ch);
    if(!(IS_AFFECTED(victim, AFF_SANCTUARY)))
        dam *=2;
    sprintf(buf,"$n's wail tears through your mind! [#r%d#n]\n\r",dam);
    act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"Your wail tears through the mind of $N! [#g%d#n]\n\r",dam);
    act(buf,ch,NULL,victim,TO_CHAR);
    do_kill(ch, victim->name);
    hurt_person(ch, victim, dam, TRUE);
 
   return;
}

void do_earthshield( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if(!(IS_FAE(ch)))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    if (ch->pcdata->ELEMENTAL_DEFENCE < 1)
    {
        send_to_char("You don't possess the ability to wrap yourself in earth magic.\n\r", ch);
        return;
    }
    if ( IS_SET(ch->newbits, NEW_MONKSKIN) )
    {
        REMOVE_BIT( ch->newbits, NEW_MONKSKIN );
        send_to_char( "The earth magic returns to the mother.\n\r", ch );
        ch->pcdata->atm = 0;
    }
    else
    {
        send_to_char( "You wrap yourself in the protective embrace of earth magic!\n\r", ch );
        SET_BIT(ch->newbits, NEW_MONKSKIN);
	ch->pcdata->atm = 10 + 2 * ch->pcdata->ELEMENTAL_DEFENCE;
    }
    return;
}

void do_faeequipment( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
 
    if (!IS_FAE(ch))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Please specify which piece of fae equipment you wish to make:\n\r Crown, Cape, Talisman, Pendant, or Rapier.",ch);
        return;
    }
    
    if ( ch->practice < 100  || ch->pcdata->quest < 400)
    {
        send_to_char("It costs 100 points of primal and 400 quest points to create fae equipment.\n\r",ch);
        return;
    }
    if      (!str_cmp(arg,"crown"    )) vnum = 29718;
    else if (!str_cmp(arg,"cloak"   )) vnum = 29719;
    else if (!str_cmp(arg,"talisman"       )) vnum = 29720;
    else if (!str_cmp(arg,"pendant"     )) vnum = 29721;
    else if (!str_cmp(arg,"rapier"	    )) vnum = 29722;
    else
    {
        send_to_char("Please specify which piece of fae equipment you wish to make:\n\r Crown, Cape, Talisman, Pendant, or Rapier.",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform an IMP.\n\r",ch);
        return;
    }
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    ch->practice -= 100;
    ch->pcdata->quest -= 400;obj->points += 400;
     
    free_string( obj->questmaker );
    obj->questmaker = str_dup( ch->name );
     
    free_string( obj->questowner );
    obj->questowner = str_dup( ch->name );
     
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}
