/*************************************************
 *  This file is property of Crimson Nightmares  *
 * Please keep header in this file as a courtesy *
 * Use of this file constitutes nothing at all.  *
 * Please add this credit to the MOTD or to some *
 * place on the MUD, if not, ok.                 *
 * Bonus System by Azarius - Crimson Nightmares  *
 * And please email                              * 
 * speedybuggie@hotmail.com so I can come look.  *
 *************************************************/

#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(_WIN32)
#include <unistd.h> /* unlink() */
#else
#include <io.h>
#define unlink _unlink
#endif
#include "merc.h"

/* Global bonus state variables */
bool global_exp    = FALSE;
bool global_dam    = FALSE;
bool global_qp     = FALSE;
bool global_sspeed = FALSE;
bool global_spellgain   = FALSE;
bool global_stancegain  = FALSE;
bool global_weapongain  = FALSE;
bool global_halfexp    = FALSE;
bool global_halfdam    = FALSE;
bool global_halfqp     = FALSE;
bool global_halfsspeed = FALSE;
bool global_halfspellgain  = FALSE;
bool global_halfstancegain = FALSE;
bool global_halfweapongain = FALSE;

int pulse_doubleexp     = 0;
int pulse_doubledam     = 0;
int pulse_doubleqp      = 0;
int pulse_doublesspeed  = 0;
int pulse_doublespellgain  = 0;
int pulse_doublestancegain = 0;
int pulse_doubleweapongain = 0;
int pulse_halfexp     = 0;
int pulse_halfdam     = 0;
int pulse_halfqp      = 0;
int pulse_halfsspeed  = 0;
int pulse_halfspellgain  = 0;
int pulse_halfstancegain = 0;
int pulse_halfweapongain = 0;

void do_bonus( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    int time_on = 0;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );
    
    if ( arg[0] == '\0')
    {
	send_to_char( "Syntax: bonus value time\n\r", ch);
	send_to_char( "Values:\n\r", ch );
	send_to_char( " exp | dam | qp | sspeed | spellgain | stancegain | weapongain\n\r", ch);
	send_to_char( " hexp | hdam | hqp | hsspeed | hspellgain | hstancegain | hweapongain\n\r", ch);
	return;
    }

    if ( !str_cmp( arg, "exp" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should double exp be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_exp = TRUE;
    	pulse_doubleexp = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #PDOUBLE EXP#n", ch->name );
        do_info(ch,buf);
        sprintf( buf,"For more info on this please see HELP TOYS" );
        do_info(ch,buf);
       	sprintf( buf, "You have turned on Double Experience.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "dam" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should double damcap be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_dam = TRUE;
    	pulse_doubledam = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #PDOUBLE DAMCAP#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Double Damcap.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "qp" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should double qp be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_qp = TRUE;
    	pulse_doubleqp = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #PDOUBLE QP#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Double QP.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "sspeed" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should double spell casting speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_sspeed = TRUE;
    	pulse_doublesspeed = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #PDOUBLE SPELL CASTING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf,"For more info on this please see HELP TOYS" );
        do_info(ch,buf);
       	sprintf( buf, "You have turned on Double Spell Casting Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "spellgain" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should double spell training speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_spellgain = TRUE;
    	pulse_doublespellgain = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #PDOUBLE SPELL TRAINING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Double Spell Training Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }                
    else if ( !str_cmp( arg, "stancegain" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should double stance training speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_stancegain = TRUE;
    	pulse_doublestancegain = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #PDOUBLE STANCE TRAINING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Double Stance Training Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "weapongain" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should double weapon training speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_weapongain = TRUE;
    	pulse_doubleweapongain = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #PDOUBLE WEAPON TRAINING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Double Weapon Training Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    if ( !str_cmp( arg, "hexp" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should half exp be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_halfexp = TRUE;
    	pulse_halfexp = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #RHALF EXP#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Half Experience.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "hdam" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should half damcap be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_halfdam = TRUE;
    	pulse_halfdam = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #RHALF DAMCAP#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Half Damcap.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "hqp" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should half qp be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_halfqp = TRUE;
    	pulse_halfqp = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #RHALF QP#n", ch->name );
        do_info(ch,buf);
               	sprintf( buf, "You have turned on Half QP.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "hsspeed" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should half spell casting speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_halfsspeed = TRUE;
    	pulse_halfsspeed = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #RHALF SPELL CASTING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Half Spell Casting Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "hspellgain" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should half spell training speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_halfspellgain = TRUE;
    	pulse_halfspellgain = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #RHALF SPELL TRAINING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on Half Spell Training Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }                
    else if ( !str_cmp( arg, "hstancegain" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should half stance training speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_halfstancegain = TRUE;
    	pulse_halfstancegain = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #Phalf STANCE TRAINING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on half Stance Training Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }
    else if ( !str_cmp( arg, "hweapongain" ) )
    {
        if (!is_number(arg2))
    	{
      	  send_to_char("A number please, how long should half weapon training speed be on??\n\r",ch);
      	  return;
    	}
    	if ((time_on = atoi(arg2)) < 1 || time_on > 60)
    	{
      	  send_to_char("From 1 to 60 minutes please.\n\r",ch);
      	  return;
    	}
    	global_halfweapongain = TRUE;
    	pulse_halfweapongain = atoi(arg2);
        sprintf( buf,"#7%s#n enabled #RHALF WEAPON TRAINING SPEED#n", ch->name );
        do_info(ch,buf);
        sprintf( buf, "You have turned on half Weapon Training Speed.\n\r" );
        send_to_char( buf, ch );
        return;
    }    
}

void update_doubleexp()
{
  if (pulse_doubleexp > 0)
  {
    if (--pulse_doubleexp == 0)
    {
      global_exp = FALSE;
      pulse_doubleexp = -1;
    }
  }
}

void update_doubleqps()
{
  if (pulse_doubleqp > 0)
  {
    if ( --pulse_doubleqp == 0)
    {
      global_qp = FALSE;
      pulse_doubleqp = -1;
    }
  }
}

void update_doubledam()
{
  if (pulse_doubledam > 0)
  {
    if (--pulse_doubledam == 0)
    {
      global_dam = FALSE;
      pulse_doubledam = -1;
    }
  }
}

void update_doublesspeed()
{
  if (pulse_doublesspeed > 0)
  {
    if ( --pulse_doublesspeed == 0)
    {
      global_sspeed = FALSE;
      pulse_doublesspeed = -1;
    }
  }
}

void update_doublespellgain()
{
  if (pulse_doublespellgain > 0)
  {
    if (--pulse_doublespellgain == 0)
    {
      global_spellgain = FALSE;
      pulse_doublespellgain = -1;
    }
  }
}

void update_doublestancegain()
{
  if (pulse_doublestancegain > 0)
  {
    if ( --pulse_doublestancegain == 0)
    {
      global_stancegain = FALSE;
      pulse_doublestancegain = -1;
    }
  }
}

void update_doubleweapongain()
{
  if (pulse_doubleweapongain > 0)
  {
    if (--pulse_doubleweapongain == 0)
    {
      global_weapongain = FALSE;
      pulse_doubleweapongain = -1;
    }
  }
}

void update_halfexp()
{
  if (pulse_halfexp > 0)
  {
    if (--pulse_halfexp == 0)
    {
      global_halfexp = FALSE;
      pulse_halfexp = -1;
    }
  }
}

void update_halfqps()
{
  if (pulse_halfqp > 0)
  {
    if ( --pulse_halfqp == 0)
    {
      global_halfqp = FALSE;
      pulse_halfqp = -1;
    }
  }
}

void update_halfdam()
{
  if (pulse_halfdam > 0)
  {
    if (--pulse_halfdam == 0)
    {
      global_halfdam = FALSE;
      pulse_halfdam = -1;
    }
  }
}

void update_halfsspeed()
{
  if (pulse_halfsspeed > 0)
  {
    if ( --pulse_halfsspeed == 0)
    {
      global_halfsspeed = FALSE;
      pulse_halfsspeed = -1;
    }
  }
}

void update_halfspellgain()
{
  if (pulse_halfspellgain > 0)
  {
    if (--pulse_halfspellgain == 0)
    {
      global_halfspellgain = FALSE;
      pulse_halfspellgain = -1;
    }
  }
}

void update_halfstancegain()
{
  if (pulse_halfstancegain > 0)
  {
    if ( --pulse_halfstancegain == 0)
    {
      global_halfstancegain = FALSE;
      pulse_halfstancegain = -1;
    }
  }
}

void update_halfweapongain()
{
  if (pulse_halfweapongain > 0)
  {
    if (--pulse_halfweapongain == 0)
    {
      global_halfweapongain = FALSE;
      pulse_halfweapongain = -1;
    }
  }
}

void do_flags( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (global_exp > 0)
	{
	sprintf(buf,"#7Double Exp#n                          :  #C%d#n minutes left.\n\r", pulse_doubleexp);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Double exp#n                      #Roff#n.\n\r",ch);

    if (global_qp > 0 )
	{
	sprintf(buf,"#7Double Qp#n                           :  #C%d#n minutes left.\n\r", pulse_doubleqp);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Double qp#n                       #Roff#n.\n\r",ch);

    if (global_dam > 0)
	{
	sprintf(buf,"#7Double Damcap#n                       :  #C%d#n minutes left.\n\r", pulse_doubledam);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Double damcap#n                   #Roff#n.\n\r",ch);

    if (global_sspeed > 0)
	{
	sprintf(buf,"#7Double Spell Casting Speed#n          :  #C%d#n minutes left.\n\r", pulse_doublesspeed);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Double Spell Casting Speed#n      #Roff#n.\n\r",ch);

    if (global_spellgain > 0)
	{
	sprintf(buf,"#7Double Spell Training Speed#n         :  #C%d#n minutes left.\n\r", pulse_doublespellgain);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Double Stance Training Speed#n    #Roff#n.\n\r",ch);

    if (global_stancegain > 0)
	{
	sprintf(buf,"#7Double Stance Training Speed#n        :  #C%d#n minutes left.\n\r", pulse_doublestancegain);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Double Stance Training Speed#n    #Roff#n.\n\r",ch);

    if (global_weapongain > 0)
	{
	sprintf(buf,"#7Double Weapon Training Speed#n        :  #C%d#n minutes left.\n\r\n\r", pulse_doubleweapongain);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Double Stance Training Speed#n    #Roff#n.\n\r\n\r",ch);	

    if (global_halfexp > 0)
	{
	sprintf(buf,"#7Half Exp#n                            :  #C%d#n minutes left.\n\r", pulse_halfexp);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Half exp#n                        #Roff#n.\n\r",ch);

    if (global_halfqp > 0 )
	{
	sprintf(buf,"#7Half Qp#n                             :  #C%d#n minutes left.\n\r", pulse_halfqp);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Half qp#n                         #Roff#n.\n\r",ch);

    if (global_halfdam > 0)
	{
	sprintf(buf,"#7Half Damcap#n                         :  #C%d#n minutes left.\n\r", pulse_halfdam);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Half damcap#n                     #Roff#n.\n\r",ch);

    if (global_halfsspeed > 0)
	{
	sprintf(buf,"#7Half Spell Casting Speed#n            :  #C%d#n minutes left.\n\r", pulse_halfsspeed);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Half Spell Casting Speed#n        #Roff#n.\n\r",ch);

    if (global_halfspellgain > 0)
	{
	sprintf(buf,"#7Half Spell Training Speed#n           :  #C%d#n minutes left.\n\r", pulse_halfspellgain);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Half Stance Training Speed#n      #Roff#n.\n\r",ch);

    if (global_halfstancegain > 0)
	{
	sprintf(buf,"#7Half Stance Training Speed#n          :  #C%d#n minutes left.\n\r", pulse_halfstancegain);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Half Stance Training Speed#n      #Roff#n.\n\r",ch);

    if (global_halfweapongain > 0)
	{
	sprintf(buf,"#7Half Weapon Training Speed#n          :  #C%d#n minutes left.\n\r\n\r", pulse_halfweapongain);
	send_to_char(buf,ch);
	}
    else
	send_to_char("#7Half Stance Training Speed#n      #Roff#n.\n\r\n\r",ch);	

return;
} 
