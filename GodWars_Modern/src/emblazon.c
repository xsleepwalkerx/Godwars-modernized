/*
 * emblazon.c — Written for Asgardian Nightmare by Gabe Volker
 * Ported to GodWars Modernized.
 *
 * do_emblazon : claim ownership of an item using a Blood Stone (vnum 9950).
 * do_scratch  : remove your ownership mark from an item you own.
 *
 * Requires obj_data.owner field (char *) — see merc.h and save.c.
 */

#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include "merc.h"

/* Vnum of the Blood Stone item required to emblazon gear */
#define VNUM_BLOOD_STONE    9950

/* Claim ownership of an item */
void do_emblazon( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *stone;

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
    {
        send_to_char( "Nice Try.\n\r", ch );
        return;
    }

    stone = get_eq_char( ch, WEAR_HOLD );

    if ( stone == NULL || stone->pIndexData->vnum != VNUM_BLOOD_STONE )
    {
        send_to_char( "You must be holding a Blood Stone.\n\r", ch );
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "What object do you wish to emblazon?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "You are not carrying that item.\n\r", ch );
        return;
    }

    if ( obj->owner != NULL && obj->owner[0] != '\0' )
    {
        if ( !str_cmp( ch->name, obj->owner ) )
            send_to_char( "It already bears your mark!\n\r", ch );
        else
            send_to_char( "Someone else owns this item.\n\r", ch );
        return;
    }

    /* Consume the Blood Stone */
    obj_from_char( stone );
    extract_obj( stone );

    free_string( obj->owner );
    obj->owner = str_dup( ch->name );

    act( "The Blood Stone fuses into a fiery dagger.", ch, obj, NULL, TO_CHAR );
    act( "You emblazon your mark onto $p.",            ch, obj, NULL, TO_CHAR );
    act( "$n writes runes onto $p with a fiery dagger.", ch, obj, NULL, TO_ROOM );
}

/* Remove ownership mark */
void do_scratch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
    {
        send_to_char( "Nice Try.\n\r", ch );
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "What object do you wish to scratch your emblem off of?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "You are not carrying that item.\n\r", ch );
        return;
    }

    if ( obj->owner == NULL || obj->owner[0] == '\0' )
    {
        send_to_char( "You do not own this item!\n\r", ch );
        return;
    }

    if ( str_cmp( ch->name, obj->owner ) )
    {
        send_to_char( "You do not own this item!\n\r", ch );
        return;
    }

    free_string( obj->owner );
    obj->owner = str_dup( "" );

    act( "You scratch your mark off of $p.",                    ch, obj, NULL, TO_CHAR );
    act( "$n scratches the marks of ownership off of $p.",      ch, obj, NULL, TO_ROOM );
}
