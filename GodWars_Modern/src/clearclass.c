#include <stdio.h>
#include <time.h>
#include "merc.h"

void do_clearclass( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int sn;
    one_argument( argument, arg );

/*  Look this over really well. it resets EVERYTHING I could think
    of.  I still say we shouldn't allow them to keep their quest
    points.   because they will go mage to quest, then clearclass 
    to the class of choice.

    I have this setting them to 1000 exp also.  Don't need vamps
    running around  saving up millions of exp,  just to reclass.

    On a side note.  if an angel has demonic armor and is allowed 
    to keep it, then SAC's it after the clearclass,  the dps are
    still written to the pfile.  So if they go demon,  they start
    out with their dps.  Let the imms know that angels and demons
    need to sac their Relics BEFORE clearing the class.

    Also added a quick check.   if the person is in objectform,
    it won't let you clearclass them (trapping them in that form)

    Remove this check when and if you ok the code.
    I simply commented out do_clearclass in act_wiz.c if you need it.
*/

/*
if (!is_name(ch->name,"Xentoth"))
{
send_to_char("\n\r#RXentoth#n has not OK'd this command yet. -#oAristoi#n\n\r", ch);
return;
}
*/


    if ( arg[0] == '\0' || is_number(arg) )
    {
        send_to_char( "Clear whose class?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
	do_remove(victim, "all");
	do_mclear(victim, "self");
//	do_cast(victim, "'dispel magic' self");

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    victim->pcdata->learned[sn] = 0;    

    victim->max_hit                         = 1000;
    victim->hit                             = 1000;
    victim->max_mana                        = 1000;
    victim->mana                            = 1000;
    victim->max_move                        = 1000;
    victim->move                            = 1000;
    victim->exp				    = 1000;
    victim->act                             = 17984;
if (IS_EXTRA(victim, EXTRA_MARRIED))
{
    victim->extra                           = 131072;
    } else {
    victim->extra                           = 0;
}
    victim->more                            = 0;
    victim->race			    = 0;
    victim->pcdata->familiar                = NULL;
    victim->pcdata->partner                 = NULL;
    victim->pcdata->propose                 = NULL;
    victim->pcdata->chobj                   = NULL;
    victim->pcdata->memorised               = NULL;
    victim->pcdata->conding                 = str_dup( "" );
    victim->pcdata->love                    = str_dup( "" );
    victim->pcdata->coterie                 = str_dup( "" );
    victim->pcdata->covenant                = str_dup( "" );
    victim->pcdata->cot_time                = str_dup( "None" );
    victim->pcdata->cov_time                = str_dup( "None" );
    victim->pcdata->cot_clans               = str_dup( "All" );
    victim->pcdata->cot_sides               = str_dup( "All" );
    victim->pcdata->leader                  = str_dup( "" );
    victim->pcdata->vleader                 = str_dup( "" );
    victim->pcdata->stancename              = str_dup( "" );
    victim->pcdata->stancelist              = str_dup( "" );
    victim->pcdata->stancework              = str_dup( "" );
    victim->lord                            = str_dup( "" );
    victim->clan                            = str_dup( "" );
    victim->side                            = str_dup( "" );
    victim->morph                           = str_dup( "" );
    victim->pload                           = str_dup( "" );
    victim->poweraction                     = str_dup( "" );
    victim->powertype                       = str_dup( "" );
    victim->hunting                         = str_dup( "" );
    victim->pcdata->denydate                = 0;
    victim->pcdata->stancemove[0]           = 0;
    victim->pcdata->stancemove[1]           = 0;
    victim->pcdata->stancemove[2]           = 0;
    victim->pcdata->stancemove[3]           = 0;
    victim->pcdata->stancemove[4]           = 0;
    victim->spectype                        = 0;
    victim->specpower                       = 0;
    victim->mounted                         = 0;
    victim->home                            = 3001;
    victim->agg                             = 0;
    victim->vampgen                         = 0;
    victim->paradox[0]                      = 0;    /* Total paradox */
    victim->paradox[1]                      = 0;    /* Current Paradox */
    victim->paradox[2]                      = 0;    /* Paradox Ticker */
    victim->damcap[0]                       = 1000;
    victim->damcap[1]                       = 0;
    victim->vampaff                         = 0;
    victim->itemaffect                      = 0;
    victim->vamppass                        = -1;
    victim->polyaff                         = 0;
    victim->immune                          = 0;
    victim->form                            = 0;
    victim->init                            = 0;
    victim->body                            = 32767;
    victim->beast                           = 20;
    victim->truebeast                       = 20;
    victim->loc_hp[0]                       = 0;
    victim->loc_hp[1]                       = 0;
    victim->loc_hp[2]                       = 0;
    victim->loc_hp[3]                       = 0;
    victim->loc_hp[4]                       = 0;
    victim->loc_hp[5]                       = 0;
    victim->loc_hp[6]                       = 0;
    victim->wpn[0]                          = 0;
    victim->wpn[1]                          = 0;
    victim->wpn[2]                          = 0;
    victim->wpn[3]                          = 0;
    victim->wpn[4]                          = 0;
    victim->wpn[5]                          = 0;
    victim->wpn[6]                          = 0;
    victim->wpn[7]                          = 0;
    victim->wpn[8]                          = 0;
    victim->wpn[9]                          = 0;
    victim->wpn[10]                         = 0;
    victim->wpn[11]                         = 0;
    victim->wpn[12]                         = 0;
    victim->spl[0]                          = 4;
    victim->spl[1]                          = 4;
    victim->spl[2]                          = 4;
    victim->spl[3]                          = 4;
    victim->spl[4]                          = 4;
    victim->spl[5]                          = 4;
    victim->cmbt[0]                         = 0;
    victim->cmbt[1]                         = 0;
    victim->cmbt[2]                         = 0;
    victim->cmbt[3]                         = 0;
    victim->cmbt[4]                         = 0;
    victim->cmbt[5]                         = 0;
    victim->cmbt[6]                         = 0;
    victim->cmbt[7]                         = 0;
    victim->stance[0]                       = 0;
    victim->stance[1]                       = 0;
    victim->stance[2]                       = 0;
    victim->stance[3]                       = 0;
    victim->stance[4]                       = 0;
    victim->stance[5]                       = 0;
    victim->stance[6]                       = 0;
    victim->stance[7]                       = 0;
    victim->stance[8]                       = 0;
    victim->stance[9]                       = 0;
    victim->stance[10]                      = 0;
    victim->stance[11]                      = 0;
    victim->stance[12]                      = 0;
    victim->stance[13]                      = 0;
    victim->stance[14]                      = 0;
    victim->stance[15]                      = 0;
    victim->embraced                        = 0;
    victim->blood[0]                        = 100;
    victim->blood[1]                        = 100;
    victim->blood[2]                        = 1;
    victim->pcdata->perm_str                = 13;
    victim->pcdata->perm_int                = 13;
    victim->pcdata->perm_wis                = 13;
    victim->pcdata->perm_dex                = 13;
    victim->pcdata->perm_con                = 13;
    victim->pcdata->quest                   = 0;
    victim->pcdata->bounty                  = 0;
    victim->pcdata->wolf                    = 0;
    victim->pcdata->rank                    = 0;
    victim->pcdata->regenerate              = 0;
    victim->pcdata->demonic                 = 0;
    victim->pcdata->atm                     = 0;
    victim->pcdata->song                    = 0;
    victim->pcdata->necromancy              = 0;
    victim->pcdata->obeah                   = 0;
    victim->pcdata->darkness                = 0;
    victim->pcdata->temporis                = 0;
    victim->pcdata->silence                 = 0;
    victim->pcdata->bodymods                = 0;
    victim->pcdata->reina                   = 0;
    victim->pcdata->bpoints                 = 0;
    victim->pcdata->dpoints                 = 0;
    victim->pcdata->gpoints                 = 0;
    victim->pcdata->rpoints                 = 0;
    victim->pcdata->wpoints                 = 0;
    victim->pcdata->spirit                  = 0;
    victim->pcdata->conjure                 = 0;
    victim->pcdata->celerity                = 0;
    victim->pcdata->firstaid                = 0;
    victim->pcdata->repair                  = 0;
    victim->pcdata->teach                   = 0;
    victim->pcdata->learn                   = 0;
    victim->pcdata->notes                   = 0;
    victim->pcdata->evileye                 = 0;
    victim->pcdata->followers               = 0;
    victim->pcdata->gnosis[0]               = 50;
    victim->pcdata->gnosis[1]               = 50;
    victim->pcdata->absorb[0]               = 0;
    victim->pcdata->absorb[1]               = 0;
    victim->pcdata->absorb[2]               = 0;
    victim->pcdata->absorb[3]               = 0;
    victim->pcdata->absorb[4]               = 0;
    victim->pcdata->absorb[5]               = 0;
    victim->pcdata->qstats[0]               = 0;
    victim->pcdata->qstats[1]               = 0;
    victim->pcdata->qstats[2]               = 0;
    victim->pcdata->qstats[3]               = 0;
    victim->pcdata->qstats[4]               = 0;
    victim->pcdata->exercise[0]             = 0;
    victim->pcdata->exercise[1]             = 0;
    victim->pcdata->exercise[2]             = 0;
    victim->pcdata->exercise[3]             = 0;
    victim->pcdata->language[0]             = 0;
    victim->pcdata->language[1]             = 0;
    victim->pcdata->stage[0]                = 0;
    victim->pcdata->stage[1]                = 0;
    victim->pcdata->stage[2]                = 0;
    victim->pcdata->status[0]               = 0;
    victim->pcdata->status[1]               = 0;
    victim->pcdata->status[2]               = 0;
    victim->pcdata->wolfform[0]             = 1;
    victim->pcdata->wolfform[1]             = 1;
    victim->pcdata->score[0]                = 0;
    victim->pcdata->score[1]                = 0;
    victim->pcdata->score[2]                = 0;
    victim->pcdata->score[3]                = 0;
    victim->pcdata->score[4]                = 0;
    victim->pcdata->score[5]                = 0;
    victim->pcdata->runes[0]                = 0;
    victim->pcdata->runes[1]                = 0;
    victim->pcdata->runes[2]                = 0;
    victim->pcdata->runes[3]                = 0;
    victim->pcdata->disc[0]                 = 0;
    victim->pcdata->disc[1]                 = 0;
    victim->pcdata->disc[2]                 = 0;
    victim->pcdata->disc[3]                 = 0;
    victim->pcdata->disc[4]                 = 0;
    victim->pcdata->disc[5]                 = 0;
    victim->pcdata->disc[6]                 = 0;
    victim->pcdata->disc[7]                 = 0;
    victim->pcdata->disc[8]                 = 0;
    victim->pcdata->disc[9]                 = 0;
    victim->pcdata->disc[10]                = 0;
    victim->pcdata->rituals[0]              = 0;
    victim->pcdata->rituals[1]              = 0;
    victim->pcdata->rituals[2]              = 0;
    victim->pcdata->rituals[3]              = 0;
    victim->pcdata->rituals[4]              = 0;
    victim->pcdata->rituals[5]              = 0;
    victim->pcdata->rituals[6]              = 0;
    victim->pcdata->rituals[7]              = 0;
    victim->pcdata->rituals[8]              = 0;
    victim->pcdata->rituals[9]              = 0;
    victim->pcdata->cland[0]                = 0;
    victim->pcdata->cland[1]                = 0;
    victim->pcdata->cland[2]                = 0;
    victim->pcdata->tgift[0]		    = 0;
    victim->pcdata->tgift[1]		    = 0;
    victim->pcdata->tgift[2]		    = 0;
    victim->pcdata->diableries[0]           = 0;
    victim->pcdata->diableries[1]           = 0;
    victim->pcdata->breed[0]                = 0;
    victim->pcdata->breed[1]                = 0;
    victim->pcdata->breed[2]                = 0;
    victim->pcdata->auspice[0]              = 0;
    victim->pcdata->auspice[1]              = 0;
    victim->pcdata->auspice[2]              = 0;
    victim->pcdata->auspice[3]              = 0;
    victim->pcdata->auspice[4]              = 0;
    victim->pcdata->tribes[0]               = 0;
    victim->pcdata->tribes[1]               = 0;
    victim->pcdata->tribes[2]               = 0;
    victim->pcdata->tribes[3]               = 0;
    victim->pcdata->tribes[4]               = 0;
    victim->pcdata->tribes[5]               = 0;
    victim->pcdata->tribes[6]               = 0;
    victim->pcdata->tribes[7]               = 0;
    victim->pcdata->tribes[8]               = 0;
    victim->pcdata->tribes[9]               = 0;
    victim->pcdata->tribes[10]              = 0;
    victim->pcdata->tribes[11]              = 0;
    victim->pcdata->tribes[12]              = 0;
    victim->pcdata->tribes[13]              = 0;
    victim->pcdata->tribes[14]              = 0;
    victim->pcdata->tribes[15]              = 0;
    victim->pcdata->tribes[16]              = 0;
    victim->pcdata->tribes[17]              = 0;
    victim->pcdata->tribes[18]              = 0;
    victim->pcdata->tribes[19]              = 0;
    victim->pcdata->powers[0]               = 0;
    victim->pcdata->powers[1]               = 0;
    victim->pcdata->powers[2]               = 0;
    victim->pcdata->powers[3]               = 0;
    victim->pcdata->powers[4]               = 0;
    victim->pcdata->powers[5]               = 0;
    victim->pcdata->powers[6]               = 0;
    victim->pcdata->powers[7]               = 0;
    victim->pcdata->powers[8]               = 0;
    victim->pcdata->powers[9]               = 0;
    victim->pcdata->powers[10]              = 0;
    victim->pcdata->powers[11]              = 0;
    victim->pcdata->powers[12]              = 0;
    victim->pcdata->powers[13]              = 0;
    victim->pcdata->powers[14]              = 0;
    victim->pcdata->powers[15]              = 0;
    victim->pcdata->powers[16]              = 0;
    victim->pcdata->powers[17]              = 0;
    victim->pcdata->powers[18]              = 0;
    victim->pcdata->powers[19]              = 0;
    victim->pcdata->powers_mod[0]           = 0;
    victim->pcdata->powers_mod[1]           = 0;
    victim->pcdata->powers_mod[2]           = 0;
    victim->pcdata->powers_mod[3]           = 0;
    victim->pcdata->powers_mod[4]           = 0;
    victim->pcdata->powers_mod[5]           = 0;
    victim->pcdata->powers_mod[6]           = 0;
    victim->pcdata->powers_mod[7]           = 0;
    victim->pcdata->powers_mod[8]           = 0;
    victim->pcdata->powers_mod[9]           = 0;
    victim->pcdata->powers_mod[10]          = 0;
    victim->pcdata->powers_mod[11]          = 0;
    victim->pcdata->powers_mod[12]          = 0;
    victim->pcdata->powers_mod[13]          = 0;
    victim->pcdata->powers_mod[14]          = 0;
    victim->pcdata->powers_mod[15]          = 0;
    victim->pcdata->powers_mod[16]          = 0;
    victim->pcdata->powers_mod[17]          = 0;
    victim->pcdata->powers_mod[18]          = 0;
    victim->pcdata->powers_mod[19]          = 0;
    victim->pcdata->powers_set[0]           = 0;
    victim->pcdata->powers_set[1]           = 0;
    victim->pcdata->powers_set[2]           = 0;
    victim->pcdata->powers_set[3]           = 0;
    victim->pcdata->powers_set[4]           = 0;
    victim->pcdata->powers_set[5]           = 0;
    victim->pcdata->powers_set[6]           = 0;
    victim->pcdata->powers_set[7]           = 0;
    victim->pcdata->powers_set[8]           = 0;
    victim->pcdata->powers_set[9]           = 0;
    victim->pcdata->powers_set[10]          = 0;
    victim->pcdata->powers_set[11]          = 0;
    victim->pcdata->powers_set[12]          = 0;
    victim->pcdata->powers_set[13]          = 0;
    victim->pcdata->powers_set[14]          = 0;
    victim->pcdata->powers_set[15]          = 0;
    victim->pcdata->powers_set[16]          = 0;
    victim->pcdata->powers_set[17]          = 0;
    victim->pcdata->powers_set[18]          = 0;
    victim->pcdata->powers_set[19]          = 0;
    victim->pcdata->powers_set_wear         = 0;
    victim->pcdata->genes[0]                = 0;
    victim->pcdata->genes[1]                = 0;
    victim->pcdata->genes[2]                = 0;
    victim->pcdata->genes[3]                = 0;
    victim->pcdata->genes[4]                = 0;
    victim->pcdata->genes[5]                = 0;
    victim->pcdata->genes[6]                = 0;
    victim->pcdata->genes[7]                = 0;
    victim->pcdata->genes[8]                = 0;
    victim->pcdata->genes[9]                = 0;
    victim->pcdata->cot_min[0]              = 0;
    victim->pcdata->cot_min[1]              = 0;
    victim->pcdata->cot_min[2]              = 0;
    victim->pcdata->cot_min[3]              = 0;
    victim->pcdata->cot_min[4]              = 0;
    victim->pcdata->cot_min[5]              = 0;
    victim->pcdata->cot_min[6]              = 0;
    victim->pcdata->cot_min[7]              = 0;
    victim->pcdata->cot_min[8]              = 0;
    victim->pcdata->cot_min[9]              = 0;
    victim->pcdata->cot_min[10]             = 0;
    victim->pcdata->cot_min[11]             = 0;
    victim->pcdata->willpower[0]            = 50;
    victim->pcdata->willpower[1]            = 50;
    victim->pcdata->resist[0]               = 0;
    victim->pcdata->resist[1]               = 0;
    victim->pcdata->resist[2]               = 0;
    victim->pcdata->resist[3]               = 0;
    victim->pcdata->resist[4]               = 0;
    victim->pcdata->power[0]                = 0;
    victim->pcdata->power[1]                = 0;
    victim->pcdata->fake_skill              = 0;
    victim->pcdata->fake_stance             = 0;
    victim->pcdata->fake_hit                = 0;
    victim->pcdata->fake_dam                = 0;
    victim->pcdata->fake_ac                 = 0;
    victim->pcdata->fake_hp                 = 0;
    victim->pcdata->fake_mana               = 0;
    victim->pcdata->fake_move               = 0;
    victim->pcdata->fake_max_hp             = 0;
    victim->pcdata->fake_max_mana           = 0;
    victim->pcdata->fake_max_move           = 0;
    victim->pcdata->fake_stats[STAT_STR]    = 0;
    victim->pcdata->fake_stats[STAT_INT]    = 0;
    victim->pcdata->fake_stats[STAT_WIS]    = 0;
    victim->pcdata->fake_stats[STAT_DEX]    = 0;
    victim->pcdata->fake_stats[STAT_CON]    = 0;
    victim->pcdata->obj_vnum                = 0;
    victim->pcdata->majesty                 = 0;
    victim->pcdata->condition[COND_THIRST]  = 48;
    victim->pcdata->condition[COND_FULL]    = 48;
    victim->pcdata->exhaustion              = 0;
    victim->pcdata->lie                     = 0;
    victim->class                           = 0;
    victim->affected_by			    = 0;
    victim->affected_by2		    = 0;
    
    if (!IS_IMMORTAL(victim))
    {
        victim->level = LEVEL_MORTAL;
        victim->trust = 0;
    }

    SET_BIT(victim->extra, EXTRA_TRUSTED);
    SET_BIT(victim->extra, EXTRA_NON_NEWBIE);
    send_to_char( "Ok.\n\r", ch );
    send_to_char( "Your class has been cleared.\n\r", victim );
    do_save(victim, "");
    return;
}


