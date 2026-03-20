/*
 *   Fun stuff by, Xentoth
 */

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


void do_ritual( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    char buf [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!IS_VAMPIRE(ch))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
/*
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        sprintf(buf,"\n\rSyntax: ritual <ritual name> <object>.\n\r
        ****Available Rituals****		****Items****
        -------------------------		-------------
        Deflection				Statue
	\n\r");
        send_to_char(buf,ch);
        return;
    }
*/
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
    stc("-=[**]_[**] NAME [**]=-=[**] Rituals [**]=-=[**] OBJECT [**]_[**]=-\n\r",ch);
    stc("[+]                                                             [+]\n\r",ch);
    stc("[+]      Deflection                              Statue         [+]\n\r",ch);
    stc("[+]                                                             [+]\n\r",ch);
    stc("-=[**]_[**]=-=[**]_[**]=-=[**]-------[**]=-=[**]_[**]=-=[**]_[**]=-\n\r",ch);
    sprintf(buf,"\n\rSyntax: ritual <ritual name> <object>.\n\r");
    send_to_char(buf,ch);
    return;
    }

    if (!str_cmp(arg1,"deflection"))
    {
        if (!str_cmp(arg2,"statue"))
        {
            if ( ( obj = get_obj_carry( ch, arg2 ) ) == NULL )
    	    {
                send_to_char( "You are not carrying that object.\n\r", ch );
                return;
    	    } 

            if (obj->pIndexData->vnum != 29628)
   	    {
   	        send_to_char("Thats the wrong item for this ritual.\n\r", ch );
   		return;
   	    }

    	    if (!IS_ARM_L(ch, SLIT_WRIST) && !IS_ARM_R(ch, SLIT_WRIST))
            {
                send_to_char("First you need to SLIT your wrist.\n\r",ch);
                return;
    	    }

            if ( ch->blood[BLOOD_CURRENT] < 200 )
            {
                send_to_char("You have insufficient blood.\n\r",ch);
                return;
            }

	    if (!str_cmp(ch->name, "Mekere"))
	    {
		act("$p splinters and cracks down the middle.", ch, obj, NULL, TO_CHAR);
		send_to_char("The gods have cursed you, causing the statue to break\n\r",ch);
		act("$p splinters and cracks down the middle.", ch, obj, NULL, TO_ROOM);
		extract_obj(obj);
		ch->blood[BLOOD_CURRENT] = 1;
		do_save(ch, "");
		return;
	    }

	    if (IS_MORE(ch, MORE_DEFLECTION))
	    {
	        send_to_char("You are already affected by the deflection ritual.\n\r",ch);
                return;
	    }	

	    act("You smear some blood onto $p.", ch, obj, NULL, TO_CHAR);
            act("$n smears some blood from $s wrist onto $p.", ch, obj, NULL, TO_ROOM);
            ch->blood[BLOOD_CURRENT] -= 200;
	    act("$p glows with a faint blue light.", ch, obj, NULL, TO_CHAR);
            act("$p glows with a faint blue light.", ch, obj, NULL, TO_ROOM);
            act("Without warning $p leaps into your chest.", ch, obj, NULL, TO_CHAR);
            act("$p leaps from $n's hand and into $s chest.", ch, obj, NULL, TO_ROOM);
	    extract_obj(obj);
	    SET_BIT(ch->more, MORE_DEFLECTION);
	    send_to_char("The deflection ritual has been completed.\n\r",ch);
	    do_save(ch,"");
	    return;
        }
    }

}

void do_ritualitem( CHAR_DATA *ch, char *argument ) 
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_VAMPIRE(ch))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
	send_to_char("Please specify which ritual item to make:\n\r Statue.\n\r",ch);
	return;
    }

    if (ch->pcdata->quest < 15)
    {
        send_to_char("It costs 15 quest points to create ritual items.\n\r",ch);
        return;
    }

    if (!str_cmp(arg,"statue"  )) vnum = 29628; 
 // else if (!str_cmp(arg,"bracer"  )) vnum = 29609; 
 // else if (!str_cmp(arg,"plate"  )) vnum = 29607; 
 // else if (!str_cmp(arg,"helm" )) vnum = 29608;
    else
    {
	send_to_char("Please specify which ritual item to make:\n\r Statue.\n\r",ch);
	return;
    }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform a IMP.\n\r",ch);
	return;
    }
    obj = create_object(pObjIndex, 50);
    obj->timer = 400;
    obj_to_char(obj, ch);
    ch->pcdata->quest -= 15;obj->points += 15;
    free_string( obj->questmaker );
    obj->questmaker = str_dup( ch->name );
    free_string( obj->questowner );
    obj->questowner = str_dup( ch->name );
    
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}
