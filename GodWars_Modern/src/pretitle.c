/* Here is my pretitle command, fairly simple to add! 
   All you do is type pretitle <pretitle>.  What it does  is puts a 
title before your name. */                     
/***/ act_info.c /***
/* Add the function, to anywhere in act_info.c */
void do_pretitle( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
   int value;
    if ( IS_NPC(ch) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
    if ( ch->pcdata->pretit == '\0' )
        ch->pcdata->pretit = "{x";

    if ( argument[0] == '\0' )
    {
        sprintf(buf, "Your current pretitle is '%s'.{x\n\r",ch->pcdata->pretit);
        send_to_char(buf,ch);
        return;
    }


    if ( strlen(argument) > 45 )
    {
        argument[45] = '{';
        argument[46] = 'x';
        argument[47] = '\0';
    }
    else
    {
        value = strlen(argument);
        argument[value] = '{';
        argument[value+1] = 'x';
        argument[value+2] = '\0';
    }


    ch->pcdata->pretit = str_dup( argument );
    send_to_char("Done.\n\r",ch);
    return;

}

/* In do_whois */
-You'll see this:

             IS_SET(wch->act,PLR_THIEF) ? "(THIEF) " : "",             
		wch->name, IS_NPC(wch) ? "" : wch->pcdata->title);

-add this between them:
             
             wch->pcdata->pretit,

/* In do_who */
-You'll see this:

	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title );
-Put before, wch->name, :

            wch->pcdata->pretit,
/***/ merc.h /***/

/* add this to struct   pc_data, after char *  title; */
char *              pretit;

/***/ interp.h and interp.c /***/
/* add this to interp.h */

DECLARE_DO_FUN( do_pretitle     );
/* add this to interp.c, after title */
{ "pretitle",       do_pretitle,    POS_DEAD,        0,  LOG_NORMAL, 1 },

/***/ save.c /***/

/* add this to void fwrite_char */
fprintf( fp, "Pretit %s~\n",    ch->pcdata->pretit       );
/* in bool load_char_obj add this, after ch->pcdata->title */

ch->pcdata->pretit                  = str_dup( "" );
/* add this to void fread_char, in case 'P' */

            if ( !str_cmp( word, "Ptit" )  || !str_cmp( word, "pretit"))
            {
                ch->pcdata->pretit = fread_string( fp );
              if (ch->pcdata->pretit[0] != '.' && ch->pcdata->pretit[0] != ','
                &&  ch->pcdata->pretit[0] != '!' && ch->pcdata->pretit[0] != '?')
                {
                    sprintf( buf, "%s", ch->pcdata->pretit );
                    free_string( ch->pcdata->pretit );
                    ch->pcdata->pretit = str_dup( buf );
                }
                fMatch = TRUE;
                break;
            }


/* Thats it... if you would like to email me, email me at: zalin69@hotmail.com
   or you can reach me on icq at 239808, i'm mostly invis tho :P */
   -Azalin
