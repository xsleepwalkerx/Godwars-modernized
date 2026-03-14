/***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include "merc.h"

void mage_damage	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type ) );
void mage_message	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type ) );
int  mage_immunity	args( ( CHAR_DATA *ch, int dam, const char *msg, int type ) );
void update_damcap	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void check_killer	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* sphere_name defined in mage.c */

bool is_memb( CHAR_DATA *ch )
{
  int test = number_percent();

  if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
  {
    stc("You struggle to move, but the earth surrounding you won't budge.\n\r",ch);
    return TRUE;
  }

  else if ( IS_SET(ch->mflags, MAGE_EMBRACED1) && test > 30 )
  {
    stc("You struggle to move, but the earth surrounding you won't budge.\n\r",ch);
    return TRUE;
  }

  else if ( IS_SET(ch->mflags, MAGE_EMBRACED1) && test <= 30 )
  {
    stc("You manage to break free of the earth surrounding you.\n\r",ch);
    REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
    return FALSE;
  }

  return FALSE;
}

bool is_mage( CHAR_DATA *ch )
{
  if ( !IS_MAGE(ch) )
  {
    stc("Huh?\n\r",ch);
    return FALSE;
  }

  return TRUE;
}    

void creategate( CHAR_DATA *ch, int inroom, int toroom, bool perm )
{
  OBJ_DATA *in_gate;
  OBJ_DATA *to_gate;
  ROOM_INDEX_DATA *pIn;
  ROOM_INDEX_DATA *pTo;

  pIn = get_room_index( inroom );
  pTo = get_room_index( toroom );

  in_gate = create_object( get_obj_index( OBJ_VNUM_WGATE ), 0 );
  obj_to_room( in_gate, pIn );
  in_gate->value[0] = toroom;
  in_gate->value[1] = MAGE_INROOM_GATE;
  in_gate->value[3] = inroom;
  if ( !perm )
  in_gate->timer = ch->spheres[MCOR];
  in_gate->item_type = ITEM_WGATE;

  to_gate = create_object( get_obj_index( OBJ_VNUM_WGATE ), 0 );
  obj_to_room( to_gate, pTo );
  to_gate->value[0] = inroom;
  to_gate->value[1] = MAGE_TOROOM_GATE;
  to_gate->value[3] = toroom;
  if ( !perm )
  to_gate->timer = ch->spheres[MCOR];
  to_gate->item_type = ITEM_WGATE;

  return;
}

void do_perception( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MCOR] < 1 )
  {
    stc("You must obtain level one Correspondence to enable your higher Perception.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MAGE_PERCEPTION) )
  {
    stc("Your higher perception fades.\n\r",ch);
    REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
    REMOVE_BIT(ch->vampaff, VAM_NIGHTSIGHT);
    REMOVE_BIT(ch->mflags, MAGE_PERCEPTION);
    return;
  }

  else
  {
    stc("Your perception soars to new levels.\n\r",ch);
    SET_BIT(ch->mflags, MAGE_PERCEPTION);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    SET_BIT(ch->act, PLR_HOLYLIGHT);
    SET_BIT(ch->vampaff, VAM_NIGHTSIGHT);
    return;
  }
 
  return;
}

// Shield, Scry, Level II Correspondence

void do_objectgate( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  bool perm = FALSE;

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MCOR] < 2 )
  {
    stc("You must obtain level two Correspondence to Open a Spacial Gate.\n\r",ch);
    return;
  }

  if ( ch->spheres[MLIF] < 2 || ch->spheres[MMAT] < 2 )
  {
    stc("This sphere must be used in conjunction with either Life 2 or Matter 2, which you have not obtained.\n\r",ch);
    return;
  }

  if ( arg1[0] == '\0' )
  {
    stc("What object do you want to attempt to gate to?\n\r",ch);
    return;
  }

  if (IS_SET(ch->tag_flags,TAG_PLAYING))
  {
        send_to_char("Not while playing freezetag!\n\r",ch);
        return;
  }

  if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
  {
    stc("You concentrate deeply on foreign planes, but can find no such object.\n\r",ch);
    return;
  }

  if ( !str_cmp(arg2, "grab" ) )
  {
   if ( IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->extra_flags, ITEM_LOYAL)
	|| IS_SET(obj->quest, QUEST_RELIC) )
   {
     act("You reach through a rift in space and try to pull $p through, but a mystical force rips it from your hands.",ch,obj,NULL,TO_CHAR);
     return;
   }
   if (obj->pIndexData->vnum == 30040)
   {
   stc("You reach through a rift in space but the quest-machine is to heavy for you.\n\r", ch );
   return;
   }
   if (
        (obj->pIndexData->vnum >= 2 && obj->pIndexData->vnum <= 59)  ||
/*
        (obj->pIndexData->vnum == 2)     || (obj->pIndexData->vnum == 3)     ||
        (obj->pIndexData->vnum == 4)     || (obj->pIndexData->vnum == 5)     ||
        (obj->pIndexData->vnum == 6)     || (obj->pIndexData->vnum == 7)     ||
*/
        (obj->pIndexData->vnum == 917)   || (obj->pIndexData->vnum == 1311)  ||
	(obj->pIndexData->vnum == 1340)  || (obj->pIndexData->vnum == 1392)  ||
        (obj->pIndexData->vnum == 1516)  || (obj->pIndexData->vnum == 1517)  ||
	(obj->pIndexData->vnum == 1518)  || (obj->pIndexData->vnum == 2299)  ||
        (obj->pIndexData->vnum == 2311)  || (obj->pIndexData->vnum == 2426)  ||
        (obj->pIndexData->vnum == 3132)  || (obj->pIndexData->vnum == 3136)  ||
        (obj->pIndexData->vnum == 3372)  || (obj->pIndexData->vnum == 3418)  ||
	(obj->pIndexData->vnum == 3426)  || (obj->pIndexData->vnum == 4002)  ||
	(obj->pIndexData->vnum == 4705)  || (obj->pIndexData->vnum == 5015)  ||
        (obj->pIndexData->vnum == 6007)  || (obj->pIndexData->vnum == 6308)  ||
	(obj->pIndexData->vnum == 7210)  || (obj->pIndexData->vnum == 7299)  ||
	(obj->pIndexData->vnum == 7909)	 || (obj->pIndexData->vnum == 8109)  ||
        (obj->pIndexData->vnum == 15133) || (obj->pIndexData->vnum == 20100) ||
	(obj->pIndexData->vnum == 26001) || (obj->pIndexData->vnum == 29002) ||
	(obj->pIndexData->vnum == 29003) || (obj->pIndexData->vnum == 29004) || 
	(obj->pIndexData->vnum == 29005) || (obj->pIndexData->vnum == 29006) ||
	(obj->pIndexData->vnum == 29007) || (obj->pIndexData->vnum == 29008) || 
	(obj->pIndexData->vnum == 29009) || (obj->pIndexData->vnum == 29010) ||
	(obj->pIndexData->vnum == 29011) || (obj->pIndexData->vnum == 30043))

   {
   act("You reach through a rift in space, but $p is too heavy.\n\r", ch,obj,NULL,TO_CHAR );
   return;
   }
   if ( obj->carried_by != NULL )
   {
     act("You reach through a rift in space and try to pull $p through, but someone snatches it away from you.",ch,obj,NULL,TO_CHAR);
     return;
   }

   /* egg shell crash fix...I hope -Xentoth */
   if ( obj->in_obj != 0 )
   {
   act("You attempt to create a rift in space to $p, but it is contained.\n\r" ,ch,obj,NULL,TO_CHAR);
   return;
   }
  }
  else
  {
   if ( IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->extra_flags, ITEM_LOYAL)
        || IS_SET(obj->quest, QUEST_RELIC) )
   {
     act("You attempt to create a rift in space to $p, but it is unlocatable.",ch,obj,NULL,TO_CHAR);
     return;
   }
   if (
        (obj->pIndexData->vnum >= 2 && obj->pIndexData->vnum <= 59)  ||
/*
        (obj->pIndexData->vnum == 2)     || (obj->pIndexData->vnum == 3)     ||
        (obj->pIndexData->vnum == 4)     || (obj->pIndexData->vnum == 5)     ||
        (obj->pIndexData->vnum == 6)     || (obj->pIndexData->vnum == 7)     ||
*/
        (obj->pIndexData->vnum == 917)   || (obj->pIndexData->vnum == 1311)  ||
        (obj->pIndexData->vnum == 1340)  || (obj->pIndexData->vnum == 1392)  ||
        (obj->pIndexData->vnum == 1516)  || (obj->pIndexData->vnum == 1517)  ||
        (obj->pIndexData->vnum == 1518)  || (obj->pIndexData->vnum == 2299)  ||
        (obj->pIndexData->vnum == 2311)  || (obj->pIndexData->vnum == 2426)  ||
        (obj->pIndexData->vnum == 3132)  || (obj->pIndexData->vnum == 3136)  ||
        (obj->pIndexData->vnum == 3372)  || (obj->pIndexData->vnum == 3418)  ||
        (obj->pIndexData->vnum == 3426)  || (obj->pIndexData->vnum == 4002)  ||
        (obj->pIndexData->vnum == 4705)  || (obj->pIndexData->vnum == 5015)  ||
        (obj->pIndexData->vnum == 6007)  || (obj->pIndexData->vnum == 6308)  ||
        (obj->pIndexData->vnum == 7210)  || (obj->pIndexData->vnum == 7299)  ||
        (obj->pIndexData->vnum == 7909)  || (obj->pIndexData->vnum == 8109)  ||
        (obj->pIndexData->vnum == 15133) || (obj->pIndexData->vnum == 20100) ||
        (obj->pIndexData->vnum == 26001) || (obj->pIndexData->vnum == 29002) ||
        (obj->pIndexData->vnum == 29003) || (obj->pIndexData->vnum == 29004) ||
        (obj->pIndexData->vnum == 29005) || (obj->pIndexData->vnum == 29006) ||
        (obj->pIndexData->vnum == 29007) || (obj->pIndexData->vnum == 29008) ||
        (obj->pIndexData->vnum == 29009) || (obj->pIndexData->vnum == 29010) ||
        (obj->pIndexData->vnum == 29011) || (obj->pIndexData->vnum == 30043))
     {
     act("You attempt to create a rift in space to $p, but it is unlocatable.",ch,obj,NULL,TO_CHAR);
     return;
     }


   if ( obj->in_obj != 0 )
   {
   act("You attempt to create a rift in space to $p, but it is contained.\n\r" ,ch,obj,NULL,TO_CHAR);
   return;
   }
   
   if ( obj->carried_by != NULL )
   {
     act("You attempt to create a rift in space to $p, but it seems to be possessed by another.",ch,obj,NULL,TO_CHAR);
     return;
   }

   if ((obj->pIndexData->vnum == 30043) || (obj->pIndexData->vnum == 20100) || 
       (obj->pIndexData->vnum == 29002) || (obj->pIndexData->vnum == 29003) || 
       (obj->pIndexData->vnum == 29004) || (obj->pIndexData->vnum == 29005) ||
       (obj->pIndexData->vnum == 29006) || (obj->pIndexData->vnum == 29007) ||
       (obj->pIndexData->vnum == 29008) || (obj->pIndexData->vnum == 29009) ||
       (obj->pIndexData->vnum == 29010) || (obj->pIndexData->vnum == 26001) ||
       (obj->pIndexData->vnum >= 2 && obj->pIndexData->vnum <= 59)  ||
/*
       (obj->pIndexData->vnum == 4)     || (obj->pIndexData->vnum == 5)     ||
       (obj->pIndexData->vnum == 6)     || (obj->pIndexData->vnum == 7)     ||
*/
       (obj->pIndexData->vnum == 29011))
   {
   act("You attempt to create a rift in space to $p, but fail.\n\r", ch,obj,NULL,TO_CHAR );
   return;
   }

  }

  if ( !str_cmp( arg2, "permanent" ) )
  {
    if ( ch->practice < 5 )
    {
      stc("You require five units of Primal energy to support a permanent rift.\n\r",ch);
      return;
    }

    perm = TRUE;
    ch->practice -= 5;
  }


  creategate( ch, ch->in_room->vnum, obj->in_room->vnum, perm );

  if ( !str_cmp(arg2, "grab" ) )
  {
    obj_from_room( obj );
    obj_to_char( obj, ch );
    act("You reach through a rift in space and pull $p through the tear.",ch,obj,NULL,TO_CHAR);
    act("$n's hand plunges through a small tear in space and reappears holding $p.",ch,obj,NULL,TO_ROOM);
  }
  else
  {
    act("You create a rent in the Tapestry to $p.",ch,obj,NULL,TO_CHAR);
    act("$n creates a small rent in reality.",ch,obj,NULL,TO_ROOM);
  }

  return;

}

void do_mgate( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  bool pull = FALSE;
  bool perm = FALSE;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MCOR] < 3 )
  {
    stc("You must obtain a level three mastery of Correspondence to use Astral Gateways.\n\r",ch);
    return;
  }

  if ( arg1[0] == '\0' )
  {
    stc("Whom do you wish to open an Astral Gate to?\n\r",ch);
    return;
  }

  if (IS_SET(ch->tag_flags,TAG_PLAYING))
  {
        send_to_char("Not while playing freezetag!\n\r",ch);
        return;
  }

  if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
  {
    stc("You scry the foreign planes, but can find no such entity.\n\r",ch);
    return;
  }

  if ( ch == victim)
  {
     stc("Why you wanna mgate to your self?,\n\r", ch);
     return;
  }
  if ( !str_cmp(arg2, "pull" ) )
  {
    if ( ch->spheres[MCOR] < 4 )
    {
      stc("You must obtain a level four mastery of Correspondence to pull another being through the Tapestry.\n\r",ch);
      return;
    }    

    if ( victim->level > 100 )
    {
      stc("Your victim is far too large to be pulled through the rent.\n\r",ch);
      return;
    }

    else
      pull = TRUE;
  }

  if ( !str_cmp( arg2, "permanent" ) )
  {
    if ( ch->practice < 5 )
    {
      stc("You require Five units of Primal Energy to create an Eternal Gateway.\n\r",ch);
      return;
    }
  
    perm = TRUE;
    ch->practice -= 5;
  }

  if ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
  {
    act("You attempt to open an astral gate to $N, but the rift quickly closes.",ch,NULL,victim,TO_CHAR);
    act("$n creates a magical rift before you, which quickly disappears.",ch,NULL,victim,TO_VICT);
    act("$n attempts to open an astral gate, but fails.",ch,NULL,victim,TO_ROOM);
    return;
  }
  if (victim->wizard == ch) 
  {  send_to_char("You can't gate to that!", ch);
	return;
  }
  if ((IS_NPC(victim) && victim->level > 500) 
      || (IS_NPC(victim) && (victim->pIndexData->vnum == MOB_VNUM_EYE || victim->pIndexData->vnum == MOB_VNUM_FROG ||
	victim->pIndexData->vnum == MOB_VNUM_RAVEN || victim->pIndexData->vnum == MOB_VNUM_CAT || victim->pIndexData->vnum == MOB_VNUM_DOG)) ) 
  {
    act("You attempt to open an astral gate to $N, but fail.",ch,NULL,victim,TO_CHAR);
    act("$n attempts to open an astral gate, but fails.",ch,NULL,victim,TO_ROOM);
    return;
  }

  creategate( ch, ch->in_room->vnum, victim->in_room->vnum, perm );
  
  if ( !pull )
  {
    act("You create a small rent in the Tapestry to $N.",ch,NULL,victim,TO_CHAR);
    act("A small tear in reality pops into existance in front of you.",ch,NULL,victim,TO_VICT); 
    act("$n creates a small tear in reality.",ch,NULL,victim,TO_ROOM);
  }

  if ( pull )
  {
    act("You create a small rent in the Tapestry and pull $N through it.",ch,NULL,victim,TO_CHAR);
    act("A small tear in reality pops into existance before you.\n\r  You are violently ripped through the tear!",ch,NULL,victim,TO_VICT);
/*    if ( IS_SET(victim->flag2, AFF2_INARENA) )
    {
      stc("Your victim is in the arena.  You are unable to pull them through.\n\r",ch);
      return;
    }
*/    
//    if ( is_inarena(ch) ) return;

    char_from_room(victim);
    char_to_room( victim, ch->in_room );
    do_look(victim,"");
    act("$n creates a small tear in reality.\n\rSeconds later, $N pops out of the rent, looking confused.",ch,NULL,victim,TO_NOTVICT);
  }

  return;
}

void do_polyappear( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MCOR] < 5 )
  {
    stc("You must become a master of the sphere of Correspondence to use Polyappearance.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MAGE_POLYAPPEAR) )
  {
    stc("Your body phases and reforms as one being.\n\r",ch);
    act("$n's many forms phase and reform as one being.",ch,NULL,NULL,TO_ROOM);
    REMOVE_BIT(ch->mflags, MAGE_POLYAPPEAR);
    return;
  }

  else
  {
    stc("Your body phases and splits into multiple figures.\n\r",ch);
    act("$n's body phases and splits into multiple figures.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->mflags, MAGE_POLYAPPEAR);
    return;
  }

  return;
}

void do_discharge( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *vch = NULL;
  CHAR_DATA *vch_next = NULL;
  char arg1[MAX_INPUT_LENGTH];
  int damage;

  argument = one_argument( argument, arg1 );

  if ( IS_NPC(ch) ) return;
  if ( !is_mage(ch) ) return;

  if ( arg1[0] == '\0' )
  {
    stc("Who do you wish to strike down?\n\r",ch);
    return;
  }

  if ( ch->spheres[MFOR] < 1 )
  {
    stc("You must have a level one knowledge of the Forces to discharge Static Electricity.\n\r",ch);
    return;
  }

  if ( str_cmp(arg1, "all" ) )
  {
    if ( ( victim = get_char_room(ch, arg1) ) == NULL )
    {
      stc("You were unable to locate your victim.\n\r",ch);
      return;
    }
   
    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

    WAIT_STATE(ch,16);
    damage = number_range(ch->spheres[MFOR] * 500,ch->spheres[MFOR] * 520);
    if ( weather_info.sky == SKY_LIGHTNING )
      damage += number_range(100,130);
    if ( weather_info.sky == SKY_RAINING )
      damage -= number_range(300,350);

    if ( IS_NPC(victim) )
    {
      mage_damage(ch, victim, damage, "electricity burst", MAGEDAM_ELECTRIC);
    }
    else
    {
      damage /= 2;
      mage_damage(ch, victim, damage, "electricity burst", MAGEDAM_ELECTRIC);
    }
  }
  else
  {
    damage = number_range( ch->spheres[MFOR] * 500, ch->spheres[MCOR] * 600);
    if ( weather_info.sky == SKY_LIGHTNING )
      damage += 50;
   if ( weather_info.sky == SKY_RAINING )
      damage -= 200;

    WAIT_STATE(ch,16);
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
       vch_next = vch->next_in_room;
       if ( vch->level < 3 ) continue;
       
       if ( vch != ch )
         mage_damage( ch, vch, damage, "electricity burst", MAGEDAM_ELECTRIC );
    }
  }

  return;

}

void do_telekinetics( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;
 
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MFOR] < 2 )
  {
    stc("You must obtain a level two knowledge of Forces to use Telekinesis.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MAGE_TELEKINETICS) )
  {
    stc("You relax your mental energies.\n\r",ch);
    REMOVE_BIT(ch->mflags, MAGE_TELEKINETICS);
    return;
  }

  else
  {
    stc("You focus your mental energies into your attacks.\n\r",ch);
    SET_BIT(ch->mflags, MAGE_TELEKINETICS);
    return;
  }

  return;
}

void do_call_lightning( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument(argument, arg);

  if ( IS_NPC(ch) ) return;
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MFOR] < 3 )
  {
    stc("You must obtain a level three knowledge of Forces to Call Lightning.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to Call Lightning upon?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room(ch,arg) ) == NULL )
  {
    stc("You were unable to locate your victim.\n\r",ch);
    return;
  }
 
    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

  if ( weather_info.sky != SKY_LIGHTNING )
  {
    stc("There is no lightning in the sky to call!\n\r",ch);
    return;
  }

  WAIT_STATE(ch, 16);
  dam = number_range( (ch->spheres[MFOR]*250),(ch->spheres[MFOR]*300) );
  if ( !IS_NPC(ch) )
    dam -= number_range( 100,150 );

  mage_damage(ch, victim, dam, "lightning blast", MAGEDAM_ELECTRIC);  
  return;
}

void do_controlweather( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg);

  if ( IS_NPC(ch) ) return;

  if (!IS_MAGE(ch) )
{  
stc("Huh\n\r",ch);
return;
} 
 if ( ch->spheres[MFOR] < 3 )
  {
    stc("You need a level three knowledge of Forces to Control the Weather.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("How do you want to change the weather?\n     Syntax: controlweather <rain,lightning,clear>\n\r",ch);
    return;
  }

  WAIT_STATE(ch, 16);
  if ( !str_cmp(arg, "rain" ) )
  {
    stc("You wave your hands over your head.\n\r",ch);
    stc("The skies blacken and rain begins to fall.\n\r",ch);
    weather_info.sky = SKY_RAINING;
  }
  else if ( !str_cmp(arg, "lightning") )
  {
    stc("You wave your hands over your head.\n\r",ch);
    stc("The skies blacken.\n\r",ch);
    stc("Lightning streaks across the sky.\n\r",ch);
    weather_info.sky = SKY_LIGHTNING;
  }
  else if ( !str_cmp(arg, "clear") )
  {
    stc("You wave your hands over your head.\n\r",ch);
    stc("The skies slowly clear up.\n\r",ch);
    weather_info.sky = SKY_CLOUDLESS;
  }
  else
  {
    stc("Invalid option.\n   Syntax: controlweather <rain,lightning,clear>\n\r",ch);
    return;
  }

  return;
}

void do_earthembrace( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;
 
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MFOR] < 4 )
  {
    stc("You must obtain a level four knowledge of the Forces to use Embrace of the Earth Mother.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to have Gaia Embrace?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room(ch, arg) ) == NULL )
  {
    stc("You were unable to locate your victim.\n\r",ch);
    return;
  }

    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

  act("You call upon the forces of the Earth to embrace $N!",ch,NULL,victim,TO_CHAR);
  act("$n begins to chant mysteriously.",ch,NULL,victim,TO_ROOM);
  act("The ground shakes and rises to embrace $N!",ch,NULL,victim,TO_NOTVICT);
  act("The ground under your feet starts to surround your body.",ch,NULL,victim,TO_VICT);
  SET_BIT(victim->mflags, MAGE_EMBRACED2);
  WAIT_STATE(ch, 16);
  return;
}

void do_tempest( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;
  
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MFOR] < 5 )
  {
    stc("You must master the Forces to use Tempest Blizzard.\n\r",ch);
    return;
  }

  if ( ch->mana < 500 )
  {
    stc("You need 500 mana to use this power.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Who do you wish to summon an Ice Storm upon?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    stc("You scry the planes, but cannot find your victim.\n\r",ch);
    return;
  }

    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

  if ( victim->in_room->area != ch->in_room->area )
  {
    stc("You locate your victim, but they are too distant.\n\r",ch);
    return;
  }

  chroom = ch->in_room;
  victimroom = victim->in_room;
  ch->mana -= 500;
  WAIT_STATE(ch, 24);

  char_from_room(ch);
  char_to_room(ch, victimroom);
  act("You call upon the Forces of Nature to obliterate $N.",ch,NULL,victim,TO_CHAR); 
  act("The sky turns an ominous white.\n\r...Seconds later, deadly shards of ice shoot out of the sky, piercing your skin.",ch,NULL,victim,TO_VICT);

  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next = vch->next;

    if ( vch->in_room == NULL )
      continue;

    if ( vch->in_room == ch->in_room )
    {
      if ( vch->level < 3 ) continue;
      if ( vch != ch )
      {
        dam = number_range(550,2050);
        if ( vch == victim )
          dam += number_range( 100,300 );
        mage_damage( ch, vch, dam, "ice shards", MAGEDAM_ICESTORM );
      }
      
      continue;
    }

    if ( vch->in_room->area == ch->in_room->area
	&& IS_OUTSIDE(vch)
	&& IS_AWAKE(vch) )
    {
      stc( "Shards of deadly ice fall from the sky.\n\r", vch );
    }
  }
  
  char_from_room(ch);
  char_to_room(ch, chroom);
  return;
}

bool is_immune( CHAR_DATA *ch, int type )
{
  if ( IS_SET(ch->imms[IMMUNITY], type) )
    return TRUE;
  else
    return FALSE;
}

bool is_resistant( CHAR_DATA *ch, int type )
{
  if ( IS_SET(ch->imms[RESISTANCE], type) )
    return TRUE;
  else
    return FALSE;
}

bool is_vulnerable( CHAR_DATA *ch, int type )
{
  if ( IS_SET(ch->imms[VULNERABLE], type ) )
    return TRUE;
  else
    return FALSE;
}

int mage_immunity( CHAR_DATA*ch, int dam, const char *msg, int type )
{
  if ( IS_NPC(ch) ) return dam;

  if ( is_immune( ch, type ) )
    dam = 0;

  else if ( is_resistant( ch, type ) )
    dam /= 2;

  else if ( is_vulnerable( ch, type ) )
    dam *= 2;

  return dam;
}

void mage_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type )
{
  int max_dam = ch->damcap[DAM_CAP] * 1;

  if ( victim->position == POS_DEAD )
    return;

  if ( ch->damcap[DAM_CHANGE] == 1 ) update_damcap(ch,victim);

  if ( dam > max_dam ) dam = max_dam;

  if ( victim != ch )
  {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if ( is_safe( ch, victim ) )
            return;
        check_killer( ch, victim );

        if ( victim->position > POS_STUNNED )
        {
            if ( victim->fighting == NULL )
                set_fighting( victim, ch );
            victim->position = POS_FIGHTING;
        }

        if ( victim->position > POS_STUNNED )
        {
            if ( ch->fighting == NULL )
                set_fighting( ch, victim );
            /*
             * If victim is charmed, ch might attack victim's master.
             */
            if ( IS_NPC(ch)
            &&   IS_NPC(victim)
            &&   IS_AFFECTED(victim, AFF_CHARM)
            &&   victim->master != NULL
            &&   victim->master->in_room == ch->in_room
            &&   number_bits( 3 ) == 0 )
            {
                stop_fighting( ch, FALSE );
                multi_hit( ch, victim->master, TYPE_UNDEFINED );
                return;
            }
        }

        /*
         * More charm stuff.
         */
        if ( victim->master == ch )
            stop_follower( victim );

        /*
         * Damage modifiers.
         */
        if ( IS_AFFECTED(ch, AFF_HIDE) )
        {
            if (!can_see(victim,ch))
	    {
	      dam *= 1.5;
              send_to_char("You use your concealment to get a surprise attack!\n\r",ch);
	    }
            REMOVE_BIT( ch->affected_by, AFF_HIDE );
            act( "$n leaps from $s concealment.", ch, NULL, NULL, TO_ROOM);
        }

        if ( IS_AFFECTED(victim, AFF_SANCTUARY) && dam > 1  )
            dam *= 0.5;

        if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) && dam > 1  )
            dam -= dam * 0.25;

        if ( dam < 0 )
            dam = 0;

    // I'll eventually add resistances, immunities, vulnerabilities, etc
    // Right about here. Amen.

    mage_message( ch, victim, dam, message, type );
    hurt_person(ch,victim,dam,FALSE);
    return;
   }

   else
    return;
}

void mage_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type)
{
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  char damm[MAX_STRING_LENGTH];
  char dam2[MAX_STRING_LENGTH];
  const char *chm;
  const char *victm;

  if ( dam == 0 )       {  chm = " miss";        victm = " misses";     }
  else if ( dam <= 50 ) {  chm = " graze";       victm = " grazes";     }
  else if ( dam <= 100) {  chm = " wound";       victm = " wounds";     }
  else if ( dam <= 200) {  chm = " mutilate";    victm = " mutilates";  }
  else if ( dam <= 300) {  chm = " massacre";    victm = " massacres";  }
  else if ( dam <= 400) {  chm = " eviscerate";  victm = " eviscerates";}
  else if ( dam <= 500) {  chm = " annihilate";  victm = " annihilates";}
  else if ( dam <= 600) {  chm = " obliterate";  victm = " obliterates";}
  else if ( dam <= 800) {  chm = " dissipate";   victm = " dissipates"; }
  else if ( dam <= 1000){  chm = " vaporize";    victm = " vaporizes";  }
  else                  {  chm = " liquify";     victm = " liquifies";  }

  if ( !IS_NPC(victim) && ( victim->hit - dam ) < 0 )
  {
    if ( type == MAGEDAM_ELECTRIC )
    {
      act("Your electric shock causes $N to spasm violently and vomit blood.",ch,NULL,victim,TO_CHAR);
      act("$n's electric shock causes $N to spasm violently and vomit blood.",ch,NULL,victim,TO_NOTVICT);
      act("$n's electric shock causes your body to spasm violently as you vomit blood.",ch,NULL,victim,TO_VICT);
      make_part(victim,"blood");
    }

    else if ( type == MAGEDAM_ICESTORM )
    {
      act("Your ice shards tear $N's flesh from $S body and leave $M mortally wounded.",ch,NULL,victim,TO_CHAR);
      act("$n's ice shards tear $N's flesh from $S body and leave $M mortally wounded.",ch,NULL,victim,TO_NOTVICT);
      act("$n's ice shards tear your flesh from your body and leave you mortally wounded.",ch,NULL,victim,TO_VICT);
      make_part(victim,"blood");
    }

    else
    {
      act("Your magical energy tears $N's body asunder.",ch,NULL,victim,TO_CHAR);
      act("$n's magical energies tear $N's body asunder.",ch,NULL,victim,TO_NOTVICT);
      act("$n's magical energies tear your body asunder.",ch,NULL,victim,TO_VICT);
      make_part(victim,"blood");
    }
  }

  if ( dam == 0 && is_immune(ch, type))
  {
    sprintf(buf1, "$N is unaffected by your %s.", message);
    sprintf(buf2, "$N is unaffected by $n's %s.", message);
    sprintf(buf3, "You are unaffected by $n's %s.", message);
  }

  if ( dam == 0 && !is_immune(ch, type))
  {
    sprintf(buf1, "You miss $N with your %s.", message);
    sprintf(buf2, "$n misses $N with $s %s.", message);
    sprintf(buf3, "$n misses you with $s %s.", message);
  }

  else
  {
    if ( ch->spheres[MFOR] > 1 )
    {
      sprintf(damm,"%d",dam);
      ADD_COLOUR(ch, damm, D_RED);
      sprintf(dam2,".[%s]",damm);
    }
    else
      sprintf(dam2,".");

    sprintf(buf1, "You%s $N with your %s%s", chm, message,dam2);
    sprintf(buf2, "$n%s $N with $s %s.", victm, message);
    sprintf(buf3, "$n%s you with $s %s.", victm, message);
  }
 
  act(buf1,ch,NULL,victim,TO_CHAR);
  act(buf2,ch,NULL,victim,TO_NOTVICT);
  act(buf3,ch,NULL,victim,TO_VICT);

  return;

}
  
void do_quintessence( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 1 )
  {
    stc("You cannot store Quintessence until you have learned level 1 Prime.\n\r",ch);
    return;
  }

  line2(ch);
  centre_text("Quintessence", ch);
  line2(ch);
  sprintf(buf,"*   You have %d points of Quintessence stored within your body.               *\n\r",ch->quint[BODY]);
  stc(buf,ch);
  sprintf(buf,"*   You have %d points of Quintessence stored within your Avatar.             *\n\r",ch->quint[AVATAR]);
  stc(buf,ch);
  line2(ch);
  return;
}

int check_quint( CHAR_DATA *ch )
{
  if ( ch->spheres[MPRI] < 3 )
    return ch->quint[AVATAR];
  else if ( ch->spheres[MPRI] >= 3 )
    return ( ch->quint[AVATAR] + ch->quint[BODY] );
  else
    return ch->quint[AVATAR];
}

void subtract_quint( CHAR_DATA *ch, int number )
{
  ch->quint[MTOTAL] = check_quint(ch);   // Find Total Quint of Char
  ch->quint[MTOTAL] -= number;           // Subtract from Max
  if ( ch->quint[MTOTAL] > 5 )
  {
    ch->quint[AVATAR] = 5;
    ch->quint[MTOTAL] -= 5;
      if ( ch->quint[MTOTAL] > 5 )
      {
        ch->quint[BODY] = 5;
        ch->quint[MTOTAL] = 0;
      }
      else
      {
        ch->quint[BODY] = ch->quint[MTOTAL];
        ch->quint[MTOTAL] = 0;
      }
  }
  else
  {
    ch->quint[AVATAR] = ch->quint[MTOTAL]; // Move all quint to Avatar
    ch->quint[BODY] = 0;                   // Drain the body
  }
  return;
}

void add_quint( CHAR_DATA *ch, int number )
{
  ch->quint[MTOTAL] = check_quint(ch);
  ch->quint[MTOTAL] += number;
  if ( ch->quint[MTOTAL] > 5 )
  {
    ch->quint[AVATAR] = 5;
    ch->quint[MTOTAL] -= 5;
    if ( ch->quint[MTOTAL] > 5 )
    {
      ch->quint[BODY] = 5;
      ch->quint[MTOTAL] = 0;
    }
    else
    {
      ch->quint[BODY] = ch->quint[MTOTAL];
      ch->quint[MTOTAL] = 0;
    }
  }
  else
  {
    ch->quint[AVATAR] = ch->quint[MTOTAL];
    ch->quint[BODY]   = 0;
  }
  return;
}

void do_rubbing( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MSL];
  char arg[MIL];
  int dam;
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 3 )
  {
    stc("You must obtain level three Prime to use Rubbing of the Bones.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }
  if ( ( victim = get_char_room(ch, arg) ) == NULL )
  {
    stc("You cannot find your victim here.\n\r",ch);
    return;
  }

    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }  

  if ( check_quint(ch) < 2 )
  {
    stc("You must have two points of quintessence to use this power.\n\r",ch);
    return;
  }

  subtract_quint(ch, 2);
  dam = ch->spheres[MPRI] * 300;
  if ( !IS_NPC(victim) )
    dam /= number_range( 1.5, 2 );
  mage_damage(ch,victim,dam,"quintessence warp",MAGEDAM_QUINT);
  WAIT_STATE(ch, 16);
  sprintf(buf, "You are stunned by %s's blast.\n\r", ch->name);
  send_to_char(buf, victim);
  WAIT_STATE(victim, 18);
  return;
}

void do_purification( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 4 )
  {
    stc("You must obtain level three Prime to use Flames of Purification.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room(ch, arg) ) == NULL )
  {
    stc("You cannot find your victim here.\n\r",ch);
    return;
  }
    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

  if ( check_quint(ch) < 3 )
  {
    stc("You must have three points of quintessence to use this power.\n\r",ch);
    return;
  }

  if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD))
  {
     send_to_char("Nothing happens.\n\r",ch);
     WAIT_STATE(ch, 6);
     return;
  }

  subtract_quint(ch, 3);
  dam = ch->spheres[MPRI] * 200;
  if ( !IS_NPC(victim) )
    dam /= number_range( 1.5, 2 );
  mage_damage(ch,victim,dam,"flames of purification",MAGEDAM_QUINT);
  WAIT_STATE(ch, 16);

  if (victim == NULL || victim->position == POS_DEAD) return;
    else if (!IS_MORE(victim, MORE_FLAMES))
    {
        send_to_char("You are engulfed in infernal flames!\n\r",victim);
        act("$n is engulfed in infernal flames!",victim,NULL,NULL,TO_ROOM);
        SET_BIT(victim->affected_by, AFF_FLAMING);
        SET_BIT(victim->more, MORE_FLAMES);
    }
    update_pos(victim);
  return;
}

void do_qblast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MPRI] < 5 )
  {
    stc("You must obtain level five Prime to use Quintessence Blast.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room(ch, arg) ) == NULL )
  {
    stc("You cannot find your victim here.\n\r",ch);
    return;
  }

    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

  if ( check_quint(ch) < 5 )
  {
    stc("You must have five points of quintessence to use this power.\n\r",ch);
    return;
  }

  subtract_quint(ch, 5);
  dam = ch->spheres[MPRI] * 500;
  if ( !IS_NPC(victim) )
    dam /= number_range( 1.5, 2 );
  mage_damage(ch,victim,dam,"quintessence blast",MAGEDAM_QUINT);
  WAIT_STATE(ch, 16);
  WAIT_STATE(victim, 18);
  return;
}

void do_dimmak( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MENT] < 1 )
  {
    stc("You must obtain level one entropy to use Dim Mak.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MFLAGS_DIMMAK) )
  {
    REMOVE_BIT(ch->mflags, MFLAGS_DIMMAK);
    stc("You no longer focus your attacks on your opponent's weakness.\n\r",ch);
    return;
  }

  else
  {
    SET_BIT(ch->mflags, MFLAGS_DIMMAK);
    stc("You now focus your attacks on your opponent's weakness.\n\r",ch);
    return;
  }

  return;
}

void do_hotienchi( CHAR_DATA *ch, char *argument )
{
  int i,test=0;
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch)) return;

  if ( ch->spheres[MLIF] < 1 )
  {
    stc("You must obtain level one Life to use Ho Tien Chi.\n\r",ch);
    return;
  }

  for( i=0;i<7;i++)
  {
    if ( ch->loc_hp[i] > 0 )
      test++;
  }

  if ( ch->hit == ch->max_hit && ch->mana == ch->max_mana && ch->move == ch->max_move && test == 0 )
  {
    stc("You are in perfect condition!\n\r",ch);
    return;
  }

  if (ch->loc_hp[6] > 0)
  {
    stc("Your wounds close up and stop bleeding.\n\r",ch);
    ch->loc_hp[6] = 0;
  }
  
  if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
  {
    stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",ch);
    ch->loc_hp[0] = 0;
    ch->loc_hp[1] = 0;
    ch->loc_hp[2] = 0;
    ch->loc_hp[3] = 0;
    ch->loc_hp[4] = 0;
    ch->loc_hp[5] = 0;
  }

  stc("A warm feeling spreads through your body.\n\r",ch);
  ch->hit += (ch->spheres[MLIF]*200);
  ch->mana += (ch->spheres[MLIF]*100);
  ch->move += (ch->spheres[MLIF]*100);
  if ( ch->hit > ch->max_hit )
    ch->hit = ch->max_hit;
  if ( ch->mana > ch->max_mana )
    ch->mana = ch->max_mana;
  if ( ch->move > ch->max_move )
    ch->move = ch->max_move;
  WAIT_STATE(ch,18);
  return;
}

void do_littledeath( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MLIF] < 2 )
  {
    stc("You must obtain level two Life to use Little Good Death.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("You cannot find your victim here.\n\r",ch);
    return;
  }

  if ( !IS_NPC(victim) )
  {
    stc("A human being is too complex for this spell.\n\r",ch);
    return;
  }

  if ( victim->level > 100 )
  {
    act("$N's pattern is too complex to destroy.",ch,NULL,victim,TO_CHAR);
    return;
  }

  WAIT_STATE(ch, 12);
  act("You mentally grasp $N's pattern and tear $M to shreds.",ch,NULL,victim,TO_CHAR);
  act("$n's eyes turn black.\n$N's body is torn to shreds before your eyes!",ch,NULL,victim,TO_ROOM);
  raw_kill(victim, ch);
  return;
}

void do_betterbody( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MLIF] < 3 )
  {
    stc("You must obtain level three Life to use Better Body.\n\r",ch);
    return;
  }

  if ( IS_SET(ch->mflags, MAGE_BETTERBODY) )
  {
    stc("Your body shrinks to its true form.\n\r",ch);
    REMOVE_BIT(ch->mflags, MAGE_BETTERBODY);
    ch->pcdata->perm_str -= 4;
    ch->pcdata->perm_con -= 2;
    ch->pcdata->perm_dex -= 3;
    return;
  }

  else
  {
    stc("Your body contorts and grows to supernatural size.\n\r",ch);
    SET_BIT(ch->mflags, MAGE_BETTERBODY);
    ch->pcdata->perm_str += 4;
    ch->pcdata->perm_dex += 3;
    ch->pcdata->perm_con += 2;
    return;
  }
  return;
}

void do_shapechange( CHAR_DATA *ch, char *argument )
{
  char arg[MIL];
  char buf[MSL];
  int choice = 0;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MLIF] < 4 )
  {
    stc("You must obtain level four Life to Change Shape.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    divide_to_char(ch);
    centre_text("Forms",ch);
    divide_to_char(ch);
    stc("    [ 0 ]  Human\n",ch);
    stc("    [ 1 ]  Tiger\n",ch);
    stc("    [ 2 ]  Lion\n",ch);
    stc("    [ 3 ]  Raven\n",ch);
    stc("    [ 4 ]  Frog\n",ch);
    stc("    [ 5 ]  Dog\n",ch);
    stc("    [ 6 ]  Hawk\n",ch);
    stc("    [ 7 ]  Mouse\n",ch);
    stc("    [ 8 ]  Spider\n",ch);
    divide_to_char(ch);
    return;
  }

  choice = is_number( arg ) ? atoi( arg ) : -1;

  if ( choice == 1 )
  {
    sprintf(buf, "%s the ferocious tiger",ch->name);
    stc("You transform into a ferocious tiger.\n\r",ch);
    act("$n transforms into a tiger.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 2 )
  {
    sprintf(buf, "%s the lion",ch->name);
    stc("You transform into a lion.\n\r",ch);
    act("$n transforms into a lion.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 3 )
  {
    sprintf(buf, "%s the raven",ch->name);
    stc("You transform into a raven.\n\r",ch);
    act("$n transforms into a raven.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 4 )
  {
    sprintf(buf, "%s the frog",ch->name);
    stc("You transform into a frog.\n\r",ch);
    act("$n transforms into a frog.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 5 )
  {
    sprintf(buf, "%s the dog",ch->name);
    stc("You transform into a small dog.\n\r",ch);
    act("$n transforms into a dog.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 6 )
  {
    sprintf(buf, "%s the hawk",ch->name);
    stc("You transform into a hawk.\n\r",ch);
    act("$n transforms into a hawk.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 7 )
  {
    sprintf(buf, "%s the mouse",ch->name);
    stc("You transform into a white mouse.\n\r",ch);
    act("$n transforms into a white mouse.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 8 )
  {
    sprintf(buf, "%s the spider",ch->name);
    stc("You transform into a black spider.\n\r",ch);
    act("$n transforms into a black spider.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 0 )
  {
    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      REMOVE_BIT(ch->mflags, MAGE_SHAPED);
      free_string(ch->morph);
      ch->morph = str_dup("");
      stc("You revert to your human form.\n\r",ch);
      act("$n reverts to $s human form.",ch,NULL,NULL,TO_ROOM);
      return;
    }
    else
    {
      stc("You aren't even shapechanged!\n\r",ch);
      return;
    }
  }
  else
  {
    stc("That's not a choice!\n\r",ch);
    do_shapechange(ch,"");
    return;
  }

  SET_BIT(ch->affected_by, AFF_POLYMORPH);
  SET_BIT(ch->mflags, MAGE_SHAPED);
  clear_stats(ch);
  free_string(ch->morph);
  ch->morph = str_dup(buf);
  return;
}  


void do_layhands( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int i,test=0;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch)) return;

  if ( ch->spheres[MLIF] < 1 )
  {
    stc("You must obtain level one Life to use Ho Tien Chi.\n\r",ch);
    return;
  }
 
  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They aren't here.\n\r",ch);
    return;
  }

  for( i=0;i<7;i++)
  {
    if ( victim->loc_hp[i] > 0 )
      test++;
  }

  if ( victim->hit == victim->max_hit && victim->mana == victim->max_mana && victim->move == victim->max_move && test == 0 )
  {
    stc("They are in perfect condition!\n\r",ch);
    return;
  }

  if (victim->loc_hp[6] > 0)
  {
    stc("Your wounds close up and stop bleeding.\n\r",victim);
    victim->loc_hp[6] = 0;
  }
  
  if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5]) != 0)
  {
    stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",victim);
    victim->loc_hp[0] = 0;
    victim->loc_hp[1] = 0;
    victim->loc_hp[2] = 0;
    victim->loc_hp[3] = 0;
    victim->loc_hp[4] = 0;
    victim->loc_hp[5] = 0;
  }

  stc("A warm feeling spreads through your body.\n\r",victim);
  victim->hit += (ch->spheres[MLIF]*75);
  victim->mana += (ch->spheres[MLIF]*50);
  victim->move += (ch->spheres[MLIF]*50);
  if ( victim->hit > victim->max_hit )
    victim->hit = victim->max_hit;
  if ( victim->mana > victim->max_mana )
    victim->mana = victim->max_mana;
  if ( victim->move > victim->max_move )
    victim->move = victim->max_move;
  WAIT_STATE(ch,18);
  act("You lay your hands on $N and rejuvenate $M.",ch,NULL,victim,TO_CHAR);
  return;
}

void do_analyze( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg[MIL];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 1 )
  {
    stc("You must obtain level one Matter to use Analyze Substance.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Analyze which object?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You are not carrying that item.\n\r",ch);
    return;
  }
  
  act("You analyze the substance pattern of $p.",ch,obj,NULL,TO_CHAR);
  (*skill_table[skill_lookup("identify")].spell_fun) (skill_lookup("identify"), 50, ch, obj );
  return;
}

void do_alterstate( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  CHAR_DATA *victim;
  char arg[MIL];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 2 )
  {
    stc("You must obtain level two Matter to use Alter State.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' && ch->fighting == NULL )
  {
    stc("Whom do you wish to use this on?\n\r",ch);
    return;
  }
  
  if ( ch->fighting == NULL )
  {
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      stc("You are unable to find them in this room.\n\r",ch);
      return;
    }
  }
  else
    victim = ch->fighting;

  if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
  {
    if ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL )
    {
      stc("They aren't using any weapons!\n\r",ch);
      return;
    }
  }

  if ( check_quint(ch) < 2 )
  {
    stc("You need two points of quintessence to use this power.\n\r",ch);
    return;
  }

  obj_from_char( obj );
  obj_to_char( obj, victim );
  act("You alter the state of $N's weapon, causing $M to drop it.",ch,NULL,victim,TO_CHAR);
  act("$n's eyes glow dark black.",ch,NULL,NULL,TO_ROOM);
  act("$p shakes violently, causing you to drop it.",ch,obj,victim,TO_VICT);
  subtract_quint( ch, 2 );
  WAIT_STATE(ch, 16);
  return;
}

void do_enchantarmor( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  AFFECT_DATA *paf;
  char arg[MIL];

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 4 )
  {
    stc("You must obtain level four Matter to use Enchant Armor.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Which piece of armor do you want to enchant?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You are not carrying that object.\n\r",ch);
    return;
  }

  if ( IS_SET(obj->quest, QUEST_MAGEENCHANT) )
  {
    stc("This object has already been enchanted.\n\r",ch);
    return;
  }

  if ( obj->item_type != ITEM_ARMOR )
  {
    stc("This item isn't a piece of armor.\n\r",ch);
    return;
  }
  
  if ( check_quint( ch ) < 3 )
  {
    stc("You need three points of quintessence to Enchant Armor.\n\r",ch);
    return;
  }

  act("You lay your hands on $p and rearrange its pattern.",ch,obj,NULL,TO_CHAR);
  act("$n's eyes glow bright blue.\n$n lays $s hands on $p.\n$p glows blue.",ch,obj,NULL,TO_ROOM);
  SET_BIT(obj->quest, QUEST_MAGEENCHANT);
  

  if ( affect_free == NULL )
  {
      paf             = alloc_perm( sizeof(*paf) );
  }
  else
  {
      paf             = affect_free;
      affect_free     = affect_free->next;
  }

  paf->type           = skill_lookup("enchant weapon");
  paf->duration       = -1;
  paf->location       = APPLY_HITROLL;
  paf->modifier       = 3;
  paf->bitvector      = 0;
  paf->next           = obj->affected;
  obj->affected       = paf;

  if ( affect_free == NULL )
  {
      paf             = alloc_perm( sizeof(*paf) );
  }
  else
  {
      paf             = affect_free;
      affect_free     = affect_free->next;
  }

  paf->type           = skill_lookup("enchant weapon");
  paf->duration       = -1;
  paf->location       = APPLY_DAMROLL;
  paf->modifier       = 3;
  paf->bitvector      = 0;
  paf->next           = obj->affected;
  obj->affected       = paf;

  WAIT_STATE(ch, 16);
  subtract_quint(ch, 3);
  return;
}

void do_hover( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MMAT] < 5 )
  {
    stc("You must obtain level 5 matter to use Hoverwalk.\n\r",ch);
    return;
  }

  if ( IS_AFFECTED(ch, AFF_FLYING) )
  {
    stc("You slowly float to the ground as your air cushion dissipates.\n\r",ch);
    act("$n slowly floats to the ground.",ch,NULL,NULL,TO_ROOM);
    REMOVE_BIT(ch->affected_by, AFF_FLYING);
    return;
  }
  else
  {
    stc("You restructure the pattern of the air around your body.\n\rYou slowly float into the air.\n\r",ch);
    act("$n slowly floats into the air.",ch,NULL,NULL,TO_ROOM);
    SET_BIT(ch->affected_by, AFF_FLYING);
    return;
  }
  
  return;
}

void do_mshadow( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;
  
  if ( ch->spheres[MMIN] < 5 )
  {
    stc("You must obtain level five Mind to use Untether.\n\r",ch);
    return;
  }

  if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
  {
      send_to_char("You fade into the plane of shadows.\n\r",ch);
      act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
      SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
      do_look(ch,"auto");
      return;
  }

  REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
  send_to_char("You fade back into the real world.\n\r",ch);
  act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
  do_look(ch,"auto");
  return;
}

void do_callspirit( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;

  if ( IS_NPC(ch) ) return;
 
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MSPI] < 1 )
  {
    stc("You must obtain level one Spirit to Call Spirits.\n\r",ch);
    return;
  }
  
  if ( ch->pcdata->followers > 5 )
  {
    stc("Nothing seems to happen.\n\r",ch);
    return;
  }

  stc("You chant an arcane spell and conjure a spirit.\n\r",ch);
  act("$n chants loudly in a foreign language.",ch,NULL,NULL,TO_ROOM);

    ch->pcdata->followers++;
    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->name = str_dup("spirit");
    victim->short_descr = str_dup("A wicked spirit");
    victim->long_descr = str_dup("A ghostly image hovers over the ground.  ");
    victim->level = 200;
    victim->hit = 5000;
    victim->max_hit = 5000;
    victim->hitroll = 50;
    victim->damroll = 50;
    victim->armor = 300;
    SET_BIT(victim->act, ACT_NOEXP);
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    act("$N materializes next to $n.",ch,NULL,victim,TO_ROOM);
    return;
}

void do_spiritkiss( CHAR_DATA *ch, char *argument )
{
  int sn,level;
  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MSPI] < 2 )
  {
    stc("You must obtain level two Spirit to use Spirit Kiss.\n\r",ch);
    return;
  }

  if ( ( sn = skill_lookup( "spirit kiss" ) ) < 0 ) return;
  
  if ( is_affected(ch,sn) )
  {
    stc("You are already blessed by the spirits.\n\r",ch);
    return;
  }

  if ( check_quint(ch) < 1 )
  {
    stc("You need one point of quintessence to call a spirit.\n\r",ch);
    return;
  }

  level = (ch->spheres[MSPI]*5);
  (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
  WAIT_STATE( ch, 12 );
  subtract_quint(ch,1);
  return;
}

void do_spiritblast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

  if ( !is_mage(ch) ) return;
  if ( ch->spheres[MSPI] < 4 )
  {
    stc("You must obtain level 4 Spirit to use Spirit Blast.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you want to use Spirit Blast on?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They aren't here.\n\r",ch);
    return;
  }

    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

  if ((victim->mana <= 1250) || (victim->move <= 1500))
  {
      stc("They are already drained.\n\r", ch);
      return;
  }

  dam = ch->spheres[MSPI] * 50;
  mage_damage(ch, victim, dam, "spirit blast", MAGEDAM_OTHER);
  victim->mana -= (dam*10);
  victim->move -= (dam*10);
  ch->mana += (dam*10);
  ch->move += (dam*10);
  if ( ch->mana > ( ch->max_mana + 500 ) )
    ch->mana = ( ch->max_mana + 500 );
  if ( ch->move > ( ch->max_move + 500 ) )
    ch->move = ( ch->max_move + 500 );
  stc("You absorb the energy from the blast.\n\r",ch);
  stc("You feel drained.\n\r",victim);
  WAIT_STATE(ch, 16);
  return;
}

void do_breach( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;

  if ( IS_NPC(ch) ) return;
  
  if ( !is_mage(ch) ) return;

  if ( ch->spheres[MSPI] < 5 )
  {
    stc("You must obtain level 5 Spirit to Breach the Gauntlet.\n\r",ch);
    return;
  }
 
  if ( check_quint( ch ) < 2 )
  {
    stc("You need two points of quintessence to breach the gauntlet.\n\r",ch);
    return;
  }

  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
      vch_next        = vch->next;
      if ( vch->in_room == NULL )
          continue;
      if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
          continue;
      if ( vch->level < 3 )
	  continue;
      if ( IS_SET(vch->affected_by, AFF_SHADOWPLANE) )
	  continue;
      if ( ch == vch )
	  continue;
      if ( vch->in_room == ch->in_room )
      {
          if ( ( mount = ch->mount ) != NULL ) 
	  {
	    if ( mount == vch )
              continue;
	  }

          if (can_see(ch,vch))
          {
	    act("$n's eyes turn dark black.\nThe room is torn apart before your very eyes, and you fall into a black nothingness.",ch,NULL,vch,TO_VICT);
     	    SET_BIT(vch->affected_by, AFF_SHADOWPLANE);
          }
       }
  }
  SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
  subtract_quint(ch,2);
  stc("You tear the room apart, reforming it in the spirit world.\n\r",ch);
  return;

}

void do_impulse( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char action[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch_next;
    int level;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_MAGE(ch))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    if (ch->spheres[MMIN] < 2)
    {
	send_to_char( "You must obtain level two Mind to use Control Impulses.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Make whom to do what?\n\r", ch );
	return;
    }

    strcpy(action,argument);

    if (is_in(action,"|quit*tok*gift*claim*consent*propose*vampi*ma *maj*sid *side *reca*/*ha *hav*"))
    {
	send_to_char( "I think not.\n\r", ch );
	return;
    }
   
    level = (ch->spheres[MMIN] * 80);

    if (!str_cmp(arg,"all"))
    {
	sprintf( buf, "Everyone wants to %s", action );
	do_say(ch,buf);
	WAIT_STATE(ch,24);
	for ( victim = ch->in_room->people; victim != NULL; victim = vch_next )
	{
	    vch_next = victim->next_in_room;
	    WAIT_STATE(ch,24);
	    if ( victim == ch ) continue;
	    if ( IS_IMMORTAL(victim) ) continue;
            if (is_safe(ch, victim)) continue;
  
	    if ( IS_NPC(victim) && victim->level >= level )
	    {
	    	act("$s ignores you.",victim,NULL,ch,TO_VICT);
		continue;
	    }

	    if (!IS_NPC(victim) && victim->pcdata->resist[WILL_DOMINATE] > 0)
	    {
		act("You ignore $N.",victim,NULL,ch,TO_CHAR);
		act("$n ignores $N.",victim,NULL,ch,TO_NOTVICT);
		act("$n ignores you.",victim,NULL,ch,TO_VICT);
		if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
		{
		    guardian_message( victim );
		    act("$n just failed to use Control Impulse on you.",ch,NULL,victim,TO_VICT);
		}
		continue;
	    }
	
	    if (!IS_NPC(victim) && IS_WILLPOWER(victim, RES_DOMINATE) && 
		victim->pcdata->willpower[0] >= 5)
	    {
		victim->pcdata->willpower[0] -= 5;
		victim->pcdata->resist[WILL_DOMINATE] = 60;
		if (!IS_WILLPOWER(victim, RES_TOGGLE))
		    REMOVE_BIT(victim->pcdata->resist[0], RES_DOMINATE);
		send_to_char("You burn some willpower to resist Control Impulse.\n\r",victim);
		act("You ignore $N.",victim,NULL,ch,TO_CHAR);
		act("$n ignores $N.",victim,NULL,ch,TO_NOTVICT);
		act("$n ignores you.",victim,NULL,ch,TO_VICT);
		if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
		{
		    guardian_message( victim );
		    act("$n just failed to use Control Impulse on you.",ch,NULL,victim,TO_VICT);
		}
		continue;
	    }

	    act("You obey $N.",victim,NULL,ch,TO_CHAR);
	    if (IS_NPC(victim)) SET_BIT(victim->act, ACT_COMMANDED);
	    interpret( victim, action );
	    if (IS_NPC(victim)) REMOVE_BIT(victim->act, ACT_COMMANDED);
	    if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
	    {
		guardian_message( victim );
		act("$n just used Control Impulse on you.",ch,NULL,victim,TO_VICT);
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Now that would be just plain stupid.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "I think not...\n\r", ch );
	return;
    }
    if( !IS_NPC(victim) && IS_NEWBIE(victim))
     {
	send_to_char("Newbie killing is not allowed\n\r", ch);
	return;
     }
    if ( ch->in_room == NULL || IS_SET(ch->in_room->room_flags,ROOM_SAFE))
     {
	send_to_char("Something in the air stops your attack!\n\r",ch);
	return;
     }
    if ( !CAN_PK(victim) && !IS_NPC(victim))
     {
	send_to_char("Harrassing Mortal players is not allowed\n\r",ch);
	return;
     }

    if (is_safe(ch, victim)) return;

    if (IS_NPC(victim))
	sprintf( buf, "I order %s to %s", victim->short_descr, action );
    else if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH))
	sprintf( buf, "I order %s to %s", victim->morph, action );
    else
	sprintf( buf, "I order %s to %s", victim->name, action );
    do_say(ch,buf);
    WAIT_STATE(ch,24);

    if ( IS_NPC(victim) && victim->level >= level )
    {
	act("You ignore $N.",victim,NULL,ch,TO_CHAR);
	act("$n ignores $N.",victim,NULL,ch,TO_NOTVICT);
	act("$n ignores you.",victim,NULL,ch,TO_VICT);
	return;
    }

    if (!IS_NPC(victim) && victim->pcdata->resist[WILL_DOMINATE] > 0)
    {
	act("You ignore $N.",victim,NULL,ch,TO_CHAR);
	act("$n ignores $N.",victim,NULL,ch,TO_NOTVICT);
	act("$n ignores you.",victim,NULL,ch,TO_VICT);
	if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
	{
	    guardian_message( victim );
	    act("$n just failed to use Control Impulse on you.",ch,NULL,victim,TO_VICT);
	}
	return;
    }

    if (!IS_NPC(victim) && IS_WILLPOWER(victim, RES_DOMINATE) && 
	victim->pcdata->willpower[0] >= 5)
    {
	victim->pcdata->willpower[0] -= 5;
	victim->pcdata->resist[WILL_DOMINATE] = 60;
	if (!IS_WILLPOWER(victim, RES_TOGGLE))
	    REMOVE_BIT(victim->pcdata->resist[0], RES_DOMINATE);
	send_to_char("You burn some willpower to resist Control Impulse.\n\r",victim);
	act("You ignore $N.",victim,NULL,ch,TO_CHAR);
	act("$n ignores $N.",victim,NULL,ch,TO_NOTVICT);
	act("$n ignores you.",victim,NULL,ch,TO_VICT);
	if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
	{
	    guardian_message( victim );
	    act("$n just failed to use Control Impulse on you.",ch,NULL,victim,TO_VICT);
	}
	return;
    }

    act("You obey $N.",victim,NULL,ch,TO_CHAR);
    if (IS_NPC(victim)) SET_BIT(victim->act, ACT_COMMANDED);
    interpret( victim, action );
    if (IS_NPC(victim)) REMOVE_BIT(victim->act, ACT_COMMANDED);
    if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN))
    {
	guardian_message( victim );
	act("$n just used Control Impulse on you.",ch,NULL,victim,TO_VICT);
    }
    return;
}

void do_possess( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg [MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_MAGE(ch))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->spheres[MMIN] < 4)
    {
	send_to_char("You must obtain level four Mind to use Possession.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to possess?\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "I think not...\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }
    if ( IS_SET(victim->act, PLR_SILENCE) )
    {
        send_to_char( "Nothing happened.\n\r",ch);
        return;
    }

    if (IS_ITEMAFF(victim, ITEMA_SHADES))
    {
	if (!IS_SET(ch->act,PLR_HOLYLIGHT) && 
	    ch->spheres[MMIN] < 6)
	{
	    send_to_char( "You are unable to make eye contact with them.\n\r", ch );
	    return;
	}
    }

    act("You stare into $N's eyes.",ch,NULL,victim,TO_CHAR);
    act("$n stares into $N's eyes.",ch,NULL,victim,TO_NOTVICT);
    act("$n stares into your eyes.",ch,NULL,victim,TO_VICT);

    if ( ( familiar = victim->wizard ) != NULL ) 
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    if (victim->level > (ch->spheres[MMIN] * 20))
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    act("You possess $N's body.",ch,NULL,victim,TO_CHAR);
    SET_BIT(victim->extra, EXTRA_ILLUSION); /*Fix by Maizoon*/
    SET_BIT(victim->act, ACT_NOPARTS);
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    return;
}

void do_mindshield( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_MAGE(ch))
    {
	send_to_char("Huh?\n\r",ch);
        return;
    }
    
    if (ch->spheres[MMIN] < 1)
    {
		send_to_char("You must obtain level one Mind to Shield your being.\n\r",ch);
		return;
    }

    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    	send_to_char("You shield your aura from those around you.\n\r",ch);
    	SET_BIT(ch->immune, IMM_SHIELDED);
	return;
    }
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
    return;
}

void do_mupkeep( CHAR_DATA *ch, char *argument)
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if(!IS_MAGE(ch))
    {
       send_to_char("Huh.\n\r",ch);
       return;
    }

    if(IS_LICH(ch) || IS_SKINDANCER(ch) || IS_NEPHANDI(ch))
    {
        mageupkeep(ch);
        return;
    }
}

void do_moutcast( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char   buf[MAX_STRING_LENGTH];
    char   arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( !IS_MAGE(ch) || ch->level != LEVEL_ARCHMAGE)
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Outcast whom?\n\r", ch );
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
    if (!IS_MAGE(victim))
    {
	send_to_char("They aren't a mage.\n\r", ch );
	return;
    }
    if ( IS_IMMORTAL(victim) )
    {
        send_to_char( "Not on Immortals's.\n\r", ch );
        return;
    }
    if ( ch == victim )
    {
        send_to_char( "You cannot outcast yourself.\n\r", ch );
        return;
    }
    if (IS_MAGE(victim) && ch->pcdata->runes[0] != victim->pcdata->runes[0])
    {
        send_to_char( "They are NOT in your clan!.\n\r", ch );
	return;
    }
        SET_BIT(victim->more, MORE_OUTCAST);
	victim->level = LEVEL_APPRENTICE;
        do_save(victim, "");
	sprintf(buf, "You have outcast %s from the clan!\n\r",victim->name);
	send_to_char(buf, ch);
	sprintf(buf, "%s has outcast you from the clan!\n\r",ch->name);
	send_to_char(buf, victim);
}

void do_induct( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char   buf[MAX_STRING_LENGTH];
    char   arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    
    if (IS_NPC(ch)) return;

    if ( !IS_MAGE(ch) || ch->level <= LEVEL_APPRENTICE)
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Induct whom?\n\r", ch );
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
    if (!IS_MAGE(victim))
    {
        send_to_char("They aren't a mage.\n\r", ch );
        return;
    }
    if ( IS_IMMORTAL(victim) )
    {
        send_to_char( "Not on Immortals's.\n\r", ch );
        return;
    }
    if ( ch == victim )
    {
        send_to_char( "You cannot induct yourself.\n\r", ch );
        return;
    }
    if (IS_MAGE(victim) && !IS_MORE(victim, MORE_OUTCAST))
    {
        send_to_char( "They are NOT outcast.\n\r", ch );
        return;
    }
        REMOVE_BIT(victim->more, MORE_OUTCAST);
        victim->level = LEVEL_APPRENTICE;
	victim->pcdata->runes[0] = ch->pcdata->runes[0];        
	do_save(victim, "");
        sprintf(buf, "You have inducted %s to the clan!\n\r",victim->name);
        send_to_char(buf, ch);
        sprintf(buf, "%s has inducted you to the clan!\n\r",ch->name);
        send_to_char(buf, victim);
}

