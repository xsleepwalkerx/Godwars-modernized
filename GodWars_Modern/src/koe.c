/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
 *  Fallen Empires public release                                          *
 *  (c)2000  'MereK' (andrew smith) and 'Aria' (Janis Marler)              *
 *  Be sure to read and abide by FE.doc in the docs directory.             *
 *  Fallen Empires can be found at xerxes.mudhaven.com port 6000           *
 *  To contact the admin: incarnat@mudhaven.com                            *
 ***************************************************************************/

/* Kindred of the East class based on White Wolf Entertainment WoD series */
/* Kindred of the East class coded by: MereK of Fallen Empires & BOTI    */
/* also coded by Kira of BOTI        */
/*
 * Note from MereK: by no means do i claim full coding credits for many of
 * these powers, as you can see much of it resembles previously known GW
 * powers, much credit must also go to previous GW coders. Hey, at least i'm
 * honest and attempted to make some of the powers look new ;) Note from
 * Kira: <quote> umm...yeah what he said :P
 */
/*
 * Modified to current glory and Ethereal Mist regiment courtesy of Ktulu
 */
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
//#include "player.h"

//KoE paths
void do_path(CHAR_DATA * ch, char *argument)
{
    char            arg1[MAX_INPUT_LENGTH];
    char            arg2[MAX_INPUT_LENGTH];
    char            buf[MAX_STRING_LENGTH];
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch))
	return;

    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (arg1[0] == '\0' && arg2[0] == '\0') 
    {
	sprintf(buf, "Path: Yin(%d), Yang (%d)\n\r Hun(%d), Po(%d).\n\r",
		ch->pcdata->powers[KOE_PYIN],
		ch->pcdata->powers[KOE_PYANG],
		ch->pcdata->powers[KOE_HUN],
		ch->pcdata->powers[KOE_PO]);
	send_to_char(buf, ch);
	return;
    }
    if (arg2[0] == '\0') 
    {
	if (!str_cmp(arg1, "Yin")) 
	{
	    send_to_char("Follow the path of perception and awareness\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYIN] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYIN] > 0)
		send_to_char("The Hunting Snake - judge your opponents before you attack. (huntingsnake)\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYIN] > 1)
		send_to_char("Piercing the Shroud - enchanced vision into the shroud.(piercetheshroud)\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYIN] > 2)
		send_to_char("Alertness of the Baboon - Sense that which is unseen.(automatic when used with Pierce the Shroud)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Yang")) 
	{
	    send_to_char("Follow the path of healing and perception.\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYANG] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYANG] > 0)
		send_to_char("Yang Healing - Heal yourself.(yangheal)\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYANG] > 1)
		send_to_char("Lifesight - Summon the chi energy to see everything.(lifesight)\n\r", ch);
	    if (ch->pcdata->powers[KOE_PYANG] > 2)
		send_to_char("N/A.\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Hun")) 
	{
	    send_to_char("Follow the path of control and higher self.\n\r", ch);
	    if (ch->pcdata->powers[KOE_HUN] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_HUN] > 0)
		send_to_char("Sharpened Senses - Enhanced senses.(SharpenSenses\n\r", ch);
	    if (ch->pcdata->powers[KOE_HUN] > 1)
		send_to_char("Spirit Talk.(spirittalk)\n\r", ch);
	    if (ch->pcdata->powers[KOE_HUN] > 2)
		send_to_char("Frenzy - Envoke the demon within.(Unleash)\n\r", ch);
	    if (ch->pcdata->powers[KOE_HUN] > 3)
		send_to_char("Tempering the P'o.(tempering)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Po")) 
	{
	    send_to_char("Follow the path of the inner demon\n\r", ch);
	    if (ch->pcdata->powers[KOE_PO] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_PO] > 0)
		send_to_char("Demon Fangs.(demonfangs)\n\r", ch);
	    if (ch->pcdata->powers[KOE_PO] > 1)
		send_to_char("Resistance - Resist command, order and charm by summoning the beast(N/A)\n\r", ch);
	    if (ch->pcdata->powers[KOE_PO] > 2)
		send_to_char("Shadow Nature - Summon your inner beast to consume you.(shadownature and shadowrelease)\n\r", ch);
	    return;
	}
	sprintf(buf, "Path: Yin(%d), Yang (%d)\n\r Hun(%d), Po(%d).\n\r",
		ch->pcdata->powers[KOE_PYIN],
		ch->pcdata->powers[KOE_PYANG],
		ch->pcdata->powers[KOE_HUN],
		ch->pcdata->powers[KOE_PO]);
	send_to_char(buf, ch);
	return;
    }
    if (!str_cmp(arg2, "improve")) 
    {
	int             improve;
	int             cost;
	int             max;

	if (!str_cmp(arg1, "Yin")) 
	{
	    improve = KOE_PYIN;
	    max = 3;
	    if (ch->pcdata->powers[KOE_PYANG] > 10
		|| ch->pcdata->powers[KOE_HUN] > 10
		|| ch->pcdata->powers[KOE_PO] > 10) 
		{
		send_to_char("You already follow a different path.\n\r", ch);
		return;
	}
    } 
	else if (!str_cmp(arg1, "Yang")) 
	{
	    improve = KOE_PYANG;
	    max = 2;
	    if (ch->pcdata->powers[KOE_PYIN] > 10
		|| ch->pcdata->powers[KOE_PO] > 10
		|| ch->pcdata->powers[KOE_HUN] > 10) 
	    { 
		send_to_char("You already follow a different path.\n\r", ch);
		return;
	    }
	} 
	else if (!str_cmp(arg1, "Hun")) 
        {
	    improve = KOE_HUN;
	    max = 4;
	    if (ch->pcdata->powers[KOE_PYANG] > 10
		|| ch->pcdata->powers[KOE_PO] > 10
		|| ch->pcdata->powers[KOE_PYIN] > 10) 
		{
		send_to_char("You already follow a different path.\n\r", ch);
		return;
	        }
	} 
	else if (!str_cmp(arg1, "Po")) 
	{
	    improve = KOE_PO;
	    max = 3;
	    if ((ch->pcdata->powers[KOE_PYANG] > 10
		 || ch->pcdata->powers[KOE_HUN] > 10
		 || ch->pcdata->powers[KOE_PYIN] > 10) && !IS_POLYAFF(ch, POLY_SHADOWDEMON)) 
        {
		send_to_char("You already follow a different path.\n\r", ch);
		return;
	}
} 
        else 
        {
	    send_to_char("Path: Yin, Yang\n\r Hun, Po ", ch);
	    return;
	}

	cost = (ch->pcdata->powers[improve] + 1) * 10;
	arg1[0] = UPPER(arg1[0]);

	if (ch->pcdata->powers[improve] >= max) 
	{
	    sprintf(buf, "You have already achieved the highest know %s level.\n\r", arg1);
	    send_to_char(buf, ch);
	    return;
	}
	if (cost > ch->practice) 
	{
	    sprintf(buf, "It costs you %d primal to advance your %s path.\n\r", cost, arg1);
	    send_to_char(buf, ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf, "You improve your %s path.\n\r", arg1);
	send_to_char(buf, ch);
    } 
    else
	send_to_char("To improve your training, type: path <yin/yang/hun/po> improve.\n\r", ch);
    return;
}

//KoE Disciplines
void do_kdiscipline(CHAR_DATA * ch, char *argument)
{
    char            arg1[MAX_INPUT_LENGTH];
    char            arg2[MAX_INPUT_LENGTH];
    char            buf[MAX_STRING_LENGTH];
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch))
	return;

    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (arg1[0] == '\0' && arg2[0] == '\0') 
    {
	sprintf(buf, "Discipline: Equilibrum(%d), Tapestry (%d)\n\rYang Prana(%d), Yin Prana(%d)\n\rBlack Wind(%d)\n\r",
		ch->pcdata->powers[KOE_EQUILIBRUM],
		ch->pcdata->powers[KOE_TAPESTRY],
		ch->pcdata->powers[KOE_DYANG],
		ch->pcdata->powers[KOE_DYIN],
		ch->pcdata->powers[KOE_BLACKWIND]);
	send_to_char(buf, ch);
	return;
    }
    if (arg2[0] == '\0') 
    {
	if (!str_cmp(arg1, "Equilibrum")) 
	{
	    send_to_char("Equilibrum: the regulation of Chi in the body\n\r", ch);
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 0)
		send_to_char("Master Flow - Raises Yin Chi gained.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 1)
		send_to_char("Adjust Balance - Adjust the balance of Chi in your enemies bodies for dabilitating affects.(adjustbalance)\n\r", ch);
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 2)
		send_to_char("Shift Balance - Shift the balance of Chi within yourself for advanced mobility.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 3)
		send_to_char("Chi Interupt - Distrupt and even stop the flow of Chi in enemies.(interupt)\n\r", ch);
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 4)
		send_to_char("Chi Master -  Control the Chi of your enemies..and control him. (master)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Tapestry")) 
	{
	    send_to_char("Tapestry: Manipulation of Reality and Area.\n\r", ch);
	    if (ch->pcdata->powers[KOE_TAPESTRY] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_TAPESTRY] > 0)
		send_to_char("Ghost Call - Summon Guardian spirits to your side.(NOT IN ATM)\n\r", ch);
	    if (ch->pcdata->powers[KOE_TAPESTRY] > 1)
		send_to_char("Chi Ward - Ward yourself against harm by binding mystical sigils on your body (advanced dodge and parry)\n\r", ch);
	    if (ch->pcdata->powers[KOE_TAPESTRY] > 2)
		send_to_char("Chi Shape - Summon the area's Chi energy to create a fearful weapon.(chiweapon)\n\r", ch);
	    if (ch->pcdata->powers[KOE_TAPESTRY] > 3)
		send_to_char("Ride the Dragon - Ride the lines of Chi to other places and beings.(ridethedragon)\n\r", ch);
	    if (ch->pcdata->powers[KOE_TAPESTRY] > 4)
		send_to_char("Chi Rift - Disrupt the natural Chi of the area and summon the raw Chi for random results.(chirift)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Yang")) 
	{
	    send_to_char("Yang Prana: Motion and Channeling of Chi.\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYANG] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYANG] > 0)
		send_to_char("Principle of Motion - Iajustsu, Mastery of Self.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYANG] > 1)
		send_to_char("Yang Mantle - Study of your enemies Yang aura, as well as summoning of a Yang forged weapon.(yangmantle/chiweapon)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYANG] > 2)
		send_to_char("Dragon Dance - Advanced Combat Techniques with Yang control.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYANG] > 3)
		send_to_char("Eight Fold Yang Mantle - Control flow of Chi to conceal yourself.\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYANG] > 4)
		send_to_char("Semblance of the Scarlet Queen - Become an living avatar of the Scarlet Queen.(scarletqueen)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Yin")) 
	{
	    send_to_char("Yin Prana - Tap and Control of Chi.\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYIN] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYIN] > 0)
		send_to_char("Shrouded Moon - Atune yourself to the shadows around you to be undetectable.(shrouded)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYIN] > 1)
		send_to_char("Yin Mantle - create a powerful Yin weapon.(chiweapon)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYIN] > 2)
		send_to_char("Bone Dance - Charm ritual for most entities and beasts.(bonedance)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYIN] > 3)
		send_to_char("Eightfold Yin Mantle - Summon Yin to protect you(automatic)\n\r\n\rSummon a Shadow Companion.(shadowcompanion)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DYIN] > 4)
		send_to_char("Semblance of the Ebon Dragon - Become an living avatar of the Ebon Dragon.(ebon)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Blackwind")) 
	{
	    send_to_char("Black Wind - Summoning P'o for enhanced speed\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLACKWIND] < 1)
		send_to_char("You have not advanced in this path yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLACKWIND] > 0)
		send_to_char("Dance of the Twister - Advanced Dodge.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLACKWIND] > 1)
		send_to_char("Fury of the Storm - Extra attack.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLACKWIND] > 2)
		send_to_char("Swiftness of the Gale - Advanced Parry.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLACKWIND] > 3)
		send_to_char("Wrath of the Hurricane - Advanced Damage.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLACKWIND] > 4)
		send_to_char("Wind Walker - Travel quickly to your prey.(windwalker)\n\r", ch);
	    return;
	}
	sprintf(buf, "Discipline: Equilibrum(%d), Tapestry (%d),  Yang Prana(%d), Yin Prana(%d), Black Wind(%d)\n\r.",
		ch->pcdata->powers[KOE_EQUILIBRUM],
		ch->pcdata->powers[KOE_TAPESTRY],
		ch->pcdata->powers[KOE_DYANG],
		ch->pcdata->powers[KOE_DYIN],
		ch->pcdata->powers[KOE_BLACKWIND]);

	send_to_char(buf, ch);
	return;
    }
    if (!str_cmp(arg2, "improve")) 
    {
	int             improve;
	int             cost;
	int             max;

	if (!str_cmp(arg1, "Equilibrum")) 
	{
	    improve = KOE_EQUILIBRUM;
	    max = 5;
	    if (ch->pcdata->powers[KOE_TAPESTRY] > 10
		|| ch->pcdata->powers[KOE_DYANG] > 10
		|| ch->pcdata->powers[KOE_DYIN] > 10
		|| ch->pcdata->powers[KOE_BLACKWIND] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n\r ", ch);
                return;
	 }
    } 
	else if (!str_cmp(arg1, "Tapestry")) 
	{
	    improve = KOE_TAPESTRY;
	    max = 5;
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 10
		|| ch->pcdata->powers[KOE_DYANG] > 10
		|| ch->pcdata->powers[KOE_DYIN] > 10
		|| ch->pcdata->powers[KOE_BLACKWIND] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n \r ", ch);
		return;
	        }
	} 
	else if (!str_cmp(arg1, "Yang")) 
	{
	    improve = KOE_DYANG;
	    max = 5;
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 10
		|| ch->pcdata->powers[KOE_TAPESTRY] > 10
		|| ch->pcdata->powers[KOE_DYIN] > 10
		|| ch->pcdata->powers[KOE_BLACKWIND] > 10) 
		{
		send_to_char("You already follow a different path.\n\r",
			     ch);
		return;
	        }
	} 
        else if (!str_cmp(arg1, "Yin")) 
	{
	    improve = KOE_DYIN;
	    max = 5;
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 10
		|| ch->pcdata->powers[KOE_TAPESTRY] > 10
		|| ch->pcdata->powers[KOE_DYANG] > 10
		|| ch->pcdata->powers[KOE_BLACKWIND] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n\r", ch);
			return;
	        }
	} 
	else if (!str_cmp(arg1, "Blackwind")) 
	{
	    improve = KOE_BLACKWIND;
	    max = 5;
	    if (ch->pcdata->powers[KOE_EQUILIBRUM] > 10
		|| ch->pcdata->powers[KOE_TAPESTRY] > 10
		|| ch->pcdata->powers[KOE_DYANG] > 10
		|| ch->pcdata->powers[KOE_DYIN] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n\r", ch);
			return;
	        }
	} 
	else 
	{
	    send_to_char("Discipline: Equilibrum, Tapestry\n\r Yang Prana, Yin Prana, Black Wind ", ch);
	    return;
	}
	cost = (ch->pcdata->powers[improve] + 1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if (ch->pcdata->powers[improve] >= max) 
        {
	    sprintf(buf, "You have already mastered the %s discipline.\n\r", arg1);
	    send_to_char(buf, ch);
	    return;
	}
	if (cost > ch->practice) 
	{
	    sprintf(buf, "It costs you %d primal to advance your %s discipline.\n\r", cost, arg1);
	    send_to_char(buf, ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf, "You improve your %s discipline.\n\r", arg1);
	send_to_char(buf, ch);
    } 
    else send_to_char("To improve your training, type: kdiscipline <equilibrium/tapestry/yin/yang/black wind> improve.\n\r", ch);
    return;
}


//KoE Shintai
void do_shintai(CHAR_DATA * ch, char *argument)
{
    char            arg1[MAX_INPUT_LENGTH];
    char            arg2[MAX_INPUT_LENGTH];
    char            buf[MAX_STRING_LENGTH];
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch))
	return;

    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (arg1[0] == '\0' && arg2[0] == '\0') 
    {
	sprintf(buf, "Shintai: Blood(%d), Bone(%d)\n\rJade(%d), Flesh(%d)\n\rGhost(%d)\n\r",
		ch->pcdata->powers[KOE_BLOOD],
		ch->pcdata->powers[KOE_BONE],
		ch->pcdata->powers[KOE_JADE],
		ch->pcdata->powers[KOE_FLESH],
		ch->pcdata->powers[KOE_GHOST]);
	send_to_char(buf, ch);
	return;
    }
    if (arg2[0] == '\0') 
    {
	if (!str_cmp(arg1, "Blood")) 
	{
	    send_to_char("Blood Shintai: Control the ebb and flow of Chi throught the body\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLOOD] < 1)
		send_to_char("You have not advanced in this Shintai yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLOOD] > 0)
		send_to_char("Permeate - Permeate your body with blood to look more fearsome in battle.(permeate)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLOOD] > 1)
		send_to_char("Blood Atemi - By controlling blood flow, you can facilitate healing faster.(bloodatemi)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLOOD] > 2)
		send_to_char("Flow Like Blood - Advanced Dodge.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLOOD] > 3)
		send_to_char("Blood Lash - Use Chi to coagulate blood into a lethal claws.(bloodlash)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BLOOD] > 4)
		send_to_char("Blood Awakening - Boil the blood withing your enemies bodies.(bloodawakening)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Bone")) 
	{
	    send_to_char("Bone Shintai: Augmentation of the body.\n\r", ch);
	    if (ch->pcdata->powers[KOE_BONE] < 1)
		send_to_char("You have not advanced in this Shintai yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_BONE] > 0)
		send_to_char("Corpse Skin - Resistance to damage from fire.(corpseskin)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BONE] > 1)
		send_to_char("White tiger Corpse - Become transparent and invisible.(whitetigercorpse)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BONE] > 2)
		send_to_char("Bone Obedience - Manipulate bone structure to create weapons.(boneblade/bonearmor/visage))\n\r", ch);
	    if (ch->pcdata->powers[KOE_BONE] > 3)
		send_to_char("Five Poison Cloud - Exhale a cloud of destructive concentrated Yin.(poisoncloud)\n\r", ch);
	    if (ch->pcdata->powers[KOE_BONE] > 4)
		send_to_char("Ch'ing Shih - Transform into a fearsome beast.(chingshih)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Jade")) 
	{
	    send_to_char("Jade Shintai - Harness Chi and become a conduit for the Earths Chi flows.\n\r", ch);
	    if (ch->pcdata->powers[KOE_JADE] < 1)
		send_to_char("You have not advanced in this Shintai yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_JADE] > 0)
		send_to_char("Stand on the Dragon's Back - Solid foundation, punch immune hurl immune.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_JADE] > 1)
		send_to_char("Tread the Thrasing Dragon - Advanced Dodge.(automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_JADE] > 2)
		send_to_char("Placate the Earth Dragon - earthly embrace.(placate)\n\r", ch);
	    if (ch->pcdata->powers[KOE_JADE] > 3)
		send_to_char("Harness the Dragons Breath - Travel using flows of Chi.(harness)\n\r", ch);
	    if (ch->pcdata->powers[KOE_JADE] > 4)
		send_to_char("Jade servant of the August Personage - Transform into various forms of the Jade Servant.(jade)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Flesh")) 
	{
	    send_to_char("Flesh Shintai - Flesh Transformation.\n\r", ch);
	    if (ch->pcdata->powers[KOE_FLESH] < 1)
		send_to_char("You have not advanced in this Shintai yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_FLESH] > 0)
		send_to_char("Limb Extention - extend your fingers into terrible claws.(extended)\n\r", ch);
	    if (ch->pcdata->powers[KOE_FLESH] > 1)
		send_to_char("Detatchment - Remove your eye to spy for you.(detatchement)\n\r", ch);
	    if (ch->pcdata->powers[KOE_FLESH] > 2)
		send_to_char("Lotus Cloud - Expell a euphoric cloud to placate enemies.(lotuscloud)\n\r", ch);
	    if (ch->pcdata->powers[KOE_FLESH] > 3)
		send_to_char("Pelesit - Living tattoo's to aid you in battle.(pelesit)\n\r", ch);
	    if (ch->pcdata->powers[KOE_FLESH] > 4)
		send_to_char("Ten Thousand Forms - Mold your flesh to resemble any animal or humanoid.(thenthousandforms)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Ghost")) 
	{
	    send_to_char("Ghost Flame Shintai - \n\r", ch);
	    if (ch->pcdata->powers[KOE_GHOST] < 1)
		send_to_char("You have not advanced in this Shintai yet.\n\r", ch);
	    if (ch->pcdata->powers[KOE_GHOST] > 0)
		send_to_char("Goblin Face - A fearsome beastly face with tusks to intimidate enemies.(goblinface)\n\r", ch);
	    if (ch->pcdata->powers[KOE_GHOST] > 1)
		send_to_char("Goblin Spark - Summon a flash of Chi to light up the area.(goblinspark)\n\r", ch);
	    if (ch->pcdata->powers[KOE_GHOST] > 2)
		send_to_char("Goblin Shapes - Create lights and confusion in the room.(goblinshapes)\n\r", ch);
	    if (ch->pcdata->powers[KOE_GHOST] > 3)
		send_to_char("Goblin Scorch - Expel a blast of burning Chi.(goblinscorch)\n\r", ch);
	    if (ch->pcdata->powers[KOE_GHOST] > 4)
		send_to_char("Goblin Lantern - Transform into Hima form.(hima)\n\r", ch);
	    return;
	}
	sprintf(buf, "Shintai: Blood(%d), Bone(%d), Jade(%d), Flesh(%d), Ghost(%d)\n\r",
		ch->pcdata->powers[KOE_BLOOD],
		ch->pcdata->powers[KOE_BONE],
		ch->pcdata->powers[KOE_JADE],
		ch->pcdata->powers[KOE_FLESH],
		ch->pcdata->powers[KOE_GHOST]);

	send_to_char(buf, ch);
	return;
    }
    if (!str_cmp(arg2, "improve")) 
    {
	int             improve;
	int             cost;
	int             max;

	if (!str_cmp(arg1, "Blood")) 
	{
	    improve = KOE_BLOOD;
	    max = 5;
	    if (ch->pcdata->powers[KOE_BONE] > 0
		|| ch->pcdata->powers[KOE_JADE] > 10
		|| ch->pcdata->powers[KOE_FLESH] > 10
		|| ch->pcdata->powers[KOE_GHOST] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n\r ", ch);
		return;
	        }
	} 
	else if (!str_cmp(arg1, "Bone")) 
	{
	    improve = KOE_BONE;
	    max = 5;
	    if (ch->pcdata->powers[KOE_BLOOD] > 10
		|| ch->pcdata->powers[KOE_JADE] > 10
		|| ch->pcdata->powers[KOE_FLESH] > 10
		|| ch->pcdata->powers[KOE_GHOST] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n\r ", ch);
		return;
	        }
	} 
	else if (!str_cmp(arg1, "Jade")) 
	{
	    improve = KOE_JADE;
	    max = 5;
	    if (ch->pcdata->powers[KOE_BLOOD] > 10
		|| ch->pcdata->powers[KOE_BONE] > 10
		|| ch->pcdata->powers[KOE_FLESH] > 10
		|| ch->pcdata->powers[KOE_GHOST] > 10) 
		{
		send_to_char("You already follow a different path.\n\r",ch);
		return;
	        }
	} 
	else if (!str_cmp(arg1, "Flesh")) 
	{
	    improve = KOE_FLESH;
	    max = 5;
	    if (ch->pcdata->powers[KOE_BLOOD] > 10
		|| ch->pcdata->powers[KOE_BONE] > 10
		|| ch->pcdata->powers[KOE_JADE] > 10
		|| ch->pcdata->powers[KOE_GHOST] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n\r ", ch);
	        return;
	        }
	} 
	else if (!str_cmp(arg1, "Ghost")) 
	{
	    improve = KOE_GHOST;
	    max = 5;
	    if (ch->pcdata->powers[KOE_BLOOD] > 10
		|| ch->pcdata->powers[KOE_BONE] > 10
		|| ch->pcdata->powers[KOE_FLESH] > 10
		|| ch->pcdata->powers[KOE_JADE] > 10) 
		{
		send_to_char("You are already devoted to a discipline.\n\r    ", ch);
		return;
	    }
	} 
	else 
	{
	    send_to_char("Shintai: Blood, Bone, Jade, Flesh, Ghost.\n\r", ch);
	    return;
	}

	cost = (ch->pcdata->powers[improve] + 1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if (ch->pcdata->powers[improve] >= max) 
	{
	    sprintf(buf, "You have already mastered the %s shintai.\n\r", arg1);
	    send_to_char(buf, ch);
	    return;
	}
	if (cost > ch->practice) 
	{
	    sprintf(buf, "It costs you %d primal to advance your %s shintai.\n\r", cost, arg1);
	    send_to_char(buf, ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf, "You improve your %s shintai.\n\r", arg1);
	send_to_char(buf, ch);
        } 
        else send_to_char("To improve your training, type: shintai <Blood, Bone, Jade, Flesh, Ghost> improve.\n\r", ch);
        return;
}
//KotE Noble path

void do_noble(CHAR_DATA * ch, char *argument)
{
    char            arg1[MAX_INPUT_LENGTH];
    char            arg2[MAX_INPUT_LENGTH];
    char            buf[MAX_STRING_LENGTH];
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch))
	return;

    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (IS_KOE(ch)) 
    {
	send_to_char("Not Available yet.\n\r", ch);
	return;
    }
    if (arg1[0] == '\0' && arg2[0] == '\0') 
    {
	sprintf(buf, "Noble Path: Chi' Muh(%d), Internalize (%d)\n\r Polishing the Dragon Scales(%d).\n\r",
		ch->pcdata->powers[KOE_CHIMUH],
		ch->pcdata->powers[KOE_INTERNALIZE],
		ch->pcdata->powers[KOE_DRAGONSCALES]);
	send_to_char(buf, ch);
	return;
    }
    if (arg2[0] == '\0') 
    {
	if (!str_cmp(arg1, "chimuh")) 
	{
	    send_to_char("Practice the ancient power of dragon tears.\n\r", ch);
	    if (ch->pcdata->powers[KOE_CHIMUH] < 1)
		send_to_char("You have not been trained in this noble path.\n\r", ch);
	    if (ch->pcdata->powers[KOE_CHIMUH] > 0)
		send_to_char("Chi Sight - Advanced Ghost Sight, LifeSight or Sharpen Senses. (chisight)\n\r", ch);
	    if (ch->pcdata->powers[KOE_CHIMUH] > 1)
		send_to_char("Dragon Ward - Channel Chi into a powerful ward of protection. (dragonward)\n\r", ch);
	    if (ch->pcdata->powers[KOE_CHIMUH] > 2)
		send_to_char("Purification - Cleanse the soul of bad Chi from befouled Dragon Nests.\n\r   When used by evil injects poison with bite.\n\r", ch);
	    if (ch->pcdata->powers[KOE_CHIMUH] > 3)
		send_to_char("Spirit Eating - Devour the spirit of fallen enemies (automatic with decap).\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "Internalize")) 
	{
	    send_to_char("The Internalize Discipline: centers on inner focus and control.\n\r", ch);
	    if (ch->pcdata->powers[KOE_INTERNALIZE] < 1)
		send_to_char("You have not been trained in this noble path.\n\r", ch);
	    if (ch->pcdata->powers[KOE_INTERNALIZE] > 0)
		send_to_char("No Mind - Appear blank to those who wish to read your aura. (mindblank)\n\r", ch);
	    if (ch->pcdata->powers[KOE_INTERNALIZE] > 1)
		send_to_char("Blood Yoga - Recharge your health through meditation. (bloodyoga)\n\r", ch);
	    if (ch->pcdata->powers[KOE_INTERNALIZE] > 2)
		send_to_char("Iron Soul - Advanced concentration during battle for excelled abilities. (automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_INTERNALIZE] > 3)
		send_to_char("Harmony of the All - More advanced concentration during battle. (automatic)\n\r", ch);
	    return;
	} 
	else if (!str_cmp(arg1, "DRAGONSCALES")) 
	{
	    send_to_char("Polishing the Dragon's Scales: Effect small Chi adjustments in their immediate vicinity.\n\r", ch);
	    if (ch->pcdata->powers[KOE_DRAGONSCALES] < 1)
		send_to_char("You have not been trained in this noble path.\n\r", ch);
	    if (ch->pcdata->powers[KOE_DRAGONSCALES] > 0)
		send_to_char("Trace the Dragon's Blood - Detect if there is a Dragon's Nest in the area. (tracethedragon)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DRAGONSCALES] > 1)
		send_to_char("Behold The Spirit's Doorway - Detect places of weakness in the Wall. (automatic)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DRAGONSCALES] > 2)
		send_to_char("Imbuing the Jade - Create a statue of jade to help you cross the Wall at places of weakness. (doorwaystatue)\n\r", ch);
	    if (ch->pcdata->powers[KOE_DRAGONSCALES] > 3)
		send_to_char("Right of Supplication - Pass the Wall without the aid of a jade statue. (supplication)\n\r", ch);
	    return;
	}
	sprintf(buf, "Noble Disciplines: Chi' Muh(%d), Internalize (%d)\n\r Polishing the Dragon Stare(%d).\n\r",
		ch->pcdata->powers[KOE_CHIMUH],
		ch->pcdata->powers[KOE_INTERNALIZE],
		ch->pcdata->powers[KOE_DRAGONSCALES]);
	send_to_char(buf, ch);
	return;
    }
    if (!str_cmp(arg2, "improve")) 
    {
	int             improve;
	int             cost;
	int             max;

	if (!str_cmp(arg1, "chimuh")) 
	{
	    improve = KOE_CHIMUH;
	    max = 4;
	    if (ch->pcdata->powers[KOE_INTERNALIZE] > 0
		|| ch->pcdata->powers[KOE_DRAGONSCALES] > 0) 
		{
		send_to_char("You can only be trained in one noble path.\n\r", ch);
		return;
	        }
	} 
	else if (!str_cmp(arg1, "internalize")) 
	{
	    improve = KOE_INTERNALIZE;
	    max = 4;
	    if (ch->pcdata->powers[KOE_CHIMUH] > 0
		|| ch->pcdata->powers[KOE_DRAGONSCALES] > 0) 
		{
		send_to_char("You can only be trained in one noble path.\n\r", ch);
		return;
	        }
	} 
	else if (!str_cmp(arg1, "dragonscales")) 
	{
	    improve = KOE_DRAGONSCALES;
	    max = 4;
	    if (ch->pcdata->powers[KOE_INTERNALIZE] > 0
		|| ch->pcdata->powers[KOE_CHIMUH] > 0) 
		{
		send_to_char("You can only be trained in one noble path.\n\r", ch);
		return;
	        }
	} 
	else 
	{
	    send_to_char("Noble Path: Chi' Muh, Internalize, Polishing the Dragon Scales", ch);
	    return;
	}

	cost = (ch->pcdata->powers[improve] + 1) * 10;
	arg1[0] = UPPER(arg1[0]);

	if (ch->pcdata->powers[improve] >= max) 
	{
	    sprintf(buf, "You have mastered the highest level of the noble %s.\n\r", arg1);
	    send_to_char(buf, ch);
	    return;
	}
	if (cost > ch->practice) 
	{
	    sprintf(buf, "It costs you %d primal to advance in your %s noble path.\n\r", cost, arg1);
	    send_to_char(buf, ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf, "You improve your %s noble path.\n\r", arg1);
	send_to_char(buf, ch);
    } 
    else send_to_char("To improve your training, type: noblepath (chimuh/internalize/dragonscales) improve.\n\r", ch);
    return;
}



//KoE Shintai powers to follow unless stated otherwise


// blood shintai - permeate, atemi, flow like blood(fight.c)
// -bloodlash, blood awaken

    void do_permeate(CHAR_DATA * ch, char *argument)
{
    char            arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);
    if (IS_NPC(ch))
	return;
    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[KOE_BLOOD] < 1) 
    {
	send_to_char("You are not proficent enough in the Blood Shintai to use this.\n\r", ch);
	return;
    }
    if (ch->position == POS_FIGHTING) 
    {
	send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PLACATE)) 
    {
	send_to_char("You are unable to use this while under protection.\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PERMEATE)) 
    {
	send_to_char("Your body resumes its regular size.\n\r", ch);
	act("$n body deflates back to normal size before your very eyes.\n\r", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], AF2_PERMEATE);
	return;
    }
    if (ch->pcdata->power[NINJAKI] < 5) 
    {
	send_to_char("You do not have enough Chi to do this\n\r", ch);
	return;
    }
    send_to_char("Your body swells and becomes horribly bloated.\n\r", ch);
    act("$n's body swells and becomes a horrifying monster.\n\r", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[KOE_CURRENT], AF2_PERMEATE);
    ch->pcdata->powers[NINJAKI] -= 5;
    return;
}

void do_atemi(CHAR_DATA * ch, char *argument)
{
    if (IS_NPC(ch))
	return;

    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[KOE_BLOOD] < 2) 
    {
	send_to_char("You are not proficent enough in the Blood Shintai to use this.\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[NINJAKI] < 20) 
    {
	send_to_char("You do not have enough Chi to do this\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PLACATE)) 
    {
	send_to_char("You are unable to use this while under protection.\n\r", ch);
	return;
    }
    if (ch->position == POS_FIGHTING) 
    {
	send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	return;
    }
    if (ch->hit > ch->max_hit)
	ch->hit = ch->max_hit;
    {
	send_to_char("The Chi flows through you healing your wounds.\n\r", ch);
	act("$n summons the blood atemi Shintai and begins healing.", ch, NULL, NULL, TO_ROOM);
	WAIT_STATE(ch, 13);
	ch->pcdata->powers[NINJAKI] -= 20;
	ch->hit = ch->hit + 500;
	return;
    }
    return;
}

//Vorpal claws w / o weaponholding, but gives parry.Add appropriate parts to fight.c and act_obj.c as well
void do_bloodlash(CHAR_DATA * ch, char *argument)
{
    //char          buf[MAX_INPUT_LENGTH];
    OBJ_DATA       *obj;
    char            arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);


    if (IS_NPC(ch))
	return;
    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[KOE_BLOOD] < 4) 
    {
	send_to_char("You are not proficent enough in the Blood Shintai to use this.\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PLACATE)) 
    {
	send_to_char("You are unable to use this while under protection.\n\r", ch);
	return;
    }
    if (ch->position == POS_FIGHTING) 
    {
	send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	return;
    }
    //if (IS_VAMPAFF(ch, VAM_CLAWS))
	if (0)
	{
	send_to_char("You are unable to summon this power while you have claws extended\n\r", ch);
	return;
	}

    if (IS_KOEAFF(ch, KOE_BLOODLASH)) 
    {
	send_to_char("Your blood claws flow back into your wrist.\n\r", ch);
	act("$n's blood claws flows back into his wrist.\n\r", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_BLOODLASH);
	REMOVE_BIT(ch->vampaff, VAM_CLAWS);
	REMOVE_BIT(ch->itemaffect, ITEMA_VORPAL);
	//ch->pcdata->powers[NINJAKI] -= 20;
	return;
    }
    if (ch->pcdata->powers[NINJAKI] < 20) 
    {
	send_to_char("You do not have enough Chi to do this\n\r", ch);
	return;
    }
    /* drops weapon from hands */
    if ((obj = get_eq_char(ch, WEAR_WIELD)) != NULL && !IS_SET(obj->spectype, SITEM_WOLFWEAPON)) 
    {
	act("$p drops from your right hand.", ch, obj, NULL, TO_CHAR);
	act("$p drops from $n's right hand.", ch, obj, NULL, TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    if ((obj = get_eq_char(ch, WEAR_HOLD)) != NULL && !IS_SET(obj->spectype, SITEM_WOLFWEAPON)) 
    {
	act("$p drops from your left hand.", ch, obj, NULL, TO_CHAR);
	act("$p drops from $n's left hand.", ch, obj, NULL, TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    send_to_char("You slice your wrist and blood coats your fingertips forming razor sharp talons.\n\r", ch);
    act("$n slits $s wrist and blood flows over $s fingertips forming razor sharp talons.\n\r", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_BLOODLASH);
    SET_BIT(ch->itemaffect, ITEMA_VORPAL);
    SET_BIT(ch->vampaff, VAM_CLAWS);
    ch->pcdata->powers[NINJAKI] -= 20;
    return;


}

void do_bloodawaken(CHAR_DATA * ch, char *argument)
{
    CHAR_DATA      *victim;
    int             dam;
    char            arg[MAX_INPUT_LENGTH];
    char            buf[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);
    if (IS_NPC(ch))
	return;
    if (!IS_KOE(ch)) 
    {
	send_to_char("Only Kindred of the East can use Blood Awakening.\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[KOE_BLOOD] < 5) 
    {
	send_to_char("You are not proficent enough in the Blood Shintai to use this.\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[NINJAKI] < 10) 
    {
	send_to_char("You do not have enough Chi to do this\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PLACATE)) 
    {
	send_to_char("You are unable to use this while under protection.\n\r", ch);
	return;
    }
    if ((victim = ch->fighting) == NULL) 
    {
	send_to_char("You can only use the blood awakening attack in battle.\n\r", ch);
	return;
    }
    if (is_safe(ch, victim))
	return;

    dam = ch->spl[RED_MAGIC] * 4;

    sprintf(buf, "Your awaken the blood within $N! [%d]\n\r", dam);
    act(buf, ch, NULL, victim, TO_CHAR);
    sprintf(buf, "$n's awakens the blood within you! [%d]\n\r", dam);
    act(buf, ch, NULL, victim, TO_VICT);
    sprintf(buf, "$n's awakens the blood within $N! [%d]\n\r", dam);
    act(buf, ch, NULL, victim, TO_NOTVICT);
    //act("You awaken the blood within $N wracking them in pain!\n\r", ch, NULL, victim, TO_CHAR);
    //act("You scream in pain as $n attacks the blood within you!", ch, NULL, victim, TO_VICT);
    hurt_person(ch, victim, dam, FALSE);
    WAIT_STATE(ch, 8);
    ch->pcdata->powers[NINJAKI] -= 20;
    return;
}

//Bone Shintai - Corpse Skin, White Tiger Corpse, Bone Obedience(2),
//Ch 'ing Shih (3 auto in fight.c, act_look.c and fight.c respectivly

void do_corpseskin(CHAR_DATA * ch, char *argument)
{
    char            arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);
    if (IS_NPC(ch))
	return;
    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[KOE_BONE] < 1) 
    {
	send_to_char("You are not proficent enough in the Bone Shintai to use this.\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PLACATE)) 
    {
	send_to_char("You are unable to use this while under protection.\n\r", ch);
	return;
    }
    if (ch->position == POS_FIGHTING) 
    {
	send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	return;
    }
    if (IS_KOEAFF(ch, KOE_CORPSESKIN)) 
    {
	send_to_char("Your skin regains some healthy colouring.\n\r", ch);
	act("$n's skin skin regains some healthy colouring.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_CORPSESKIN);
	//ch->pcdata->powers[NINJAKI] -= 5;
	return;
    }
    if (ch->pcdata->powers[NINJAKI] < 5) 
    {
	send_to_char("You do not have enough Chi to do this\n\r", ch);
	return;
    }
    send_to_char("Your skin becomes palid and corpse like.\n\r", ch);
    act("$n's skin becomes palid and corpse like.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_CORPSESKIN);
    ch->pcdata->powers[NINJAKI] -= 5;
    return;
}

//don 't forget to add to bool can_see in handler.c!
void do_whitetigercorpse(CHAR_DATA * ch, char *argument)
{
    char            arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);
    if (IS_NPC(ch))
	return;
    if (!IS_KOE(ch)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if (ch->pcdata->powers[KOE_BONE] < 2) 
    {
	send_to_char("You are not proficent enough in the Bone Shintai to use this.\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PLACATE)) 
    {
	send_to_char("You are unable to use this while under protection.\n\r", ch);
	return;
    }
    if (ch->position == POS_FIGHTING) 
    {
	send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	return;
    }
    if (IS_SET(ch->affected_by, AFF_NINJAHIDE)) 
    {
	send_to_char("Your skin and bones become visible again.\n\r", ch);
	act("$n's skin and bones become visible again.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_NINJAHIDE);
	//ch->pcdata->powers[NINJAKI] -= 10;
	return;
    }
    if (ch->pcdata->powers[NINJAKI] < 10) 
    {
	send_to_char("You do not have enough Chi to do this\n\r", ch);
	return;
    }
    send_to_char("You flow Chi into your skin and bones making yourself invisible to monsters.\n\r", ch);
    act("$n's becomes transparent and disappears.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->affected_by, AFF_NINJAHIDE);
    ch->pcdata->powers[NINJAKI] -= 10;
    return;
}

//following 3 powers are for bone
    //obedience

	void do_boneblade(CHAR_DATA * ch, char *argument)
	{
        char            arg[MAX_INPUT_LENGTH];
        argument = one_argument(argument, arg);
        if (IS_NPC(ch))
	return;
        if (!IS_KOE(ch)) 
          {
	    send_to_char("Huh?\n\r", ch);
	    return;
          }
        if (ch->pcdata->powers[KOE_BONE] < 3) 
        {
	send_to_char("You are not proficent enough in the Bone Shintai to use this.\n\r", ch);
	return;
        }
    if (ch->position == POS_FIGHTING) {
	send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	return;
    }
    if (IS_AFF2(ch, AF2_PLACATE)) 
    {
	send_to_char("You are unable to use this while under protection.\n\r", ch);
	return;
    }
    if (IS_KOEAFF(ch, KOE_BLOODLASH)) 
    {
	send_to_char("You are unable to your forearm bone blade while using Blood Lash.\n\r", ch);
	return;
    }
    if (IS_KOEAFF(ch, KOE_BONEBLADE)) 
    {
	send_to_char("You forearm Bone Blade slides back into your arm.\n\r", ch);
	act("$n slides the bone blade back into $s arm.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_BONEBLADE);
	//ch->pcdata->powers[NINJAKI] -= 10;
	return;
    }
    if (ch->pcdata->powers[NINJAKI] < 10) 
    {
	send_to_char("You do not have enough Chi to do this\n\r", ch);
	return;
    }
    send_to_char("You create a Bone Blade from your forearm.\n\r", ch);
    act("$n's creates a bone blade from $s forearm.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_BONEBLADE);
    ch->pcdata->powers[NINJAKI] -= 10;
    return;
    }

    void do_bonearmor(CHAR_DATA * ch, char *argument)
    {
	char            arg[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg);
	if (IS_NPC(ch))
	    return;

	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_BONE] < 3) 
	{
	    send_to_char("You are not proficent enough in the Bone Shintai to use this.\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	    return;
	}
	if (IS_KOEAFF(ch, KOE_BONEARMOR)) 
	{
	    send_to_char("You dismiss your bone armor.\n\r", ch);
	    act("$n's bone armor melts away.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_BONEARMOR);
	    //ch->pcdata->power[NINJAKI] -= 15;
	    return;
	}
	if (ch->pcdata->powers[NINJAKI] < 15) 
	{
	    send_to_char("You do not have enough Chi to do this\n\r", ch);
	    return;
	}
	send_to_char("You cover your body in a thick layer of bone and spikes.\n\r", ch);
	act("$n's covers $s body in a thick layer of bone and spikes.", ch, NULL, NULL, TO_ROOM);
	SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_BONEARMOR);
	ch->pcdata->powers[NINJAKI] -= 15;
	return;
    }


    void do_visage(CHAR_DATA * ch, char *argument)
    {
	CHAR_DATA      *victim;
	char            arg[MAX_INPUT_LENGTH];
	char            buf[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	    return;
	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_BONE] < 3) 
	{
	    send_to_char("You are not proficent enough in the Bone Shintai to use this.\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if (arg[0] == '\0') 
	{
	    send_to_char("Change to look like whom?\n\r", ch);
	    return;
	}
	if (IS_POLYAFF(ch, POLY_SHADOWBERSERKER)
	    || IS_POLYAFF(ch, POLY_SHADOWDEMON)
	    || IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
	    {
	    send_to_char("Not while in another form!\n\r", ch);
	    return;
	    }
	if (IS_SET(ch->polyaff, POLY_HIMA)) 
	{
	    send_to_char("Not while in another form!\n\r", ch);
	    return;
	}
	if (IS_POLYAFF(ch, POLY_CHINGSHIH)) 
	{
	    send_to_char("Not while in another form!\n\r", ch);
	    return;
	}
	if ((victim = get_char_room(ch, arg)) == NULL) 
	{
	    send_to_char("They aren't here.\n\r", ch);
	    return;
	}
	if (IS_NPC(victim)) 
	{
	    send_to_char("Beasts and monsters are outside the realm of the Bone Shintai.\n\r", ch);
	    return;
	}
	if (IS_IMMORTAL(victim) && victim != ch) 
	{
	    send_to_char("You can only mask avatars or lower.\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[NINJAKI] < 30) 
	{
	    send_to_char("You do not have enough Chi to do this\n\r", ch);
	    return;
	}
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	    return;
	}
	if (ch == victim) 
	{
	    if (IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_VAMPAFF(ch, VAM_DISGUISED)) 
	    {
		send_to_char("You already look like yourself!\n\r", ch);
		return;
	    }
	    sprintf(buf, "Your bone structure twists and contorts into a mirror of %s.", ch->name);
	    act(buf, ch, NULL, victim, TO_CHAR);
	    sprintf(buf, "%s's bones twist and contort to appear like %s.", ch->morph, ch->name);
	    act(buf, ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	    REMOVE_BIT(ch->vampaff, VAM_DISGUISED);
	    free_string(ch->morph);
	    ch->morph = str_dup("");
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_DISGUISED)) 
	{
	    if (ch->pcdata->powers[NINJAKI] < 30) 
	    {
		send_to_char("You do not have enough Chi to do this\n\r", ch);
		return;
	    }
	    sprintf(buf, "Your bone structure twists and contorts into a mirror of %s.", victim->name);
	    act(buf, ch, NULL, victim, TO_CHAR);
	    sprintf(buf, "%s's bones twist and contort to appear like %s.", ch->morph, victim->name);
	    act(buf, ch, NULL, victim, TO_NOTVICT);
	    sprintf(buf, "%s's bones twist and contort to appear like  you!", ch->morph);
	    act(buf, ch, NULL, victim, TO_VICT);
	    free_string(ch->morph);
	    ch->morph = str_dup(victim->name);
	    ch->pcdata->powers[NINJAKI] -= 30;
	    return;
	}
	if (ch->pcdata->powers[NINJAKI] < 30) 
	{
	    send_to_char("You do not have enough Chi to do this\n\r", ch);
	    return;
	}
	sprintf(buf, "Your bone structure twists and contorts into a mirror of %s.", victim->name);
	act(buf, ch, NULL, victim, TO_CHAR);
	sprintf(buf, "%s's bones twist and contort to appear like %s.", ch->name, victim->name);
	act(buf, ch, NULL, victim, TO_NOTVICT);
	sprintf(buf, "%s's bones twist and contort to appear like you!", ch->name);
	act(buf, ch, NULL, victim, TO_VICT);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->vampaff, VAM_DISGUISED);
	free_string(ch->morph);
	ch->morph = str_dup(victim->name);
	ch->pcdata->powers[NINJAKI] -= 30;
	return;
    }

    //all 5 breath weapons in one shot..pretty snazzy.
	void do_poison_cloud(CHAR_DATA * ch, char *argument)
	{
	CHAR_DATA      *victim;
	char            arg[MAX_INPUT_LENGTH];
	int             level;

	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	    return;
	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh.\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_BONE] < 4) 
	{
	    send_to_char("You are not proficent enough in the Bone Shintai to use this.\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[NINJAKI] < 20) 
	{
	    send_to_char("You do not have enough Chi to do this\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if ((victim = get_char_room(ch, arg)) == NULL)
	    if ((victim = ch->fighting) == NULL) 
	    {
		send_to_char("They aren't here.\n\r", ch);
		return;
	    }
	if (ch == victim) 
	{
	    send_to_char("Not on yourself.\n\r", ch);
	    return;
	}
	if ((victim = ch->fighting) == NULL) 
	{
	    send_to_char("You aren't fighting anyone.\n\r", ch);
	    return;
	}
	/*
	 * do_cast(ch,"fire breath"); do_cast(ch,"frost breath");
	 * do_cast(ch,"lightning breath"); do_cast(ch,"gas");
	 * do_cast(ch,"acid breath");
	 */
	act("You open your mouth and release the 5 poison cloud at $N!", ch, NULL, victim, TO_CHAR);
	act("$n opens $s mouth and releases the 5 poison cloud at $N.", ch, NULL, victim, TO_NOTVICT);
	act("$n opens $s mouth at you and expells the 5 poison cloud at your direction!", ch, NULL, victim, TO_VICT);

	if ((IS_KOE(victim)) || IS_VAMPIRE(victim))
	    level = ch->spl[RED_MAGIC] * 2;
	else level = ch->spl[RED_MAGIC] / 2;
	act("$n opens $s mouth at you and expells a blast of #Rfire#n in your direction!", ch, NULL, victim, TO_ROOM);
	act("You open your mouth and expell a blast of #Rfire#n at $N!", ch, NULL, victim, TO_CHAR);
	act("$n opens $s mouth and expells a blast of #Rfire#n at $N.", ch, NULL, victim, TO_NOTVICT);
	hurt_person(ch, victim, level, FALSE);
	/*
	 * if (IS_CLASS(victim, CLASS_WRAITH)) level = ch->spl[YELLOW_MAGIC] *
	 * 2; else
	 */
	level = ch->spl[YELLOW_MAGIC] * 2;
	act("$n opens $s mouth at you and expells a bolt of #ylightning#n in your direction!", ch, NULL, victim, TO_ROOM);
	act("You open your mouth at $s and expell a bolt of #ylightning#n!", ch, NULL, victim, TO_CHAR);
	act("$n opens $s mouth and releases a bolt of #ylightning#n at $N.", ch, NULL, victim, TO_NOTVICT);
	hurt_person(ch, victim, level, FALSE);
	level = ch->spl[GREEN_MAGIC] * 2;
	act("$n opens $s mouth at you and expells a noxious #Ggreen#n cloud in your direction!", ch, NULL, victim, TO_ROOM);
	act("You open your mouth and release a noxious #Ggreen#n cloud at $N!", ch, NULL, victim, TO_CHAR);
	act("$n opens $s mouth and releases a noxious #Ggreen#n cloud at $N.", ch, NULL, victim, TO_NOTVICT);
	hurt_person(ch, victim, level, FALSE);
	if ((IS_VAMPIRE(victim)) || IS_KOE(victim))
	    level = 1;
	else level = ch->spl[GREEN_MAGIC] * 2;
	act("You open your mouth and release a #Pgas#n cloud at $N!", ch, NULL, victim, TO_CHAR);
	act("$n opens $s mouth and releases a #Pgas#n cloud at $N.", ch, NULL, victim, TO_NOTVICT);
	act("$n opens $s mouth at you and expells a #Pgas#n cloud in your direction!", ch, NULL, victim, TO_ROOM);
	hurt_person(ch, victim, level, FALSE);
	if (IS_DEMON(victim))
	    level = ch->spl[BLUE_MAGIC] * 2;
	else level = ch->spl[BLUE_MAGIC];
	act("You open your mouth and release shower of #Lice#n at $N!", ch, NULL, victim, TO_CHAR);
	act("$n opens $s mouth and releases a shower of #Lice#n at $N.", ch, NULL, victim, TO_NOTVICT);
	act("$n opens $s mouth at you and expells a shower of #Lice#n in your direction!", ch, NULL, victim, TO_ROOM);
	hurt_person(ch, victim, level, FALSE);
	WAIT_STATE(ch, 21);
	ch->pcdata->powers[NINJAKI] -= 25;

	if (ch->fighting == NULL)
	    ch->fighting = victim;
	if (victim->fighting == NULL)
	    victim->fighting = ch;
	return;

    }

    void do_chingshih(CHAR_DATA * ch, char *argument){
	char            buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
	    return;
	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_BONE] < 5) 
	{
	    send_to_char("You are not proficent enough in the Bone Shintai to use this.\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if (ch->stance[0] != -1)
	    do_stance(ch, "");
	if (ch->mounted == IS_RIDING)
	    do_dismount(ch, "");

	if (IS_POLYAFF(ch, POLY_SHADOWBERSERKER)
	    || IS_POLYAFF(ch, POLY_SHADOWDEMON)
	    || IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
	    {
	    send_to_char("Not while in another form!\n\r", ch);
	    return;
	    }
	if (IS_SET(ch->polyaff, POLY_HIMA)) 
	{
	    send_to_char("Not while in another form!\n\r", ch);
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_DISGUISED)) 
	{
	    send_to_char("Not while in another form!\n\r", ch);
	    return;
	}
	
       /*   if (ch->pcdata->wolf > 0) 
        {
         send_to_char( "You are too worked up to transform back(in rage).\n\r", ch );
         return;
        }*/
	
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	    return;
	}
	if (!IS_SET(ch->polyaff, POLY_CHINGSHIH) && !IS_AFFECTED(ch, AFF_POLYMORPH)) {
	    if (ch->pcdata->powers[NINJAKI] < 10) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    act("You transform into Ch'ing Shih form.", ch, NULL, NULL, TO_CHAR);
	    act("$n transforms into Ch'ing Shih form.", ch, NULL, NULL, TO_ROOM);
	    SET_BIT(ch->polyaff, POLY_CHINGSHIH);
	    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	    ch->pcdata->mod_str = 30;
	    ch->pcdata->mod_dex = 30;
	    ch->pcdata->mod_con = 30;
	    sprintf(buf, "A ravnous Ch'ing Shih [%s]", ch->name);
	    free_string(ch->morph);
	    ch->morph = str_dup(buf);
	    ch->pcdata->powers[NINJAKI] -= 10;
	   // ch->pcdata->wolf += 100;
	    return;
	}
	act("You return to your natural Kuei-jin form.", ch, NULL, NULL, TO_CHAR);
	act("$n returns to its original Kuei-jin form.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->polyaff, POLY_CHINGSHIH);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	clear_stats(ch);
	free_string(ch->morph);
	ch->morph = str_dup("");
//	ch->pcdata->wolf = 0;
	return;
        }

    //Jade Shintai - Stand on the Dragons Back(various files), Tread the Thrashing Dragon(fight.c)
	// -Placate the Earth Dragon, Harness the Dragons Breath(fight.c), Jade Servant

	void do_placate(CHAR_DATA * ch, char *argument)
	{
	if (IS_NPC(ch))
	    return;

	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_JADE] < 1) 
	{
	    send_to_char("You are not proficent enough in the Jade Shintai to use this.\n\r", ch);
	    return;
	}
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("Not while fighting!\n\r", ch);
	    return;
	}
	if (ch->fight_timer > 0) 
	{
	    send_to_char("Your fight timer has not expired yet.\n\r", ch);
	    return;
	}
	//Remove embrace in handler.c
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
	    REMOVE_BIT(ch->affected_by2, AF2_PLACATE);
	    send_to_char("You mourn the loss of the earth dragons embrace.\n\r", ch);
	    return;
	    }
	if (ch->pcdata->powers[NINJAKI] < 50) 
	{
	    send_to_char("You do not have enough Chi to do this.\n\r", ch);
	    return;
	}
	send_to_char("You feel safe under the earth dragons protection protection.\n\r", ch);
	ch->pcdata->powers[NINJAKI] -= 50;
	SET_BIT(ch->affected_by2, AF2_PLACATE);
	return;
    }

    void do_harness(CHAR_DATA * ch, char *argument)
    {
	char            arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA      *victim;
	one_argument(argument, arg);

	if (IS_NPC(ch))
	    return;

	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_JADE] < 4) 
	{
	    send_to_char("You are not proficent enough in the Jade Shintai to use this.\n\r", ch);
	    return;
	}
	if ((victim = get_char_world(ch, arg)) == NULL) 
	{
	    send_to_char("To whom do you wish to harness the dragons breath to?\n\r", ch);
	    return;
	}
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("No way! You are fighting.\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[NINJAKI] < 15) 
	{
	    send_to_char("You do not have enough Chi to do this.\n\r", ch);
	    return;
	}
	if (!IS_IMMUNE(victim, IMM_SUMMON) && !IS_NPC(victim)) 
	{
	    send_to_char("You failed.\n\r", ch);
	    return;
	}
	if IS_SET
	    (ch->in_room->room_flags, ROOM_ARENA) 
	    {
	    send_to_char("You cannot harness the dragon while in arena.\n\r", ch);
	    return;
	    }
	if IS_SET
	    (victim->in_room->room_flags, ROOM_ARENA) 
	    {
	    send_to_char("You cannot travel into the arena.\n\r", ch);
	    return;
	    }
	if (ch->fight_timer > 0) 
	{
	    send_to_char("Your fight timer has not expired yet.\n\r", ch);
	    return;
	}
	/*
	 * if IS_SET(ch->in_room->room_flags, ROOM_INDOORS)
	 * {send_to_char("You cannot harness the dragon indoors!\n\r",
	 * ch);return;} if IS_SET(victim->in_room->room_flags, ROOM_INDOORS)
	 * {send_to_char("You cannot travel into buildings.\n\r",
	 * ch);return;}
	 */
	location = victim->in_room;

	if (room_is_private(location)) 
	{
	    send_to_char("That room is private right now.\n\r", ch);
	    return;
	}
	act("You harness the dragons breath to ride the wind.", ch, NULL, NULL, TO_CHAR);
	act("$n raises up as $s harnesses the dragons breath.", ch, NULL, NULL, TO_ROOM);
	ch->pcdata->powers[NINJAKI] -= 50;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");
	act("You arrive at your destination.", ch, NULL, NULL, TO_CHAR);
	act("$n floats down from the sky.", ch, NULL, NULL, TO_CHAR);
	return;
    }

    void do_jadeservant(CHAR_DATA * ch, char *argument)
    {
	char            arg[MAX_INPUT_LENGTH];
	char            buf[MAX_STRING_LENGTH];
	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	    return;

	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_JADE] < 5) 
	{
	    send_to_char("You are not proficent enough in the Jade Shintai to use this.\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if (IS_POLYAFF(ch, POLY_SHADOWBERSERKER) || IS_POLYAFF(ch, POLY_SHADOWDEMON) || IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
	    {
	    send_to_char("You cannot accept the Jade Servent while transformed.\n\r", ch);
	    return;
	    }
	if (IS_SET(ch->polyaff, POLY_HIMA)) 
	{
	    send_to_char("You cannot accept the Jade Servent while transformed.\n\r", ch);
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_DISGUISED)) 
	{
	    send_to_char("You cannot accept the Jade Servent while transformed.\n\r", ch);
	    return;
	}
	if (IS_POLYAFF(ch, POLY_CHINGSHIH)) 
	{
	    send_to_char("You cannot accept the Jade Servent while transformed.\n\r", ch);
	    return;
	}
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	    return;
	}
	if (arg[0] == '\0') 
	{
	    send_to_char("You can be possessed by the following jade servants: tortoise, tiger, phoenix and human.\n\r", ch);
	    return;
	}
	if (!str_cmp(arg, "tortoise")) 
	{
	    if (IS_AFFECTED(ch, AFF_POLYMORPH)) 
	    {
		send_to_char("You can only polymorph from human form.\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 15) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    if (ch->stance[0] != -1)
		do_stance(ch, "");
	    if (ch->mounted == IS_RIDING)
		do_dismount(ch, "");
	    clear_stats(ch);
	    act("You allow the essence of the Black Tortoise to enter you.", ch, NULL, NULL, TO_CHAR);
	    act("$n allows the essence of the Black Tortoise to consume $s.", ch, NULL, NULL, TO_ROOM);
	    ch->pcdata->mod_str = 30;
	    SET_BIT(ch->polyaff, POLY_BLACKTORTOISE);
	    SET_BIT(ch->vampaff, VAM_CHANGED);
	    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_FROSTAURA);
	    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	    sprintf(buf, "%s the servant of the Black Tortoise", ch->name);
	    free_string(ch->morph);
	    ch->morph = str_dup(buf);
	    ch->pcdata->powers[NINJAKI] -= 15;
	    return;
	} 
	else if (!str_cmp(arg, "Tiger")) 
	{
	    if (IS_AFFECTED(ch, AFF_POLYMORPH)) 
	    {
		send_to_char("You can only polymorph from human form.\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 15) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    if (ch->stance[0] != -1)
		do_stance(ch, "");
	    if (ch->mounted == IS_RIDING)
		do_dismount(ch, "");
	    act("You allow the essence of the White Tiger to enter you.", ch, NULL, NULL, TO_CHAR);
	    act("$n allows the essence of the White Tiger to consume $s.", ch, NULL, NULL, TO_ROOM);
	    clear_stats(ch);
	    if (ch->wpn[0] > 0) 
	    {
		ch->hitroll += (ch->wpn[0]);
	    }
	    ch->pcdata->mod_dex = 30;
	    SET_BIT(ch->polyaff, POLY_WHITETIGER);
	    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_LIGHTNINGAURA);
	    SET_BIT(ch->vampaff, VAM_CHANGED);
	    sprintf(buf, "%s the servant of the White Tiger", ch->name);
	    free_string(ch->morph);
	    ch->morph = str_dup(buf);
	    ch->pcdata->powers[NINJAKI] -= 15;
	    return;
	} 
	else if (!str_cmp(arg, "Phoenix")) 
	{
	    if (IS_AFFECTED(ch, AFF_POLYMORPH)) 
	    {
		send_to_char("You can only polymorph from human form.\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 15) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    if (ch->stance[0] != -1)
		do_stance(ch, "");
	    if (ch->mounted == IS_RIDING)
		do_dismount(ch, "");
	    act("You allow the essence of the Scarlet Phoenix to enter you.", ch, NULL, NULL, TO_CHAR);
	    act("$n allows the essence of the Scarlet Phoenix to consume $s.", ch, NULL, NULL, TO_ROOM);
	    clear_stats(ch);
	    if (ch->wpn[0] > 0) 
	    {
		ch->armor -= (ch->wpn[0] * 3);
	    }
	    ch->pcdata->mod_con = 30;
	    SET_BIT(ch->polyaff, POLY_SCARLETPHOENIX);
	    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_FLAMEAURA);
	    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	    SET_BIT(ch->vampaff, VAM_CHANGED);
	    sprintf(buf, "%s the servant of the Scarlet Phoenix", ch->name);
	    free_string(ch->morph);
	    ch->morph = str_dup(buf);
	    ch->pcdata->powers[NINJAKI] -= 15;
	    return;
	} 
	else if (!str_cmp(arg, "human")) 
	{
	    if (!IS_AFFECTED(ch, AFF_POLYMORPH)) 
	    {
		send_to_char("You are already in human form.\n\r", ch);
		return;
	    }
	    if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BLACKTORTOISE)) 
	    {
		REMOVE_BIT(ch->polyaff, POLY_BLACKTORTOISE);
		REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_FROSTAURA);
	    } 
	    else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WHITETIGER)) 
	    {
		REMOVE_BIT(ch->polyaff, POLY_WHITETIGER);
		if (ch->hit < 1)
		    ch->hit = 1;
		REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_LIGHTNINGAURA);
	    } 
	    else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_SCARLETPHOENIX)) 
	    {
		REMOVE_BIT(ch->polyaff, POLY_SCARLETPHOENIX);
		REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_FLAMEAURA);
	    }
	    			/* else { // In case they try to change to
				 * human from a non-vamp form
				 * send_to_char("You seem to be stuck in this
				 * form.\n\r", ch); return; } */
	    act("You transform into human form.", ch, NULL, NULL, TO_CHAR);
	    act("$n transforms into human form.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	    REMOVE_BIT(ch->vampaff, VAM_CHANGED);
	    clear_stats(ch);
	    free_string(ch->morph);
	    ch->morph = str_dup("");
	    return;
	} 
	else send_to_char("You can summon between 'tortois', 'tiger', 'phoenix and 'human' forms.\n\r", ch);
	return;
    }

    //Flesh Shintai - Long Limbs, Detachment, Lotus Cloud, Pelesit, Ten Thousand Forms
	void do_extendedfingers(CHAR_DATA * ch, char *argument)
	{
	char            arg[MAX_INPUT_LENGTH];
	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	    return;
	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_FLESH] < 1 && IS_KOE(ch)) 
	{
	    send_to_char("You are not proficent enough in the Flesh Shintai to use this.\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_CLAWS)) 
	{
	    send_to_char("You fingers constrict to their normal length.\n\r", ch);
	    act("$n's fingers constrict to normal length.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
	    return;
	}
	if (ch->pcdata->powers[NINJAKI] < 15 && IS_KOE(ch)) 
	{
	    send_to_char("You do not have enough Chi to do this.\n\r", ch);
	    return;
	}
	send_to_char("Your fingers extend to twice their normal size.\n\r", ch);
	act("$n's fingers extend to twice their normal size.", ch, NULL, NULL, TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
	if (IS_KOE(ch))
	    ch->pcdata->powers[NINJAKI] -= 15;
	/*
	 * if (IS_CLASS(ch, CLASS_DHAMPYR)) ch->pcdata->powers[NINJAKI] -= 5;
	 */
	return;
    }

    //originally eyespy for demons
//	,credit given when author discovered(Kavir ?)
	    void do_detatchment(CHAR_DATA * ch, char *argument)
	    {
	    CHAR_DATA      *victim;
	    CHAR_DATA      *familiar;

	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_FLESH] < 2) 
	{
	    send_to_char("You are not proficent enough in the Flesh Shintai to use this.\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[NINJAKI] < 30) 
	{
	    send_to_char("You do not have enough Chi to do this.\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	    return;
	}
	if (IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R)) 
	{
	    send_to_char("But you don't have any more eyes to pluck out!\n\r", ch);
	    return;
	}
	if (!IS_HEAD(ch, LOST_EYE_L) && number_range(1, 2) == 1) 
	{
	    act("You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR);
	    act("$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM);
	} 
	else if (!IS_HEAD(ch, LOST_EYE_R)) 
	{
	    act("You pluck out your right eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR);
	    act("$n plucks out $s right eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM);
	} 
	else 
	{
	    act("You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR);
	    act("$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM);
	}
	if ((familiar = ch->pcdata->familiar) != NULL) 
	{
	    make_part(ch, "eyeball");
	    return;
	}
	victim = create_mobile(get_mob_index(MOB_VNUM_EYE));
	if (victim == NULL) {
	    send_to_char("Error - please inform imms.\n\r", ch);
	    return;
	}
	char_to_room(victim, ch->in_room);
	ch->pcdata->powers[NINJAKI] -= 30;
	ch->pcdata->familiar = victim;
	victim->wizard = ch;
	return;
	}

	//used imm 's ' peace ' command as basis
	    void do_lotuscloud(CHAR_DATA * ch, char *argument)
	    {
	    CHAR_DATA      *rch;

	    if (!IS_KOE(ch)) 
	    {
		send_to_char("Huh?\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[KOE_FLESH] < 3) 
	    {
		send_to_char("You are not proficent enough in the Flesh Shintai to use this.\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 100) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
		send_to_char("You are unable to use this while under protection.\n\r", ch);
		return;
	    }
	    if (ch->position == POS_FIGHTING) 
	    {
		send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		return;
	    }
	    if (ch->fight_timer > 0) 
	    {
		send_to_char("Your fight timer has not expired yet.\n\r", ch);
		return;
	    }
	    //sprintf(buf, "%s: Peace %s", ch->name, argument);
	    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room) 
	    {
		if (rch->fighting != NULL)
		    stop_fighting(rch, TRUE);
	    }
	    act("You belch out a purple euphoric cloud", ch, NULL, NULL, TO_CHAR);
	    act("$n belches out a euphoric purple cloud...your will to fight disappears.", ch, NULL, NULL, TO_ROOM);
	    ch->pcdata->powers[NINJAKI] -= 100;
	    return;
	}

	void do_pelesit(CHAR_DATA * ch, char *argument)
	{
	    char            arg[MAX_INPUT_LENGTH];
	    argument = one_argument(argument, arg);

	    if (IS_NPC(ch))
		return;
	    if (!IS_KOE(ch)) 
	    {
		send_to_char("Huh?\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[KOE_FLESH] < 4 && IS_KOE(ch)) 
	    {
		send_to_char("You are not proficent enough in the Flesh Shintai to use this.\n\r", ch);
		return;
	    }
	    /*
	     * if (ch->pcdata->powers[DHAMPYR_SHINTAI] < 4 && IS_CLASS(ch,
	     * CLASS_DHAMPYR)) { send_to_char("You are not proficent enough
	     * in the Shintai powers to use this.\n\r", ch); return; }
	     */
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
		send_to_char("You are unable to use this while under protection.\n\r", ch);
		return;
	    }
	    if (ch->position == POS_FIGHTING) 
	    {
		send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		return;
	    }
	    if (IS_KOEAFF(ch, KOE_PELESIT)) 
	    {
		send_to_char("You wave your hands over your arms dismissing the living tattoo's.\n\r", ch);
		act("$n's waves $s hands over $s arms and dismisses the living tattoo's.", ch, NULL, NULL, TO_ROOM);
		REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_PELESIT);
		//REMOVE_BIT(ch->pcdata->stats[UNI_AFF], KOE_PELESIT);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 15 && IS_KOE(ch)) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    /*
	     * if (ch->pcdata->powers[NINJAKI] < 5 && IS_CLASS(ch,
	     * CLASS_DHAMPYR)) { send_to_char("You do not have enough Chi to
	     * do this.\n\r", ch); return; }
	     */
	    send_to_char("You draw some mystical living tattoo's on your arms.\n\r", ch);
	    act("$n's draws some mystical living tattoo's on $s arms.", ch, NULL, NULL, TO_ROOM);
	    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_PELESIT);
	    //SET_BIT(ch->pcdata->stats[UNI_AFF], KOE_PELESIT);
	    if (IS_KOE(ch))
		ch->pcdata->powers[NINJAKI] -= 15;
	    /*
	     * if (IS_CLASS(ch, CLASS_DHAMPYR)) ch->pcdata->powers[NINJAKI]
	     * -= 5;
	     */

	    return;
	}

	void do_thousandforms(CHAR_DATA * ch, char *argument)
	{
	    CHAR_DATA      *victim;
	    char            arg[MAX_INPUT_LENGTH];
	    char            buf[MAX_INPUT_LENGTH];

	    argument = one_argument(argument, arg);

	    if (IS_NPC(ch))
		return;
	    if (!IS_KOE(ch)) 
	    {
		send_to_char("Huh?\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[KOE_FLESH] < 5) 
	    {
		send_to_char("You are not proficent enough in the Flesh Shintai to use this.\n\r", ch);
		return;
	    }
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
		send_to_char("You are unable to use this while under protection.\n\r", ch);
		return;
	    }
	    if (ch->position == POS_FIGHTING) 
	    {
		send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		return;
	    }
	    if (arg[0] == '\0') 
	    {
		send_to_char("Change to look like whom?\n\r", ch);
		return;
	    }
	    if (IS_POLYAFF(ch, POLY_SHADOWBERSERKER)
		|| IS_POLYAFF(ch, POLY_SHADOWDEMON)
		|| IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
		{
		send_to_char("You cannot accept the Jade Servent while transformed.\n\r", ch);
		return;
	        }
if (0)
	    {
		send_to_char("Not while in another form!\n\r", ch);
		return;
	    }
	    if (IS_SET(ch->polyaff, POLY_HIMA)) 
	    {
		send_to_char("Not while in another form!\n\r", ch);
		return;
	    }
	    if (IS_POLYAFF(ch, POLY_CHINGSHIH)) 
	    {
		send_to_char("Not while in another form!\n\r", ch);
		return;
	    }
	    /*
	     * if (IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_VAMPAFF(ch,
	     * VAM_DISGUISED)) { send_to_char("Not while polymorphed.\n\r",
	     * ch); return; }
	     */
	    if ((victim = get_char_room(ch, arg)) == NULL) 
	    {
		send_to_char("They aren't here.\n\r", ch);
		return;
	    }
	    if (IS_IMMORTAL(victim) && victim != ch) 
	    {
		send_to_char("You can only mask avatars or lower.\n\r", ch);
		return;
	    }
	    if (ch == victim) 
	    {
		if (!IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_VAMPAFF(ch, VAM_DISGUISED)) {
		    send_to_char("You already look like yourself!\n\r", ch);
		    return;
	    }
		sprintf(buf, "You mold your flesh to look like %s.", ch->name);
		act(buf, ch, NULL, victim, TO_CHAR);
		sprintf(buf, "%s's molds $s flesh to look like %s.", ch->morph, ch->name);
		act(buf, ch, NULL, victim, TO_ROOM);
		REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
		free_string(ch->morph);
		ch->morph = str_dup("");
		return;
	    }
	    if (IS_VAMPAFF(ch, VAM_DISGUISED)) 
	    {
		if (ch->pcdata->powers[NINJAKI] < 25) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		sprintf(buf, "You mold your flesh to look like  %s.", victim->name);
		act(buf, ch, NULL, victim, TO_CHAR);
		sprintf(buf, "%s's molds $s flesh to look like  %s.", ch->morph, victim->name);
		act(buf, ch, NULL, victim, TO_NOTVICT);
		sprintf(buf, "%s's mold your flesh to look exactly like you!", ch->morph);
		act(buf, ch, NULL, victim, TO_VICT);
		free_string(ch->morph);
		ch->morph = str_dup(victim->name);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 25) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    sprintf(buf, "Your mold your flesh to look like %s.", victim->name);
	    act(buf, ch, NULL, victim, TO_CHAR);
	    sprintf(buf, "%s's molds $s flesh to look like  %s.", ch->name, victim->name);
	    act(buf, ch, NULL, victim, TO_NOTVICT);
	    sprintf(buf, "%s's mold your flesh to look exactly like you!", ch->name);
	    act(buf, ch, NULL, victim, TO_VICT);
	    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
	    free_string(ch->morph);
	    ch->morph = str_dup(victim->name);
	    return;
	}

	//Ghost Flame Shintai - Goblin Face, Goblin Spark(magic.c), Goblin Shapes
	    // Goblin Scorch, Goblin Lantern
	    void do_goblinface(CHAR_DATA * ch, char *argument){
	    char            arg[MAX_INPUT_LENGTH];
	    argument = one_argument(argument, arg);

	    if (IS_NPC(ch))
		return;
	    if (!IS_KOE(ch)) 
	    {
		send_to_char("Huh?\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[KOE_GHOST] < 1) 
	    {
		send_to_char("You are not proficent enough in the Ghost Shintai to use this.\n\r", ch);
		return;
	    }
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
		send_to_char("You are unable to use this while under protection.\n\r", ch);
		return;
	    }
	    if (IS_KOEAFF(ch, KOE_GOBLINFACE)) 
	    {
		send_to_char("Your face once again becomes normal.\n\r", ch);
		act("$n's face once again becomes bearable to look at.", ch, NULL, NULL, TO_ROOM);
		REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_GOBLINFACE);
		REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_TUSK);
		REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 15) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    send_to_char("You summon the horrible goblin face.\n\r", ch);
	    act("$n's summons the horrible goblin face ability.", ch, NULL, NULL, TO_ROOM);
	    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_GOBLINFACE);
	    SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_TUSK);
	    send_to_char("Large tusks burst from your lower jaw.\n\r", ch);
	    act("Large Tusks burst from $n's lower jaw.", ch, NULL, NULL, TO_ROOM);
	    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
	    send_to_char("Your eyes glow with an eerie green glow.\n\r", ch);
	    act("$n's eyes begin to glow with an eerie green glow.", ch, NULL, NULL, TO_ROOM);
	    ch->pcdata->powers[NINJAKI] -= 15;
	    return;
	}

	void do_goblinspark(CHAR_DATA * ch, char *argument)
	{
	    int             sn;
	    int             level;
	    int             spelltype;
	    CHAR_DATA      *rch;
	    AFFECT_DATA     af;
	    char            buf[MAX_INPUT_LENGTH];
	    if (IS_NPC(ch))
		return;
	    if (!IS_KOE(ch)) 
	    {
		send_to_char("Huh?\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[KOE_GHOST] < 2) 
	    {
		send_to_char("You are not proficent enough in the Ghost Shintai to use this.\n\r", ch);
		return;
	    }
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
		send_to_char("You are unable to use this while under protection.\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 25) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    if (ch->goblinspark > 0) 
	    {
		send_to_char("You are still weary from the last time you used the goblin spark.\n\r", ch);
		return;
	    }
	    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
		REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    //act("$n glows bright purple and sparks fly in every direction.", ch, NULL, NULL, TO_ROOM);
	    send_to_char("You summon the goblin spark!\n\r", ch);
	    if ((sn = skill_lookup("goblin spark")) < 0)
		return;
	    spelltype = skill_table[sn].target;
	    level = 50;
	    (*skill_table[sn].spell_fun) (sn, level, ch, NULL);

	    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room) {
		if (IS_AFF2(rch, AF2_TOTALBLIND) || (rch == ch)) 
		{
		    continue;
		}
		if (is_safe(ch, rch) || is_safe(rch, ch) || IS_AFF2(rch, AF2_TOTALBLIND))
		    continue;

		if (!IS_NPC(rch) && rch->level < 2)
		    continue;
		if (saves_spell(level * 2, rch))
		    continue;

		af.type = sn;
		af.location = APPLY_DEX;
		af.modifier = -2;
		af.duration = 1 + level;
		affect_to_char(rch, &af);
		af.location = APPLY_AFF2;
		af.modifier = AF2_TOTALBLIND;
		affect_to_char(rch, &af);
		if (!IS_NPC(rch))
		    sprintf(buf, "%s stumbles back blinded from the blinding flash of the goblin spark!\n\r", rch->name);
		else
		    sprintf(buf, "%s stumbles back in agony as they are blinded by the flash of the goblin spark!\n\r", rch->short_descr);
		send_to_char(buf, ch);
		SET_BIT(rch->affected_by2, AF2_TOTALBLIND);
		if (IS_AFFECTED(rch, AFF_SHADOWPLANE))
		    REMOVE_BIT(rch->affected_by, AFF_SHADOWPLANE);

	    }
	    ch->goblinspark = 20;
	    WAIT_STATE(ch, 24);
	    ch->pcdata->powers[NINJAKI] -= 25;
	    return;
	}


	void do_goblinscorch(CHAR_DATA * ch, char *argument)
	{
	    CHAR_DATA      *victim;
	    CHAR_DATA      *vch = NULL;
	    CHAR_DATA      *vch_next = NULL;
	    char            arg1[MAX_INPUT_LENGTH];
	    char          buf[MAX_STRING_LENGTH];
	    int             damage;
	    victim = ch;
	    argument = one_argument(argument, arg1);

	    if (IS_NPC(ch))
		return;
	    if (!IS_KOE(ch)) 
	    {
		send_to_char("Huh?\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[KOE_GHOST] < 4) 
	    {
		send_to_char("You are not proficent enough in the Ghost Shintai to use this.\n\r", ch);
		return;
	    }
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
		send_to_char("You are unable to use this while under protection.\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[NINJAKI] < 10) 
	    {
		send_to_char("You do not have enough Chi to do this.\n\r", ch);
		return;
	    }
	    //if (ch->position == POS_FIGHTING)
if(0)
		{
		send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		return;
		}
	    damage = number_range(400, ch->pcdata->powers[NINJAKI] * 11);
	    ch->pcdata->powers[NINJAKI] = 0;
	    if (damage > 6000)
		damage = 5000;
	    act("You open your mouth and release your stored up Chi!", ch, NULL, victim, TO_CHAR);
	    sprintf(buf, "You open your mouth and release your stored up Chi! #G[#R%d#G]#n", damage);
		 send_to_char(buf, ch);
	    act("$n opens $s mouth and releases a shower of burning Chi.", ch, NULL, victim, TO_NOTVICT);
	    sprintf(buf, "%s opens their mouth at you and expells a shower of burning Chi in your direction ! !#C[#L%d#C]#n", ch->name, damage);
		 send_to_char(buf, victim);
	    act("$n opens $s mouth at you and expells a shower of burning Chi at your direction!", ch, NULL, victim, TO_VICT);
	    for (vch = ch->in_room->people; vch != NULL; vch = vch_next) 
            {
		vch_next = vch->next_in_room;
		if (vch == ch)
		    continue;
		if (is_safe(ch, vch) || is_safe(vch, ch))
		    continue;
		if (vch->fighting == NULL && vch->position > POS_STUNNED)
		    vch->fighting = ch;
		hurt_person(ch, vch, damage, FALSE);
	    }
	    WAIT_STATE(ch, 16);
	    return;
	}

	void do_hima(CHAR_DATA * ch, char *argument)
	{
	    char            buf[MAX_STRING_LENGTH];

	    if (IS_NPC(ch))
		return;
	    if (!IS_KOE(ch)) 
	    {
		send_to_char("Huh?\n\r", ch);
		return;
	    }
	    if (ch->pcdata->powers[KOE_GHOST] < 5) 
	    {
		send_to_char("You are not proficent enough in the Ghost Flame Shintai to use this.\n\r", ch);
		return;
	    }
	    if (IS_AFF2(ch, AF2_PLACATE)) 
	    {
		send_to_char("You are unable to use this while under protection.\n\r", ch);
		return;
	    }
	    if (ch->position == POS_FIGHTING) 
	    {
		send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		return;
	    }
	    if (ch->stance[0] != -1)
		do_stance(ch, "");
	    if (ch->mounted == IS_RIDING)
		do_dismount(ch, "");
	    if (IS_POLYAFF(ch, POLY_CHINGSHIH)) 
	    {
		send_to_char("Not in another form!\n\r", ch);
		return;
	    }
	    if (IS_POLYAFF(ch, POLY_SHADOWBERSERKER)
		|| IS_POLYAFF(ch, POLY_SHADOWDEMON)
		|| IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
		{
		send_to_char("Not while in another form!\n\r", ch);
		return;
	        }
	    if (IS_VAMPAFF(ch, VAM_DISGUISED)) 
	    {
		send_to_char("Not while in another form!\n\r", ch);
		return;
	    }
	    if (!IS_SET(ch->polyaff, POLY_HIMA)) 
	    {
		if (ch->pcdata->powers[NINJAKI] < 50) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		act("You transform into Hima form.", ch, NULL, NULL, TO_CHAR);
		act("$n transforms into Hima form.", ch, NULL, NULL, TO_ROOM);
		SET_BIT(ch->polyaff, POLY_HIMA);
		SET_BIT(ch->affected_by, AFF_POLYMORPH);
		sprintf(buf, "A being of bright Chi Fire [%s]", ch->name);
		free_string(ch->morph);
		ch->morph = str_dup(buf);
		SET_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_FLAMEAURA);
		ch->pcdata->mod_str = 30;
		ch->pcdata->mod_dex = 30;
		ch->pcdata->mod_con = 30;
		ch->pcdata->powers[NINJAKI] -= 50;
		return;
	    } 
	    else if (IS_SET(ch->polyaff, POLY_HIMA)) 
	    {
		act("You return to your natural Kuei-jin form.", ch, NULL, NULL, TO_CHAR);
		act("$n returns to its original Kuei-jin form.", ch, NULL, NULL, TO_ROOM);
		REMOVE_BIT(ch->polyaff, POLY_HIMA);
		REMOVE_BIT(ch->pcdata->powers[KOE_CURRENT], KOE_FLAMEAURA);
		REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		clear_stats(ch);
		free_string(ch->morph);
		ch->morph = str_dup("");
		return;
	    } 
	    else
		return;
	}

	//KoE Paths to follow unless stated otherwise

// Yin Path - Ghost Sight:huntingsnake(readaura), piercetheshroud(shadowsight / truesight), sense powers(handler.c)
	    // see 's through normal shields, but special KoE aura' s
	    //do
	  // damage
	void do_huntingsnake(CHAR_DATA * ch, char *argument)
	{
	CHAR_DATA      *victim;
	OBJ_DATA       *obj;
	char            arg[MAX_INPUT_LENGTH];
	char            buf[MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg);

	if (IS_NPC(ch))
	    return;
	if (!IS_KOE(ch)) 
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_PYIN] < 1) 
	{
	    send_to_char("You are not proficent enough in the Yin Path to use this.\n\r", ch);
	    return;
	}
	if (ch->pcdata->powers[KOE_PO] > 4) 
	{
	    send_to_char("You do not know the Yin path.\n\r", ch);
	    return;
	}
/*	if (ch->pcdata->power[NINJA_KI] < 50) 
	{
	    send_to_char("You do not have enough Chi to do this.\n\r", ch);
	    return;
	}*/
	if (ch->position == POS_FIGHTING) 
	{
	    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
	    return;
	}
	if (arg[0] == '\0') 
	{
	    send_to_char("Read the aura on what?\n\r", ch);
	    return;
	}
	if (IS_AFF2(ch, AF2_PLACATE)) 
	{
	    send_to_char("You are unable to use this while under protection.\n\r", ch);
	    return;
	}
	if ((victim = get_char_room(ch, arg)) == NULL) 
	{
	    if ((obj = get_obj_carry(ch, arg)) == NULL) 
	    {
		send_to_char("Read the aura on what?\n\r", ch);
		return;
	    }
	    act("$n examines $p closely.", ch, obj, NULL, TO_ROOM);
	    spell_identify(skill_lookup("identify"), ch->level, ch, obj);
	    return;
	}
	if (!IS_NPC(victim) && (IS_KOEAFF(victim, KOE_FROSTAURA) || IS_KOEAFF(victim, KOE_FLAMEAURA) || IS_KOEAFF(victim, KOE_LIGHTNINGAURA))) 
	{
	    send_to_char("You reel back as your eyes are burned by the avatars aura!.\n\r", ch);
	    ch->hit = ch->hit - 5000;
	    return;
	}
	act("$n examines $N intently.", ch, NULL, victim, TO_NOTVICT);
	act("$n examines your aura.", ch, NULL, victim, TO_VICT);

	if (IS_NPC(victim))
	    sprintf(buf, "%s is an NPC.\n\r", victim->short_descr);
	else 
	{
	    sprintf(buf, "%s.\n\r", victim->name);
	    send_to_char(buf, ch);
	}


	if (!IS_NPC(victim)) 
	{
	    sprintf(buf, "Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r", get_curr_str(victim), get_curr_int(victim), get_curr_wis(victim), get_curr_dex(victim), get_curr_con(victim));
	    send_to_char(buf, ch);
	}
	sprintf(buf, "Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r", victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move);
	send_to_char(buf, ch);
	if (!IS_NPC(victim))
	    sprintf(buf, "Hitroll:%d, Damroll:%d, AC:%d.\n\r", char_hitroll(victim), char_damroll(victim), char_ac(victim));
	else
	    sprintf(buf, "AC:%d.\n\r", char_ac(victim));
	send_to_char(buf, ch);
	if (!IS_NPC(victim)) 
	{
	    sprintf(buf, "Status:%d, ", victim->race);
	    send_to_char(buf, ch);
	}
	if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	    act("$N is pregnant.", ch, NULL, victim, TO_CHAR);
	ch->pcdata->power[POWER_CURRENT] -= 50;
	return;
	}

void do_piercetheshroud(CHAR_DATA * ch, char *argument)
{
		if (IS_NPC(ch))
		    return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_PYIN] < 2) 
		{
		    send_to_char("You are not proficent enough in the Yin Path to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_PO] > 4) 
		{
		    send_to_char("You do not know the Yin path.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if (IS_SET(ch->act, PLR_HOLYLIGHT)) 
		{
		    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
		    REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
		    send_to_char("Your senses dull and become mundane.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 10) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		send_to_char("The land of spirits opens up to your vision.\n\r", ch);
		SET_BIT(ch->act, PLR_HOLYLIGHT);
		SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
		ch->pcdata->powers[NINJAKI] -= 10;
		return;
	    }

	    //Yang Path - yangheal, lifesight
	void do_yangheal(CHAR_DATA * ch, char *argument)
	{

		if (IS_NPC(ch))
		    return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_PYANG] < 1) 
		{
		    send_to_char("You are not proficent enough in the Yang Path to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 10) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("Not while fighting!\n\r", ch);
		    return;
		}
		if (ch->hit >= ch->max_hit)
		    ch->hit = ch->max_hit;
		else if (ch->hit < 30000) 
		{
		    send_to_char("You summon the yang forces within you.\n\r", ch);
		    act("$n summons the yang within $s to heal.", ch, NULL, NULL, TO_ROOM);
		    ch->hit = ch->hit + number_range(256, 1024) + ch->pcdata->powers[NINJAKI] * 2;
		    WAIT_STATE(ch, 16);
		    ch->pcdata->powers[NINJAKI] -= 10;
		}
		return;
	    }
	    void do_lifesight(CHAR_DATA * ch, char *argument)
	    {
		if (IS_NPC(ch))
		    return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if              (ch->pcdata->powers[KOE_PYANG] < 2) 
		{
		    send_to_char("You are not proficent enough in the Yin path to use this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if (IS_SET(ch->act, PLR_HOLYLIGHT)) 
		{
		    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
		    send_to_char("Your attunement to the living fades.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 10) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		send_to_char("Your vision becomes attuned with all that is living.\n\r", ch);
		SET_BIT(ch->act, PLR_HOLYLIGHT);
		ch->pcdata->powers[NINJAKI] -= 10;
		return;
	    }

	    //Hun Path - Demon Arts / Frenzy, Sharpen Senses, Spirit Speech(channel), Tempering the P 'o



	void do_sharpensenses(CHAR_DATA * ch, char *argument)
	{
		if (IS_NPC(ch))
		    return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if              (ch->pcdata->powers[KOE_HUN] < 2) 
		{
		    send_to_char("You are not proficent enough in the Hun Path to use this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if (IS_SET(ch->act, PLR_HOLYLIGHT)) 
		{
		    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
		    send_to_char("Your sharpened senses dull.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 10) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		send_to_char("Your vision becomes sharp.\n\r", ch);
		SET_BIT(ch->act, PLR_HOLYLIGHT);
		ch->pcdata->powers[NINJAKI] -= 10;
		return;
	    }

	    //enhances players rage, or a ww or vampires rage as well !
	void do_unleash(CHAR_DATA * ch, char *argument)
	{
		char            arg[MAX_INPUT_LENGTH];
		CHAR_DATA      *victim;
		                one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if ((ch->pcdata->powers[KOE_HUN] < 1) && (!IS_POLYAFF(ch, POLY_SHADOWBERSERKER))) 
		{
		    send_to_char("You are not proficent enough in the Hun Path to use this.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 20) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if ((victim = get_char_room(ch, arg)) == NULL) 
		{
		    send_to_char("Evoke a frenzy in who?\n\r", ch);
		    return;
		}
		if (victim == ch && ch->pcdata->wolf < 60) 
		{
		    send_to_char("You summon the beast within!\n\r", ch);
		    act("$n growls as $s summons the beast within!", ch, NULL, NULL, TO_ROOM);
		    ch->pcdata->wolf += 100;
		    ch->pcdata->powers[NINJAKI] -= 20;
		    if (ch->pcdata->wolf > 110)
			ch->pcdata->wolf = 110;
		    WAIT_STATE(ch, 12);
		    return;
		} 
		else if ((IS_VAMPIRE(victim)) || IS_WEREWOLF(victim)) {
		    if (victim->pcdata->wolf < 100 && IS_VAMPIRE(victim)) 
		    {
			send_to_char("You summon the beast within\n\r", ch);
			act("$n growls as summon their beast within!", ch, NULL, NULL, TO_ROOM);
			victim->pcdata->wolf += 50;
			ch->pcdata->powers[NINJAKI] -= 15;
			WAIT_STATE(ch, 12);
			return;
		    } 
		    else if (victim->pcdata->wolf < 300 && IS_WEREWOLF(victim)) 
		    {
			send_to_char("You summon the beast within\n\r", ch);
			act("$n growls as summon their beast within!", ch, NULL, NULL, TO_ROOM);
			victim->pcdata->wolf += 50;
			ch->pcdata->powers[NINJAKI] -= 15;
			WAIT_STATE(ch, 12);
			return;
		    }
		} 
		else send_to_char("They don't have a beast to summon form.\n\r", ch);
		return;
	    }

/*	    void do_tempering(CHAR_DATA * ch, char *argument)
	    {
		CHAR_DATA      *victim;
		              //char arg[MAX_INPUT_LENGTH];

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_HUN] < 4) 
		{
		    send_to_char("You are not proficent enough in the Hun Path to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 10) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		//takes someone out of shadownature
		    if (argument[0] == '\0' || (victim = get_char_room(ch, argument)) == NULL) 
		    {
		    send_to_char("Use the tempering on whom?\n\r", ch);
		    return;
		    }
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (!IS_NPC(victim) && victim->pcdata->wolf >= 20) 
		{
		    act("$n looks into your soul and attempts to calm the beast.", ch, NULL, victim, TO_VICT);
		    act("You look at $N's soul and attempt to tame their beast.", ch, NULL, victim, TO_CHAR);
		    act("$n and $N lock eyes and $n attemps to control the $N's beast.", ch, NULL, victim, TO_NOTVICT);

		    if (number_percent() > 35) 
		    {
			send_to_char("You have failed to tame the beast within.\n\r", ch);
			return;
			WAIT_STATE(ch, 5);
			ch->pcdata->powers[NINJAKI] -= 5;
		    } 
		    else 
		    {
			if (IS_SET(victim->special, SPC_WOLFMAN)) 
			{
			    victim->pcdata->wolf -= number_range(50, 75);
			    if (victim->pcdata->wolf < 100)
				do_unwerewolf(victim, "");
			    ch->pcdata->powers[NINJAKI] -= 10;
			    return;
			}
			if (victim->pcdata->wolf < 30) 
			{
			    send_to_char("Their beast is not strong enough to calm.\n\r", ch);
			    return;
			}
			victim->pcdata->wolf -= number_range(20, 40);
			WAIT_STATE(ch, 5);
			ch->pcdata->powers[NINJAKI] -= 10;
			return;
		    }
		}
		send_to_char("They do not have a beast to tame.\n\r", ch);
		return;
	    }*/

	    //Po Path - DemonFangs, Resistance(comm.c), Shadow Nature.

	void do_demonfangs(CHAR_DATA * ch, char *argument)
	{
		char arg[MAX_INPUT_LENGTH];
		     argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_PO] < 1) 
		{
		    send_to_char("You are not proficent enough in the Po Path to use this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if (IS_VAMPAFF(ch, VAM_FANGS)) 
		{
		    send_to_char("You shudder as your fangs withdraw.\n\r", ch);
		    act("$n growls as $s fangs withdraw.", ch, NULL, NULL, TO_ROOM);
		    REMOVE_BIT(ch->vampaff, VAM_FANGS);
		    //ch->pcdata->powers[NINJAKI] -= 10;
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 10) 
		{
		    send_to_char("You do not have enough Chi to do this\n\r", ch);
		    return;
		}
		send_to_char("You scream in rage as a pair of fangs grow from your mouth.\n\r", ch);
		act("$n screams in rage as a pair of fangs grow from $s mouth.", ch, NULL, NULL, TO_ROOM);
		SET_BIT(ch->vampaff, VAM_FANGS);
		ch->pcdata->powers[NINJAKI] -= 10;
		return;
	    }

	  void do_shadownature(CHAR_DATA * ch, char *argument)
	  {
		char buf[MAX_STRING_LENGTH];
		     //CHAR_DATA * victim;
		if(IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_PO] < 3) 
		{
		    send_to_char("You are not proficent enough in the Po Path to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 50) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (IS_POLYAFF(ch, POLY_CHINGSHIH)) 
		{
		    send_to_char("Not in another form!\n\r", ch);
		    return;
		}
		if (IS_POLYAFF(ch, POLY_SHADOWBERSERKER) || IS_POLYAFF(ch, POLY_SHADOWDEMON) || IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
		{
		    send_to_char("You are already consumed by your shadownature!\n\r", ch);
		    return;
		}
		if (IS_SET(ch->polyaff, POLY_HIMA)) 
		{
		    send_to_char("Not while in another form!\n\r", ch);
		    return;
		}
		if (IS_VAMPAFF(ch, VAM_DISGUISED)) 
		{
		    send_to_char("Not while in another form!\n\r", ch);
		    return;
		}
		if (ch->shadowtimer > 1) 
		{
		    send_to_char("Your are still possessed by your shadownature!.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		act("You summon forth your shadow nature", ch, NULL, NULL, TO_CHAR);
		switch (number_range(1, 4)) 
		{
		default:
		case 1:
		    SET_BIT(ch->affected_by, AFF_POLYMORPH);
		    SET_BIT(ch->polyaff, POLY_SHADOWBERSERKER);
		    SET_BIT(ch->vampaff, VAM_DISGUISED);
		    ch->pcdata->wolf += 200;
		    send_to_char("You scream in fury and become a shadow berserker!.\n\r", ch);
		    act("$n becomes a shadow berserker!.", ch, NULL, NULL, TO_ROOM);
		    sprintf(buf, "A Kuei-jin Shadow Berserker [%s]", ch->name);
		    free_string(ch->morph);
		    ch->morph = str_dup(buf);
		    ch->shadowtimer = 0;
		    break;
		case 2:
		    SET_BIT(ch->affected_by, AFF_POLYMORPH);
		    SET_BIT(ch->polyaff, POLY_SHADOWDEMON);
		    SET_BIT(ch->vampaff, VAM_DISGUISED);
		    send_to_char("Your body twists and forms into your shadow demon form!.\n\r", ch);
		    act("$n becomes a shadow demon!.", ch, NULL, NULL, TO_ROOM);
		    sprintf(buf, "A Kuei-jin Shadow Demon [%s]", ch->name);
		    free_string(ch->morph);
		    ch->morph = str_dup(buf);
		    send_to_char("Your horns extend out of your head.\n\r", ch);
		    act("A pair of pointed horns extend from $n's head.", ch, NULL, NULL, TO_ROOM);
		    SET_BIT(ch->pcdata->bodymods, MOD_HORNS);
       		    act("A huge tail extends from $n's back.", ch, NULL, NULL, TO_ROOM);
 	            send_to_char("Your tail slides out of your back side.\n\r", ch);
 		    SET_BIT(ch->pcdata->bodymods, MOD_TAIL);
		    send_to_char("Your eyes start glowing red.\n\r", ch);
		    act("$n's eyes start glowing red.", ch, NULL, NULL, TO_ROOM);
		    SET_BIT(ch->vampaff, VAM_NIGHTSIGHT);
		    ch->pcdata->mod_str = 30;
		    ch->pcdata->mod_dex = 30;
		    ch->pcdata->mod_con = 30;
		    ch->shadowtimer = 0;
		    break;
		case 3:
		    SET_BIT(ch->affected_by, AFF_POLYMORPH);
		    SET_BIT(ch->polyaff, POLY_SHADOWMONKEY);
		    SET_BIT(ch->vampaff, VAM_DISGUISED);
		    send_to_char("Your body twists and forms into your shadow monkey form!.\n\r", ch);
		    act("$n becomes a shadow monkey!.", ch, NULL, NULL, TO_ROOM);
		    sprintf(buf, "A Kuei-jin Shadow Monkey [%s]", ch->name);
		    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
		    send_to_char("Your senses become more attuned to the shadows.\n\r", ch);
		    free_string(ch->morph);
		    ch->pcdata->wolf += 65;
		    ch->morph = str_dup(buf);
		    ch->shadowtimer = 0;
		    break;
		case 4:
		    send_to_char("You fail to summon your shadow nature!.\n\r", ch);
		    break;
		}
		ch->pcdata->powers[NINJAKI] -= 50;
		WAIT_STATE(ch, 10);
		return;
	    }
	    //shadowrelease in interp.c
		void do_unshadownature(CHAR_DATA * ch, char *argument)
		{

		char arg[MAX_INPUT_LENGTH];
		     argument = one_argument(argument, arg);
		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch))
		   return;
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->shadowtimer > 1) 
		{
		    send_to_char("Your shadow nature timer has not run out!.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if (!IS_POLYAFF(ch, POLY_SHADOWBERSERKER) && !IS_POLYAFF(ch, POLY_SHADOWDEMON) && !IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
		    {
		    send_to_char("You are not affected by your shadow nature!\n\r", ch);
		    return;
		    }
		if (IS_POLYAFF(ch, POLY_SHADOWBERSERKER)) 
		{
		    REMOVE_BIT(ch->polyaff, POLY_SHADOWBERSERKER);
//		    ch->pcdata->wolf = 0;
		} 
		else if (IS_POLYAFF(ch, POLY_SHADOWDEMON)) 
		{
		    REMOVE_BIT(ch->polyaff, POLY_SHADOWDEMON);
		    send_to_char("Your tail slides back into your back.\n\r", ch);
		    act("$n's tail slides back into $s back.", ch, NULL, NULL, TO_ROOM);
		    REMOVE_BIT(ch->pcdata->bodymods, MOD_TAIL);
		    send_to_char("Your horns slide back into your head.\n\r", ch);
		    act("$n's horns slide back into $s head.", ch, NULL, NULL, TO_ROOM);
		    REMOVE_BIT(ch->pcdata->bodymods, MOD_HORNS);
		    send_to_char("The red glow in your eyes fades.\n\r", ch);
		    act("The red glow in $n's eyes fades.", ch, NULL, NULL, TO_ROOM);
		    REMOVE_BIT(ch->vampaff, VAM_NIGHTSIGHT);
//		    ch->pcdata->wolf = 0;
		}
		else if (IS_POLYAFF(ch, POLY_SHADOWMONKEY)) 
		{
		    REMOVE_BIT(ch->polyaff, POLY_SHADOWMONKEY);
		    REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
		    send_to_char("Your senses become less sharp.\n\r", ch);
		}
		ch->shadowtimer = 0;
		act("You transform back into human form.", ch, NULL, NULL, TO_CHAR);
		act("$n transforms back from shadowform.", ch, NULL, NULL, TO_ROOM);
		REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		REMOVE_BIT(ch->vampaff, VAM_DISGUISED);
		clear_stats(ch);
		free_string(ch->morph);
		ch->morph = str_dup("");
//		ch->pcdata->wolf = 0;
		return;

	    }

	    //KoE Disciplines to follow unless stated otherwise

    // Equilibrum Discipline:Master Flow(fight.c), Adjust Balance,
		//Shift Balance(fight.c), Chi Interupt, Chi Master
	void do_adjustbalance(CHAR_DATA * ch, char *argument)
	{

		CHAR_DATA      *victim;
		char arg[MAX_INPUT_LENGTH];
		int  sn;
		int  level;
		int  spelltype;
		     argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_EQUILIBRUM] < 2) 
		{
		    send_to_char("You are not proficent enough in the Equilibrum Discipline to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 25) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if ((victim = get_char_room(ch, arg)) == NULL)
		    if ((victim = ch->fighting) == NULL) 
		    {
			send_to_char("They aren't here.\n\r", ch);
			return;
		    }
		if (ch == victim) 
		{
		    send_to_char("Not on yourself.\n\r", ch);
		    return;
		}
		if ((sn = skill_lookup("chibalance")) < 0)
		    return;
		spelltype = skill_table[sn].target;
		level = ch->spl[spelltype] * 0.5;
		(*skill_table[sn].spell_fun) (sn, level, ch, victim);
		WAIT_STATE(ch, 12);
		ch->pcdata->powers[NINJAKI] -= 25;
		return;
	    }

	    //talon like attack
	    /*
	     * void do_chiinterupt(CHAR_DATA * ch, char *argument) {
	     * CHAR_DATA *victim;
	     * 
	     * if (IS_NPC(ch)) return; if (!IS_KOE(ch)) {
	     * send_to_char("Huh?\n\r", ch); return; } if
	     * (ch->pcdata->powers[KOE_EQUILIBRUM] < 4) { send_to_char("You
	     * are not proficent enough in the Equilibrum Discipline to use
	     * this.\n\r", ch); return; } if ((victim = ch->fighting) ==
	     * NULL) { send_to_char("You aren't fighting anyone.\n\r", ch);
	     * return; } if (is_safe(ch, victim)) return;
	     * 
	     * if (number_percent() > 10) multi_hit(ch, victim,
	     * gsn_chiinterrupt); else damage(ch, victim, 0,
	     * gsn_chiinterrupt); WAIT_STATE(ch, 8); return; }
	     */
		// command power, need all colours 200 to use, slightly higher mobs
	    /*
	     * void do_chimaster( CHAR_DATA *ch, char *argument ) { char
	     * arg[MAX_INPUT_LENGTH]; char buf[MAX_STRING_LENGTH]; CHAR_DATA
	     * *victim; argument = one_argument( argument, arg ); if
	     * (IS_NPC(ch)) return; if (!IS_KOE(ch))
	     * {send_to_char("Huh?\n\r",ch);return;}
	     * 
	     * if (ch->pcdata->powers[KOE_EQUILIBRUM] < 5) {send_to_char("You
	     * are not proficent enough in the Equilibrum Discipline to use
	     * this.\n\r",ch);return;} if (ch->pcdata->powers[NINJAKI] < 50)
	     * {send_to_char("You do not have enough Chi to do
	     * this.\n\r",ch);return;} if ( IS_AFF2(ch, AF2_PLACATE) )
	     * {send_to_char( "You are unable to use this while under
	     * protection.\n\r", ch );return;}
	     * 
	     * if (ch->spl[RED_MAGIC] < 200 || ch->spl[BLUE_MAGIC] < 200 ||
	     * ch->spl[PURPLE_MAGIC] < 200 || ch->spl[GREEN_MAGIC] < 200 ||
	     * ch->spl[YELLOW_MAGIC] < 200 ) {send_to_char( "Your mind is to
	     * undisiplined to use this yet.\n\r", ch );return;}
	     * 
	     * if ( arg[0] == '\0' || argument[0] == '\0' ) {send_to_char(
	     * "Command whom to do what?\n\r", ch );return;}
	     * 
	     * if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	     * {send_to_char( "They aren't here.\n\r", ch );return;}
	     * 
	     * if ( victim == ch ) {send_to_char( "How can you command
	     * yourself?\n\r", ch );return;}
	     * 
	     * if ( IS_IMMORTAL(victim) && !IS_MAGE(victim) ) {send_to_char( "I
	     * think not...\n\r", ch );return;}
	     * 
	     * if (!IS_NPC(victim) && victim->pcdata->powers[KOE_PO] > 1 &&
	     * IS_KOE(victim)) {send_to_char( "Something prevents you from
	     * commanding them..odd.\n\r", ch ); send_to_char( "Your P'o
	     * bubbles to the surface to resist being commanded.\n\r", victim
	     * ); return; }
	     * 
	     * int cmd; for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ ) {
	     * if ( argument[0] == cmd_table[cmd].name[0] &&   !str_prefix(
	     * argument, cmd_table[cmd].name ) &&   cmd_table[cmd].level <=
	     * victim->trust ) { if (!str_cmp(cmd_table[cmd].name, "quit")
	     * || !str_cmp(cmd_table[cmd].name, "accept")      ||
	     * !str_cmp(cmd_table[cmd].name, "train")       ||
	     * !str_cmp(cmd_table[cmd].name, "kick")        ||
	     * !str_cmp(cmd_table[cmd].name, "berserk")     ||
	     * !str_cmp(cmd_table[cmd].name, "consent")     ||
	     * !str_cmp(cmd_table[cmd].name, "vampire")     ||
	     * !str_cmp(cmd_table[cmd].name, "propose")     ||
	     * !str_cmp(cmd_table[cmd].name, "gift")        ||
	     * !str_cmp(cmd_table[cmd].name, "put")         ||
	     * !str_cmp(cmd_table[cmd].name, "give")        ||
	     * !str_cmp(cmd_table[cmd].name, "drop")        ||
	     * !str_cmp(cmd_table[cmd].name, "claim")       ||
	     * !str_cmp(cmd_table[cmd].name, "token")       ||
	     * !str_cmp(cmd_table[cmd].name, "bounty")      ||
	     * !str_cmp(cmd_table[cmd].name, "talon")       ||
	     * !str_cmp(cmd_table[cmd].name, "mimattack")   ||
	     * !str_cmp(cmd_table[cmd].name, "steal")       )
	     * 
	     * { send_to_char( "You cannot command them to do that.\n\r", ch );
	     * return; } break; } } if (!IS_NPC(ch) &&
	     * IS_SET(victim->in_room->room_flags,ROOM_SAFE) && ch->race < 5
	     * && victim->race >=5 ) //    if (!IS_NPC(ch) &&
	     * IS_SET(ch->in_room->room_flags,ROOM_SAFE) ) { send_to_char( "A
	     * divine force prevents you from commanding them.\n\r", ch );
	     * return; }
	     * 
	     * if (IS_NPC(victim)) sprintf( buf, "I think %s wants to %s",
	     * victim->short_descr, argument ); else if (!IS_NPC(victim) &&
	     * IS_AFFECTED(victim, AFF_POLYMORPH)) sprintf( buf, "I think %s
	     * wants to %s", victim->morph, argument ); else sprintf( buf, "I
	     * think %s wants to %s", victim->name, argument );
	     * do_say(ch,buf);
	     * 
	     * if ( IS_NPC(victim) && victim->level > 700 ) { act("You shake off
	     * $N's suggestion.",victim,NULL,ch,TO_CHAR); act("$n shakes off
	     * $N's suggestion.",victim,NULL,ch,TO_NOTVICT); act("$n shakes
	     * off your suggestion.",victim,NULL,ch,TO_VICT); act("$s mind is
	     * too strong to overcome.",victim,NULL,ch,TO_VICT); return; }
	     * else if ( victim->spl[BLUE_MAGIC] >= (ch->spl[RED_MAGIC]) ) {
	     * act("You shake off $N's suggestion.",victim,NULL,ch,TO_CHAR);
	     * act("$n shakes off $N's
	     * suggestion.",victim,NULL,ch,TO_NOTVICT); act("$n shakes off
	     * your suggestion.",victim,NULL,ch,TO_VICT); act("$s mind is too
	     * strong to overcome.",victim,NULL,ch,TO_VICT); return; }
	     * act("You blink in confusion.",victim,NULL,NULL,TO_CHAR);
	     * act("$n blinks in confusion.",victim,NULL,NULL,TO_ROOM);
	     * strcpy(buf,"Yes, you're right, I do..."); do_say(victim,buf);
	     * interpret( victim, argument ); WAIT_STATE(ch, 24); return; }
	     */

		// Tapestry Discipline - GhostCall, Chi Ward(fight.c), Chi Shape,
		//Ride the Dragon, Chi Rift(add parts to magic.c)
/*		void do_ghostcall(CHAR_DATA * ch, char *argument){
		CHAR_DATA      *victim;
		     //CHAR_DATA * familiar;
		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_TAPESTRY] < 1) 
		{
		    send_to_char("You are not proficent enough in the Tapestry Discipline to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 50) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("You cannot concentrate enough while fighting.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->pcdata->followers > 1) 
		{
		    send_to_char("You are drained from summoning your last wraith (1 per login).\n\r", ch);
		    return;
		}
		ch->pcdata->followers++;

		victim = create_mobile(get_mob_index(29603));
		victim->short_descr = str_dup("A Wraith Spirit");
		victim->name = str_dup("Wraith Spirit");
		victim->long_descr = str_dup("A wraithly spirit floats here searching for living prey.\n\r");
		victim->level = 600;
		victim->armor = -300;
		victim->hitroll = 200;
		victim->damroll = 200;
		victim->hit = 10000;
		victim->max_hit = 10000;
		free_string(victim->lord);
		victim->lord = str_dup(ch->name);
		SET_BIT(victim->act, ACT_NOEXP);
		SET_BIT(victim->act, ACT_MOUNT);
		char_to_room(victim, ch->in_room);
		act("$N fades into this plane of existance.", ch, NULL, victim, TO_CHAR);
		act("$N fades into this plane of existance.", ch, NULL, victim, TO_ROOM);
		ch->pcdata->powers[NINJAKI] -= 50;
		ch->pcdata->familiar = victim;
		victim->wizard = ch;
		return;
	    }*/

	    void do_ridethedragon(CHAR_DATA * ch, char *argument)
	    {
		char arg[MAX_INPUT_LENGTH];
		ROOM_INDEX_DATA *location;
		CHAR_DATA      *victim;
		one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;

		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_TAPESTRY] < 4) 
		{
		    send_to_char("You are not proficent enough in the Tapestry Discipline to use this.\n\r", ch);
		    return;
		}
		if ((victim = get_char_world(ch, arg)) == NULL) 
		{
		    send_to_char("To whom do you wish to ride the Chi lines to?\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 15) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (!IS_IMMUNE(victim, IMM_SUMMON) && !IS_NPC(victim)) 
		{
		    send_to_char("You failed.\n\r", ch);
		    return;
		}
		if IS_SET (ch->in_room->room_flags, ROOM_ARENA) 
		{
		    send_to_char("You cannot ride the dragon while in arena.\n\r", ch);
		    return;
		}
		if IS_SET (victim->in_room->room_flags, ROOM_ARENA) 
		{
		    send_to_char("You cannot travel into the arena.\n\r", ch);
		    return;
		}
		location = victim->in_room;
		if (room_is_private(location)) 
		{
		    send_to_char("That room is private right now.\n\r", ch);
		    return;
		}
		act("You summon the power to ride the dragon Chi lines.", ch, NULL, NULL, TO_CHAR);
		act("$n vanishs as $s rides the Dragon Chi lines.", ch, NULL, NULL, TO_ROOM);
		ch->pcdata->powers[NINJAKI] -= 50;
		char_from_room(ch);
		char_to_room(ch, location);
		do_look(ch, "auto");
		act("You arrive at your destination.", ch, NULL, NULL, TO_CHAR);
		act("$n arrives via the Dragon Chi lines.", ch, NULL, NULL, TO_CHAR);
		return;
	    }


	    void do_chirift(CHAR_DATA * ch, char *argument)
	    {

		CHAR_DATA      *victim;
		char            arg[MAX_INPUT_LENGTH];
		int             sn;
		int             level;
		int             spelltype;
		                argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_TAPESTRY] < 5) 
		{
		    send_to_char("You are not proficent enough in the Tapestry Discipline to use this.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 25) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if ((victim = get_char_room(ch, arg)) == NULL)
		    if ((victim = ch->fighting) == NULL) 
		    {
			send_to_char("They aren't here.\n\r", ch);
			return;
		    }
		if (ch == victim) 
		{
		    send_to_char("Not on yourself.\n\r", ch);
		    return;
		}
		if ((sn = skill_lookup("chi rift")) < 0)
		    return;
		spelltype = skill_table[sn].target;
		level = ch->spl[spelltype] * 3;
		(*skill_table[sn].spell_fun) (sn, level, ch, victim);
		WAIT_STATE(ch, 12);
		ch->pcdata->powers[NINJAKI] -= 25;
		return;
	    }

	    //this is also used for Yang
		//Yin Prana and Tapestry chi weapons
	void do_chiweapon(CHAR_DATA * ch, char *argument)
	{
		    OBJ_INDEX_DATA *pObjIndex;
		    OBJ_DATA       *obj;
		    char            arg[MAX_INPUT_LENGTH];
		    int             vnum = 0;

		                    argument = one_argument(argument, arg);

		    if (IS_NPC(ch))
		       return;
		    if (!IS_KOE(ch)) 
		    {
			send_to_char("Your are unable to create Chi weapons!\n\r", ch);
			return;
		    }
		    if (ch->position == POS_FIGHTING) 
		    {
			send_to_char("No way! You are fighting.\n\r", ch);
			return;
		    }
		    if (arg[0] == '\0') 
		    {
			send_to_char("Please specify what kind of equipment you want to create:\n\r", ch);
			send_to_char("ChiDagger(Tapestry Discipline), YangStaff(Yang Discipline), YinFang(Yin Discipline)\n\r", ch);
			return;
		    }
		    if (!str_cmp(arg, "ChiDagger") && ch->pcdata->powers[KOE_TAPESTRY] > 2)
			vnum = 29687;
		    else if (!str_cmp(arg, "YangStaff") && ch->pcdata->powers[KOE_DYANG] > 1)
			vnum = 29688;
		    else if (!str_cmp(arg, "YinFang") && ch->pcdata->powers[KOE_DYIN] > 1)
			vnum = 29689;
		    else 
		    {
			send_to_char("You are trying to create something that does not exist or beyond your power.\n\r", ch);
			return;
		    }
		    if (ch->pcdata->powers[NINJAKI] < 25) 
		    {
			send_to_char("You do not have enough Chi to do this.\n\r", ch);
			return;
		    }
		    if (vnum == 0 || (pObjIndex = get_obj_index(vnum)) == NULL) 
		    {
			send_to_char("Missing object, please inform Ktulu.\n\r", ch);
			return;
		    }
		    obj = create_object(pObjIndex, 0);
		    //obj->timer = number_range(2, 5);
		    //gives timer for Chi
			//created weapon
			    obj = create_object(pObjIndex, 50);
		    obj_to_char(obj, ch);
		    act("$p appears in a flash of light.", ch, obj, NULL, TO_CHAR);
		    act("$p appears in $n's hands in a flash of light", ch, obj, NULL, TO_ROOM);
		    //ch->pcdata->powers[NINJAKI] -= 50;
		    ch->pcdata->powers[NINJAKI] -= 25;
		    //It asks for 25
			//,then took away 50..? Boggle..heh
			    return;
		}


	    //Yang Discipline - Principle(fight.c), Yang Mantle, Dragon Dance(fight.c), Eightfold
		// again, readaura type power
		void do_yangmantle(CHAR_DATA * ch, char *argument)
		{
		CHAR_DATA      *victim;
		OBJ_DATA       *obj;
		char            arg[MAX_INPUT_LENGTH];
		char            buf[MAX_INPUT_LENGTH];

		                argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_DYANG] < 2) 
		{
		    send_to_char("You are not proficent enough in the Yang Prana Discipline to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 50) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (arg[0] == '\0') 
		{
		    send_to_char("Read the aura on what?\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if ((victim = get_char_room(ch, arg)) == NULL) 
		{
		    if ((obj = get_obj_carry(ch, arg)) == NULL) 
		    {
			send_to_char("Read the aura on what?\n\r", ch);
			return;
		    }
		    act("$n examines $p closely.", ch, obj, NULL, TO_ROOM);
		    spell_identify(skill_lookup("identify"), ch->level, ch, obj);
		    return;
		}
		if (!IS_NPC(victim) && (IS_KOEAFF(victim, KOE_FROSTAURA) || IS_KOEAFF(victim, KOE_FLAMEAURA) || IS_KOEAFF(victim, KOE_LIGHTNINGAURA))) 
		{
		    send_to_char("You stagger back as your eyes are burned by the avatars aura!.\n\r", ch);
		    ch->hit = ch->hit - 500;
		    return;
		}
		act("$n examines $N intently.", ch, NULL, victim, TO_NOTVICT);
		act("$n examines you intently.", ch, NULL, victim, TO_VICT);
		if (IS_NPC(victim))
		    sprintf(buf, "%s is an NPC.\n\r", victim->short_descr);
		else 
		{
		    sprintf(buf, "%s.\n\r", victim->name);
		    send_to_char(buf, ch);
		}

		if (!IS_NPC(victim)) 
		{
		    sprintf(buf, "Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r", get_curr_str(victim), get_curr_int(victim), get_curr_wis(victim), get_curr_dex(victim), get_curr_con(victim));
		    send_to_char(buf, ch);
		}
		sprintf(buf, "Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r", victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move);
		send_to_char(buf, ch);
		if (!IS_NPC(victim))
		    sprintf(buf, "Hitroll:%d, Damroll:%d, AC:%d.\n\r", char_hitroll(victim), char_damroll(victim), char_ac(victim));
		else
		    sprintf(buf, "AC:%d.\n\r", char_ac(victim));
		send_to_char(buf, ch);
		if (!IS_NPC(victim)) 
		{
		    sprintf(buf, "Status:%d, ", victim->race);
		    send_to_char(buf, ch);
		}
		if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
		    act("$N is pregnant.", ch, NULL, victim, TO_CHAR);

		ch->pcdata->powers[NINJAKI] -= 50;
		return;
	    }

/*	    void do_yangvanish(CHAR_DATA * ch, char *argument)
	    {
		if (IS_NPC(ch))
		    return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_DYANG] < 4) 
		{
		    send_to_char("You are not proficent enough in the Yang Prana Discipline to use this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if (IS_SET(ch->act, WW_VANISH)) 
		{
		    REMOVE_BIT(ch->act, WW_VANISH);
		    send_to_char("You slowly fade into existance.\n\r", ch);
		    act("$n slowly fades into existance.", ch, NULL, NULL, TO_ROOM);
		    return;
		}
		send_to_char("You slowly fade out of existance.\n\r", ch);
		act("$n slowly fades out of existance.", ch, NULL, NULL, TO_ROOM);
		SET_BIT(ch->act, WW_VANISH);
		ch->pcdata->powers[NINJAKI] -= 20;
		return;
	    }
*/
	    void do_scarletqueen(CHAR_DATA * ch, char *argument)
	    {
		char            arg[MAX_INPUT_LENGTH];
		                argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_DYANG] < 5) 
		{
		    send_to_char("You are not proficent enough in the Yang Prana Discipline to use this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if IS_SET (ch->in_room->room_flags, ROOM_ARENA) 
		{
		    send_to_char("You cannot use this while in arena.\n\r", ch);
		    return;
		}
		if (arg[0] == '\0') 
		{
		    if (!IS_AFFECTED(ch, AFF_ETHEREAL)) 
		    {
			if (ch->pcdata->powers[NINJAKI] < 50) 
			{
			    send_to_char("You do not have enough Chi to do this.\n\r", ch);
			    return;
			}
			send_to_char("You become an avatar for the Scarlet Queen.\n\r", ch);
			act("$n becomes an avatar for the Scarlet Queen.\n\r", ch, NULL, NULL, TO_ROOM);
			SET_BIT(ch->affected_by, AFF_ETHEREAL);
			ch->pcdata->powers[NINJAKI] -= 50;
			do_look(ch, "auto");
			if (IS_EXTRA(ch, TIED_UP)) 
			{
			    act("The ropes binding you fall through your ethereal form.", ch, NULL, NULL, TO_CHAR);
			    act("The ropes binding $n fall through $s ethereal form.", ch, NULL, NULL, TO_ROOM);
			    REMOVE_BIT(ch->extra, TIED_UP);
			    REMOVE_BIT(ch->extra, GAGGED);
			    REMOVE_BIT(ch->extra, BLINDFOLDED);
			}
			if (is_affected(ch, gsn_web)) 
			{
			    act("The webbing entrapping $n falls through $s ethereal form.", ch, NULL, NULL, TO_ROOM);
			    send_to_char("The webbing entrapping you falls through your ethereal form.\n\r", ch);
			    affect_strip(ch, gsn_web);
			}
			return;
		    }
		    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
		    send_to_char("You feel the Scarlet Queen leave your body.\n\r", ch);
		    act("$n's becomes more translucent and substantial as the Scarlet Queen leaves $s.", ch, NULL, NULL, TO_ROOM);
		    do_look(ch, "auto");
		    return;
		}
	    }

    //Yin Prana Discipline:Shrouded Moon, Yin Mantle(do_chiweapon, fight.c and below), Bone Dance,
		//Eightfold Yin Mantle, Semblance of the Ebon Dragon
		void do_shroudedmoon(CHAR_DATA * ch, char *argument)
		{
		if (IS_NPC(ch))
		    return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_DYIN] < 1 && IS_KOE(ch)) 
		{
		    send_to_char("You are not proficent enough in the Flesh Shintai to use this.\n\r", ch);
		    return;
		}
		/*
		 * if (ch->pcdata->powers[DHAMPYR_DISCIPLINES] < 2 &&
		 * IS_CLASS(ch, CLASS_DHAMPYR)) { send_to_char("You are not
		 * proficent enough in the Shintai powers to use this.\n\r",
		 * ch); return; }
		 */
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (!IS_IMMUNE(ch, IMM_SHIELDED)) 
		{
		    SET_BIT(ch->immune, IMM_SHIELDED);
		    send_to_char("You shroud your aura.\n\r", ch);
		    act("$n shrouds his aura.", ch, NULL, NULL, TO_ROOM);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 15 && IS_KOE(ch)) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		/*
		 * if (ch->pcdata->powers[NINJAKI] < 5 && IS_CLASS(ch,
		 * CLASS_DHAMPYR)) { send_to_char("You do not have enough Chi
		 * to do this.\n\r", ch); return; }
		 */
		send_to_char("You release the shroud on your aura.\n\r", ch);
		act("$n releases the shroud about their aura.", ch, NULL, NULL, TO_ROOM);
		REMOVE_BIT(ch->immune, IMM_SHIELDED);
		if (IS_KOE(ch))
		    ch->pcdata->powers[NINJAKI] -= 15;
		/*
		 * if (IS_CLASS(ch, CLASS_DHAMPYR))
		 * ch->pcdata->powers[NINJAKI] -= 5;
		 */

		return;
	    }
	    //based off of Kai 's "tame" for Ranger code.
	    /*
	     * void do_bonedance(CHAR_DATA * ch, char *argument) { CHAR_DATA
	     * *victim; AFFECT_DATA af; char arg[MAX_INPUT_LENGTH]; int sn =
	     * 0; int level = magic_average(ch); argument =
	     * one_argument(argument, arg);
	     * 
	     * if (IS_NPC(ch)) return; if (!IS_KOE(ch)) {
	     * send_to_char("Huh?\n\r", ch); return; } if
	     * (ch->pcdata->powers[KOE_DYIN] < 3) { send_to_char("You are not
	     * proficent enough in the Yin Prana Discipline to use
	     * this.\n\r", ch); return; } if (ch->pcdata->powers[NINJAKI] <
	     * 20) { send_to_char("You do not have enough Chi to do
	     * this.\n\r", ch); return; } if (ch->position == POS_FIGHTING) {
	     * send_to_char("No way! You are fighting.\n\r", ch); return; }
	     * if (arg[0] == '\0') { send_to_char("Who do you wish to
	     * bind??\n\r", ch); return; } if ((victim = get_char_room(ch,
	     * arg)) == NULL) { send_to_char("They aren't here.\n\r", ch);
	     * return; } if (!IS_NPC(victim)) { send_to_char("Not on
	     * players.\n\r", ch); return; } if (IS_AFF2(ch, AF2_PLACATE)) {
	     * send_to_char("You are unable to use this while under
	     * protection.\n\r", ch); return; } if (IS_NPC(victim) &&
	     * victim->level > 150) { send_to_char("The beast is too strong
	     * willed.\n\r", ch); return; } //victim = 0; if (victim->master)
	     * stop_follower(victim); add_follower(victim, ch); af.type = sn;
	     * af.duration = number_fuzzy(level / 4); af.location = 0;
	     * af.modifier = 0; af.bitvector = AFF_CHARM;
	     * affect_to_char(victim, &af); SET_BIT(victim->act, ACT_MOUNT);
	     * act("$n envokes the Bone Dance and binds you.", ch, NULL,
	     * victim, TO_VICT); act("You envoke the Bone Dance and bind
	     * $N.", ch, NULL, victim, TO_CHAR); ch->pcdata->powers[NINJAKI]
	     * -= 20; return; }
	     */
	void do_shadowcompanion(CHAR_DATA * ch, char *argument)
	{
		CHAR_DATA      *victim;

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_DYIN] < 4) 
		{
		    send_to_char("You are not proficent enough in the Yin Discipline to use this.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 50) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (ch->pcdata->followers > 1) 
		{
		    send_to_char("You are drained from summoning your last beast (1 per login).\n\r", ch);
		    return;
		}
		ch->pcdata->followers++;

		victim = create_mobile(get_mob_index(29603));
		victim->short_descr = str_dup("A pawned beast");
		victim->name = str_dup("shadowspawn");
		victim->long_descr = str_dup("A beast of the shadows stalks the night for prey.\n\r");
		victim->level = 500;
		victim->armor = ch->armor / 2;
		victim->hitroll = ch->hitroll / 2;
		victim->damroll = ch->damroll / 2;
		victim->hit = ch->hit / 2;
		victim->max_hit = ch->max_hit / 2;
		free_string(victim->lord);
		victim->lord = str_dup(ch->name);
		SET_BIT(victim->act, ACT_NOEXP);
		SET_BIT(victim->act, ACT_MOUNT);
		char_to_room(victim, ch->in_room);
		act("$N seperates from the shadows to serve.", ch, NULL, victim, TO_CHAR);
		act("$N seperates from the shadows to serve.", ch, NULL, victim, TO_ROOM);
		ch->pcdata->powers[NINJAKI] -= 50;
		ch->pcdata->familiar = victim;
		victim->wizard = ch;
		return;
	    }

	    void do_ebondragon(CHAR_DATA * ch, char *argument){
		char arg[MAX_INPUT_LENGTH];
		     argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_DYIN] < 5) 
		{
		    send_to_char("You are not proficent enough in the Yin Prana Discipline to use this.\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if IS_SET (ch->in_room->room_flags, ROOM_ARENA) 
		{
		    send_to_char("You cannot use this while in arena.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if (arg[0] == '\0') 
		{
		    if (!IS_AFFECTED(ch, AFF_SHADOWPLANE)) 
		    {
			if (ch->pcdata->powers[NINJAKI] < 50) 
			{
			    send_to_char("You do not have enough Chi to do this.\n\r", ch);
			    return;
			}
			send_to_char("You become an avatar for the Ebon Dragon.\n\r", ch);
			act("$n becomes an avatar for the Ebon Dragon.\n\r", ch, NULL, NULL, TO_ROOM);
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			ch->pcdata->powers[NINJAKI] -= 50;
			do_look(ch, "auto");
			return;
		    }
		    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
		    send_to_char("You feel the Ebon Dragon leave your body.\n\r", ch);
		    act("$n's becomes more translucent and substantial as the Ebon Dragon leaves $s.", ch, NULL, NULL, TO_ROOM);
		    do_look(ch, "auto");
		    return;
		}
	    }

	    //Black Wind Discipline - dodge, attack, parry(all fight.c), wind walk
		void do_windwalker(CHAR_DATA * ch, char *argument)
		{
		char arg[MAX_INPUT_LENGTH];
		ROOM_INDEX_DATA *location;
		CHAR_DATA      *victim;
		                one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;

		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[KOE_BLACKWIND] < 5) 
		{
		    send_to_char("You are not proficent enough in the Black Wind Discipline to use this.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		if ((victim = get_char_world(ch, arg)) == NULL) 
		{
		    send_to_char("To whom do you wish to ride the wind to?\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if (ch->pcdata->powers[NINJAKI] < 35) 
		{
		    send_to_char("You do not have enough Chi to do this.\n\r", ch);
		    return;
		}
		if (!IS_IMMUNE(victim, IMM_SUMMON) && !IS_NPC(victim)) 
		{
		    send_to_char("You failed.\n\r", ch);
		    return;
		}
		if IS_SET (ch->in_room->room_flags, ROOM_ARENA) 
		{
		    send_to_char("You wind walk while in arena.\n\r", ch);
		    return;
		}
		if IS_SET (victim->in_room->room_flags, ROOM_ARENA) 
		{
		    send_to_char("You cannot travel into the arena.\n\r", ch);
		    return;
		}
		/*
		 * if IS_SET(ch->in_room->room_flags, ROOM_INDOORS)
		 * {send_to_char("You cannot wind walk indoors!\n\r",
		 * ch);return;} if IS_SET(victim->in_room->room_flags,
		 * ROOM_INDOORS) {send_to_char("You cannot wind walk into
		 * buildings.\n\r", ch);return;}
		 */
		location = victim->in_room;

		if (room_is_private(location)) 
		{
		    send_to_char("That room is private right now.\n\r", ch);
		    return;
		}
		act("You summon the Black Wind to ride the skies.", ch, NULL, NULL, TO_CHAR);
		act("$n raises up as $s rides the Black Wind.", ch, NULL, NULL, TO_ROOM);
		ch->pcdata->powers[NINJAKI] -= 35;
		char_from_room(ch);
		char_to_room(ch, location);
		do_look(ch, "auto");
		act("You arrive at your destination.", ch, NULL, NULL, TO_CHAR);
		act("$n floats down from the sky.", ch, NULL, NULL, TO_ROOM);
		return;
	    }

	    //End Kindred of the East Powers, extra stuff ie.fight.c commences

		// classing command.
		void do_awaken(CHAR_DATA * ch, char *argument)
		{
		CHAR_DATA      *victim;
		char            arg[MAX_INPUT_LENGTH];
		                argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (IS_KOE(ch) && !IS_SET(ch->special, SPC_PRINCE) && ch->vampgen <= 2 && ch->vampgen <= 3) {
		    send_to_char("You can't awaken any Kuei-jin!\n\r", ch);
		    return;
		}
		if (arg[0] == '\0') 
		{
		    send_to_char("Awaken whom?\n\r", ch);
		    return;
		}
		if ((victim = get_char_room(ch, arg)) == NULL) 
		{
		    send_to_char("They aren't here.\n\r", ch);
		    return;
		}
		if (IS_NPC(victim)) 
		{
		    send_to_char("Not on NPC's.\n\r", ch);
		    return;
		}
		if (IS_IMMORTAL(victim)) 
		{
		    send_to_char("Not on Immortals's.\n\r", ch);
		    return;
		}
		if (ch == victim) 
		{
		    send_to_char("You cannot awaken yourself.\n\r", ch);
		    return;
		}
		if (IS_KOE(victim)) 
		{
		    send_to_char("They are already a Kuei-jin!\n\r", ch);
		    return;
		}
		/*
		 * if (!IS_SET(victim->special, SPC_RPTRUSTED)) {
		 * send_to_char("They are not rp trusted yet!.\n\r", ch);
		 * return; }
		 */ if (victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim)) {
		    send_to_char("You can only spawn avatars.\n\r", ch);
		    return;
		}
		if (!IS_IMMUNE(victim, IMM_VAMPIRE)) 
		{
		    send_to_char("You cannot awaken an unwilling person.\n\r", ch);
		    return;
		}
		if (victim->hit > 0) 
		{
		    send_to_char("They must be dying to be awakened.\n\r", ch);
		    return;
		}
		act("You send $N's into the Yomi World!!", ch, NULL, victim, TO_CHAR);
		act("$n sends $N's into the Yomi World!!", ch, NULL, victim, TO_NOTVICT);
		act("$n sends $N to the Yomma World!!", ch, NULL, victim, TO_VICT);
		victim->class = CLASS_KOE;
		send_to_char("Welcome to eternal damnation....\n\r", victim);
		free_string(victim->lord);
		victim->lord = str_dup(ch->name);
		free_string(victim->clan);
		victim->clan = str_dup("");
		victim->vampgen = ch->vampgen + 1;
		save_char_obj(ch);
		save_char_obj(victim);
		return;
	    }

	    void do_enlighten(CHAR_DATA * ch, char *argument)
	    {
		CHAR_DATA      *victim;
		char            arg[MAX_INPUT_LENGTH];
		char            buf[MAX_STRING_LENGTH];
		                argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;

		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (IS_KOE(ch) && !IS_SET(ch->special, SPC_PRINCE) && ch->vampgen <=3) 
		{
		    send_to_char("You cannot enlighten any Kuei-jin!\n\r", ch);
		    return;
		}
		if (arg == "/0") 
		{
		    send_to_char("Who do you wish to enlighten?\n\r", ch);
		    return;
		}
		if ((victim = get_char_room(ch, arg)) == NULL) 
		{
		    send_to_char("They aren't here.\n\r", ch);
		    return;
		}
		if (IS_NPC(victim)) 
		{
		    send_to_char("You cannot enlighten a mob, they're too stupid.\n\r", ch);
		    return;
		}
		if (ch == victim) 
		{
		    send_to_char("You're trying to clan yourself.. Riiiight...\n\r", ch);
		    return;
		}
		if (ch->clan == victim->clan) 
		{
		    send_to_char("They already follow you.\n\r", ch);
		    return;
		}
		if (!IS_IMMUNE(victim, IMM_VAMPIRE)) 
		{
		    send_to_char("You cannot enlighten an unwilling person.\n\r", ch);
		    return;
		}
		if (victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim)) 
		{
		    send_to_char("You can only induct those of your stature(Must be avatars).\n\r", ch);
		    return;
		}
		if ((ch->pcdata->powers[KOE_BLOOD] != victim->pcdata->powers[KOE_BLOOD]) || (ch->pcdata->powers[KOE_BONE] != victim->pcdata->powers[KOE_BONE]) || (ch->pcdata->powers[KOE_JADE] != victim->pcdata->powers[KOE_JADE]) || (ch->pcdata->powers[KOE_FLESH] != victim->pcdata->powers[KOE_FLESH]) || (ch->pcdata->powers[KOE_GHOST] != victim->pcdata->powers[KOE_GHOST])) 
		{
		    send_to_char("You really think someone of another path will follow you?\n\r", ch);
		    return;
		}
		free_string(victim->lord);
		victim->lord = str_dup(ch->name);
		free_string(victim->clan);
		victim->clan = str_dup(ch->clan);
		save_char_obj(ch);
		save_char_obj(victim);
		sprintf(buf, "%s is now a member of %s.\n\r", victim->name, ch->clan);
		send_to_char(buf, ch);
		sprintf(buf, "%s has made you a member of %s!\n\r", ch->name, ch->clan);
		send_to_char(buf, victim);
		return;
	    }

	    void do_uplift(CHAR_DATA * ch, char *argument)
	    {
		CHAR_DATA      *victim;
		char            arg[MAX_INPUT_LENGTH];
		char            buf[MAX_STRING_LENGTH];
		                argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;

		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (IS_KOE(ch) && ch->vampgen <= 3) 
		{
		    send_to_char("You cannot uplift any Kuei-jin!\n\r", ch);
		    return;
		}
		if (arg == "/0") 
		{
		    send_to_char("Who do you wish to uplift?\n\r", ch);
		    return;
		}
		if ((victim = get_char_room(ch, arg)) == NULL) 
		{
		    send_to_char("They aren't here.\n\r", ch);
		    return;
		}
		if (IS_NPC(victim)) 
		{
		    send_to_char("You cannot uplift a mob, they're too stupid.\n\r", ch);
		    return;
		}
		if (ch == victim) 
		{
		    send_to_char("You're trying to uplift yourself.. Riiiight...\n\r", ch);
		    return;
		}
		/*
		 * if ((ch->clan) != (victim->clan)) { send_to_char("But they
		 * aren't in your court.\n\r",ch); return; }
		 */

		if (!IS_IMMUNE(victim, IMM_VAMPIRE)) 
		{
		    send_to_char("You cannot uplift an unwilling person.\n\r", ch);
		    return;
		}
		if (victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim)) 
		{
		    send_to_char("You can only uplift those of your stature(Must be avatars).\n\r", ch);
		    return;
		}
		if ((ch->pcdata->powers[KOE_BLOOD] != victim->pcdata->powers[KOE_BLOOD]) || (ch->pcdata->powers[KOE_BONE] != victim->pcdata->powers[KOE_BONE]) || (ch->pcdata->powers[KOE_JADE] != victim->pcdata->powers[KOE_JADE]) || (ch->pcdata->powers[KOE_FLESH] != victim->pcdata->powers[KOE_FLESH]) || (ch->pcdata->powers[KOE_GHOST] != victim->pcdata->powers[KOE_GHOST])) 
		{
		    send_to_char("You really think you can uplift someone of a different court?\n\r", ch);
		    return;
		}
		if (IS_SET(ch->special, SPC_PRINCE)) 
		{
		    sprintf(buf, "%s is no longer a Mandarin of %s.\n\r", victim->name, ch->clan);
		    send_to_char(buf, ch);
		    sprintf(buf, "%s has taken your Mandarin status!\n\r", ch->name);
		    send_to_char(buf, victim);
		    REMOVE_BIT(victim->special, SPC_PRINCE);
		    return;
		}
		if (!IS_SET(ch->special, SPC_PRINCE)) 
		{
		    sprintf(buf, "%s is now a Mandarin of the %s.\n\r", victim->name, ch->clan);
		    send_to_char(buf, ch);
		    sprintf(buf, "%s has made you a Mandarin of the %s!\n\r", ch->name, ch->clan);
		    send_to_char(buf, victim);
		    SET_BIT(victim->special, SPC_PRINCE);
		    return;
		}
	    }

	    //akuma = a 'rogue' Kindred of the East, pretty much here for RP
	//	reasons
		    // may want to add 'demon arts' for only
		    //Akuma Kuei - jin in the future
			// and limit what commands they can use(ie.no Hun commands)
	void do_akuma(CHAR_DATA * ch, char *argument)
	{
			char            arg[MAX_INPUT_LENGTH];
			char            buf[MAX_INPUT_LENGTH];

			                argument = one_argument(argument, arg);

			if (IS_NPC(ch))
			   return;

			if (!IS_KOE(ch)) 
			{
			    send_to_char("Huh?\n\r", ch);
			    return;
			}
			if (strlen(ch->clan) < 1) 
			{
			    send_to_char("But you are not in a clan!\n\r", ch);
			    return;
			}
			if (IS_SET(ch->special, SPC_AKUMA)) 
			{
			    send_to_char("You renounce your Akuma status!\n\r", ch);
			    sprintf(buf, "#r%s#n is no longer an Akuma!\n\r", ch->name);
			    do_info(ch, buf);
			    REMOVE_BIT(ch->special, SPC_AKUMA);
			    return;
			}
			send_to_char("You are now an Akuma!\n\r", ch);
			sprintf(buf, "#r%s#n has become an Akuma!\n\r", ch->name);
			do_info(ch, buf);
			SET_BIT(ch->special, SPC_AKUMA);
			return;
		    }

	    //Power of Kuei - jin depends on dharmic journey not age, mod in fight.c to reflect
		// this.(ages generally should go up about 0.1 to 0.2 in damage)
		void do_dharma(CHAR_DATA * ch, char *argument)
		{
		int             improve = 0;
		int             cost = 0;
		int             max = 0;
		char            arg1[MAX_INPUT_LENGTH];
		char            arg2[MAX_INPUT_LENGTH];
		char            buf[MAX_STRING_LENGTH];
		                argument = one_argument(argument, arg1);
		                argument = one_argument(argument, arg2);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (arg1[0] == '\0' && arg2[0] == '\0') 
		{
		    sprintf(buf, "Dharmic levels Aquired(%d).\n\r", ch->pcdata->powers[KOE_DHARMA]);
		    send_to_char(buf, ch);
		    if (ch->pcdata->powers[KOE_DHARMA] < 1)
			send_to_char("You have not begun your Dharmic Journey yet.\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 1)
			send_to_char("Sataa.\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 2)
			send_to_char("Knaa\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 3)
			send_to_char("Re\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 4)
			send_to_char("Koa\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 5)
			send_to_char("Den\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 6)
			send_to_char("Natou\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 7)
			send_to_char("Surua\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 8)
			send_to_char("Dah\n\r", ch);
		    if (ch->pcdata->powers[KOE_DHARMA] >= 8)
			send_to_char("You have completed your Dharmic Journey.\n\r", ch);
		    return;
		}
		if (arg2[0] == '\0') 
		{
		    if (!str_cmp(arg1, "level")) 
		    {
			send_to_char("@~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~@\n\r", ch);
			send_to_char(">                               <\n\r", ch);
			send_to_char("| Dharmic levels of Advancement |\n\r", ch);
			send_to_char(">~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~<\n\r", ch);
			send_to_char("| 1.           Sataa            |\n\r", ch);
			send_to_char("> 2.           Knaa             <\n\r", ch);
			send_to_char("| 3.           Re               |\n\r", ch);
			send_to_char("> 4.           Koa              <\n\r", ch);
			send_to_char("| 5.           Den              |\n\r", ch);
			send_to_char("> 6.           Natou            <\n\r", ch);
			send_to_char("| 7.           Surua            |\n\r", ch);
			send_to_char("> 8.           Dah              <\n\r", ch);
			send_to_char("@~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^^@\n\r", ch);
			return;

		    }
		    sprintf(buf, "Dharma Advancement aquired (%d).\n\r", ch->pcdata->powers[KOE_DHARMA]);
		    sprintf(buf, "\n\rType Dharma level learn to improve a Dharmic Advancement.\n\r");
		    send_to_char(buf, ch);
		    return;
		}
		if (!str_cmp(arg2, "learn")) 
		{


		    if (!str_cmp(arg1, "level")) 
		    {
			improve = KOE_DHARMA;
			max = 8;
		    }
		    cost = (ch->pcdata->powers[improve] + 1) * 15;
		    arg1[0] = UPPER(arg1[0]);
		    if (ch->pcdata->powers[improve] >= max) 
		    {
			sprintf(buf, "You have learned all your Dharmic levels.\n\r");
			send_to_char(buf, ch);
			return;
		    }
		    if (cost > ch->practice) 
		    {
			sprintf(buf, "It costs you %d primal learn a new Dharma level.\n\r", cost);
			send_to_char(buf, ch);
			return;
		    }
		    ch->pcdata->powers[improve] += 1;
		    ch->practice -= cost;
		    sprintf(buf, "You improve your Dharmic journy.\n\r");
		    send_to_char(buf, ch);
		} else
		    send_to_char("To improve Dharmic Journey, type: Dharma Level Learn.\n\r", ch);
		return;
	    }

	    void do_consume(CHAR_DATA * ch, char *argument)
	    {
		OBJ_DATA       *corpse;
		OBJ_DATA       *c_next;
		OBJ_DATA       *obj;
		OBJ_DATA       *obj_next;
		char            arg[MAX_INPUT_LENGTH];
		char            buf[MAX_INPUT_LENGTH];
		int             points = 0;
		int             bonush = number_range(20, 50), bonusm = number_range(20, 50);
		int             bonusmv = number_range(20, 50);
		                one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (ch->position == POS_FIGHTING) 
		{
		    send_to_char("No way! You are fighting.\n\r", ch);
		    return;
		}
		if (IS_AFF2(ch, AF2_PLACATE)) 
		{
		    send_to_char("You are unable to use this while under protection.\n\r", ch);
		    return;
		}
		for (corpse = ch->in_room->contents; corpse != NULL; corpse = c_next) {
		    c_next = corpse->next_content;
		    if (corpse->item_type != ITEM_CORPSE_NPC && corpse->item_type != ITEM_CORPSE_PC)
			continue;

		    if (IS_KOE(ch)) 
		    {
			points = number_range(1, 20);
			if (ch->hit < ch->max_hit)
			    ch->hit += bonush;
			else
			    bonush = 0;
			if (ch->mana < ch->max_mana)
			    ch->mana += bonusm;
			else
			    bonusm = 0;
			if (ch->move < ch->max_move)
			    ch->move += bonusmv;
			else
			    bonusmv = 0;
			sprintf(buf, "You receive %dHp, %dMana, %dMove, from consuming that corpse.\n\r", bonush, bonusm, bonusmv);
			send_to_char(buf, ch);
		    }		/* else if (IS_CLASS(ch, CLASS_DHAMPYR)) {
				 * points = number_range(1, 4); } */
		    ch->pcdata->powers[NINJAKI] += points;
		    if (ch->pcdata->powers[NINJAKI] >= 500 && IS_KOE(ch)) 
		    {
			ch->pcdata->powers[NINJAKI] = 500;
		    }		/* else if (IS_CLASS(ch, CLASS_DHAMPYR) &&
				 * ch->pcdata->powers[NINJAKI] >= 20) {
				 * ch->pcdata->powers[NINJAKI] = 20; } */
		    act("$n devours the corpse consuming its chi .", ch, NULL, NULL, TO_ROOM);
		    send_to_char(buf, ch);
		    if (corpse->item_type != ITEM_CORPSE_NPC) 
		    {
			for (obj = corpse->contains; obj; obj = obj_next) 
			{
			    obj_next = obj->next_content;
			    obj_from_obj(obj);
			    obj_to_room(obj, ch->in_room);
			}
		    }
		    if (corpse != NULL)
			extract_obj(corpse);
		}
		return;
	    }

	    void do_kotearmor(CHAR_DATA * ch, char *argument)
	    {
		OBJ_INDEX_DATA *pObjIndex;
		OBJ_DATA       *obj;
		char            arg[MAX_INPUT_LENGTH];
		int             vnum = 0;

		                argument = one_argument(argument, arg);

		if (IS_NPC(ch))
		   return;

		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		if (arg[0] == '\0') 
		{
		    send_to_char("Kindred of the East Eq: Robe, Sandles, Pendent, Bracelet.\n\r", ch);
		    return;
		}
		if (!str_cmp(arg, "robe"))
		    vnum = 29714;
		else if (!str_cmp(arg, "sandles"))
		    vnum = 29715;
		else if (!str_cmp(arg, "pendent"))
		    vnum = 29716;
		else if (!str_cmp(arg, "bracelet"))
		    vnum = 29717;
		else 
		{
		    send_to_char("Kindred of the East Eq: Robe, Sandles, Pendent, Bracelet.\n\r", ch);
		    return;
		}
		if (ch->practice < 70) 
		{
		    send_to_char("It costs 70 points of primal to create Kindred equipment.\n\r", ch);
		    return;
		}
		if (vnum == 0 || (pObjIndex = get_obj_index(vnum)) == NULL) 
		{
		    send_to_char("Missing object, please inform Ktulu.\n\r", ch);
		    return;
		}
		ch->practice -= 70;
		obj = create_object(pObjIndex, 50);
		obj_to_char(obj, ch);
		act("$p appears in your hands.", ch, obj, NULL, TO_CHAR);
		act("$p appears in $n hands.", ch, obj, NULL, TO_ROOM);
		return;
	    }

	    void do_pathdisciplineshintai(CHAR_DATA * ch, char *argument)
	    {

		if (IS_NPC(ch))
		    return;
		if (!IS_KOE(ch)) 
		{
		    send_to_char("Huh?\n\r", ch);
		    return;
		}
		ch->pcdata->powers[KOE_PYIN] = 3;
		ch->pcdata->powers[KOE_PYANG] = 2;
		ch->pcdata->powers[KOE_HUN] = 4;
		ch->pcdata->powers[KOE_PO] = 3;
		ch->pcdata->powers[KOE_EQUILIBRUM] = 5;
		ch->pcdata->powers[KOE_TAPESTRY] = 5;
		ch->pcdata->powers[KOE_DYANG] = 5;
		ch->pcdata->powers[KOE_DYIN] = 5;
		ch->pcdata->powers[KOE_DEMONART] = 5;
		ch->pcdata->powers[KOE_PYIN] = 5;
		ch->pcdata->powers[KOE_BLACKWIND] = 5;
		ch->pcdata->powers[KOE_BLOOD] = 5;
		ch->pcdata->powers[KOE_BONE] = 5;
		ch->pcdata->powers[KOE_JADE] = 5;
		ch->pcdata->powers[KOE_FLESH] = 5;
		ch->pcdata->powers[KOE_GHOST] = 5;
		ch->pcdata->powers[KOE_DHARMA] = 8;
		ch->pcdata->powers[NINJAKI] = 600;
		ch->pcdata->powers[KOE_CHIMUH] = 5;
		ch->pcdata->powers[KOE_INTERNALIZE] = 5;
		ch->pcdata->powers[KOE_DRAGONSCALES] = 5;
		send_to_char("abuse this again and die.\n\r", ch);
		return;
	    }
