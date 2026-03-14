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
#include <unistd.h> /* Added by Aristoi for unlink */

// Corpse Timers
#define CS1 75
#define CS2 60
#define CS3 45
#define CS4 30
#define CS5 15
#define CS6 7

void randomize_object(int vnum);

void do_timedexp( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	int value;

        one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "How much time?\n\r", ch );
        return;
    }

    value = is_number( arg ) ? atoi( arg ) : -1;

    dxp_info.dxp = 1;
    dxp_info.time = value * 480;
    sprintf(buf, "Double exp has been activated\n\r");
    do_info(ch, buf);
    return;
}

void do_doubleexp( CHAR_DATA *ch, char *argument )
{
        char  buf[MAX_STRING_LENGTH];
//arg[MAX_INPUT_LENGTH];

//    DESCRIPTOR_DATA *d;

if (!ch->desc || *argument == '\0')
{
  send_to_char("Syntax: doubleexp  on/off!\n\r",ch);
  return;
}

   if ( strcmp(argument,"on")== 0 )
      {
        dxp_info.dxp = 1;
        sprintf(buf, "Double exp has been activated by %s.\n\r", ch->name);
        do_info(ch, buf);
        return;
      }

   else if ( strcmp(argument,"off")== 0 )
      {
        sprintf(buf,"Double exp has been deactivated by %s.\n\r", ch->name);
        do_info(ch, buf);
        dxp_info.dxp = 0;
        return;
      }
   else
      {
        send_to_char("The Commands are On and Off",ch);
        return;
      }
}

void do_halfexp( CHAR_DATA *ch, char *argument )
{
        char  buf[MAX_STRING_LENGTH];
//arg[MAX_INPUT_LENGTH];

//    DESCRIPTOR_DATA *d;

if (!ch->desc || *argument == '\0')
{
  send_to_char("Syntax: halfexp  on/off!\n\r",ch);
  return;
}

   if ( strcmp(argument,"on")== 0 )
      {
        dxp_info.dxp = 2;
        sprintf(buf, "%s has turned on half-exp due to excessive begging.\n\r", ch->name);
        do_info(ch, buf);
        return;
      }

   else if ( strcmp(argument,"off")== 0 )
      {
        sprintf(buf,"%s has turned off half-exp.\n\r", ch->name);
        do_info(ch, buf);
        dxp_info.dxp = 0;
        return;
      }
   else
      {
        send_to_char("The Commands are On and Off",ch);
        return;
      }
}

void do_expoff ( CHAR_DATA *ch, char *argument )
{
    do_doubleexp( ch, "off" );
    dxp_info.time = 0;
    return;
}


void do_setbreed( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA   *victim;
    char        arg1[MAX_INPUT_LENGTH];
    char        arg2[MAX_INPUT_LENGTH];
    char        arg3[MAX_INPUT_LENGTH];
    sh_int      breed = 0;
    sh_int      value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( IS_NPC( ch ) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
        send_to_char( "Syntax : setbreed [victim] [breed/all] [level]\n\r\n\r", ch);
        send_to_char( " Breed being one of:\n\r", ch );
        send_to_char( "  Homid, Lupus, or Metis\n\r", ch );
        return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
     
    if ( IS_NPC(victim ) )
    {
       send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
        
    if ( !IS_WEREWOLF(victim) && !IS_ABOMINATION(victim))
    {
        send_to_char( "Only on Werewolves and Abomination!\n\r", ch );
        return;
    }
     
        
    if ( !str_cmp( arg2, "all" ) )
    {
        value = is_number( arg3 ) ? atoi( arg3 ) : -1;
    
        if ( value < 0 || value > 5 )
        {
            send_to_char( "Range is 0 - 5.\n\r", ch );
            return;
        }
    
        if ( value == 0 ) value = -1;
        
        for ( breed = 0 ; breed <= BREED_MAX ; breed++ )
                victim->pcdata->breed[breed] = value;
     
        send_to_char("Done.\n\r", ch );
        return;
    }
        
   else if ( !str_cmp( arg2, "lupus" ) )   breed = BREED_LUPUS;
    else if ( !str_cmp( arg2, "homid" ) )      breed = BREED_HOMID;
   else if ( !str_cmp( arg2, "metis" ) )    breed = BREED_METIS;
    else 
    {
        do_setbreed( ch, "" );
        return;
    }
    value = is_number( arg3 );
    
    if ( value < 0 || value > 5 )
    {
        send_to_char( "Range is 0 - 5.\n\r", ch );
        return;
    }
         victim->pcdata->breed[breed] = value;
         send_to_char("Done.\n\r", ch );
   return;
}


void do_setauspice( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA   *victim;
    char        arg1[MAX_INPUT_LENGTH];
    char        arg2[MAX_INPUT_LENGTH];
    char        arg3[MAX_INPUT_LENGTH];
    sh_int      auspice = 0;
    sh_int      value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( IS_NPC( ch ) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
        send_to_char( "Syntax : setauspice [victim] [auspice/all] [level]\n\r\n\r", ch);
        send_to_char( " Auspice being one of:\n\r", ch );
        send_to_char( "  Ragabash, Theurge, Philodox, Galliard, or Ahroun.\n\r", ch );
        return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
     
    if ( IS_NPC(victim ) )
    {
       send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
        
    if ( !IS_WEREWOLF(victim) && !IS_ABOMINATION(victim))
    {
        send_to_char( "Only on Werewolves and Abomination!\n\r", ch );
        return;
    }
     
        
    if ( !str_cmp( arg2, "all" ) )
    {
        value = is_number( arg3 ) ? atoi( arg3 ) : -1;
    
        if ( value < 0 || value > 5 )
        {
            send_to_char( "Range is 0 - 5.\n\r", ch );
            return;
        }
    
        if ( value == 0 ) value = -1;
        
        for ( auspice = 0 ; auspice <= AUSPICE_MAX ; auspice++ )
                victim->pcdata->auspice[auspice] = value;
     
        send_to_char("Done.\n\r", ch );
        return;
    }
        
   else if ( !str_cmp( arg2, "ragabash" ) )   auspice = AUSPICE_RAGABASH;
    else if ( !str_cmp( arg2, "theurge" ) )      auspice = AUSPICE_THEURGE;
   else if ( !str_cmp( arg2, "philodox" ) )    auspice = AUSPICE_PHILODOX;
    else if ( !str_cmp( arg2, "galliard" ) )      auspice = AUSPICE_GALLIARD;
   else if ( !str_cmp( arg2, "ahroun" ) )    auspice = AUSPICE_AHROUN;   
    else 
    {
        do_setauspice( ch, "" );
        return;
    }
    value = is_number( arg3 );
    
    if ( value < 0 || value > 5 )
    {
        send_to_char( "Range is 0 - 5.\n\r", ch );
        return;
    }
         victim->pcdata->auspice[auspice] = value;
         send_to_char("Done.\n\r", ch );
   return;
}


void do_settribe( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA   *victim;
    char        arg1[MAX_INPUT_LENGTH];
    char        arg2[MAX_INPUT_LENGTH];
    char        arg3[MAX_INPUT_LENGTH];
    sh_int      tribes= 0;
    sh_int      value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( IS_NPC( ch ) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
        send_to_char( "Syntax : settribe [victim] [tribe/all] [level]\n\r\n\r", ch);
        send_to_char( " Tribe being one of:\n\r", ch );
        send_to_char( "  Blackfuries, Bonegnawers, Childrenofgaia, Fianna, Getofffenris,\n\r", ch );
        send_to_char( "  Glasswalkers, Redtalons, Shadowlords, Silentstriders, Silverfangs,\n\r", ch );
        send_to_char( "  Stargazers, Uktena, or Wendigos.\n\r", ch );
        return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
     
    if ( IS_NPC(victim ) )
    {
       send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
        
    if ( !IS_WEREWOLF(victim) && !IS_ABOMINATION(victim))
    {
        send_to_char( "Only on Werewolves and Abomination!\n\r", ch );
        return;
    }
     
        
    if ( !str_cmp( arg2, "all" ) )
    {
        value = is_number( arg3 ) ? atoi( arg3 ) : -1;
    
        if ( value < 0 || value > 5 )
        {
            send_to_char( "Range is 0 - 5.\n\r", ch );
            return;
        }
    
        if ( value == 0 ) value = -1;
        
        for ( tribes = 0 ; tribes <= TRIBE_MAX ; tribes++ )
                victim->pcdata->tribes[tribes] = value;
     
        send_to_char("Done.\n\r", ch );
        return;
    }
        
   else if ( !str_cmp( arg2, "blackfuries" ) )   tribes = TRIBE_BLACK_FURIES;
    else if ( !str_cmp( arg2, "bonegnawers" ) )   tribes = TRIBE_BONE_GNAWERS;
   else if ( !str_cmp( arg2, "childrenofgaia" ) )   tribes = TRIBE_CHILDREN_OF_GAIA;
    else if ( !str_cmp( arg2, "fianna" ) )  tribes = TRIBE_FIANNA;
   else if ( !str_cmp( arg2, "getoffenris" ) )     tribes = TRIBE_GET_OF_FENRIS;
   else if ( !str_cmp( arg2, "glasswalkers" ) )     tribes = TRIBE_GLASS_WALKERS;   
   else if ( !str_cmp( arg2, "redtalons" ) )     tribes = TRIBE_RED_TALONS;   
   else if ( !str_cmp( arg2, "shadowlords" ) )     tribes = TRIBE_SHADOW_LORDS;
   else if ( !str_cmp( arg2, "silentstriders" ) )     tribes = TRIBE_SILENT_STRIDERS;
   else if ( !str_cmp( arg2, "silverfangs" ) )     tribes = TRIBE_SILVER_FANGS;
   else if ( !str_cmp( arg2, "stargazers" ) )     tribes = TRIBE_STARGAZERS;
   else if ( !str_cmp( arg2, "uktena" ) )     tribes = TRIBE_UKTENA;      
   else if ( !str_cmp( arg2, "wendigos" ) )     tribes = TRIBE_WENDIGOS;   
    else 
    {
        do_setauspice( ch, "" );
        return;
    }
    value = is_number( arg3 );
    
    if ( value < 0 || value > 5 )
    {
        send_to_char( "Range is 0 - 5.\n\r", ch );
        return;
    }
         victim->pcdata->tribes[tribes] = value;
         send_to_char("Done.\n\r", ch );
   return;
}



void randomize_object(int vnum)
{
 OBJ_DATA *obj;
 ROOM_INDEX_DATA	*rm;
 int count = 0;
 int a;

 for (a = 1; a <= 60000; a++)
 {
    if (get_room_index(a) != NULL) count++;
 }

 for (a = 1; a <= 60000; a++)
 {
    if ((rm = get_room_index(a)) != NULL && number_range(1, count * 2) == 1)
	break;
    rm = NULL;
 }

 if (rm) 
 {
     obj = create_object(get_obj_index(vnum),0);
     if (vnum >= 24 && vnum <= 59)
     {
         obj->timer = 30000;
     }
     if (!obj) return;
     obj_to_room(obj, rm);
}
else randomize_object(vnum);
return;
}

/*
void alter_corpse(OBJ_DATA *obj)
{
  CHAR_DATA *rch;
  char *p;
  char buf[MSL];
  char message[MSL];
  char newname[MSL];

  if(obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
  {
    sprintf(buf,"BUG! Alter_Corpse: %s[%d] Not a Corpse!",obj->name,obj->pIndexData->vnum);
    return;
  }

  if((obj->in_obj)|| (obj->carried_by))
    return;

  rch = obj->in_room->people;
  switch(obj->timer)
  {
    default: if(IS_SET(obj->spectype,C_TWITCHING))
			 {
             if(number_range(1,50)==5)
             {
				 sprintf(message,"%s twitches violently.",obj->short_descr);break;
			 }
             else if(number_range(1,15)==1)
             {
				 sprintf(message,"%s twitches.",obj->short_descr);break;
			 }
             else return;
			 }
             return;
   case CS1: sprintf(message,"%s begins to smell bad.",obj->short_descr);
	     sprintf(buf,"foul-smelling ");
	     break;
   case CS2: sprintf(message,"%s is growing mold.",obj->short_descr);
	     sprintf(buf,"moldy ");
	     break;
   case CS3: sprintf(message,"%s begins to crumble.",obj->short_descr);
	     sprintf(buf,"rotting ");
	     break;
   case CS4: sprintf(message,"%s is starting to be unbearable.",obj->short_descr);
	     sprintf(buf,"putrid ");
	     break;
   case CS5: sprintf(message,"Maggots begin to pour out of %s.",obj->short_descr);
	     sprintf(buf,"maggot-infested ");
	     break;
   case CS6: sprintf(buf,"maggot-infested remains of the ");
	     sprintf(message,"The maggots munch noisily on %s.",obj->short_descr);
	     break;
  }

  if(rch != NULL){
  act(message,rch,NULL,NULL,TO_ROOM);
  act(message,rch,NULL,NULL,TO_CHAR);}
  if(IS_SET(obj->spectype,C_TWITCHING))return;
  p = obj->short_descr;
  sprintf(newname,"The %s%s is lying here.",buf,p+4);
  obj->description = str_dup(newname);
}
*/

void do_dismember(CHAR_DATA *ch,char *argument)
{
  char arg[MSL];
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  argument=one_argument(argument,arg);

  if(arg[0]== '\0')
  {
    stc("Dismember whom?\n\r",ch);
    return;
  }
  if((victim = get_char_room(ch,arg))==NULL)
  {
    stc("They are not here.\n\r",ch);
    return;
  }
  if(victim->trust > ch->trust)
  {
    stc("I don't THINK so!\n\r",ch);
    return;
  }

        /* Head */
  SET_BIT(victim->loc_hp[0],LOST_EYE_L);
  SET_BIT(victim->loc_hp[0],LOST_EYE_R);
  SET_BIT(victim->loc_hp[0],LOST_EAR_L);
  SET_BIT(victim->loc_hp[0],LOST_EAR_R);
  SET_BIT(victim->loc_hp[0],LOST_NOSE);
  SET_BIT(victim->loc_hp[0],BROKEN_NOSE);
  SET_BIT(victim->loc_hp[0],BROKEN_JAW);
  SET_BIT(victim->loc_hp[0],LOST_NOSE);
  SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
  SET_BIT(victim->loc_hp[0],LOST_TONGUE);
  SET_BIT(victim->loc_hp[0],LOST_TOOTH_16);
  SET_BIT(victim->loc_hp[0],LOST_TOOTH_8);
  SET_BIT(victim->loc_hp[0],LOST_TOOTH_4);
  SET_BIT(victim->loc_hp[0],LOST_TOOTH_2);
  SET_BIT(victim->loc_hp[0],LOST_TOOTH_1);
  SET_BIT(victim->loc_hp[6],BLEEDING_HEAD);
  SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);

		/* Body */
  SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);
  SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);
  SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
  SET_BIT(victim->loc_hp[1],BROKEN_NECK);
  SET_BIT(victim->loc_hp[1],CUT_THROAT);
  SET_BIT(victim->loc_hp[1],CUT_STOMACH);
  SET_BIT(victim->loc_hp[1],CUT_CHEST);
  SET_BIT(victim->loc_hp[1],LOST_ENTRAILS);
  SET_BIT(victim->loc_hp[6],BLEEDING_STOMACH);
  SET_BIT(victim->loc_hp[6],BLEEDING_CHEST);

		/* Arms */
  SET_BIT(victim->loc_hp[2],LOST_ARM);
  SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	{
	if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
	take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
	    take_item(victim,obj);

  SET_BIT(victim->loc_hp[3],LOST_ARM);
  SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
        if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	{
	if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
	take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
	    take_item(victim,obj);
	if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
	    take_item(victim,obj);

	    /* Legs */
  SET_BIT(victim->loc_hp[4],LOST_LEG);
  SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
	if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	{
	if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
	take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	    take_item(victim,obj);

  SET_BIT(victim->loc_hp[5],LOST_LEG);
  SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
	if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	{
	if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
	take_item(victim,obj);
	}
	if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
	    take_item(victim,obj);

        make_part(victim,"arm");
        make_part(victim,"arm");
        make_part(victim,"leg");
        make_part(victim,"leg");
	make_part(victim,"nose");
	make_part(victim,"entrails");
	make_part(victim,"eyeball");
	make_part(victim,"eyeball");
	make_part(victim,"ear");
	make_part(victim,"ear");
	make_part(victim,"tongue");
        make_part(victim,"blood");
        make_part(victim,"blood");

    
	act("#RBlood#n sprays around the room, $n has dismembered $N!",ch,NULL,victim,TO_NOTVICT);
	act("You dismembered $N!",ch,NULL,victim,TO_CHAR);
	act("$n has dismembered you!",ch,NULL,victim,TO_VICT);
	victim->in_room->blood += 1000;
}


/*
 * Append a string to a file.
 */
void append_decap( CHAR_DATA *ch, char *file, char *str )
{
    FILE *fp;
    char *strtime;

    strtime                    = ctime( &current_time );

    if ( IS_NPC(ch) || str[0] == '\0' )
	return;

    fflush( fpReserve );
    fclose( fpReserve );
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
	perror( file );
	send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
	fprintf( fp, "<font color=""#C0C0C0"">[%.10s] <font color=""#008B8B""> %s: %s<BR>\n", strtime, ch->name, str );
	fflush( fp );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void do_mindflex( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch)) return;

	if (!IS_MAGE(ch))
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

    act("You focus your mind to break your bindings.",ch,NULL,NULL,TO_CHAR);
    act("$n concentrates really hard.",ch,NULL,NULL,TO_ROOM);

   if (get_curr_int(ch) < 20)
   return;

   if ( IS_EXTRA(ch, TIED_UP) )
    {
	act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR);
	act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->extra, TIED_UP);
    }
    if (is_affected(ch, gsn_web))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	affect_strip(ch, gsn_web);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }
    if (is_affected(ch, gsn_tendrils))
    {
	act("The tendrils of darkness entrapping $n break away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The tendrils of darkness entrapping you break away.\n\r",ch);
	affect_strip(ch, gsn_tendrils);
    }
    if (IS_AFFECTED(ch, AFF_SHADOW))
    {
	act("The tendrils of darkness entrapping $n break away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The tendrils of darkness entrapping you break away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_SHADOW);
    }
    if (is_affected(ch, gsn_jail))
    {
	act("The band of water entrapping $n splash to the ground.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The bands of water entrapping you splash to the ground.\n\r",ch);
	affect_strip(ch, gsn_jail);
    }
    if (IS_AFFECTED(ch, AFF_JAIL))
    {
	act("The band of water entrapping $n splash to the ground.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The bands of water entrapping you splash to the ground.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_JAIL);
    }
    if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
    {
        act("The earth surrounding $n starts to crack.",ch,NULL,NULL,TO_ROOM);
        stc("The earth surrounding you starts to crack.\n\r",ch);
        REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
        SET_BIT(ch->mflags, MAGE_EMBRACED1);
    }
    if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
    {
        act("The earth surrounding $n crumbles to dust.",ch,NULL,NULL,TO_ROOM);
        stc("The earth surrounding you crumbles to dust.\n\r",ch);
        REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
    }
    WAIT_STATE(ch,12);
    return;
}

void do_contort( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch)) return;

	if (!IS_NINJA(ch))
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

    act("You focus your training to slip out of the bonds.",ch,NULL,NULL,TO_CHAR);
    act("$n bends and twists $s body.",ch,NULL,NULL,TO_ROOM);

   if (get_curr_dex(ch) < 20)
   return;

   if ( IS_EXTRA(ch, TIED_UP) )
    {
	act("The ropes restraining you fall away.",ch,NULL,NULL,TO_CHAR);
	act("The ropes restraining $n fall away.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->extra, TIED_UP);
    }
    if (is_affected(ch, gsn_web))
    {
	act("The webbing entrapping $n falls away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you falls away.\n\r",ch);
	affect_strip(ch, gsn_web);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	act("The webbing entrapping $n falls away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you falls away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }
    if (is_affected(ch, gsn_tendrils))
    {
	act("The tendrils of darkness entrapping $n fall away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The tendrils of darkness entrapping you fall away.\n\r",ch);
	affect_strip(ch, gsn_tendrils);
    }
    if (IS_AFFECTED(ch, AFF_SHADOW))
    {
	act("The tendrils of darkness entrapping $n fall away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The tendrils of darkness entrapping you fall away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_SHADOW);
    }
    if (is_affected(ch, gsn_jail))
    {
	act("The band of water entrapping $n splash to the ground.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The bands of water entrapping you splash to the ground.\n\r",ch);
	affect_strip(ch, gsn_jail);
    }
    if (IS_AFFECTED(ch, AFF_JAIL))
    {
	act("The band of water entrapping $n splash to the ground.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The bands of water entrapping you splash to the ground.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_JAIL);
    }
    if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
    {
        act("The earth surrounding $n starts to crack.",ch,NULL,NULL,TO_ROOM);
        stc("The earth surrounding you starts to crack.\n\r",ch);
        REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
        SET_BIT(ch->mflags, MAGE_EMBRACED1);
    }
    if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
    {
        act("The earth surrounding $n crumbles to dust.",ch,NULL,NULL,TO_ROOM);
        stc("The earth surrounding you crumbles to dust.\n\r",ch);
        REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
    }
    WAIT_STATE(ch,12);
    return;
}

void do_mists( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;

    if (!is_name(ch->name,"Ktulu"))
    {
        send_to_char("Only the lord of the land can control the darkness.\n\r",ch);
		return;
    }

    if (ch->in_room == NULL)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
/*
    if ( ch->in_room->sector_type == SECT_INSIDE || !IS_OUTSIDE(ch) )
    {
	send_to_char("You can only use this power outside.\n\r",ch);
	return;
    }
*/
    act( "$n steeples $s fingers, and focuses $s mind.", ch, NULL, NULL, TO_ROOM );
    if (IS_SET(world_affects, WORLD_MIST))
    {
	send_to_char("You banish the Darkness.\n\r",ch);
	REMOVE_BIT(world_affects, WORLD_MIST);
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected != CON_PLAYING ) continue;
	    if ( (victim = d->character) == NULL ) continue;
	    if ( IS_NPC(victim) ) continue;
	    if ( victim->in_room == NULL ) continue;
	    if ( ch == victim ) continue;
	    if ( victim->in_room->sector_type == SECT_INSIDE ) continue;
	    if ( ch->in_room == victim->in_room )
		act("$n mutters a few words and the Darkness disperses.", 
ch, NULL, victim, TO_VICT);
	    else
		send_to_char("The Darkness disperses.\n\r", victim);
	}
    }
    else
    {
	send_to_char("You summon the Darkness.\n\r",ch);
	SET_BIT(world_affects, WORLD_MIST);
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected != CON_PLAYING ) continue;
	    if ( (victim = d->character) == NULL ) continue;
	    if ( IS_NPC(victim) ) continue;
	    if ( victim->in_room == NULL ) continue;
	    if ( ch == victim ) continue;
	    if ( victim->in_room->sector_type == SECT_INSIDE ) continue;
	    if ( ch->in_room == victim->in_room )
		act("$n mutters a few words and calls forth the Darkness.", ch, NULL, victim, TO_VICT);
	    else
		send_to_char("a strange black ichor rises from the ground#n.\n\r", victim);
	}
    }
    return;
}

int col_str_len(char *txt)
{
    int pos, len;

    len = 0;

    for (pos = 0; txt[pos] != '\0'; pos++)
    {
        if (txt[pos] == '{')
            pos ++;
        else
            len++;

        continue;
    }

    return len;
}

void divide_to_char(CHAR_DATA *ch)
{
send_to_char("-------------------------------------------------------------------------------\r\n",
ch); 
}

int  col_str_len        args( ( char * text ) );

void centre_text( char *text, CHAR_DATA *ch )
{
    int number;

    if ( ch == NULL ) return;
    if ( col_str_len( text ) > SCREEN_WIDTH-2 )
    {
        bug( "Centre Text - string too long!", 0 ); 
				send_to_char( text, ch );
        return;
    }
    for ( number = 0 ; number <  SCREEN_WIDTH ; number++ )
    {
        send_to_char( " ", ch );
        if ( number == ( ( SCREEN_WIDTH - col_str_len( text ) ) / 2 ) - 1)
        {
            send_to_char( text, ch );
	    stc("\n\r",ch);
            return;
        }
    }
    return;
}

void line( CHAR_DATA *ch )
{
    int number;

    if ( ch == NULL )
        return;
    for ( number = 0 ; number < SCREEN_WIDTH ; number++ )
        send_to_char( "-", ch );
    send_to_char( "\n\r", ch );
    return;
}

void line2( CHAR_DATA *ch )
{
    int number;

    if ( ch == NULL )
        return;
    for ( number = 0 ; number < SCREEN_WIDTH ; number++ )
        send_to_char( "*", ch );
    send_to_char( "\n\r", ch );
    return;
}

void cent_to_char(char *txt, CHAR_DATA *ch)
{
    int len,pos;
    char buf[MAX_STRING_LENGTH];

    len = (80 - col_str_len(txt)) / 2;
    for (pos = 0; pos < len; pos++)
    {
        buf[pos] = ' ';
    }
    buf[pos]= '\0';
    send_to_char(buf, ch);
    send_to_char(txt, ch);
    send_to_char("\n\r",ch);
}

/*
 * A quick little check to see if we have more than one
 * person connecting from the same ip.
 *
 * By Jobo, from the godwars mud Dystopia.
 */

void do_multicheck(CHAR_DATA *ch, char *argument)
{

  char *immalt = "Becker Merlin Chandri";
  DESCRIPTOR_DATA *d;
  DESCRIPTOR_DATA *d2;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;
      
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if (d->connected != CON_PLAYING) continue;  
    for ( d2 = d->next; d2 != NULL; d2 = d2->next )
    {
      if (!str_cmp(d->host, d2->host))
      {
	if (is_name(ch->name, immalt) ) return;
        if (d2->connected != CON_PLAYING) continue;
        if (d2->character == NULL || d->character == NULL) continue;
	if (IS_IMMORTAL(d->character)) continue;
        if (IS_IMMORTAL(d2->character)) continue;
	found = TRUE;
        sprintf(buf, "%s and %s are multiplaying.\n\r",d2->character->name, d->character->name);
        send_to_char(buf,ch);
      }
    }
  }
  if (!found) send_to_char("Noone is multiplaying atm.\n\r",ch);
  return;
}


void do_shell( CHAR_DATA *ch, char *argument )
{
     /*  char buf[MAX_STRING_LENGTH];

        if ( IS_NPC(ch) )
        return;

        if (IS_SET(ch->affected_by2, AF2_SHELL))
        {
        send_to_char( "You are no longer in Shell.\n\r", ch );

        sprintf(buf,"#r%s#n is no longer in the shell, prepare for a copyover!#n",ch->name);
        do_info(ch,buf);

        REMOVE_BIT(ch->affected_by2, AF2_SHELL);
        return;
        }

        else if (!IS_SET(ch->affected_by2, AF2_SHELL))
        {
        send_to_char( "You are now safe to code.\n\r", ch );
        sprintf(buf,"#r%s#n is coding, leave em be!#n",ch->name);
        do_info(ch,buf);

        SET_BIT(ch->affected_by2, AF2_SHELL);
        return;
        }

else */
return;

}

void do_classrecall( CHAR_DATA *ch, char *argument )
{
    int  hqrecall;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *location;

    if ((IS_NPC(ch) && strlen(ch->lord) < 2 && ch->level < 1000) || 
		(ch->class == CLASS_NONE) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (IS_ABOMINATION(ch) || IS_LICH(ch) || IS_SKINDANCER(ch) || IS_BAALI(ch)/*
		|| IS_BLACK_SPIRAL_DANCER(ch)*/ || IS_NEPHANDI(ch))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    
    if (ch->fight_timer > 0)
    {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
    }

  /* CL Class recalls are different */
    if (IS_VAMPIRE(ch) && ch->vampgen == 2)
	hqrecall = 18000;
    else if (IS_MAGE(ch) && ch->level == LEVEL_ARCHMAGE)
	hqrecall = 18252;
    else if (IS_DEMON(ch) && !IS_ANGEL(ch) && IS_SET(ch->act, PLR_DEMON))
	hqrecall = 18150;
    else if (IS_WEREWOLF(ch) && ch->vampgen == 2)
	hqrecall = 18050;
    else if (IS_ANGEL(ch) && IS_SET(ch->act, PLR_DEMON))
	hqrecall = 18200;
    else if (IS_NINJA(ch) && ch->vampgen == 2)
	hqrecall = 18100;

  /* Princes and what not */
    else if (IS_DEMON(ch) && IS_EXTRA(ch, EXTRA_PRINCE))
	if (IS_ANGEL(ch))         hqrecall = 18201;
        else hqrecall = 18151;
    else if (IS_NINJA(ch) && IS_EXTRA(ch, EXTRA_PRINCE))
        hqrecall = 18101;
    else if (IS_MAGE(ch) && ch->level == LEVEL_MAGE)
        hqrecall = 18251;
    else if (IS_VAMPIRE(ch) && IS_EXTRA(ch, EXTRA_PRINCE))
        hqrecall = 18001;
    else if (IS_WEREWOLF(ch) && IS_EXTRA(ch, EXTRA_PRINCE))
        hqrecall = 18051;

  /* All other class member recalls */
    else if(IS_VAMPIRE(ch))
	hqrecall = 18002;
    else if (IS_MAGE(ch))
	hqrecall = 18250;
    else if (IS_DEMON(ch) && !IS_ANGEL(ch))
	hqrecall = 18152;
    else if (IS_WEREWOLF(ch))
	hqrecall = 18052;
    else if (IS_ANGEL(ch))
	hqrecall = 18202;
    else if (IS_NINJA(ch))
	hqrecall = 18102;
    
    act( "A look of concentration crosses $n's face.", ch, NULL, NULL, TO_ROOM );

    if ( ( location = get_room_index( hqrecall ) ) == NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "You are unable to recall to your HQ.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
	if ( number_bits( 1 ) == 0 )
	{
	    WAIT_STATE( ch, 4 );
	    sprintf( buf, "You failed!\n\r" );
	    send_to_char( buf, ch );
	    return;
	}
	sprintf( buf, "You recall from combat!\n\r");
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );
    }

    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
}
