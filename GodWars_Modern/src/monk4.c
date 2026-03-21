/***************************************************************************
 *  monk4.c — Monk equipment crafting.
 *
 *  Ported from Planes-of-Hades (Aronacus).  Allows monks to spend 60
 *  primal (ch->practice) to manifest a piece of blessed monk armour.
 *
 *  Object vnums must exist in your area files.  If an area file with the
 *  following vnums is not loaded the command will safely report an error:
 *    123   — Belt          321   — Cape         2134  — Ring
 *    2512  — Leggings      3214  — Sleeves       12342 — Visor/Ratan
 *    12345 — Boots        13241  — Helmet        13441 — Collar
 *    14325 — Plate        29706  — Bracer        29707 — Cloak
 *    29708 — Gloves
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#elif !defined(_WIN32)
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "monk.h"

/* -----------------------------------------------------------------------
 * do_monkeq — Manifest a piece of blessed monk armour (60 primal).
 * ----------------------------------------------------------------------- */
void do_monkeq( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA       *obj;
    char            arg[MAX_INPUT_LENGTH];
    int             vnum = 0;

    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        stc("What piece of monk armour do you wish to manifest?\n\r", ch);
        stc("Belt, Cape, Plate, Ring, Leggings, Sleeves, Visor,\n\r", ch);
        stc("Boots, Collar, Gloves, Helmet, Ratan, Cloak, Bracer.\n\r", ch);
        return;
    }

    if      ( !str_cmp(arg, "belt"      ) ) vnum = 123;
    else if ( !str_cmp(arg, "cape"      ) ) vnum = 321;
    else if ( !str_cmp(arg, "ring"      ) ) vnum = 2134;
    else if ( !str_cmp(arg, "leggings"  ) ) vnum = 2512;
    else if ( !str_cmp(arg, "sleeves"   ) ) vnum = 3214;
    else if ( !str_cmp(arg, "visor"     ) ) vnum = 12342;
    else if ( !str_cmp(arg, "boots"     ) ) vnum = 12345;
    else if ( !str_cmp(arg, "collar"    ) ) vnum = 13441;
    else if ( !str_cmp(arg, "plate"     ) ) vnum = 14325;
    else if ( !str_cmp(arg, "gloves"    ) ) vnum = 29708;
    else if ( !str_cmp(arg, "helmet"    ) ) vnum = 13241;
    else if ( !str_cmp(arg, "ratan"     ) ) vnum = 12342;
    else if ( !str_cmp(arg, "bracer"    ) ) vnum = 29706;
    else if ( !str_cmp(arg, "cloak"     ) ) vnum = 29707;
    else
    {
        stc("What piece of monk armour do you wish to manifest?\n\r", ch);
        stc("Belt, Cape, Plate, Ring, Leggings, Sleeves, Visor,\n\r", ch);
        stc("Boots, Collar, Gloves, Helmet, Ratan, Cloak, Bracer.\n\r", ch);
        return;
    }

    if ( ch->practice < 60 )
    {
        stc("Manifesting monk armour costs 60 primal.\n\r", ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index(vnum)) == NULL )
    {
        stc("That piece of monk armour is not available on this realm.\n\r", ch);
        bug("do_monkeq: missing vnum %d", vnum);
        return;
    }

    ch->practice -= 60;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p materialises in a flash of white light.", ch, obj, NULL, TO_CHAR);
    act("$p materialises in a flash of white light.", ch, obj, NULL, TO_ROOM);
    return;
}
