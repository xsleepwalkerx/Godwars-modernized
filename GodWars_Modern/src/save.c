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
 *  God Wars Mud improvements copyright (C) 1994, 1995, 1996 by Richard    *
 *  Woolcock.  This mud is NOT to be copied in whole or in part, or to be  *
 *  run without the permission of Richard Woolcock.  Nobody else has       *
 *  permission to authorise the use of this code.                          *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif



/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];

char *print_flags(int flag)
{
    int count, pos = 0;
    static char buf[52];


    for (count = 0; count < 32;  count++)
    {
        if (IS_SET(flag,1<<count))
        {
            if (count < 26)
                buf[pos] = 'A' + count;
            else
                buf[pos] = 'a' + (count - 26);
            pos++;
        }
    }

    if (pos == 0)
    {
        buf[pos] = '0';
        pos++;
    }

     buf[pos] = '\0';

    return buf;
}

/*
 * Local functions.
 */
void	fwrite_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fwrite_obj	args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
			    FILE *fp, int iNest ) );
void	fread_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fread_obj	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fread_tiny_obj	args( ( CHAR_DATA *ch,  FILE *fp ) );



/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj( CHAR_DATA *ch )
{
    char strsave[MAX_STRING_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) || ch->level < 2 )
	return;

    if ( ch->desc != NULL && ch->desc->connected != CON_PLAYING )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;
/*
    save_char_obj_backup( ch );
*/
    ch->save_time = current_time;
    fflush( fpReserve );
    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
    if ( ( fp = fopen( strsave, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
	{
	    fwrite_char( ch, fp );
	    if ( ch->carrying != NULL ) 
		fwrite_obj( ch, ch->carrying, fp, 0 );
	}
	fprintf( fp, "#END\n" );
    }
    fflush( fp );
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );

    return;
}

void save_char_obj_backup( CHAR_DATA *ch )
{
    char chlevel [15];
    char buf [MAX_INPUT_LENGTH];
    char strsave[MAX_STRING_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) || ch->level < 2 )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    ch->save_time = current_time;
    fflush( fpReserve );
    fclose( fpReserve );
    sprintf( strsave, "%sstore/%s", PLAYER_DIR, capitalize(ch->name) );
    if ( ( fp = fopen( strsave, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
	{
	    fwrite_char( ch, fp );
	    if ( ch->carrying != NULL ) 
		fwrite_obj( ch, ch->carrying, fp, 0 );
	}
	fprintf( fp, "#END\n" );
	if      (ch->level >= 11) sprintf(chlevel,"<Implementor>");
	else if (ch->level == 10) sprintf(chlevel,"<High Judge>");
	else if (ch->level == 9 ) sprintf(chlevel,"<Judge>");
	else if (ch->level == 8 ) sprintf(chlevel,"<Enforcer>");
	else if (ch->level == 7 ) sprintf(chlevel,"<Quest Maker>");
	else if (ch->level == 6 ) sprintf(chlevel,"<Builder>");
	else if (ch->level == 5 ) sprintf(chlevel,"<Avatar>");
	else if (ch->level == 4 ) sprintf(chlevel,"<Avatar>");
	else if (ch->level == 3 ) sprintf(chlevel,"<Avatar>");
	else                      sprintf(chlevel,"<Mortal>");
	if (strlen(ch->lasttime) > 1)
	    sprintf(buf,"%s Last logged in on %s.\n\r", chlevel, ch->lasttime);
	else
	    sprintf(buf,"%s New player logged in on %s.\n\r", chlevel, ch->createtime);
	fprintf( fp, buf);
    }
    fflush( fp );
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    AFFECT_DATA *paf;
    int sn,i;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"		);

    fprintf( fp, "Name         %s~\n",	ch->name		);

    fprintf( fp, "ShortDescr   %s~\n",	ch->short_descr		);
    fprintf( fp, "LongDescr    %s~\n",	ch->long_descr		);
    fprintf( fp, "Description  %s~\n",	ch->description		);
    fprintf( fp, "Lord         %s~\n",	ch->lord		);
    fprintf( fp, "Clan         %s~\n",	ch->clan		);
    fprintf( fp, "Side         %s~\n",	ch->side		);
    fprintf( fp, "Morph        %s~\n",	ch->morph		);
    fprintf( fp, "Createtime   %s~\n",	ch->createtime		);
    fprintf( fp, "Lasttime     %s~\n",	ch->lasttime		);
    fprintf( fp, "Lasthost     %s~\n",	ch->lasthost		);
    fprintf( fp, "Poweraction  %s~\n",	ch->poweraction		);
    fprintf( fp, "Powertype    %s~\n",	ch->powertype		);
    fprintf( fp, "Prompt       %s~\n",	ch->prompt		);
    fprintf( fp, "Cprompt      %s~\n",	ch->cprompt		);
    fprintf( fp, "Sex          %d\n",	ch->sex			);
    fprintf( fp, "Race         %d\n",	ch->race		);
    fprintf( fp, "Class        %d\n",	ch->class		);
    fprintf( fp, "Immune       %d\n",	ch->immune		);
    fprintf( fp, "Polyaff      %d\n",	ch->polyaff		);
    fprintf( fp, "Itemaffect   %d\n",	ch->itemaffect		);
    fprintf( fp, "Vampaff      %d\n",	ch->vampaff		);
    fprintf( fp, "Vamppass     %d\n",	ch->vamppass		);
    fprintf( fp, "Form         %d\n",	ch->form		);
    fprintf( fp, "Body         %d\n",	ch->body		);
    fprintf( fp, "Beast        %d\n",	ch->beast		);
    fprintf( fp, "Truebeast    %d\n",	ch->truebeast		);
    fprintf( fp, "Vampgen      %d\n",	ch->vampgen		);
    fprintf( fp, "Spectype     %d\n",	ch->spectype		);
    fprintf( fp, "Specpower    %d\n",	ch->specpower		);
    fprintf( fp, "Home         %d\n",	ch->home		);
    fprintf( fp, "Level        %d\n",	ch->level		);
    fprintf( fp, "AFlame       %d\n",   ch->pcdata->aflame	);
    fprintf( fp, "AChat        %d\n",   ch->pcdata->achat	);
    fprintf( fp, "Custom       %d\n",   ch->pcdata->custom	);
    fprintf( fp, "Security     %d\n",	ch->pcdata->security	); /* OLC */
    fprintf( fp, "Trust        %d\n",	ch->trust		);
    fprintf( fp, "Paradox      %d %d %d\n", ch->paradox[0], ch->paradox[1], 
                                             ch->paradox[2] );
    fprintf( fp, "Played       %d\n",
	ch->played + (int) (current_time - ch->logon)		);
    fprintf( fp, "Room         %d\n",
	(  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
	&& ch->was_in_room != NULL )
	    ? ch->was_in_room->vnum
	    : ch->in_room->vnum );
    if (ch->wiznet)
        fprintf( fp, "Wizn %s\n",   print_flags(ch->wiznet));
    fprintf( fp, "PkPdMkMd     %d %d %d %d\n",
	ch->pkill, ch->pdeath, ch->mkill, ch->mdeath );

    fprintf( fp, "Weapons      %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	ch->wpn[0], ch->wpn[1], ch->wpn[2], ch->wpn[3], ch->wpn[4], 
	ch->wpn[5], ch->wpn[6], ch->wpn[7], ch->wpn[8], ch->wpn[9], 
	ch->wpn[10], ch->wpn[11], ch->wpn[12] );
    fprintf( fp, "Spells       %d %d %d %d %d %d\n",
	ch->spl[0], ch->spl[1], ch->spl[2], ch->spl[3], ch->spl[4], ch->spl[5] );
    fprintf( fp, "Combat       %d %d %d %d %d %d %d %d\n",
	ch->cmbt[0], ch->cmbt[1], ch->cmbt[2], ch->cmbt[3],
	ch->cmbt[4], ch->cmbt[5], ch->cmbt[6], ch->cmbt[7] );
    fprintf( fp, "Stance       %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	ch->stance[0], ch->stance[1], ch->stance[2], ch->stance[3],
	ch->stance[4], ch->stance[5], ch->stance[6], ch->stance[7],
	ch->stance[8], ch->stance[9], ch->stance[10],ch->stance[11],
	ch->stance[12],ch->stance[13],ch->stance[14],ch->stance[15]);
    fprintf( fp, "Locationhp   %d %d %d %d %d %d %d\n",
	ch->loc_hp[0], ch->loc_hp[1], ch->loc_hp[2], ch->loc_hp[3],
	ch->loc_hp[4], ch->loc_hp[5], ch->loc_hp[6] );
    fprintf( fp, "HpManaMove   %d %d %d %d %d %d\n",
	ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move );
    fprintf( fp, "Agg          %d\n",	ch->agg			);
    fprintf( fp, "Gold         %d\n",	ch->gold		);
    fprintf( fp, "Bank         %d\n",   ch->bank		); 
    fprintf( fp, "Monkstuff    %d\n",   ch->monkstuff		);
    fprintf( fp, "Monkcrap     %d\n",   ch->monkcrap		);
    fprintf( fp, "Chi	       %d %d\n",  ch->chi[0], ch->chi[1]   );
    fprintf( fp, "Focus        %d %d\n",  ch->focus[0], ch->focus[1] );
    fprintf( fp, "Monkab       %d %d %d %d\n", ch->monkab[0], ch->monkab[1], ch->monkab[2], ch->monkab[3] );
    fprintf( fp, "Exp          %d\n",	ch->exp			);
    fprintf( fp, "Act          %d\n",   ch->act			);
    fprintf( fp, "Extra        %d\n",   ch->extra		);
    fprintf( fp, "More         %d\n",   ch->more		);
    fprintf( fp, "AffectedBy   %d\n",	ch->affected_by		);
    fprintf( fp, "AffectedBy2  %d\n",   ch->affected_by2	);
    fprintf( fp, "Spheres      %d %d %d %d %d %d %d %d %d\n",
         ch->spheres[0], ch->spheres[1], ch->spheres[2], ch->spheres[3],
         ch->spheres[4], ch->spheres[5], ch->spheres[6], ch->spheres[7],
         ch->spheres[8] );
    fprintf( fp, "Mflags       %d\n",  ch->mflags      );
    fprintf( fp, "Quint        %d %d %d\n",  ch->quint[0], ch->quint[1], ch->quint[2]);

    /* Bug fix from Alander */
    fprintf( fp, "Position     %d\n",
        ch->position == POS_FIGHTING ? POS_STANDING : ch->position );

    fprintf( fp, "Practice     %d\n",	ch->practice		);
    fprintf( fp, "SavingThrow  %d\n",	ch->saving_throw	);
    fprintf( fp, "Alignment    %d\n",	ch->alignment		);
    fprintf( fp, "Hitroll      %d\n",	ch->hitroll		);
    fprintf( fp, "Damroll      %d\n",	ch->damroll		);
    fprintf( fp, "Armor        %d\n",	ch->armor		);
    fprintf( fp, "Wimpy        %d\n",	ch->wimpy		);
    fprintf( fp, "Deaf         %d\n",	ch->deaf		);
        /* Save note board status */
        /* Save number of boards in case that number changes */
        fprintf (fp, "Boards       %d ", MAX_BOARD);
        for (i = 0; i < MAX_BOARD; i++)
                fprintf (fp, "%s %ld ", boards[i].short_name, ch->pcdata->last_note[i]);
        fprintf (fp, "\n");
    if ( IS_NPC(ch) )
    {
	fprintf( fp, "Vnum         %d\n",	ch->pIndexData->vnum	);
    }
    else
    {
	fprintf( fp, "Password     %s~\n",	ch->pcdata->pwd		);
	fprintf( fp, "Cfat         %s~\n",	ch->pcdata->cfat	);
	fprintf( fp, "Bamfin       %s~\n",	ch->pcdata->bamfin	);
	fprintf( fp, "Bamfout      %s~\n",	ch->pcdata->bamfout	);
	fprintf( fp, "Title        %s~\n",	ch->pcdata->title	);
	fprintf( fp, "Conception   %s~\n",	ch->pcdata->conception	);
	fprintf( fp, "Parents      %s~\n",	ch->pcdata->parents	);
	fprintf( fp, "Cparents     %s~\n",	ch->pcdata->cparents	);
	fprintf( fp, "Marriage     %s~\n",	ch->pcdata->marriage	);
	fprintf( fp, "Email        %s~\n",	ch->pcdata->email	);
	fprintf( fp, "Conding      %s~\n",	ch->pcdata->conding	);
	fprintf( fp, "Love         %s~\n",	ch->pcdata->love	);
	fprintf( fp, "Coterie      %s~\n",	ch->pcdata->coterie	);
	fprintf( fp, "Covenant     %s~\n",	ch->pcdata->covenant	);
	fprintf( fp, "CotTime      %s~\n",	ch->pcdata->cot_time	);
	fprintf( fp, "CovTime      %s~\n",	ch->pcdata->cov_time	);
	fprintf( fp, "CotClans     %s~\n",	ch->pcdata->cot_clans	);
	fprintf( fp, "CotSides     %s~\n",	ch->pcdata->cot_sides	);
	fprintf( fp, "Stancename   %s~\n",	ch->pcdata->stancename	);
	fprintf( fp, "Stancelist   %s~\n",	ch->pcdata->stancelist	);
	fprintf( fp, "Stancework   %s~\n",	ch->pcdata->stancework	);
	fprintf( fp, "Leader       %s~\n",	ch->pcdata->leader	);
	fprintf( fp, "VLeader      %s~\n",	ch->pcdata->vleader	);
	fprintf( fp, "DenyDate     %d\n",	ch->pcdata->denydate	);
	fprintf( fp, "Stancemove   %d %d %d %d %d\n",	
	    ch->pcdata->stancemove[0],
	    ch->pcdata->stancemove[1],
	    ch->pcdata->stancemove[2],
	    ch->pcdata->stancemove[3],
	    ch->pcdata->stancemove[4]	);
	fprintf( fp, "AttrPerm     %d %d %d %d %d\n",
	    ch->pcdata->perm_str,
	    ch->pcdata->perm_int,
	    ch->pcdata->perm_wis,
	    ch->pcdata->perm_dex,
	    ch->pcdata->perm_con );

	fprintf( fp, "AttrMod      %d %d %d %d %d\n",
	    ch->pcdata->mod_str, 
	    ch->pcdata->mod_int, 
	    ch->pcdata->mod_wis,
	    ch->pcdata->mod_dex, 
	    ch->pcdata->mod_con );

	fprintf( fp, "Bounty       %d\n", ch->pcdata->bounty );
	fprintf( fp, "Quest        %d\n", ch->pcdata->quest );
	fprintf( fp, "Wolf         %d\n", ch->pcdata->wolf );
	fprintf( fp, "Rank         %d\n", ch->pcdata->rank );
	fprintf( fp, "Regenerate   %d\n", ch->pcdata->regenerate );
	fprintf( fp, "Demonic      %d\n", ch->pcdata->demonic );
	fprintf( fp, "Atm          %d\n", ch->pcdata->atm );
	fprintf( fp, "Necromancy   %d\n", ch->pcdata->necromancy );
	fprintf( fp, "Obeah        %d\n", ch->pcdata->obeah );
	fprintf( fp, "Darkness     %d\n", ch->pcdata->darkness );
	fprintf( fp, "Temporis     %d\n", ch->pcdata->temporis );
	fprintf( fp, "Silence      %d\n", ch->pcdata->silence );
	fprintf( fp, "Bodymods     %d\n", ch->pcdata->bodymods );
	fprintf( fp, "Reina        %d\n", ch->pcdata->reina );
	fprintf( fp, "Bpoints      %d\n", ch->pcdata->bpoints );
	fprintf( fp, "Dpoints      %d\n", ch->pcdata->dpoints );
	fprintf( fp, "Gpoints      %d\n", ch->pcdata->gpoints );
	fprintf( fp, "Rpoints      %d\n", ch->pcdata->rpoints );
	fprintf( fp, "Wpoints      %d\n", ch->pcdata->wpoints );
	fprintf( fp, "Spirit       %d\n", ch->pcdata->spirit );
	fprintf( fp, "Conjure      %d\n", ch->pcdata->conjure );
	fprintf( fp, "Celerity     %d\n", ch->pcdata->celerity );
	fprintf( fp, "Firstaid     %d\n", ch->pcdata->firstaid );
	fprintf( fp, "Repair       %d\n", ch->pcdata->repair );
	fprintf( fp, "Teach        %d\n", ch->pcdata->teach );
	fprintf( fp, "Learn        %d\n", ch->pcdata->learn );
	fprintf( fp, "Notes        %d\n", ch->pcdata->notes );
	fprintf( fp, "Evileye      %d\n", ch->pcdata->evileye );
	fprintf( fp, "Gnosis       %d %d\n",
	    ch->pcdata->gnosis[0],
	    ch->pcdata->gnosis[1] );
	fprintf( fp, "Absorb       %d %d %d %d %d %d\n",
	    ch->pcdata->absorb[0],
	    ch->pcdata->absorb[1],
	    ch->pcdata->absorb[2],
	    ch->pcdata->absorb[3],
	    ch->pcdata->absorb[4],
	    ch->pcdata->absorb[5] );
	fprintf( fp, "Qstats       %d %d %d %d %d\n",
	    ch->pcdata->qstats[0],
	    ch->pcdata->qstats[1],
	    ch->pcdata->qstats[2],
	    ch->pcdata->qstats[3],
	    ch->pcdata->qstats[4] );
	fprintf( fp, "Exercise     %d %d %d %d\n",
	    ch->pcdata->exercise[0],
	    ch->pcdata->exercise[1],
	    ch->pcdata->exercise[2],
	    ch->pcdata->exercise[3] );
	fprintf( fp, "Language     %d %d\n",
	    ch->pcdata->language[0],
	    ch->pcdata->language[1] );
	fprintf( fp, "Stage        %d %d %d\n",
	    ch->pcdata->stage[0],
	    ch->pcdata->stage[1],
	    ch->pcdata->stage[2] );
	fprintf( fp, "Status       %d %d %d\n",
	    ch->pcdata->status[0],
	    ch->pcdata->status[1],
	    ch->pcdata->status[2] );
	fprintf( fp, "Wolfform     %d %d\n",
	    ch->pcdata->wolfform[0],
	    ch->pcdata->wolfform[1] );
	fprintf( fp, "Score        %d %d %d %d %d %d\n",
	    ch->pcdata->score[0],
	    ch->pcdata->score[1],
	    ch->pcdata->score[2],
	    ch->pcdata->score[3],
	    ch->pcdata->score[4],
	    ch->pcdata->score[5]);
	fprintf( fp, "Runes        %d %d %d %d\n",
	    ch->pcdata->runes[0],
	    ch->pcdata->runes[1],
	    ch->pcdata->runes[2],
	    ch->pcdata->runes[3]);
	fprintf( fp, "Disc         %d %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->disc[0],
	    ch->pcdata->disc[1],
	    ch->pcdata->disc[2],
	    ch->pcdata->disc[3],
	    ch->pcdata->disc[4],
	    ch->pcdata->disc[5],
	    ch->pcdata->disc[6],
	    ch->pcdata->disc[7],
	    ch->pcdata->disc[8],
	    ch->pcdata->disc[9],
	    ch->pcdata->disc[10]);
	fprintf( fp, "Rituals      %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->rituals[0],
	    ch->pcdata->rituals[1],
	    ch->pcdata->rituals[2],
	    ch->pcdata->rituals[3],
	    ch->pcdata->rituals[4],
	    ch->pcdata->rituals[5],
	    ch->pcdata->rituals[6],
	    ch->pcdata->rituals[7],
	    ch->pcdata->rituals[8],
	    ch->pcdata->rituals[9]);
	fprintf( fp, "Clandisc     %d %d %d\n",
	    ch->pcdata->cland[0],
	    ch->pcdata->cland[1],
	    ch->pcdata->cland[2]);

        fprintf( fp, "Tribegift    %d %d %d\n",
            ch->pcdata->tgift[0],
            ch->pcdata->tgift[1],
            ch->pcdata->tgift[2]);

	fprintf( fp, "Diableries   %d %d\n",
	    ch->pcdata->diableries[0],
	    ch->pcdata->diableries[1] );
	fprintf( fp, "Breed        %d %d %d\n",
	    ch->pcdata->breed[0],
	    ch->pcdata->breed[1],
	    ch->pcdata->breed[2]);
	fprintf( fp, "Auspice      %d %d %d %d %d\n",
	    ch->pcdata->auspice[0], ch->pcdata->auspice[1],
	    ch->pcdata->auspice[2], ch->pcdata->auspice[3],
	    ch->pcdata->auspice[4]);
	fprintf( fp, "Tribes       %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->tribes[0], ch->pcdata->tribes[1], ch->pcdata->tribes[2],
	    ch->pcdata->tribes[3], ch->pcdata->tribes[4], ch->pcdata->tribes[5], 
	    ch->pcdata->tribes[6], ch->pcdata->tribes[7], ch->pcdata->tribes[8],
	    ch->pcdata->tribes[9], ch->pcdata->tribes[10],ch->pcdata->tribes[11],
	    ch->pcdata->tribes[12],ch->pcdata->tribes[13],ch->pcdata->tribes[14],
	    ch->pcdata->tribes[15],ch->pcdata->tribes[16],ch->pcdata->tribes[17],
	    ch->pcdata->tribes[18],ch->pcdata->tribes[19]);
	fprintf( fp, "Powers       %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->powers[0], ch->pcdata->powers[1], ch->pcdata->powers[2],
	    ch->pcdata->powers[3], ch->pcdata->powers[4], ch->pcdata->powers[5], 
	    ch->pcdata->powers[6], ch->pcdata->powers[7], ch->pcdata->powers[8],
	    ch->pcdata->powers[9], ch->pcdata->powers[10],ch->pcdata->powers[11],
	    ch->pcdata->powers[12],ch->pcdata->powers[13],ch->pcdata->powers[14],
	    ch->pcdata->powers[15],ch->pcdata->powers[16],ch->pcdata->powers[17],
	    ch->pcdata->powers[18],ch->pcdata->powers[19],ch->pcdata->powers[20] );
	fprintf( fp, "PowersMod    %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->powers_mod[0], ch->pcdata->powers_mod[1], ch->pcdata->powers_mod[2],
	    ch->pcdata->powers_mod[3], ch->pcdata->powers_mod[4], ch->pcdata->powers_mod[5], 
	    ch->pcdata->powers_mod[6], ch->pcdata->powers_mod[7], ch->pcdata->powers_mod[8],
	    ch->pcdata->powers_mod[9], ch->pcdata->powers_mod[10],ch->pcdata->powers_mod[11],
	    ch->pcdata->powers_mod[12],ch->pcdata->powers_mod[13],ch->pcdata->powers_mod[14],
	    ch->pcdata->powers_mod[15],ch->pcdata->powers_mod[16],ch->pcdata->powers_mod[17],
	    ch->pcdata->powers_mod[18],ch->pcdata->powers_mod[19],ch->pcdata->powers_mod[20]);
	fprintf( fp, "PowersSet    %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->powers_set[0], ch->pcdata->powers_set[1], ch->pcdata->powers_set[2],
	    ch->pcdata->powers_set[3], ch->pcdata->powers_set[4], ch->pcdata->powers_set[5], 
	    ch->pcdata->powers_set[6], ch->pcdata->powers_set[7], ch->pcdata->powers_set[8],
	    ch->pcdata->powers_set[9], ch->pcdata->powers_set[10],ch->pcdata->powers_set[11],
	    ch->pcdata->powers_set[12],ch->pcdata->powers_set[13],ch->pcdata->powers_set[14],
	    ch->pcdata->powers_set[15],ch->pcdata->powers_set[16],ch->pcdata->powers_set[17],
	    ch->pcdata->powers_set[18],ch->pcdata->powers_set[19],ch->pcdata->powers_set[20]);
	fprintf( fp, "PowersWear   %d\n", ch->pcdata->powers_set_wear );
	fprintf( fp, "Genes        %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->genes[0],
	    ch->pcdata->genes[1],
	    ch->pcdata->genes[2],
	    ch->pcdata->genes[3],
	    ch->pcdata->genes[4],
	    ch->pcdata->genes[5],
	    ch->pcdata->genes[6],
	    ch->pcdata->genes[7],
	    ch->pcdata->genes[8],
	    ch->pcdata->genes[9] );
	fprintf( fp, "CotMin       %d %d %d %d %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->cot_min[0],
	    ch->pcdata->cot_min[1],
	    ch->pcdata->cot_min[2],
	    ch->pcdata->cot_min[3],
	    ch->pcdata->cot_min[4],
	    ch->pcdata->cot_min[5],
	    ch->pcdata->cot_min[6],
	    ch->pcdata->cot_min[7],
	    ch->pcdata->cot_min[8],
	    ch->pcdata->cot_min[9],
	    ch->pcdata->cot_min[10],
	    ch->pcdata->cot_min[11] );
	fprintf( fp, "Blood        %d %d %d\n",
	    ch->blood[0],
	    ch->blood[1],
	    ch->blood[2] );
	fprintf( fp, "Willpower    %d %d\n",
	    ch->pcdata->willpower[0],
	    ch->pcdata->willpower[1] );
	fprintf( fp, "Resist       %d %d %d %d %d\n",
	    ch->pcdata->resist[0],
	    ch->pcdata->resist[1],
	    ch->pcdata->resist[2],
	    ch->pcdata->resist[3],
	    ch->pcdata->resist[4] );
	fprintf( fp, "Power        %d %d %d\n",
	    ch->pcdata->power[0],
	    ch->pcdata->power[1],
            ch->pcdata->power[2] );

	fprintf( fp, "FakeCon      %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->fake_skill, 
	    ch->pcdata->fake_stance, 
	    ch->pcdata->fake_hit,
	    ch->pcdata->fake_dam,
	    ch->pcdata->fake_ac,
	    ch->pcdata->fake_hp,
	    ch->pcdata->fake_mana,
	    ch->pcdata->fake_move);

	fprintf( fp, "FakeTwo      %d %d %d %d %d %d %d %d\n",
	    ch->pcdata->fake_max_hp,
	    ch->pcdata->fake_max_mana,
	    ch->pcdata->fake_max_move,
	    ch->pcdata->fake_stats[STAT_STR],
	    ch->pcdata->fake_stats[STAT_INT],
	    ch->pcdata->fake_stats[STAT_WIS],
	    ch->pcdata->fake_stats[STAT_DEX],
	    ch->pcdata->fake_stats[STAT_CON]);

	if (ch->pcdata->obj_vnum != 0)
	    fprintf( fp, "Objvnum      %d\n", ch->pcdata->obj_vnum );

	fprintf( fp, "Majesty      %d\n", ch->pcdata->majesty );

	fprintf( fp, "Condition    %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2] );

	fprintf( fp, "Exhaustion   %d\n", ch->pcdata->exhaustion );
	fprintf( fp, "TempClass    %d\n", ch->pcdata->class );

	fprintf( fp, "ELEMENTAL_ATTACK   %d\n", ch->pcdata->ELEMENTAL_ATTACK );
	fprintf( fp, "ELEMENTAL_DEFENCE  %d\n", ch->pcdata->ELEMENTAL_DEFENCE );

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
	    {
		fprintf( fp, "Skill        %d '%s'\n",
		    ch->pcdata->learned[sn], skill_table[sn].name );
	    }
	}
    }

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	/* Thx Alander */
	if ( paf->type < 0 || paf->type >= MAX_SKILL )
	    continue;

	fprintf( fp, "AffectData   '%s' %3d %3d %3d %10d\n",
	    skill_table[paf->type].name,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector
	    );
    }

    fprintf( fp, "End\n\n" );
    return;
}



/*
 * Write the char optimised for small harddisks.
 */

/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
    bool possessed_item = FALSE;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content != NULL )
	fwrite_obj( ch, obj->next_content, fp, iNest );

    if ( (obj->chobj != NULL && (!IS_NPC(obj->chobj) &&
	obj->chobj->pcdata->obj_vnum != 0)) || obj->item_type == ITEM_KEY)
	possessed_item = TRUE;

    if (obj->item_type == ITEM_KEY)
	return;

    fprintf( fp, "#OBJECT\n" );
    fprintf( fp, "Nest         %d\n",	iNest			     );
    fprintf( fp, "Name         %s~\n",	obj->name		     );
    fprintf( fp, "ShortDescr   %s~\n",	obj->short_descr	     );
    fprintf( fp, "Description  %s~\n",	obj->description	     );
    if (obj->chpoweron != NULL && strlen(obj->chpoweron) > 1 && str_cmp(obj->chpoweron,"(null)") )
    	fprintf( fp, "Poweronch    %s~\n",	obj->chpoweron	     );
    if (obj->chpoweroff != NULL && strlen(obj->chpoweroff) > 1 && str_cmp(obj->chpoweroff,"(null)"))
    	fprintf( fp, "Poweroffch   %s~\n",	obj->chpoweroff      );
    if (obj->chpoweruse != NULL && strlen(obj->chpoweruse) > 1 && str_cmp(obj->chpoweruse,"(null)"))
    	fprintf( fp, "Powerusech   %s~\n",	obj->chpoweruse      );
    if (obj->victpoweron != NULL && strlen(obj->victpoweron) > 1 && str_cmp(obj->victpoweron,"(null)"))
    	fprintf( fp, "Poweronvict  %s~\n",	obj->victpoweron     );
    if (obj->victpoweroff != NULL && strlen(obj->victpoweroff) > 1 && str_cmp(obj->victpoweroff,"(null)"))
    	fprintf( fp, "Poweroffvict %s~\n",	obj->victpoweroff    );
    if (obj->victpoweruse != NULL && strlen(obj->victpoweruse) > 1 && str_cmp(obj->victpoweruse,"(null)"))
    	fprintf( fp, "Powerusevict %s~\n",	obj->victpoweruse    );
    if (obj->actcommands != NULL && strlen(obj->actcommands) > 1)
    	fprintf( fp, "ActCommands  %s~\n",	obj->actcommands     );
    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    	fprintf( fp, "Questmaker   %s~\n",	obj->questmaker      );
    if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    	fprintf( fp, "Questowner   %s~\n",	obj->questowner      );
    fprintf( fp, "Vnum         %d\n",	obj->pIndexData->vnum	     );

    fprintf( fp, "ExtraFlags   %d\n",	obj->extra_flags	     );
    fprintf( fp, "WearFlags    %d\n",	obj->wear_flags		     );
    fprintf( fp, "WearLoc      %d\n",	obj->wear_loc		     );
    fprintf( fp, "ItemType     %d\n",	obj->item_type		     );
    fprintf( fp, "Weight       %d\n",	obj->weight		     );
    if (obj->spectype != 0)
    	fprintf( fp, "Spectype     %d\n",	obj->spectype	     );
    if (obj->specpower != 0)
    	fprintf( fp, "Specpower    %d\n",	obj->specpower	     );
    fprintf( fp, "Condition    %d\n",	obj->condition		     );
    fprintf( fp, "Toughness    %d\n",	obj->toughness		     );
    fprintf( fp, "Resistance   %d\n",	obj->resistance		     );
    if (obj->quest != 0)
    	fprintf( fp, "Quest        %d\n",	obj->quest	     );
    if (obj->points != 0)
    	fprintf( fp, "Points       %d\n",	obj->points	     );
    fprintf( fp, "Level        %d\n",	obj->level		     );
    if (IS_SET(obj->quest, QUEST_VANISH) || possessed_item)
    {
	fprintf( fp, "Timer        1\n" );
    }
    else 
	fprintf( fp, "Timer        %d\n",	obj->timer 	     );
    fprintf( fp, "Cost         %d\n",	obj->cost		     );
    fprintf( fp, "Values       %d %d %d %d\n",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3]	     );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1      '%s'\n", 
		skill_table[obj->value[1]].name );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2      '%s'\n", 
		skill_table[obj->value[2]].name );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;

    case ITEM_SCROLL:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1      '%s'\n", 
		skill_table[obj->value[1]].name );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2      '%s'\n", 
		skill_table[obj->value[2]].name );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
/*
	if ( paf->type < 0 || paf->type >= MAX_SKILL ) continue;
*/
	fprintf( fp, "AffectData   %d %d %d\n",
	    paf->duration, paf->modifier, paf->location );
    }
/*
    if ( vnum >= 17 && ( vnum < 30003 || vnum > 30037 ) )
    {
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	{
**
	    if ( paf->type < 0 || paf->type >= MAX_SKILL ) continue;
**
	    fprintf( fp, "AffectData   %d %d %d\n",
		paf->duration, paf->modifier, paf->location );
	}
    }
*/
    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
    {
	fprintf( fp, "ExtraDescr   %s~ %s~\n",
	    ed->keyword, ed->description );
    }
/*
    if ( vnum >= 17 && ( vnum < 30003 || vnum > 30037 ) )
    {
	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    fprintf( fp, "ExtraDescr   %s~ %s~\n",
		ed->keyword, ed->description );
	}
    }
*/
    fprintf( fp, "End\n\n" );

    if ( obj->contains != NULL )
	fwrite_obj( ch, obj->contains, fp, iNest + 1 );

    return;
}



/*
 * Write an object and its contents.
 */

/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
    static PC_DATA pcdata_zero;
    char strsave[MAX_STRING_LENGTH];
    char *strtime;
    CHAR_DATA *ch;
    FILE *fp;
    bool found;

    if ( char_free == NULL )
    {
	ch				= alloc_perm( sizeof(*ch) );
    }
    else
    {
	ch				= char_free;
	char_free			= char_free->next;
    }
    clear_char( ch );

    if ( pcdata_free == NULL )
    {
	ch->pcdata			= alloc_perm( sizeof(*ch->pcdata) );
    }
    else
    {
	ch->pcdata			= pcdata_free;
	pcdata_free			= pcdata_free->next;
    }
    *ch->pcdata				= pcdata_zero;

    d->character			= ch;
    ch->desc				= d;
    ch->name				= str_dup( name );
    ch->act				= PLR_BLANK
					| PLR_COMBINE
					| PLR_PROMPT;
    ch->extra				= 0;
    ch->more				= 0;
    ch->pcdata->familiar		= NULL;
    ch->pcdata->partner			= NULL;
    ch->pcdata->propose			= NULL;
    ch->pcdata->chobj			= NULL;
    ch->pcdata->memorised		= NULL;
    /* every characters starts at default board from login.. this board
       should be read_level == 0 !
    */
    ch->pcdata->board                   = &boards[DEFAULT_BOARD];
    ch->pcdata->pwd			= str_dup( "" );
    ch->pcdata->cfat			= str_dup( "" );
    ch->pcdata->bounty			= 0;
    ch->pcdata->bamfin			= str_dup( "" );
    ch->pcdata->bamfout			= str_dup( "" );
    ch->pcdata->title			= str_dup( "" );
    ch->pcdata->conception		= str_dup( "" );
    ch->pcdata->parents			= str_dup( "" );
    ch->pcdata->cparents		= str_dup( "" );
    ch->pcdata->marriage		= str_dup( "" );
    ch->pcdata->email			= str_dup( "" );
    ch->pcdata->conding			= str_dup( "" );
    ch->pcdata->love			= str_dup( "" );
    ch->pcdata->coterie			= str_dup( "" );
    ch->pcdata->cot_time		= str_dup( "None" );
    ch->pcdata->covenant		= str_dup( "" );
    ch->pcdata->cov_time		= str_dup( "None" );
    ch->pcdata->cot_clans		= str_dup( "All" );
    ch->pcdata->cot_sides		= str_dup( "All" );
    ch->pcdata->leader			= str_dup( "" );
    ch->pcdata->vleader			= str_dup( "" );
    ch->pcdata->stancename		= str_dup( "" );
    ch->pcdata->stancelist		= str_dup( "" );
    ch->pcdata->stancework		= str_dup( "" );
    ch->lord				= str_dup( "" );
    ch->clan				= str_dup( "" );
    ch->side				= str_dup( "" );
    ch->morph				= str_dup( "" );
    ch->pload				= str_dup( "" );
    ch->prompt				= str_dup( "" );
    ch->cprompt				= str_dup( "" );
    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    free_string(ch->lasttime);
    ch->createtime			= str_dup( strtime );
    ch->lasttime			= str_dup( "" );
    ch->lasthost			= str_dup( "" );
    ch->poweraction			= str_dup( "" );
    ch->powertype			= str_dup( "" );
    ch->hunting				= str_dup( "" );
    ch->pcdata->denydate		= 0;
    ch->pcdata->stancemove[0]		= 0;
    ch->pcdata->stancemove[1]		= 0;
    ch->pcdata->stancemove[2]		= 0;
    ch->pcdata->stancemove[3]		= 0;
    ch->pcdata->stancemove[4]		= 0;
    ch->spectype			= 0;
    ch->specpower			= 0;
    ch->mounted				= 0;
    ch->home				= 3001;
    ch->agg				= 0;
    ch->vampgen				= 0;
    ch->paradox[0]		        = 0;	/* Total paradox */
    ch->paradox[1]		        = 0;	/* Current Paradox */
    ch->paradox[2]	          	= 0;	/* Paradox Ticker */
    ch->damcap[0]	          	= 1000;
    ch->damcap[1]	          	= 0;
    ch->vampaff				= 0;
    ch->itemaffect			= 0;
    ch->vamppass			= -1;
    ch->polyaff				= 0;
    ch->immune				= 0;
    ch->form				= 0;
    ch->init				= 0;
    ch->body				= 32767;
    ch->beast				= 20;
    ch->truebeast			= 20;
    ch->loc_hp[0]			= 0;
    ch->loc_hp[1]			= 0;
    ch->loc_hp[2]			= 0;
    ch->loc_hp[3]			= 0;
    ch->loc_hp[4]			= 0;
    ch->loc_hp[5]			= 0;
    ch->loc_hp[6]			= 0;
    ch->wpn[0]				= 0;
    ch->wpn[1]				= 0;
    ch->wpn[2]				= 0;
    ch->wpn[3]				= 0;
    ch->wpn[4]				= 0;
    ch->wpn[5]				= 0;
    ch->wpn[6]				= 0;
    ch->wpn[7]				= 0;
    ch->wpn[8]				= 0;
    ch->wpn[9]				= 0;
    ch->wpn[10]				= 0;
    ch->wpn[11]				= 0;
    ch->wpn[12]				= 0;
    ch->spl[0]				= 4;
    ch->spl[1]				= 4;
    ch->spl[2]				= 4;
    ch->spl[3]				= 4;
    ch->spl[4]				= 4;
    ch->spl[5]                          = 4;
    ch->cmbt[0]				= 0;
    ch->cmbt[1]				= 0;
    ch->cmbt[2]				= 0;
    ch->cmbt[3]				= 0;
    ch->cmbt[4]				= 0;
    ch->cmbt[5]				= 0;
    ch->cmbt[6]				= 0;
    ch->cmbt[7]				= 0;
    ch->stance[0]			= 0;
    ch->stance[1]			= 0;
    ch->stance[2]			= 0;
    ch->stance[3]			= 0;
    ch->stance[4]			= 0;
    ch->stance[5]			= 0;
    ch->stance[6]			= 0;
    ch->stance[7]			= 0;
    ch->stance[8]			= 0;
    ch->stance[9]			= 0;
    ch->stance[10]			= 0;
    ch->stance[11]			= 0;
    ch->stance[12]			= 0;
    ch->stance[13]			= 0;
    ch->stance[14]			= 0;
    ch->stance[15]			= 0;
    ch->pkill				= 0;
    ch->pdeath				= 0;
    ch->mkill				= 0;
    ch->mdeath				= 0;
    ch->embraced			= 0;
    ch->blood[0]			= 100;
    ch->blood[1]			= 100;
    ch->blood[2]			= 1;
    ch->pcdata->perm_str		= 13;
    ch->pcdata->perm_int		= 13; 
    ch->pcdata->perm_wis		= 13;
    ch->pcdata->perm_dex		= 13;
    ch->pcdata->perm_con		= 13;
    ch->pcdata->quest			= 0;
    ch->pcdata->bounty			= 0;
    ch->pcdata->wolf			= 0;
    ch->pcdata->rank			= 0;
    ch->pcdata->regenerate		= 0;
    ch->pcdata->demonic			= 0;
    ch->pcdata->atm			= 0;
    ch->pcdata->song			= 0;
    ch->pcdata->necromancy		= 0;
    ch->pcdata->obeah			= 0;
    ch->pcdata->darkness		= 0;
    ch->pcdata->temporis		= 0;
    ch->pcdata->silence			= 0;
    ch->pcdata->bodymods		= 0;
    ch->pcdata->reina			= 0;
    ch->pcdata->bpoints			= 0;
    ch->pcdata->dpoints			= 0;
    ch->pcdata->gpoints			= 0;
    ch->pcdata->rpoints			= 0;
    ch->pcdata->wpoints			= 0;
    ch->pcdata->spirit			= 0;
    ch->pcdata->conjure			= 0;
    ch->pcdata->celerity		= 0;
    ch->pcdata->firstaid		= 0;
    ch->pcdata->repair			= 0;
    ch->pcdata->teach			= 0;
    ch->pcdata->learn			= 0;
    ch->pcdata->notes			= 0;
    ch->pcdata->evileye			= 0;
    ch->pcdata->followers		= 0;
    ch->pcdata->gnosis[0]		= 50;
    ch->pcdata->gnosis[1]		= 50;
    ch->pcdata->absorb[0]		= 0;
    ch->pcdata->absorb[1]		= 0;
    ch->pcdata->absorb[2]		= 0;
    ch->pcdata->absorb[3]		= 0;
    ch->pcdata->absorb[4]		= 0;
    ch->pcdata->absorb[5]		= 0;
    ch->pcdata->qstats[0]		= 0;
    ch->pcdata->qstats[1]		= 0;
    ch->pcdata->qstats[2]		= 0;
    ch->pcdata->qstats[3]		= 0;
    ch->pcdata->qstats[4]		= 0;
    ch->pcdata->exercise[0]		= 0;
    ch->pcdata->exercise[1]		= 0;
    ch->pcdata->exercise[2]		= 0;
    ch->pcdata->exercise[3]		= 0;
    ch->pcdata->language[0]		= 0;
    ch->pcdata->language[1]		= 0;
    ch->pcdata->stage[0]		= 0;
    ch->pcdata->stage[1]		= 0;
    ch->pcdata->stage[2]		= 0;
    ch->pcdata->status[0]		= 0;
    ch->pcdata->status[1]		= 0;
    ch->pcdata->status[2]		= 0;
    ch->pcdata->wolfform[0]		= 1;
    ch->pcdata->wolfform[1]		= 1;
    ch->pcdata->score[0]		= 0;
    ch->pcdata->score[1]		= 0;
    ch->pcdata->score[2]		= 0;
    ch->pcdata->score[3]		= 0;
    ch->pcdata->score[4]		= 0;
    ch->pcdata->score[5]		= 0;
    ch->pcdata->runes[0]		= 0;
    ch->pcdata->runes[1]		= 0;
    ch->pcdata->runes[2]		= 0;
    ch->pcdata->runes[3]		= 0;
    ch->pcdata->disc[0]			= 0;
    ch->pcdata->disc[1]			= 0;
    ch->pcdata->disc[2]			= 0;
    ch->pcdata->disc[3]			= 0;
    ch->pcdata->disc[4]			= 0;
    ch->pcdata->disc[5]			= 0;
    ch->pcdata->disc[6]			= 0;
    ch->pcdata->disc[7]			= 0;
    ch->pcdata->disc[8]			= 0;
    ch->pcdata->disc[9]			= 0;
    ch->pcdata->disc[10]		= 0;
    ch->pcdata->rituals[0]		= 0;
    ch->pcdata->rituals[1]		= 0;
    ch->pcdata->rituals[2]		= 0;
    ch->pcdata->rituals[3]		= 0;
    ch->pcdata->rituals[4]		= 0;
    ch->pcdata->rituals[5]		= 0;
    ch->pcdata->rituals[6]		= 0;
    ch->pcdata->rituals[7]		= 0;
    ch->pcdata->rituals[8]		= 0;
    ch->pcdata->rituals[9]		= 0;
    ch->pcdata->cland[0]		= 0;
    ch->pcdata->cland[1]		= 0;
    ch->pcdata->cland[2]		= 0;
    ch->pcdata->tgift[0]		= 0;
    ch->pcdata->tgift[1]                = 0;
    ch->pcdata->tgift[2]                = 0;
    ch->pcdata->diableries[0]		= 0;
    ch->pcdata->diableries[1]		= 0;
    ch->pcdata->breed[0]		= 0;
    ch->pcdata->breed[1]		= 0;
    ch->pcdata->breed[2]		= 0;
    ch->pcdata->auspice[0]		= 0;
    ch->pcdata->auspice[1]		= 0;
    ch->pcdata->auspice[2]		= 0;
    ch->pcdata->auspice[3]		= 0;
    ch->pcdata->auspice[4]		= 0;
    ch->pcdata->tribes[0]		= 0;
    ch->pcdata->tribes[1]		= 0;
    ch->pcdata->tribes[2]		= 0;
    ch->pcdata->tribes[3]		= 0;
    ch->pcdata->tribes[4]		= 0;
    ch->pcdata->tribes[5]		= 0;
    ch->pcdata->tribes[6]		= 0;
    ch->pcdata->tribes[7]		= 0;
    ch->pcdata->tribes[8]		= 0;
    ch->pcdata->tribes[9]		= 0;
    ch->pcdata->tribes[10]		= 0;
    ch->pcdata->tribes[11]		= 0;
    ch->pcdata->tribes[12]		= 0;
    ch->pcdata->tribes[13]		= 0;
    ch->pcdata->tribes[14]		= 0;
    ch->pcdata->tribes[15]		= 0;
    ch->pcdata->tribes[16]		= 0;
    ch->pcdata->tribes[17]		= 0;
    ch->pcdata->tribes[18]		= 0;
    ch->pcdata->tribes[19]		= 0;
    ch->pcdata->powers[0]		= 0;
    ch->pcdata->powers[1]		= 0;
    ch->pcdata->powers[2]		= 0;
    ch->pcdata->powers[3]		= 0;
    ch->pcdata->powers[4]		= 0;
    ch->pcdata->powers[5]		= 0;
    ch->pcdata->powers[6]		= 0;
    ch->pcdata->powers[7]		= 0;
    ch->pcdata->powers[8]		= 0;
    ch->pcdata->powers[9]		= 0;
    ch->pcdata->powers[10]		= 0;
    ch->pcdata->powers[11]		= 0;
    ch->pcdata->powers[12]		= 0;
    ch->pcdata->powers[13]		= 0;
    ch->pcdata->powers[14]		= 0;
    ch->pcdata->powers[15]		= 0;
    ch->pcdata->powers[16]		= 0;
    ch->pcdata->powers[17]		= 0;
    ch->pcdata->powers[18]		= 0;
    ch->pcdata->powers[19]		= 0;
    ch->pcdata->powers[20]		= 0;
    ch->pcdata->powers_mod[0]		= 0;
    ch->pcdata->powers_mod[1]		= 0;
    ch->pcdata->powers_mod[2]		= 0;
    ch->pcdata->powers_mod[3]		= 0;
    ch->pcdata->powers_mod[4]		= 0;
    ch->pcdata->powers_mod[5]		= 0;
    ch->pcdata->powers_mod[6]		= 0;
    ch->pcdata->powers_mod[7]		= 0;
    ch->pcdata->powers_mod[8]		= 0;
    ch->pcdata->powers_mod[9]		= 0;
    ch->pcdata->powers_mod[10]		= 0;
    ch->pcdata->powers_mod[11]		= 0;
    ch->pcdata->powers_mod[12]		= 0;
    ch->pcdata->powers_mod[13]		= 0;
    ch->pcdata->powers_mod[14]		= 0;
    ch->pcdata->powers_mod[15]		= 0;
    ch->pcdata->powers_mod[16]		= 0;
    ch->pcdata->powers_mod[17]		= 0;
    ch->pcdata->powers_mod[18]		= 0;
    ch->pcdata->powers_mod[19]		= 0;
    ch->pcdata->powers_mod[20]		= 0;
    ch->pcdata->powers_set[0]		= 0;
    ch->pcdata->powers_set[1]		= 0;
    ch->pcdata->powers_set[2]		= 0;
    ch->pcdata->powers_set[3]		= 0;
    ch->pcdata->powers_set[4]		= 0;
    ch->pcdata->powers_set[5]		= 0;
    ch->pcdata->powers_set[6]		= 0;
    ch->pcdata->powers_set[7]		= 0;
    ch->pcdata->powers_set[8]		= 0;
    ch->pcdata->powers_set[9]		= 0;
    ch->pcdata->powers_set[10]		= 0;
    ch->pcdata->powers_set[11]		= 0;
    ch->pcdata->powers_set[12]		= 0;
    ch->pcdata->powers_set[13]		= 0;
    ch->pcdata->powers_set[14]		= 0;
    ch->pcdata->powers_set[15]		= 0;
    ch->pcdata->powers_set[16]		= 0;
    ch->pcdata->powers_set[17]		= 0;
    ch->pcdata->powers_set[18]		= 0;
    ch->pcdata->powers_set[19]		= 0;
    ch->pcdata->powers_set[20]		= 0;
    ch->pcdata->powers_set_wear		= 0;
    ch->pcdata->genes[0]		= 0;
    ch->pcdata->genes[1]		= 0;
    ch->pcdata->genes[2]		= 0;
    ch->pcdata->genes[3]		= 0;
    ch->pcdata->genes[4]		= 0;
    ch->pcdata->genes[5]		= 0;
    ch->pcdata->genes[6]		= 0;
    ch->pcdata->genes[7]		= 0;
    ch->pcdata->genes[8]		= 0;
    ch->pcdata->genes[9]		= 0;
    ch->pcdata->cot_min[0]		= 0;
    ch->pcdata->cot_min[1]		= 0;
    ch->pcdata->cot_min[2]		= 0;
    ch->pcdata->cot_min[3]		= 0;
    ch->pcdata->cot_min[4]		= 0;
    ch->pcdata->cot_min[5]		= 0;
    ch->pcdata->cot_min[6]		= 0;
    ch->pcdata->cot_min[7]		= 0;
    ch->pcdata->cot_min[8]		= 0;
    ch->pcdata->cot_min[9]		= 0;
    ch->pcdata->cot_min[10]		= 0;
    ch->pcdata->cot_min[11]		= 0;
    ch->pcdata->willpower[0]		= 50;
    ch->pcdata->willpower[1]		= 50;
    ch->pcdata->resist[0]		= 0;
    ch->pcdata->resist[1]		= 0;
    ch->pcdata->resist[2]		= 0;
    ch->pcdata->resist[3]		= 0;
    ch->pcdata->resist[4]		= 0;
    ch->pcdata->power[0]		= 0;
    ch->pcdata->power[1]		= 0;
    ch->pcdata->fake_skill		= 0;
    ch->pcdata->fake_stance		= 0;
    ch->pcdata->fake_hit		= 0;
    ch->pcdata->fake_dam		= 0;
    ch->pcdata->fake_ac			= 0;
    ch->pcdata->fake_hp			= 0;
    ch->pcdata->fake_mana		= 0;
    ch->pcdata->fake_move		= 0;
    ch->pcdata->fake_max_hp		= 0;
    ch->pcdata->fake_max_mana		= 0;
    ch->pcdata->fake_max_move		= 0;
    ch->pcdata->fake_stats[STAT_STR]	= 0;
    ch->pcdata->fake_stats[STAT_INT]	= 0;
    ch->pcdata->fake_stats[STAT_WIS]	= 0;
    ch->pcdata->fake_stats[STAT_DEX]	= 0;
    ch->pcdata->fake_stats[STAT_CON]	= 0;
    ch->pcdata->obj_vnum		= 0;
    ch->pcdata->majesty			= 0;
    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->exhaustion		= 0;
    ch->pcdata->lie			= 0;
    ch->pcdata->class			= 0;
    ch->pcdata->ELEMENTAL_ATTACK        = 0;   
    ch->pcdata->ELEMENTAL_DEFENCE       = 0; 

    found = FALSE;
    fflush( fpReserve );
    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYER" ) ) fread_char( ch, fp );
	    else if ( !str_cmp( word, "OBJECT" ) ) fread_obj( ch, fp );
	    else if ( !str_cmp( word, "O"      ) ) fread_tiny_obj( ch, fp );
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fflush( fp );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}



bool load_char_short( DESCRIPTOR_DATA *d, char *name )
{
    static PC_DATA pcdata_zero;
    char strsave[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    char *strtime;
    FILE *fp;
    bool found;

    if ( char_free == NULL )
    {
	ch				= alloc_perm( sizeof(*ch) );
    }
    else
    {
	ch				= char_free;
	char_free			= char_free->next;
    }
    clear_char( ch );

    if ( pcdata_free == NULL )
    {
	ch->pcdata			= alloc_perm( sizeof(*ch->pcdata) );
    }
    else
    {
	ch->pcdata			= pcdata_free;
	pcdata_free			= pcdata_free->next;
    }
    *ch->pcdata				= pcdata_zero;

    d->character			= ch;
    ch->desc				= d;
    ch->name				= str_dup( name );
    ch->act				= PLR_BLANK
					| PLR_COMBINE
					| PLR_PROMPT;
    ch->extra				= 0;
    ch->more				= 0;
    ch->pcdata->familiar		= NULL;
    ch->pcdata->partner			= NULL;
    ch->pcdata->propose			= NULL;
    ch->pcdata->chobj			= NULL;
    ch->pcdata->memorised		= NULL;
    ch->pcdata->pwd			= str_dup( "" );
    ch->pcdata->cfat			= str_dup( "" );
    ch->pcdata->bounty			= 0;
    ch->pcdata->bamfin			= str_dup( "" );
    ch->pcdata->bamfout			= str_dup( "" );
    ch->pcdata->title			= str_dup( "" );
    ch->pcdata->conception		= str_dup( "" );
    ch->pcdata->parents			= str_dup( "" );
    ch->pcdata->cparents		= str_dup( "" );
    ch->pcdata->marriage		= str_dup( "" );
    ch->pcdata->email			= str_dup( "" );
    ch->pcdata->conding			= str_dup( "" );
    ch->pcdata->love			= str_dup( "" );
    ch->pcdata->coterie			= str_dup( "" );
    ch->pcdata->cot_time		= str_dup( "None" );
    ch->pcdata->covenant		= str_dup( "" );
    ch->pcdata->cov_time		= str_dup( "None" );
    ch->pcdata->cot_clans		= str_dup( "All" );
    ch->pcdata->cot_sides		= str_dup( "All" );
    ch->pcdata->leader			= str_dup( "" );
    ch->pcdata->vleader			= str_dup( "" );
    ch->pcdata->stancename		= str_dup( "" );
    ch->pcdata->stancelist		= str_dup( "" );
    ch->pcdata->stancework		= str_dup( "" );
    ch->lord				= str_dup( "" );
    ch->clan				= str_dup( "" );
    ch->side				= str_dup( "" );
    ch->morph				= str_dup( "" );
    ch->pload				= str_dup( "" );
    ch->prompt				= str_dup( "" );
    ch->cprompt				= str_dup( "" );
    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    free_string(ch->lasttime);
    ch->createtime			= str_dup( strtime );
    ch->lasttime			= str_dup( "" );
    ch->lasthost			= str_dup( "" );
    ch->poweraction			= str_dup( "" );
    ch->powertype			= str_dup( "" );
    ch->hunting				= str_dup( "" );
    ch->pcdata->denydate		= 0;
    ch->pcdata->stancemove[0]		= 0;
    ch->pcdata->stancemove[1]		= 0;
    ch->pcdata->stancemove[2]		= 0;
    ch->pcdata->stancemove[3]		= 0;
    ch->pcdata->stancemove[4]		= 0;
    ch->spectype			= 0;
    ch->specpower			= 0;
    ch->mounted				= 0;
    ch->home				= 3001;
    ch->agg				= 0;
    ch->vampgen				= 0;
    ch->paradox[0]		        = 0;	/* Total paradox */
    ch->paradox[1]		        = 0;	/* Current Paradox */
    ch->paradox[2]	          	= 0;	/* Paradox Ticker */
    ch->damcap[0]	          	= 1000;
    ch->damcap[1]	          	= 0;
    ch->vampaff				= 0;
    ch->itemaffect			= 0;
    ch->vamppass			= -1;
    ch->polyaff				= 0;
    ch->immune				= 0;
    ch->form				= 0;
    ch->init				= 0;
    ch->body				= 32767;
    ch->beast				= 20;
    ch->truebeast			= 20;
    ch->loc_hp[0]			= 0;
    ch->loc_hp[1]			= 0;
    ch->loc_hp[2]			= 0;
    ch->loc_hp[3]			= 0;
    ch->loc_hp[4]			= 0;
    ch->loc_hp[5]			= 0;
    ch->loc_hp[6]			= 0;
    ch->wpn[0]				= 0;
    ch->wpn[1]				= 0;
    ch->wpn[2]				= 0;
    ch->wpn[3]				= 0;
    ch->wpn[4]				= 0;
    ch->wpn[5]				= 0;
    ch->wpn[6]				= 0;
    ch->wpn[7]				= 0;
    ch->wpn[8]				= 0;
    ch->wpn[9]				= 0;
    ch->wpn[10]				= 0;
    ch->wpn[11]				= 0;
    ch->wpn[12]				= 0;
    ch->spl[0]				= 4;
    ch->spl[1]				= 4;
    ch->spl[2]				= 4;
    ch->spl[3]				= 4;
    ch->spl[4]				= 4;
    ch->spl[5]                          = 4;
    ch->cmbt[0]				= 0;
    ch->cmbt[1]				= 0;
    ch->cmbt[2]				= 0;
    ch->cmbt[3]				= 0;
    ch->cmbt[4]				= 0;
    ch->cmbt[5]				= 0;
    ch->cmbt[6]				= 0;
    ch->cmbt[7]				= 0;
    ch->stance[0]			= 0;
    ch->stance[1]			= 0;
    ch->stance[2]			= 0;
    ch->stance[3]			= 0;
    ch->stance[4]			= 0;
    ch->stance[5]			= 0;
    ch->stance[6]			= 0;
    ch->stance[7]			= 0;
    ch->stance[8]			= 0;
    ch->stance[9]			= 0;
    ch->stance[10]			= 0;
    ch->stance[11]			= 0;
    ch->stance[12]			= 0;
    ch->stance[13]			= 0;
    ch->stance[14]			= 0;
    ch->stance[15]			= 0;
    ch->pkill				= 0;
    ch->pdeath				= 0;
    ch->mkill				= 0;
    ch->mdeath				= 0;
    ch->embraced			= 0;
    ch->blood[0]			= 100;
    ch->blood[1]			= 100;
    ch->blood[2]			= 1;
    ch->pcdata->perm_str		= 13;
    ch->pcdata->perm_int		= 13; 
    ch->pcdata->perm_wis		= 13;
    ch->pcdata->perm_dex		= 13;
    ch->pcdata->perm_con		= 13;
    ch->pcdata->quest			= 0;
    ch->pcdata->bounty			= 0;
    ch->pcdata->wolf			= 0;
    ch->pcdata->rank			= 0;
    ch->pcdata->regenerate		= 0;
    ch->pcdata->demonic			= 0;
    ch->pcdata->atm			= 0;
    ch->pcdata->song			= 0;
    ch->pcdata->necromancy		= 0;
    ch->pcdata->obeah			= 0;
    ch->pcdata->darkness		= 0;
    ch->pcdata->temporis		= 0;
    ch->pcdata->silence			= 0;
    ch->pcdata->bodymods		= 0;
    ch->pcdata->reina			= 0;
    ch->pcdata->bpoints			= 0;
    ch->pcdata->dpoints			= 0;
    ch->pcdata->gpoints			= 0;
    ch->pcdata->rpoints			= 0;
    ch->pcdata->wpoints			= 0;
    ch->pcdata->spirit			= 0;
    ch->pcdata->conjure			= 0;
    ch->pcdata->celerity		= 0;
    ch->pcdata->firstaid		= 0;
    ch->pcdata->repair			= 0;
    ch->pcdata->teach			= 0;
    ch->pcdata->learn			= 0;
    ch->pcdata->notes			= 0;
    ch->pcdata->evileye			= 0;
    ch->pcdata->followers		= 0;
    ch->pcdata->gnosis[0]		= 50;
    ch->pcdata->gnosis[1]		= 50;
    ch->pcdata->absorb[0]		= 0;
    ch->pcdata->absorb[1]		= 0;
    ch->pcdata->absorb[2]		= 0;
    ch->pcdata->absorb[3]		= 0;
    ch->pcdata->absorb[4]		= 0;
    ch->pcdata->absorb[5]		= 0;
    ch->pcdata->qstats[0]		= 0;
    ch->pcdata->qstats[1]		= 0;
    ch->pcdata->qstats[2]		= 0;
    ch->pcdata->qstats[3]		= 0;
    ch->pcdata->qstats[4]		= 0;
    ch->pcdata->exercise[0]		= 0;
    ch->pcdata->exercise[1]		= 0;
    ch->pcdata->exercise[2]		= 0;
    ch->pcdata->exercise[3]		= 0;
    ch->pcdata->language[0]		= 0;
    ch->pcdata->language[1]		= 0;
    ch->pcdata->stage[0]		= 0;
    ch->pcdata->stage[1]		= 0;
    ch->pcdata->stage[2]		= 0;
    ch->pcdata->status[0]		= 0;
    ch->pcdata->status[1]		= 0;
    ch->pcdata->status[2]		= 0;
    ch->pcdata->wolfform[0]		= 1;
    ch->pcdata->wolfform[1]		= 1;
    ch->pcdata->score[0]		= 0;
    ch->pcdata->score[1]		= 0;
    ch->pcdata->score[2]		= 0;
    ch->pcdata->score[3]		= 0;
    ch->pcdata->score[4]		= 0;
    ch->pcdata->score[5]		= 0;
    ch->pcdata->runes[0]		= 0;
    ch->pcdata->runes[1]		= 0;
    ch->pcdata->runes[2]		= 0;
    ch->pcdata->runes[3]		= 0;
    ch->pcdata->disc[0]			= 0;
    ch->pcdata->disc[1]			= 0;
    ch->pcdata->disc[2]			= 0;
    ch->pcdata->disc[3]			= 0;
    ch->pcdata->disc[4]			= 0;
    ch->pcdata->disc[5]			= 0;
    ch->pcdata->disc[6]			= 0;
    ch->pcdata->disc[7]			= 0;
    ch->pcdata->disc[8]			= 0;
    ch->pcdata->disc[9]			= 0;
    ch->pcdata->disc[10]		= 0;
    ch->pcdata->rituals[0]		= 0;
    ch->pcdata->rituals[1]		= 0;
    ch->pcdata->rituals[2]		= 0;
    ch->pcdata->rituals[3]		= 0;
    ch->pcdata->rituals[4]		= 0;
    ch->pcdata->rituals[5]		= 0;
    ch->pcdata->rituals[6]		= 0;
    ch->pcdata->rituals[7]		= 0;
    ch->pcdata->rituals[8]		= 0;
    ch->pcdata->rituals[9]		= 0;
    ch->pcdata->cland[0]		= 0;
    ch->pcdata->cland[1]		= 0;
    ch->pcdata->cland[2]		= 0;
    ch->pcdata->tgift[0]                = 0;
    ch->pcdata->tgift[1]                = 0;
    ch->pcdata->tgift[2]                = 0;
    ch->pcdata->diableries[0]		= 0;
    ch->pcdata->diableries[1]		= 0;
    ch->pcdata->breed[0]		= 0;
    ch->pcdata->breed[1]		= 0;
    ch->pcdata->breed[2]		= 0;
    ch->pcdata->auspice[0]		= 0;
    ch->pcdata->auspice[1]		= 0;
    ch->pcdata->auspice[2]		= 0;
    ch->pcdata->auspice[3]		= 0;
    ch->pcdata->auspice[4]		= 0;
    ch->pcdata->tribes[0]		= 0;
    ch->pcdata->tribes[1]		= 0;
    ch->pcdata->tribes[2]		= 0;
    ch->pcdata->tribes[3]		= 0;
    ch->pcdata->tribes[4]		= 0;
    ch->pcdata->tribes[5]		= 0;
    ch->pcdata->tribes[6]		= 0;
    ch->pcdata->tribes[7]		= 0;
    ch->pcdata->tribes[8]		= 0;
    ch->pcdata->tribes[9]		= 0;
    ch->pcdata->tribes[10]		= 0;
    ch->pcdata->tribes[11]		= 0;
    ch->pcdata->tribes[12]		= 0;
    ch->pcdata->tribes[13]		= 0;
    ch->pcdata->tribes[14]		= 0;
    ch->pcdata->tribes[15]		= 0;
    ch->pcdata->tribes[16]		= 0;
    ch->pcdata->tribes[17]		= 0;
    ch->pcdata->tribes[18]		= 0;
    ch->pcdata->tribes[19]		= 0;
    ch->pcdata->powers[0]		= 0;
    ch->pcdata->powers[1]		= 0;
    ch->pcdata->powers[2]		= 0;
    ch->pcdata->powers[3]		= 0;
    ch->pcdata->powers[4]		= 0;
    ch->pcdata->powers[5]		= 0;
    ch->pcdata->powers[6]		= 0;
    ch->pcdata->powers[7]		= 0;
    ch->pcdata->powers[8]		= 0;
    ch->pcdata->powers[9]		= 0;
    ch->pcdata->powers[10]		= 0;
    ch->pcdata->powers[11]		= 0;
    ch->pcdata->powers[12]		= 0;
    ch->pcdata->powers[13]		= 0;
    ch->pcdata->powers[14]		= 0;
    ch->pcdata->powers[15]		= 0;
    ch->pcdata->powers[16]		= 0;
    ch->pcdata->powers[17]		= 0;
    ch->pcdata->powers[18]		= 0;
    ch->pcdata->powers[19]		= 0;
    ch->pcdata->powers[20]		= 0;
    ch->pcdata->powers_mod[0]		= 0;
    ch->pcdata->powers_mod[1]		= 0;
    ch->pcdata->powers_mod[2]		= 0;
    ch->pcdata->powers_mod[3]		= 0;
    ch->pcdata->powers_mod[4]		= 0;
    ch->pcdata->powers_mod[5]		= 0;
    ch->pcdata->powers_mod[6]		= 0;
    ch->pcdata->powers_mod[7]		= 0;
    ch->pcdata->powers_mod[8]		= 0;
    ch->pcdata->powers_mod[9]		= 0;
    ch->pcdata->powers_mod[10]		= 0;
    ch->pcdata->powers_mod[11]		= 0;
    ch->pcdata->powers_mod[12]		= 0;
    ch->pcdata->powers_mod[13]		= 0;
    ch->pcdata->powers_mod[14]		= 0;
    ch->pcdata->powers_mod[15]		= 0;
    ch->pcdata->powers_mod[16]		= 0;
    ch->pcdata->powers_mod[17]		= 0;
    ch->pcdata->powers_mod[18]		= 0;
    ch->pcdata->powers_mod[19]		= 0;
    ch->pcdata->powers_mod[20]		= 0;
    ch->pcdata->powers_set[0]		= 0;
    ch->pcdata->powers_set[1]		= 0;
    ch->pcdata->powers_set[2]		= 0;
    ch->pcdata->powers_set[3]		= 0;
    ch->pcdata->powers_set[4]		= 0;
    ch->pcdata->powers_set[5]		= 0;
    ch->pcdata->powers_set[6]		= 0;
    ch->pcdata->powers_set[7]		= 0;
    ch->pcdata->powers_set[8]		= 0;
    ch->pcdata->powers_set[9]		= 0;
    ch->pcdata->powers_set[10]		= 0;
    ch->pcdata->powers_set[11]		= 0;
    ch->pcdata->powers_set[12]		= 0;
    ch->pcdata->powers_set[13]		= 0;
    ch->pcdata->powers_set[14]		= 0;
    ch->pcdata->powers_set[15]		= 0;
    ch->pcdata->powers_set[16]		= 0;
    ch->pcdata->powers_set[17]		= 0;
    ch->pcdata->powers_set[18]		= 0;
    ch->pcdata->powers_set[19]		= 0;
    ch->pcdata->powers_set[20]		= 0;
    ch->pcdata->powers_set_wear		= 0;
    ch->pcdata->genes[0]		= 0;
    ch->pcdata->genes[1]		= 0;
    ch->pcdata->genes[2]		= 0;
    ch->pcdata->genes[3]		= 0;
    ch->pcdata->genes[4]		= 0;
    ch->pcdata->genes[5]		= 0;
    ch->pcdata->genes[6]		= 0;
    ch->pcdata->genes[7]		= 0;
    ch->pcdata->genes[8]		= 0;
    ch->pcdata->genes[9]		= 0;
    ch->pcdata->cot_min[0]		= 0;
    ch->pcdata->cot_min[1]		= 0;
    ch->pcdata->cot_min[2]		= 0;
    ch->pcdata->cot_min[3]		= 0;
    ch->pcdata->cot_min[4]		= 0;
    ch->pcdata->cot_min[5]		= 0;
    ch->pcdata->cot_min[6]		= 0;
    ch->pcdata->cot_min[7]		= 0;
    ch->pcdata->cot_min[8]		= 0;
    ch->pcdata->cot_min[9]		= 0;
    ch->pcdata->cot_min[10]		= 0;
    ch->pcdata->cot_min[11]		= 0;
    ch->pcdata->willpower[0]		= 50;
    ch->pcdata->willpower[1]		= 50;
    ch->pcdata->resist[0]		= 0;
    ch->pcdata->resist[1]		= 0;
    ch->pcdata->resist[2]		= 0;
    ch->pcdata->resist[3]		= 0;
    ch->pcdata->resist[4]		= 0;
    ch->pcdata->power[0]		= 0;
    ch->pcdata->power[1]		= 0;
    ch->pcdata->fake_skill		= 0;
    ch->pcdata->fake_stance		= 0;
    ch->pcdata->fake_hit		= 0;
    ch->pcdata->fake_dam		= 0;
    ch->pcdata->fake_ac			= 0;
    ch->pcdata->fake_hp			= 0;
    ch->pcdata->fake_mana		= 0;
    ch->pcdata->fake_move		= 0;
    ch->pcdata->fake_max_hp		= 0;
    ch->pcdata->fake_max_mana		= 0;
    ch->pcdata->fake_max_move		= 0;
    ch->pcdata->fake_stats[STAT_STR]	= 0;
    ch->pcdata->fake_stats[STAT_INT]	= 0;
    ch->pcdata->fake_stats[STAT_WIS]	= 0;
    ch->pcdata->fake_stats[STAT_DEX]	= 0;
    ch->pcdata->fake_stats[STAT_CON]	= 0;
    ch->pcdata->obj_vnum		= 0;
    ch->pcdata->majesty			= 0;
    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->exhaustion		= 0;
    ch->pcdata->lie			= 0;
    ch->pcdata->class			= 0;
    ch->pcdata->ELEMENTAL_ATTACK	= 0;
    ch->pcdata->ELEMENTAL_DEFENCE	= 0;

    found = FALSE;
    fflush( fpReserve );
    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYER" ) ) fread_char ( ch, fp );
	    else if ( !str_cmp( word, "OBJECT" ) ) break;
	    else if ( !str_cmp( word, "OBJ"    ) ) break;
	    else if ( !str_cmp( word, "O"      ) ) break;
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fflush( fp );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if ( !str_cmp( word, "Absorb" ) )
	    {
		ch->pcdata->absorb[0] = fread_number( fp );
		ch->pcdata->absorb[1] = fread_number( fp );
		ch->pcdata->absorb[2] = fread_number( fp );
		ch->pcdata->absorb[3] = fread_number( fp );
		ch->pcdata->absorb[4] = fread_number( fp );
		ch->pcdata->absorb[5] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Act",		ch->act,		fread_number( fp ) );
	    KEY( "AffectedBy",	ch->affected_by,	fread_number( fp ) );
	    KEY( "AffectedBy2", ch->affected_by2,	fread_number( fp ) );
	    KEY( "Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( "Armor",	ch->armor,		fread_number( fp ) );
	    KEY( "AFlame",	ch->pcdata->aflame,	fread_number( fp ) );
	    KEY( "AChat",	ch->pcdata->achat,	fread_number( fp ) );

	    if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
	    {
		AFFECT_DATA *paf;

		if ( affect_free == NULL )
		{
		    paf		= alloc_perm( sizeof(*paf) );
		}
		else
		{
		    paf		= affect_free;
		    affect_free	= affect_free->next;
		}

		if ( !str_cmp( word, "Affect" ) )
		{
		    /* Obsolete 2.0 form. */
		    paf->type	= fread_number( fp );
		}
		else
		{
		    int sn;

		    sn = skill_lookup( fread_word( fp ) );
		    if ( sn < 0 )
			bug( "Fread_char: unknown skill.", 0 );
		    else
			paf->type = sn;
		}

		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= ch->affected;
		ch->affected	= paf;
		fMatch = TRUE;
		break;
	    }

	    KEY( "Agg",		ch->agg,		fread_number( fp ) );

	    if ( !str_cmp( word, "Auspice" ) )
	    {
		ch->pcdata->auspice[0] = fread_number( fp );
		ch->pcdata->auspice[1] = fread_number( fp );
		ch->pcdata->auspice[2] = fread_number( fp );
		ch->pcdata->auspice[3] = fread_number( fp );
		ch->pcdata->auspice[4] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Atm",     	ch->pcdata->atm,	fread_number( fp ) );

	    if ( !str_cmp( word, "AttrMod"  ) )
	    {
		ch->pcdata->mod_str  = fread_number( fp );
		ch->pcdata->mod_int  = fread_number( fp );
		ch->pcdata->mod_wis  = fread_number( fp );
		ch->pcdata->mod_dex  = fread_number( fp );
		ch->pcdata->mod_con  = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrPerm" ) )
	    {
		ch->pcdata->perm_str = fread_number( fp );
		ch->pcdata->perm_int = fread_number( fp );
		ch->pcdata->perm_wis = fread_number( fp );
		ch->pcdata->perm_dex = fread_number( fp );
		ch->pcdata->perm_con = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'B':
	    KEY( "Bank",	ch->bank,		fread_number( fp ) );
	    KEY( "Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );
            KEY( "Bounty",      ch->pcdata->bounty,     fread_number( fp ) );
	    KEY( "Beast",       ch->beast,		fread_number( fp ) );
	    if ( !str_cmp( word, "Blood" ) )
	    {
		ch->blood[0] = fread_number( fp );
		ch->blood[1] = fread_number( fp );
		ch->blood[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
                /* Read in board status */
            if (!str_cmp(word, "Boards" ))
            {
                        int i,num = fread_number (fp); /* number of boards saved */
                        char *boardname;
 
                        for (; num ; num-- ) /* for each of the board saved */
                        {
                                boardname = fread_word (fp);
                                i = board_lookup (boardname); /* find board number */
 
                                if (i == BOARD_NOTFOUND) /* Does board still exist ? */
                                {
                                        sprintf (buf, "fread_char: %s had unknown board name: %s. Skipped.", ch->name, boardname);
                                        log_string (buf);
                                        fread_number (fp); /* read last_note and skip info */
                                }
                                else /* Save it */
                                        ch->pcdata->last_note[i] = fread_number(fp);
                        }        /* for */
 
                        fMatch = TRUE;
            } /* Boards */
	    KEY( "Body",        ch->body,		fread_number( fp ) );
	    KEY( "Bodymods",   	ch->pcdata->bodymods,	fread_number( fp ) );
	    KEY( "Bpoints",    	ch->pcdata->bpoints,	fread_number( fp ) );
	    if ( !str_cmp( word, "Breed" ) )
	    {
		ch->pcdata->breed[0] = fread_number( fp );
		ch->pcdata->breed[1] = fread_number( fp );
		ch->pcdata->breed[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'C':
	    KEY( "Celerity",   	ch->pcdata->celerity,	fread_number( fp ) );
	    KEY( "Clan",	ch->clan,		fread_string( fp ) );
	    KEY( "Custom",	ch->pcdata->custom,     fread_number( fp ) );
	    KEY( "Cfat",	ch->pcdata->cfat,	fread_string( fp ) );

	    if ( !str_cmp( word, "Chi" ) )
	    {
		ch->chi[0] = fread_number( fp );
		ch->chi[1] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    
	    if ( !str_cmp( word, "Clandisc" ) )
	    {
		ch->pcdata->cland[0] = fread_number( fp );
		ch->pcdata->cland[1] = fread_number( fp );
		ch->pcdata->cland[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Class",	ch->class,     		fread_number( fp ) );

	    if ( !str_cmp( word, "Combat" ) )
	    {
		ch->cmbt[0]	= fread_number( fp );
		ch->cmbt[1]	= fread_number( fp );
		ch->cmbt[2]	= fread_number( fp );
		ch->cmbt[3]	= fread_number( fp );
		ch->cmbt[4]	= fread_number( fp );
		ch->cmbt[5]	= fread_number( fp );
		ch->cmbt[6]	= fread_number( fp );
		ch->cmbt[7]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Conception",	ch->pcdata->conception,	fread_string( fp ) );
	    KEY( "Conding",	ch->pcdata->conding,	fread_string( fp ) );

	    if ( !str_cmp( word, "Condition" ) )
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Conjure",    	ch->pcdata->conjure,	fread_number( fp ) );
	    if ( !str_cmp( word, "CotMin" ) )
	    {
		ch->pcdata->cot_min[0] = fread_number( fp );
		ch->pcdata->cot_min[1] = fread_number( fp );
		ch->pcdata->cot_min[2] = fread_number( fp );
		ch->pcdata->cot_min[3] = fread_number( fp );
		ch->pcdata->cot_min[4] = fread_number( fp );
		ch->pcdata->cot_min[5] = fread_number( fp );
		ch->pcdata->cot_min[6] = fread_number( fp );
		ch->pcdata->cot_min[7] = fread_number( fp );
		ch->pcdata->cot_min[8] = fread_number( fp );
		ch->pcdata->cot_min[9] = fread_number( fp );
		ch->pcdata->cot_min[10] = fread_number( fp );
		ch->pcdata->cot_min[11] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Coterie",	ch->pcdata->coterie,	fread_string( fp ) );
	    KEY( "CotTime",	ch->pcdata->cot_time,	fread_string( fp ) );
	    KEY( "Covenant",	ch->pcdata->covenant,	fread_string( fp ) );
	    KEY( "CovTime",	ch->pcdata->cov_time,	fread_string( fp ) );
	    KEY( "CotClans",	ch->pcdata->cot_clans,	fread_string( fp ) );
	    KEY( "CotSides",	ch->pcdata->cot_sides,	fread_string( fp ) );
	    KEY( "Cparents",	ch->pcdata->cparents,	fread_string( fp ) );
	    KEY( "Cprompt",	ch->cprompt,		fread_string( fp ) );
	    KEY( "Createtime",	ch->createtime,		fread_string( fp ) );
	    break;

	case 'D':
	    KEY( "Damroll",	ch->damroll,		fread_number( fp ) );
	    KEY( "Darkness",    ch->pcdata->darkness,	fread_number( fp ) );
	    KEY( "Deaf",	ch->deaf,		fread_number( fp ) );
	    KEY( "Demonic",     ch->pcdata->demonic,	fread_number( fp ) );
	    KEY( "DenyDate",	ch->pcdata->denydate,	fread_number( fp ) );
	    KEY( "Description",	ch->description,	fread_string( fp ) );

	    if ( !str_cmp( word, "Diableries" ) )
	    {
		ch->pcdata->diableries[0] = fread_number( fp );
		ch->pcdata->diableries[1] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Disc" ) )
	    {
		ch->pcdata->disc[0] = fread_number( fp );
		ch->pcdata->disc[1] = fread_number( fp );
		ch->pcdata->disc[2] = fread_number( fp );
		ch->pcdata->disc[3] = fread_number( fp );
		ch->pcdata->disc[4] = fread_number( fp );
		ch->pcdata->disc[5] = fread_number( fp );
		ch->pcdata->disc[6] = fread_number( fp );
		ch->pcdata->disc[7] = fread_number( fp );
		ch->pcdata->disc[8] = fread_number( fp );
		ch->pcdata->disc[9] = fread_number( fp );
		ch->pcdata->disc[10] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Dpoints",    	ch->pcdata->dpoints,	fread_number( fp ) );
	    break;

	case 'E':
            KEY( "ELEMENTAL_ATTACK", ch->pcdata->ELEMENTAL_ATTACK, fread_number( fp ) );
            KEY( "ELEMENTAL_DEFENCE", ch->pcdata->ELEMENTAL_DEFENCE, fread_number( fp ) );
	    if ( !str_cmp( word, "ELEMENTAL_ATTACK" ) )
		fMatch = TRUE;
	    KEY( "Email",	ch->pcdata->email,	fread_string( fp ) );
	    if ( !str_cmp( word, "End" ) )
		return;
	    KEY( "Evileye",   	ch->pcdata->evileye,	fread_number( fp ) );
	    if ( !str_cmp( word, "Exercise" ) )
	    {
		ch->pcdata->exercise[0] = fread_number( fp );
		ch->pcdata->exercise[1] = fread_number( fp );
		ch->pcdata->exercise[2] = fread_number( fp );
		ch->pcdata->exercise[3] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Exhaustion",	ch->pcdata->exhaustion,	fread_number( fp ) );
	    KEY( "Exp",		ch->exp,		fread_number( fp ) );
	    KEY( "Extra",	ch->extra,		fread_number( fp ) );
	    break;

	case 'F':
	   if ( !str_cmp( word, "Focus" ) )
	    {
		ch->focus[0] = fread_number( fp );
		ch->focus[1] = fread_number( fp );
	        fMatch = TRUE;
		break;
	    }
 
	   if ( !str_cmp( word, "FakeCon" ) )
	    {
		ch->pcdata->fake_skill	= fread_number( fp );
		ch->pcdata->fake_stance	= fread_number( fp );
		ch->pcdata->fake_hit	= fread_number( fp );
		ch->pcdata->fake_dam	= fread_number( fp );
		ch->pcdata->fake_ac	= fread_number( fp );
		ch->pcdata->fake_hp	= fread_number( fp );
		ch->pcdata->fake_mana	= fread_number( fp );
		ch->pcdata->fake_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "FakeTwo" ) )
	    {
		ch->pcdata->fake_max_hp		= fread_number( fp );
		ch->pcdata->fake_max_mana	= fread_number( fp );
		ch->pcdata->fake_max_move	= fread_number( fp );
		ch->pcdata->fake_stats[STAT_STR]= fread_number( fp );
		ch->pcdata->fake_stats[STAT_INT]= fread_number( fp );
		ch->pcdata->fake_stats[STAT_WIS]= fread_number( fp );
		ch->pcdata->fake_stats[STAT_DEX]= fread_number( fp );
		ch->pcdata->fake_stats[STAT_CON]= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Firstaid",   	ch->pcdata->firstaid,	fread_number( fp ) );
	    KEY( "Form",        ch->form,		fread_number( fp ) );
	    break;

	case 'G':
	    if ( !str_cmp( word, "Genes" ) )
	    {
		ch->pcdata->genes[0] = fread_number( fp );
		ch->pcdata->genes[1] = fread_number( fp );
		ch->pcdata->genes[2] = fread_number( fp );
		ch->pcdata->genes[3] = fread_number( fp );
		ch->pcdata->genes[4] = fread_number( fp );
		ch->pcdata->genes[5] = fread_number( fp );
		ch->pcdata->genes[6] = fread_number( fp );
		ch->pcdata->genes[7] = fread_number( fp );
		ch->pcdata->genes[8] = fread_number( fp );
		ch->pcdata->genes[9] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Gnosis" ) )
	    {
		ch->pcdata->gnosis[0] = fread_number( fp );
		ch->pcdata->gnosis[1] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Gold",	ch->gold,		fread_number( fp ) );
	    KEY( "Gpoints",    	ch->pcdata->gpoints,	fread_number( fp ) );
	    break;

	case 'H':
	    KEY( "Hitroll",	ch->hitroll,		fread_number( fp ) );
	    KEY( "Home",        ch->home,		fread_number( fp ) );

	    if ( !str_cmp( word, "HpManaMove" ) )
	    {
		ch->hit		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->max_mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'I':
	    KEY( "Immune",        ch->immune,		fread_number( fp ) );
	    KEY( "Itemaffect",    ch->itemaffect,	fread_number( fp ) );
	    break;

	case 'L':
	    if ( !str_cmp( word, "Language" ) )
	    {
		ch->pcdata->language[0] = fread_number( fp );
		ch->pcdata->language[1] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Lasthost",	ch->lasthost,		fread_string( fp ) );
	    KEY( "Lasttime",	ch->lasttime,		fread_string( fp ) );
	    KEY( "Leader",	ch->pcdata->leader,	fread_string( fp ) );
	    KEY( "Learn",   	ch->pcdata->learn,	fread_number( fp ) );
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    if ( !str_cmp( word, "Locationhp" ) )
	    {
		ch->loc_hp[0]	= fread_number( fp );
		ch->loc_hp[1]	= fread_number( fp );
		ch->loc_hp[2]	= fread_number( fp );
		ch->loc_hp[3]	= fread_number( fp );
		ch->loc_hp[4]	= fread_number( fp );
		ch->loc_hp[5]	= fread_number( fp );
		ch->loc_hp[6]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "LongDescr",	ch->long_descr,		fread_string( fp ) );
	    KEY( "Lord",	ch->lord,		fread_string( fp ) );
	    KEY( "Love",	ch->pcdata->love,	fread_string( fp ) );
	    break;

	case 'M':
	    if ( !str_cmp( word, "Monkab" ) )
            {
            ch->monkab[0] = fread_number( fp );
            ch->monkab[1] = fread_number( fp );
            ch->monkab[2] = fread_number( fp );
            ch->monkab[3] = fread_number( fp );
            fMatch = TRUE;
            break;
            }
            KEY( "Monkstuff",       ch->monkstuff, fread_number( fp ) );
            KEY( "Monkcrap",        ch->monkcrap, fread_number( fp ) );

	    KEY( "Majesty",     ch->pcdata->majesty,	fread_number( fp ) );
	    KEY( "Marriage",	ch->pcdata->marriage,	fread_string( fp ) );
	    KEY( "More",	ch->more,		fread_number( fp ) );
            KEY( "Mflags",      ch->mflags,             fread_number( fp ) );
	    KEY( "Morph",	ch->morph,		fread_string( fp ) );
	    break;

	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
		/*
		 * Name already set externally.
		 */
		fread_to_eol( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Necromancy",	ch->pcdata->necromancy,	fread_number( fp ) );
	    KEY( "Notes",   	ch->pcdata->notes,	fread_number( fp ) );
	    break;

	case 'O':
	    KEY( "Obeah",	ch->pcdata->obeah,	fread_number( fp ) );
	    KEY( "Objvnum",     ch->pcdata->obj_vnum,	fread_number( fp ) );
	    break;

	case 'P':
	    if ( !str_cmp( word, "Paradox" ) )
	    {
		ch->paradox[0]	= fread_number( fp );
		ch->paradox[1]	= fread_number( fp );
		ch->paradox[2]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Parents",	ch->pcdata->parents,	fread_string( fp ) );
	    KEY( "Password",	ch->pcdata->pwd,	fread_string( fp ) );

	    KEY( "Played",	ch->played,		fread_number( fp ) );
	    KEY( "Polyaff",     ch->polyaff,		fread_number( fp ) );

	    if ( !str_cmp( word, "Power" ) )
	    {
		ch->pcdata->power[0] = fread_number( fp );
		ch->pcdata->power[1] = fread_number( fp );
                ch->pcdata->power[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Powers" ) )
	    {
		ch->pcdata->powers[0] = fread_number( fp );
		ch->pcdata->powers[1] = fread_number( fp );
		ch->pcdata->powers[2] = fread_number( fp );
		ch->pcdata->powers[3] = fread_number( fp );
		ch->pcdata->powers[4] = fread_number( fp );
		ch->pcdata->powers[5] = fread_number( fp );
		ch->pcdata->powers[6] = fread_number( fp );
		ch->pcdata->powers[7] = fread_number( fp );
		ch->pcdata->powers[8] = fread_number( fp );
		ch->pcdata->powers[9] = fread_number( fp );
		ch->pcdata->powers[10] = fread_number( fp );
		ch->pcdata->powers[11] = fread_number( fp );
		ch->pcdata->powers[12] = fread_number( fp );
		ch->pcdata->powers[13] = fread_number( fp );
		ch->pcdata->powers[14] = fread_number( fp );
		ch->pcdata->powers[15] = fread_number( fp );
		ch->pcdata->powers[16] = fread_number( fp );
		ch->pcdata->powers[17] = fread_number( fp );
		ch->pcdata->powers[18] = fread_number( fp );
		ch->pcdata->powers[19] = fread_number( fp );
		ch->pcdata->powers[20] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "PowersMod" ) )
	    {
		ch->pcdata->powers_mod[0] = fread_number( fp );
		ch->pcdata->powers_mod[1] = fread_number( fp );
		ch->pcdata->powers_mod[2] = fread_number( fp );
		ch->pcdata->powers_mod[3] = fread_number( fp );
		ch->pcdata->powers_mod[4] = fread_number( fp );
		ch->pcdata->powers_mod[5] = fread_number( fp );
		ch->pcdata->powers_mod[6] = fread_number( fp );
		ch->pcdata->powers_mod[7] = fread_number( fp );
		ch->pcdata->powers_mod[8] = fread_number( fp );
		ch->pcdata->powers_mod[9] = fread_number( fp );
		ch->pcdata->powers_mod[10] = fread_number( fp );
		ch->pcdata->powers_mod[11] = fread_number( fp );
		ch->pcdata->powers_mod[12] = fread_number( fp );
		ch->pcdata->powers_mod[13] = fread_number( fp );
		ch->pcdata->powers_mod[14] = fread_number( fp );
		ch->pcdata->powers_mod[15] = fread_number( fp );
		ch->pcdata->powers_mod[16] = fread_number( fp );
		ch->pcdata->powers_mod[17] = fread_number( fp );
		ch->pcdata->powers_mod[18] = fread_number( fp );
		ch->pcdata->powers_mod[19] = fread_number( fp );
		ch->pcdata->powers_mod[20] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "PowersSet" ) )
	    {
		ch->pcdata->powers_set[0] = fread_number( fp );
		ch->pcdata->powers_set[1] = fread_number( fp );
		ch->pcdata->powers_set[2] = fread_number( fp );
		ch->pcdata->powers_set[3] = fread_number( fp );
		ch->pcdata->powers_set[4] = fread_number( fp );
		ch->pcdata->powers_set[5] = fread_number( fp );
		ch->pcdata->powers_set[6] = fread_number( fp );
		ch->pcdata->powers_set[7] = fread_number( fp );
		ch->pcdata->powers_set[8] = fread_number( fp );
		ch->pcdata->powers_set[9] = fread_number( fp );
		ch->pcdata->powers_set[10] = fread_number( fp );
		ch->pcdata->powers_set[11] = fread_number( fp );
		ch->pcdata->powers_set[12] = fread_number( fp );
		ch->pcdata->powers_set[13] = fread_number( fp );
		ch->pcdata->powers_set[14] = fread_number( fp );
		ch->pcdata->powers_set[15] = fread_number( fp );
		ch->pcdata->powers_set[16] = fread_number( fp );
		ch->pcdata->powers_set[17] = fread_number( fp );
		ch->pcdata->powers_set[18] = fread_number( fp );
		ch->pcdata->powers_set[19] = fread_number( fp );
		ch->pcdata->powers_mod[20] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "PowersWear",	ch->pcdata->powers_set_wear,fread_number( fp ) );

	    KEY( "Poweraction",	ch->poweraction,	fread_string( fp ) );
	    KEY( "Powertype",	ch->powertype,		fread_string( fp ) );
	    KEY( "Position",	ch->position,		fread_number( fp ) );
	    KEY( "Practice",	ch->practice,		fread_number( fp ) );
	    if ( !str_cmp( word, "PkPdMkMd" ) )
	    {
		ch->pkill	= fread_number( fp );
		ch->pdeath	= fread_number( fp );
		ch->mkill	= fread_number( fp );
		ch->mdeath	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Prompt",	ch->prompt,		fread_string( fp ) );
	    break;

	case 'Q':
	    if ( !str_cmp( word, "Qstats" ) )
	    {
		ch->pcdata->qstats[0] = fread_number( fp );
		ch->pcdata->qstats[1] = fread_number( fp );
		ch->pcdata->qstats[2] = fread_number( fp );
		ch->pcdata->qstats[3] = fread_number( fp );
		ch->pcdata->qstats[4] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
            if ( !str_cmp( word, "Quint" ) )
            {
                ch->quint[0]    = fread_number( fp );
                ch->quint[1]    = fread_number( fp );
                ch->quint[2]    = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    KEY( "Quest",        ch->pcdata->quest,	fread_number( fp ) );
	    break;

	case 'R':
	    KEY( "Race",        ch->race,		fread_number( fp ) );
	    KEY( "Rank",        ch->pcdata->rank,	fread_number( fp ) );

	    KEY( "Regenerate",	ch->pcdata->regenerate,	fread_number( fp ) );
	    KEY( "Reina",    	ch->pcdata->reina,	fread_number( fp ) );
	    KEY( "Repair",   	ch->pcdata->repair,	fread_number( fp ) );

	    if ( !str_cmp( word, "Resist" ) )
	    {
		ch->pcdata->resist[0] = fread_number( fp );
		ch->pcdata->resist[1] = fread_number( fp );
		ch->pcdata->resist[2] = fread_number( fp );
		ch->pcdata->resist[3] = fread_number( fp );
		ch->pcdata->resist[4] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Rituals" ) )
	    {
		ch->pcdata->rituals[0] = fread_number( fp );
		ch->pcdata->rituals[1] = fread_number( fp );
		ch->pcdata->rituals[2] = fread_number( fp );
		ch->pcdata->rituals[3] = fread_number( fp );
		ch->pcdata->rituals[4] = fread_number( fp );
		ch->pcdata->rituals[5] = fread_number( fp );
		ch->pcdata->rituals[6] = fread_number( fp );
		ch->pcdata->rituals[7] = fread_number( fp );
		ch->pcdata->rituals[8] = fread_number( fp );
		ch->pcdata->rituals[9] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Rpoints",    	ch->pcdata->rpoints,	fread_number( fp ) );

	    if ( !str_cmp( word, "Runes" ) )
	    {
		ch->pcdata->runes[0] = fread_number( fp );
		ch->pcdata->runes[1] = fread_number( fp );
		ch->pcdata->runes[2] = fread_number( fp );
		ch->pcdata->runes[3] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'S':
	    KEY( "SavingThrow",	ch->saving_throw,	fread_number( fp ) );

	    if ( !str_cmp( word, "Score" ) )
	    {
		ch->pcdata->score[0] = fread_number( fp );
		ch->pcdata->score[1] = fread_number( fp );
		ch->pcdata->score[2] = fread_number( fp );
		ch->pcdata->score[3] = fread_number( fp );
		ch->pcdata->score[4] = fread_number( fp );
		ch->pcdata->score[5] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

            if ( !str_cmp( word, "Spheres" ) )
            {
                ch->spheres[0] = fread_number( fp );
                ch->spheres[1] = fread_number( fp );
                ch->spheres[2] = fread_number( fp );
                ch->spheres[3] = fread_number( fp );
                ch->spheres[4] = fread_number( fp );
                ch->spheres[5] = fread_number( fp );
                ch->spheres[6] = fread_number( fp );
                ch->spheres[7] = fread_number( fp );
                ch->spheres[8] = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    KEY( "Security",   	ch->pcdata->security,	fread_number( fp ) ); /* OLC */
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    KEY( "ShortDescr",	ch->short_descr,	fread_string( fp ) );
	    KEY( "Side",	ch->side,		fread_string( fp ) );
	    KEY( "Silence",    	ch->pcdata->silence,	fread_number( fp ) );

	    if ( !str_cmp( word, "Skill" ) )
	    {
		int sn;
		int value;

		value = fread_number( fp );
		sn    = skill_lookup( fread_word( fp ) );
/*
		if ( sn < 0 )
		    bug( "Fread_char: unknown skill.", 0 );
		else
		    ch->pcdata->learned[sn] = value;
*/
		if ( sn >= 0 )
		    ch->pcdata->learned[sn] = value;
		fMatch = TRUE;
	    }

	    KEY( "Specpower",	ch->specpower,	fread_number( fp ) );
	    KEY( "Spectype",	ch->spectype,	fread_number( fp ) );

	    if ( !str_cmp( word, "Spells" ) )
	    {
		ch->spl[0]	= fread_number( fp );
		ch->spl[1]	= fread_number( fp );
		ch->spl[2]	= fread_number( fp );
		ch->spl[3]	= fread_number( fp );
		ch->spl[4]	= fread_number( fp );
                ch->spl[5]      = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Spirit",    	ch->pcdata->spirit,	fread_number( fp ) );

	    if ( !str_cmp( word, "Stage" ) )
	    {
		ch->pcdata->stage[0] = fread_number( fp );
		ch->pcdata->stage[1] = fread_number( fp );
		ch->pcdata->stage[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Stance" ) )
	    {
		ch->stance[0]	= fread_number( fp );
		ch->stance[1]	= fread_number( fp );
		ch->stance[2]	= fread_number( fp );
		ch->stance[3]	= fread_number( fp );
		ch->stance[4]	= fread_number( fp );
		ch->stance[5]	= fread_number( fp );
		ch->stance[6]	= fread_number( fp );
		ch->stance[7]	= fread_number( fp );
		ch->stance[8]	= fread_number( fp );
		ch->stance[9]	= fread_number( fp );
		ch->stance[10]	= fread_number( fp );
		ch->stance[11]	= fread_number( fp );
		ch->stance[12]	= fread_number( fp );
		ch->stance[13]	= fread_number( fp );
		ch->stance[14]	= fread_number( fp );
		ch->stance[15]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Stancename",	ch->pcdata->stancename,	fread_string( fp ) );
	    KEY( "Stancelist",	ch->pcdata->stancelist,	fread_string( fp ) );
	    KEY( "Stancework",	ch->pcdata->stancework,	fread_string( fp ) );

	    if ( !str_cmp( word, "Stancemove" ) )
	    {
		ch->pcdata->stancemove[0] = fread_number( fp );
		ch->pcdata->stancemove[1] = fread_number( fp );
		ch->pcdata->stancemove[2] = fread_number( fp );
		ch->pcdata->stancemove[3] = fread_number( fp );
		ch->pcdata->stancemove[4] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Status" ) )
	    {
		ch->pcdata->status[0] = fread_number( fp );
		ch->pcdata->status[1] = fread_number( fp );
		ch->pcdata->status[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( "Teach",   	ch->pcdata->teach,	fread_number( fp ) );
	    KEY( "TempClass",	ch->pcdata->class,	fread_number( fp ) );
	    KEY( "Temporis",    ch->pcdata->temporis,	fread_number( fp ) );

            if ( !str_cmp( word, "Tribegift" ) )
            {
                ch->pcdata->tgift[0] = fread_number( fp );
                ch->pcdata->tgift[1] = fread_number( fp );
                ch->pcdata->tgift[2] = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    if ( !str_cmp( word, "Tribes" ) )
	    {
		ch->pcdata->tribes[0] = fread_number( fp );
		ch->pcdata->tribes[1] = fread_number( fp );
		ch->pcdata->tribes[2] = fread_number( fp );
		ch->pcdata->tribes[3] = fread_number( fp );
		ch->pcdata->tribes[4] = fread_number( fp );
		ch->pcdata->tribes[5] = fread_number( fp );
		ch->pcdata->tribes[6] = fread_number( fp );
		ch->pcdata->tribes[7] = fread_number( fp );
		ch->pcdata->tribes[8] = fread_number( fp );
		ch->pcdata->tribes[9] = fread_number( fp );
		ch->pcdata->tribes[10] = fread_number( fp );
		ch->pcdata->tribes[11] = fread_number( fp );
		ch->pcdata->tribes[12] = fread_number( fp );
		ch->pcdata->tribes[13] = fread_number( fp );
		ch->pcdata->tribes[14] = fread_number( fp );
		ch->pcdata->tribes[15] = fread_number( fp );
		ch->pcdata->tribes[16] = fread_number( fp );
		ch->pcdata->tribes[17] = fread_number( fp );
		ch->pcdata->tribes[18] = fread_number( fp );
		ch->pcdata->tribes[19] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Truebeast",	ch->truebeast,		fread_number( fp ) );
	    KEY( "Trust",	ch->trust,		fread_number( fp ) );

	    if ( !str_cmp( word, "Title" ) )
	    {
		ch->pcdata->title = fread_string( fp );
		if ( isalpha(ch->pcdata->title[0])
		||   isdigit(ch->pcdata->title[0]) )
		{
		    sprintf( buf, " %s", ch->pcdata->title );
		    free_string( ch->pcdata->title );
		    ch->pcdata->title = str_dup( buf );
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'V':
	    KEY( "VLeader",	   ch->pcdata->vleader,	fread_string( fp ) );
	    KEY( "Vampaff",        ch->vampaff,		fread_number( fp ) );
	    KEY( "Vampgen",        ch->vampgen,		fread_number( fp ) );
	    KEY( "Vamppass",       ch->vamppass,	fread_number( fp ) );
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    if ( !str_cmp( word, "Weapons" ) )
	    {
		ch->wpn[0]	= fread_number( fp );
		ch->wpn[1]	= fread_number( fp );
		ch->wpn[2]	= fread_number( fp );
		ch->wpn[3]	= fread_number( fp );
		ch->wpn[4]	= fread_number( fp );
		ch->wpn[5]	= fread_number( fp );
		ch->wpn[6]	= fread_number( fp );
		ch->wpn[7]	= fread_number( fp );
		ch->wpn[8]	= fread_number( fp );
		ch->wpn[9]	= fread_number( fp );
		ch->wpn[10]	= fread_number( fp );
		ch->wpn[11]	= fread_number( fp );
		ch->wpn[12]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Willpower" ) )
	    {
		ch->pcdata->willpower[0] = fread_number( fp );
		ch->pcdata->willpower[1] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Wimpy",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wizn",        ch->wiznet,             fread_flag( fp ) );
	    KEY( "Wolf",        ch->pcdata->wolf,	fread_number( fp ) );
	    if ( !str_cmp( word, "Wolfform" ) )
	    {
		ch->pcdata->wolfform[0] = fread_number( fp );
		ch->pcdata->wolfform[1] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Wpoints",    	ch->pcdata->wpoints,	fread_number( fp ) );
	    break;
	}

	if ( !fMatch )
	{
	    bug( "Fread_char: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}

void fread_obj( CHAR_DATA *ch, FILE *fp )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;

    if ( obj_free == NULL )
    {
	obj		= alloc_perm( sizeof(*obj) );
    }
    else
    {
	obj		= obj_free;
	obj_free	= obj_free->next;
    }

    *obj		= obj_zero;
    obj->name		= str_dup( "" );
    obj->short_descr	= str_dup( "" );
    obj->description	= str_dup( "" );
    obj->chpoweron	= str_dup( "(null)" );
    obj->chpoweroff	= str_dup( "(null)" );
    obj->chpoweruse	= str_dup( "(null)" );
    obj->victpoweron	= str_dup( "(null)" );
    obj->victpoweroff	= str_dup( "(null)" );
    obj->victpoweruse	= str_dup( "(null)" );
    obj->actcommands	= str_dup( "" );
    obj->questmaker	= str_dup( "" );
    obj->questowner	= str_dup( "" );
    obj->spectype	= 0;
    obj->specpower	= 0;
    obj->condition	= 100;
    obj->toughness	= 5;
    obj->resistance	= 100;
    obj->wear_loc	= -1;
    obj->quest		= 0;
    obj->points		= 0;

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( "ActCommands",	obj->actcommands,	fread_string( fp ) );
	    if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
	    {
		AFFECT_DATA *paf;

		if ( affect_free == NULL )
		{
		    paf		= alloc_perm( sizeof(*paf) );
		}
		else
		{
		    paf		= affect_free;
		    affect_free	= affect_free->next;
		}

		paf->type	= -1;
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= 0;
		paf->next	= obj->affected;
		obj->affected	= paf;
		fMatch		= TRUE;
		break;
	    }
	    break;

	case 'C':
	    KEY( "Condition",	obj->condition,		fread_number( fp ) );
	    KEY( "Cost",	obj->cost,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Description",	obj->description,	fread_string( fp ) );
	    break;

	case 'E':
	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) )
	    {
		EXTRA_DESCR_DATA *ed;

		if ( extra_descr_free == NULL )
		{
		    ed			= alloc_perm( sizeof(*ed) );
		}
		else
		{
		    ed			= extra_descr_free;
		    extra_descr_free	= extra_descr_free->next;
		}

		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || !fVnum )
		{
		    bug( "Fread_obj: incomplete object.", 0 );
		    free_string( obj->name        );
		    free_string( obj->description );
		    free_string( obj->short_descr );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
		{
		    obj->next	= object_list;
		    object_list	= obj;
		    obj->pIndexData->count++;
		    if ( iNest == 0 || rgObjNest[iNest] == NULL )
			obj_to_char( obj, ch );
		    else
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    return;
		}
	    }
	    break;

	case 'I':
	    KEY( "ItemType",	obj->item_type,		fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "Level",	obj->level,		fread_number( fp ) );
	    break;

	case 'N':
	    KEY( "Name",	obj->name,		fread_string( fp ) );

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_obj: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    break;

	case 'P':
	    KEY( "Points",	obj->points,		fread_number( fp ) );
	    KEY( "Poweronch",	obj->chpoweron,		fread_string( fp ) );
	    KEY( "Poweroffch",	obj->chpoweroff,	fread_string( fp ) );
	    KEY( "Powerusech",	obj->chpoweruse,	fread_string( fp ) );
	    KEY( "Poweronvict",	obj->victpoweron,	fread_string( fp ) );
	    KEY( "Poweroffvict",obj->victpoweroff,	fread_string( fp ) );
	    KEY( "Powerusevict",obj->victpoweruse,	fread_string( fp ) );
	    break;

	case 'Q':
	    KEY( "Quest",	obj->quest,		fread_number( fp ) );
	    KEY( "Questmaker",	obj->questmaker,	fread_string( fp ) );
	    KEY( "Questowner",	obj->questowner,	fread_string( fp ) );
	    break;

	case 'R':
	    KEY( "Resistance",	obj->resistance,	fread_number( fp ) );
	    break;

	case 'S':
	    KEY( "ShortDescr",	obj->short_descr,	fread_string( fp ) );
	    KEY( "Spectype",	obj->spectype,		fread_number( fp ) );
	    KEY( "Specpower",	obj->specpower,		fread_number( fp ) );

	    if ( !str_cmp( word, "Spell" ) )
	    {
		int iValue;
		int sn;

		iValue = fread_number( fp );
		sn     = skill_lookup( fread_word( fp ) );
		if ( iValue < 0 || iValue > 3 )
		{
		    bug( "Fread_obj: bad iValue %d.", iValue );
		}
		else if ( sn < 0 )
		{
		    bug( "Fread_obj: unknown skill.", 0 );
		}
		else
		{
		    obj->value[iValue] = sn;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Toughness",	obj->toughness,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Values" ) )
	    {
		obj->value[0]	= fread_number( fp );
		obj->value[1]	= fread_number( fp );
		obj->value[2]	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		fMatch		= TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Vnum" ) )
	    {
		int vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		{
		    vnum = 30041;
		    if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
			bug( "Fread_obj: bad vnum %d.", vnum );
		    else
			fVnum = TRUE;
		}
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "WearFlags",	obj->wear_flags,	fread_number( fp ) );
	    KEY( "WearLoc",	obj->wear_loc,		fread_number( fp ) );
	    KEY( "Weight",	obj->weight,		fread_number( fp ) );
	    break;

	}

	if ( !fMatch )
	{
	    bug( "Fread_obj: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}

void fread_tiny_obj( CHAR_DATA *ch, FILE *fp )
{
    OBJ_DATA *obj;
    int iNest, vnum, level;
    bool fNest = FALSE;

    vnum		= fread_number( fp );
    level		= fread_number( fp );

    if ( ( obj = create_object( get_obj_index( vnum ), level ) ) == NULL )
    {
	bug( "Fread_tiny_obj: bad vnum %d.", vnum );
	return;
    }

    iNest		= fread_number( fp );

    if ( iNest < 0 || iNest >= MAX_NEST )
    {
	bug( "Fread_tiny_obj: bad nest %d.", iNest );
    }
    else
    {
	rgObjNest[iNest] = obj;
	fNest = TRUE;
    }

    obj->wear_loc 	= fread_number( fp );
    obj->cost	 	= fread_number( fp );
    obj->timer	 	= fread_number( fp );
    obj->condition 	= fread_number( fp );
    obj->resistance 	= fread_number( fp );
    obj->quest	 	= fread_number( fp );
    obj->value[0] 	= fread_number( fp );
    obj->value[1] 	= fread_number( fp );
    obj->value[2] 	= fread_number( fp );
    obj->value[3] 	= fread_number( fp );

    if ( iNest == 0 || rgObjNest[iNest] == NULL )
	obj_to_char( obj, ch );
    else
	obj_to_obj( obj, rgObjNest[iNest-1] );

    return;
}



void do_finger( CHAR_DATA *ch, char *argument )
{
    char strsave[MAX_INPUT_LENGTH*2];
    char buf[MAX_STRING_LENGTH*2];
    char letter;
    CHAR_DATA *victim;
    FILE *fp;
    char *word;
    bool fMatch;
    bool drop_out = FALSE;

    char *f_name	= capitalize( argument );
    char *f_title	= "";
    char *f_lasthost	= "";
    char *f_lasttime	= "";
    int   f_extra	= 0;
    int   f_sex		= 0;
    char *f_conception	= "";
    char *f_createtime	= 0;
    int   f_level	= 0;
    int   f_race	= 0;
    char *f_marriage	= "";
    int   f_pkill	= 0;
    int   f_pdeath	= 0;
    int   f_mkill	= 0;
    int   f_mdeath	= 0;
    char *f_email	= "";
    int   f_played	= 0;
    int   f_bounty	= 0;

    if ( IS_NPC(ch) ) return;

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_finger( ch, argument ) ) != NULL )
    {
	/* Just point directly to the string, no str_dup needed */
	f_title		= victim->pcdata->title;
	f_lasthost	= victim->lasthost;
	f_lasttime	= victim->lasttime;
	f_extra		= victim->extra;
	f_sex		= victim->sex;
	f_conception	= str_dup( victim->pcdata->conception );
	f_createtime	= victim->createtime;
	f_level		= victim->level;
	f_race		= victim->race;
	f_marriage	= victim->pcdata->marriage;
	f_pkill		= victim->pkill;
	f_pdeath	= victim->pdeath;
	f_mkill		= victim->mkill;
	f_mdeath	= victim->mdeath;
	f_email		= victim->pcdata->email;
	f_played	= victim->played;
	f_bounty	= victim->pcdata->bounty;
    }
    else
    {
	if ( !char_exists(FILE_PLAYER,argument) )
	{
	    send_to_char( "That player doesn't exist.\n\r", ch );
	    return;
	}

	fflush( fpReserve );
	fclose( fpReserve );
	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize(argument) );
	if ( ( fp = fopen( strsave, "r" ) ) != NULL )
	{
	    if ( ( letter = fread_letter( fp ) ) == EOF)
	    {
		send_to_char("Their playerfile has been corrupted.\n\r",ch);
		fflush( fp );
		fclose( fp );
		fpReserve = fopen( NULL_FILE, "r" );
		return;
	    }
	    fread_to_eol( fp );

	    while ( !drop_out )
	    {
		word   = feof( fp ) ? "End" : fread_word( fp );
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
		default:
		case '*':
		    fMatch = TRUE;
		    fread_to_eol( fp );
		    break;
		case 'B':
		    KEY( "Bounty",	f_bounty,		fread_number( fp ) );
		    break;
		case 'C':
		    KEY( "CTM",		f_createtime,		fread_string( fp ) );
		    KEY( "Createtime",	f_createtime,		fread_string( fp ) );
		    KEY( "CPT",		f_conception,		fread_string( fp ) );
		    KEY( "Conception",	f_conception,		fread_string( fp ) );
		    break;

		case 'E':
		    KEY( "EML",		f_email,		fread_string( fp ) );
		    KEY( "Email",	f_email,		fread_string( fp ) );
		    if ( !str_cmp( word, "End" ) ) drop_out = TRUE;
		    KEY( "EXT",		f_extra,		fread_number( fp ) );
		    KEY( "Extra",	f_extra,		fread_number( fp ) );
		    break;

		case 'K':
		    if ( !str_cmp( word, "KAD" ) )
		    {
			f_pkill		= fread_number( fp );
			f_pdeath	= fread_number( fp );
			f_mkill		= fread_number( fp );
			f_mdeath	= fread_number( fp );
			fMatch = TRUE;
			break;
		    }
		    break;

		case 'M':
		    KEY( "MRG",		f_marriage,		fread_string( fp ) );
		    KEY( "Marriage",	f_marriage,		fread_string( fp ) );
		    break;

		case 'L':
		    KEY( "LHS",		f_lasthost,		fread_string( fp ) );
		    KEY( "Lasthost",	f_lasthost,		fread_string( fp ) );
		    KEY( "LTM",		f_lasttime,		fread_string( fp ) );
		    KEY( "Lasttime",	f_lasttime,		fread_string( fp ) );
		    KEY( "LVL",		f_level,		fread_number( fp ) );
		    KEY( "Level",	f_level,		fread_number( fp ) );
		    break;

		case 'P':
		    KEY( "PLY",		f_played,		fread_number( fp ) );
		    KEY( "Played",	f_played,		fread_number( fp ) );
		    if ( !str_cmp( word, "PkPdMkMd" ) )
		    {
			f_pkill		= fread_number( fp );
			f_pdeath	= fread_number( fp );
			f_mkill		= fread_number( fp );
			f_mdeath	= fread_number( fp );
			fMatch = TRUE;
			break;
		    }
		    break;

		case 'R':
		    KEY( "RAC",		f_race,			fread_number( fp ) );
		    KEY( "Race",	f_race,			fread_number( fp ) );
		    break;

		case 'S':
		    KEY( "SEX",		f_sex,			fread_number( fp ) );
		    KEY( "Sex",		f_sex,			fread_number( fp ) );
		    break;

		case 'T':
		    if ( !str_cmp( word, "TTL" ) || !str_cmp( word, "Title" ) )
		    {
			f_title = fread_string( fp );
			if ( isalpha(ch->pcdata->title[0])
			||   isdigit(ch->pcdata->title[0]) )
			{
			    sprintf( buf, " %s", f_title );
			    free_string( f_title );
			    f_title = str_dup( buf );
			}
			fMatch = TRUE;
			break;
		    }
		    break;
		}
		if ( !fMatch ) fread_to_eol( fp );
	    }
	}
	else
	{
	    bug( "Do_finger: fopen", 0 );
	    perror( strsave );
	}
	fflush( fp );
	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
    }
    send_to_char("#l--------------------------------------------------------------------------------#n\n\r",ch);
    sprintf( buf, "%s%s%s.\n\r", f_name, drop_out ? " ": "", f_title );
    send_to_char( buf, ch );
    send_to_char("#l--------------------------------------------------------------------------------#n\n\r",ch);
    if ( IS_QUESTMAKER(ch) ) /* Only Immortals can see hostnames - Kai */
    {
    	sprintf( buf, "Last connected from %s at %s.\n\r", f_lasthost, f_lasttime );
	send_to_char( buf, ch );
    }
    else
    {
    switch ( f_level )
    {
        default:
        sprintf( buf, "Last connected at %s.\n\r", f_lasttime );
        send_to_char( buf, ch );
	break;
        case LEVEL_BUILDER:
            send_to_char("You may not see the last time a god connected.\n\r",ch);
            break;
        case LEVEL_QUESTMAKER:
            send_to_char("You may not see the last time a god connected.\n\r",ch);
            break;
        case LEVEL_ENFORCER:
            send_to_char("You may not see the last time a god connected.\n\r",ch);
            break;
        case LEVEL_JUDGE:
            send_to_char("You may not see the last time a god connected.\n\r",ch);
            break;
        case LEVEL_HIGHJUDGE:
            send_to_char("You may not see the last time a god connected.\n\r",ch);
            break;
        case LEVEL_IMPLEMENTOR:
            send_to_char("You may not see the last time a god connected.\n\r",ch);
	    break;
    }
    }

/*
    else
    {
	sprintf( buf, "Last connected at %s.\n\r", f_lasttime );
	send_to_char( buf, ch );
    }
*/
    send_to_char("#l--------------------------------------------------------------------------------#n\n\r",ch);
    sprintf( buf, "Sex: %s. ", 
	f_sex == SEX_MALE ? "Male" : f_sex == SEX_FEMALE ? "Female" : "None");
    send_to_char( buf, ch );
    if ( IS_SET( f_extra, EXTRA_PREGNANT ) ) 
	other_age( ch, f_extra, TRUE, f_conception );
    other_age( ch, f_extra, FALSE, f_createtime );

    switch ( f_level )
    {
	default:
	    send_to_char("They are mortal, ",ch);
	    break;
	case LEVEL_AVATAR:
	case LEVEL_APPRENTICE:
	case LEVEL_MAGE:
	case LEVEL_ARCHMAGE:
	{
	    switch ( f_race )
	    {
		default:	send_to_char("They are a Supreme God, ",ch);
				break;
		case 0:		send_to_char("They are an Avatar, ",ch);
				break;
		case 1: case 2: case 3: case 4:
				send_to_char("They are an Immortal, ",ch);
				break;
		case 5: case 6: case 7: case 8: case 9:
				send_to_char("They are a Godling, ",ch);
				break;
		case 10: case 11: case 12: case 13: case 14:
				send_to_char("They are a Demigod, ",ch);
				break;
		case 15: case 16: case 17: case 18: case 19:
				send_to_char("They are a Lesser God, ",ch);
				break;
		case 20: case 21: case 22: case 23: case 24:
				send_to_char("They are a Greater God, ",ch);
				break;
	    }
	    break;
	}
	case LEVEL_BUILDER:
	    send_to_char("They are a Builder, ",ch);
	    break;
	case LEVEL_QUESTMAKER:
	    send_to_char("They are a Quest Maker, ",ch);
	    break;
	case LEVEL_ENFORCER:
	    send_to_char("They are an Enforcer, ",ch);
	    break;
	case LEVEL_JUDGE:
	    send_to_char("They are a Judge, ",ch);
	    break;
	case LEVEL_HIGHJUDGE:
	    send_to_char("They are a High Judge, ",ch);
	    break;
	case LEVEL_IMPLEMENTOR:
	    send_to_char("They are an Implementor, ",ch);
	    break;
    }

    sprintf(buf, "(%d status), ", f_race);
    send_to_char(buf, ch); 
    if (f_played > 0) 
	f_played = (2*(f_played / 7200));
    else
	f_played = 0; /* Avoid negative playing hours */
    sprintf(buf,"and have been playing for %d hours.\n\r", f_played);
    send_to_char(buf,ch);
    if (strlen(f_marriage) > 2)
    {
	if (IS_SET(f_extra, EXTRA_MARRIED))
	    sprintf(buf,"They are married to %s.\n\r",f_marriage);
	else
	    sprintf(buf,"They are engaged to %s.\n\r",f_marriage);
	send_to_char(buf,ch);
    }
    sprintf(buf,"Player kills: %d, Player Deaths: %d, Bounty: %d.\n\r",
	f_pkill, f_pdeath, f_bounty);
    send_to_char(buf,ch);
    sprintf(buf,"Mob kills: %d, Mob Deaths: %d.\n\r",
	f_mkill, f_mdeath);
    send_to_char(buf,ch);
    send_to_char("#l--------------------------------------------------------------------------------#n\n\r",ch);
    if (strlen(f_email) > 2 && 
	(!IS_SET(f_extra, EXTRA_INVIS_EMAIL) || IS_IMMORTAL(ch)) )
    {
	sprintf(buf,"Email: %s\n\r", f_email);
	send_to_char(buf,ch);
	send_to_char("#l--------------------------------------------------------------------------------#n\n\r",ch);
    }
    return;
}
