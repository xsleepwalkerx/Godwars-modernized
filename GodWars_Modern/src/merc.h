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
 *  Woolcock.                                                               *
 ***************************************************************************/

/***************************************************************************
 *  GodWars Modernized - Merged from EM (Eternal Malice) and Genesis       *
 *  Incorporates Dragon, Drow, Sorcerer, Elemental, Stance, Trap,          *
 *  Werewolf Auspice/Breed/Tribe subsystems, and more from Genesis.        *
 *  Modernized: C99 standard types, cleaned prototypes, merged classes.    *
 ***************************************************************************/

#ifndef MERC_H
#define MERC_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "monk.h"
#include "warlock.h"
#include "mage.h"
#include "memwatch.h"
#include "colordef.h"

/*
 * Macro compatibility shims for old-style args() declarations.
 * New code should use direct prototypes; these support legacy files.
 */
#define args( list )          list
#define DECLARE_DO_FUN( fun ) DO_FUN    fun
#define DECLARE_SPEC_FUN(fun) SPEC_FUN  fun
#define DECLARE_SPELL_FUN(fun)SPELL_FUN fun

/*
 * Boolean convenience — stdbool.h provides bool/true/false in C99.
 */
#ifndef FALSE
#define FALSE   false
#endif
#ifndef TRUE
#define TRUE    true
#endif

/*
 * Standard integer aliases used throughout codebase.
 * Prefer int32_t / int16_t in new code.
 */
typedef int16_t  sh_int;

/*
 * Forward-declare all structure types.
 */
typedef struct  affect_data         AFFECT_DATA;
typedef struct  area_data           AREA_DATA;
typedef struct  kill_data           KILL_DATA;
typedef struct  ban_data            BAN_DATA;
typedef struct  buf_type            BUFFER;
typedef struct  char_data           CHAR_DATA;
typedef struct  descriptor_data     DESCRIPTOR_DATA;
typedef struct  disabled_data       DISABLED_DATA;
typedef struct  dxp_data            DXP_DATA;
typedef struct  editing_data        EDITING_DATA;   /* OLC */
typedef struct  exit_data           EXIT_DATA;
typedef struct  extra_descr_data    EXTRA_DESCR_DATA;
typedef struct  help_data           HELP_DATA;
typedef struct  l_board             LEADER_BOARD;
typedef struct  mob_index_data      MOB_INDEX_DATA;
typedef struct  prog_list           PROG_LIST;      /* MobProg program list */
typedef struct  note_data           NOTE_DATA;
typedef struct  obj_data            OBJ_DATA;
typedef struct  obj_index_data      OBJ_INDEX_DATA;
typedef struct  pc_data             PC_DATA;
typedef struct  reset_data          RESET_DATA;
typedef struct  room_index_data     ROOM_INDEX_DATA;
typedef struct  shop_data           SHOP_DATA;
typedef struct  time_info_data      TIME_INFO_DATA;
typedef struct  trap_data           TRAP_DATA;      /* Genesis trap system */
typedef struct  weather_data        WEATHER_DATA;

/*
 * Function pointer types.
 */
typedef void DO_FUN   ( CHAR_DATA *ch, char *argument );
typedef bool SPEC_FUN ( CHAR_DATA *ch );
typedef void SPELL_FUN( int sn, int level, CHAR_DATA *ch, void *vo );


/*
 * String and memory sizes.
 */
#define MAX_KEY_HASH         1024
#define MAX_STRING_LENGTH    8192
#define MAX_INPUT_LENGTH      400
#define MSL                  MAX_STRING_LENGTH
#define MIL                  MAX_INPUT_LENGTH
#define SCREEN_WIDTH           79

/*
 * Game parameters.
 */
#define SKILL_ADEPT               100
#define SKILL_THAC0_32             18
#define SKILL_THAC0_00              6
#define VERSION_NUMBER            "2.0-Modern"
#define DONATION_ROOM_WEAPON     6641
#define DONATION_ROOM_ARMOR      6643
#define DONATION_ROOM_REST       6644
#define MAX_VAMPIRE_POWER           3
#define MAX_CLAN                   11
#define MAX_ART                    12
#define OPTIMISED_PFILES            0

#define is_full_name is_name

/* Automap parameters */
#define MAPX     10
#define MAPY      8
#define MAXDEPTH  2

#define AGE_NEWBIE          10

/* Vampire age/rank titles */
#define AGE_CHILDE          0
#define AGE_NEONATE         1
#define AGE_ANCILLA         2
#define AGE_ELDER           3
#define AGE_METHUSELAH      4
#define AGE_ANTEDILUVIAN    5
/* Werewolf age/rank titles */
#define AGE_PUP             0
#define AGE_ADULT           1
#define AGE_YOUTH           2
#define AGE_ANCIENT         3

/* Dragon age stages (dragon.c — vampgen 0..11, 12 stages)
 * Note: AGE_ADULT=1 and AGE_ANCIENT=3 (werewolf) conflict with
 * VYOUNG=1 and JUVENILE=3; those two cases use numeric literals in dragon.c.
 */
#define AGE_HATCHLING       0
#define AGE_VYOUNG          1
#define AGE_YOUNG           2
#define AGE_JUVENILE        3
#define AGE_YADULT          4
/* stage 5: adult dragon — use literal 5 in switch (conflicts with AGE_ADULT=1) */
#define AGE_MATURE          6
#define AGE_OLD             7
#define AGE_VOLD            8
/* stage 9: ancient dragon — use literal 9 in switch (conflicts with AGE_ANCIENT=3) */
#define AGE_WYRM            10
#define AGE_AWYRM           11

/* classpoints[] array indices */
#define CP_CURRENT  0   /* classpoints[0] = current class points */
#define CP_MAX      1   /* classpoints[1] = max class points */

#define PARADOX_TICK        10
#define MAX_SKILL          165
#define MAX_SPELL           62
#define MAX_LEVEL           12
#define NO_WATCH            10
#define LEVEL_HERO          (MAX_LEVEL - 9)
#define LEVEL_IMMORTAL      (MAX_LEVEL - 5)
#define IM                  (MAX_LEVEL - 5)
#define LEVEL_MORTAL        (MAX_LEVEL - 10)
#define LEVEL_AVATAR        (MAX_LEVEL - 9)
#define LEVEL_APPRENTICE    (MAX_LEVEL - 8)
#define LEVEL_MAGE          (MAX_LEVEL - 7)
#define LEVEL_ARCHMAGE      (MAX_LEVEL - 6)
#define LEVEL_NINJA         (MAX_LEVEL - 6)
#define LEVEL_MONK          (MAX_LEVEL - 6)
#define LEVEL_BUILDER       (MAX_LEVEL - 5)
#define LEVEL_QUESTMAKER    (MAX_LEVEL - 4)
#define LEVEL_ENFORCER      (MAX_LEVEL - 3)
#define LEVEL_JUDGE         (MAX_LEVEL - 2)
#define LEVEL_HIGHJUDGE     (MAX_LEVEL - 1)
#define LEVEL_IMPLEMENTOR   (MAX_LEVEL)

#define PULSE_PER_SECOND     4
#define PULSE_GAIN_EXP       ( 1 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE       ( 1 )
#define PULSE_MOBILE         ( 4 * PULSE_PER_SECOND)
#define PULSE_TICK           (60 * PULSE_PER_SECOND)
#define PULSE_AREA           (60 * PULSE_PER_SECOND)
#define PULSE_DEATHTRAP      ( 5 * PULSE_PER_SECOND)
#define MAXCPTIMER           14400

/*
 * Boards (bulletin boards).
 */
#define MAX_BOARD    8

/*
 * Login class bits — used in nanny() character creation flow.
 */
#define LOGIN_NONE          0
#define LOGIN_NEWBIE        1
#define LOGIN_MORTAL        2
#define LOGIN_GOD           4
#define LOGIN_VAMPIRE       8
#define LOGIN_WEREWOLF      16
#define LOGIN_MAGE          32
#define LOGIN_DEMON         64
#define LOGIN_HIGHLANDER    128
#define LOGIN_NINJA         512
#define LOGIN_MONK          1024
/* Genesis additions */
#define LOGIN_SORCERER      2048
#define LOGIN_DROW          4096
#define LOGIN_DRAGON        8192
#define LOGIN_ELEMENTAL     16384

/*
 * Sun / sky conditions.
 */
#define SUN_DARK        0
#define SUN_RISE        1
#define SUN_LIGHT       2
#define SUN_SET         3

#define SKY_CLOUDLESS   0
#define SKY_CLOUDY      1
#define SKY_RAINING     2
#define SKY_LIGHTNING   3

/*
 * Well-known mob virtual numbers.
 */
#define MOB_VNUM_CITYGUARD    3060
#define MOB_VNUM_VAMPIRE      3404
#define MOB_VNUM_GUARDIAN    30001
#define MOB_VNUM_MOUNT       30006
#define MOB_VNUM_FROG        30007
#define MOB_VNUM_RAVEN       30008
#define MOB_VNUM_CAT         30009
#define MOB_VNUM_DOG         30010
#define MOB_VNUM_EYE         30012
#define MOB_VNUM_MOLERAT     30013
#define MOB_VNUM_YAK         30014
#define MOB_VNUM_ASTRAL      30015
#define MOB_VNUM_CLONE       30016

/*
 * =========================================================================
 * CLASS SYSTEM
 * Combined from EM (bitfield hybrid classes) + Genesis (discrete classes).
 * The EM model is used as primary: CLASS_VAMPIRE=1, WEREWOLF=2, etc.
 * Genesis-exclusive classes (Dragon, Drow, Sorcerer, Elemental) are added.
 * =========================================================================
 */
#define CLASS_NONE              0
#define CLASS_VAMPIRE           1
#define CLASS_WEREWOLF          2
#define CLASS_ABOMINATION       3   /* Vampire + Werewolf */
#define CLASS_MAGE              4
#define CLASS_LICH              5   /* Vampire + Mage */
#define CLASS_SKINDANCER        6   /* Werewolf + Mage */
#define CLASS_DEMON             8
#define CLASS_BAALI             9   /* Vampire + Demon */
#define CLASS_BLACK_SPIRAL_DANCER 10 /* Werewolf + Demon */
#define CLASS_NEPHANDI          12  /* Mage + Demon */
#define CLASS_HIGHLANDER        16
#define CLASS_GHOUL             32
#define CLASS_GARGOYLE          64
#define CLASS_ANGEL             72  /* Includes CLASS_DEMON bit */
#define CLASS_NINJA             128
#define CLASS_MONK              512
#define CLASS_EXIT              1024
#define CLASS_KOE               2048
#define CLASS_FAE               4096
/* Genesis-exclusive classes */
#define CLASS_SORCERER          8192
#define CLASS_DROW              16384
#define CLASS_DRAGON            32768
#define CLASS_ELEMENTAL         65536

/*
 * Vampire Disciplines — indices into pcdata->disc[].
 */
#define DISC_ANIMALISM          0
#define DISC_AUSPEX             1
#define DISC_CELERITY           2
#define DISC_CHIMERSTRY         3
#define DISC_DEMENTATION        4
#define DISC_DOMINATE           5
#define DISC_FORTITUDE          6
#define DISC_NECROMANCY         7
#define DISC_OBFUSCATE          8  /* was 9 in EM — renumbered for contiguity */
#define DISC_OBTENEBRATION      9
#define DISC_POTENCE            10
#define DISC_PRESENCE           11
#define DISC_PROTEAN            12
#define DISC_QUIETUS            13
#define DISC_SERPENTIS          14
#define DISC_SPIRITUS           15
#define DISC_THAUMATURGY        16
#define DISC_VICISSITUDE        17
#define DISC_MAX                18

/*
 * Werewolf Breed — indices into pcdata->breed[].
 */
#define BREED_HOMID     0
#define BREED_LUPUS     1
#define BREED_METIS     2
#define BREED_MAX       2

/*
 * Werewolf Auspice — indices into pcdata->auspice[].
 */
#define AUSPICE_RAGABASH    0
#define AUSPICE_THEURGE     1
#define AUSPICE_PHILODOX    2
#define AUSPICE_GALLIARD    3
#define AUSPICE_AHROUN      4
#define AUSPICE_MAX         4

/*
 * Werewolf Tribe — indices into pcdata->tribes[].
 */
#define TRIBE_BLACK_FURIES      0
#define TRIBE_BONE_GNAWERS      1
#define TRIBE_CHILDREN_OF_GAIA  2
#define TRIBE_FIANNA            3
#define TRIBE_GET_OF_FENRIS     4
#define TRIBE_GLASS_WALKERS     5
#define TRIBE_RED_TALONS        6
#define TRIBE_SHADOW_LORDS      7
#define TRIBE_SILENT_STRIDERS   8
#define TRIBE_SILVER_FANGS      9
#define TRIBE_STARGAZERS        10
#define TRIBE_UKTENA            11
#define TRIBE_WENDIGOS          12
#define TRIBE_MAX               12

/*
 * Drow abilities — bits in pcdata->drow[1].
 */
#define DROW_POWERS     0   /* index into drow[] for power level */
#define DROW_SPEED      1   /* index into drow[] for speed */
#define DROW_ACID       1
#define DROW_WEB        2
#define DROW_DARK       4
#define DROW_FIRE       8
#define DROW_BITE       16
#define DROW_DUALWIELD  32
#define DROW_BFIGHT     64
#define DROW_SPIDER     128
#define DROW_EYES       256
#define DROW_READ       512
#define DROW_NIGHT      1024
#define DROW_SIGHT      2048
#define DROW_PIMM       4096
#define DROW_SKIP       8192
#define DROW_ATTUNE     16384
#define DROW_MIGHT      32768
#define DROW_TOUGH      65536
#define DROW_GLOBE      262144
#define DROW_MAX        DROW_GLOBE  /* highest DROW bit for iteration */

/* EXTRA_VAMPIRE — extra bit marking a drow with vampire blood */
#define EXTRA_VAMPIRE   EXTRA_NIGHTSHADE  /* reuse NIGHTSHADE bit as vampire mark */

/*
 * Dragon ability bits — in pcdata->dragskills.
 */
#define DRAG_FANGS      1
#define DRAG_NIGHT      2
#define DRAG_HORNS      4
#define DRAG_CLAWS      8
#define DRAG_WINGS      16
#define DRAG_BREATH     32
#define DRAG_TAIL       64
#define DRAG_TRUESIGHT  128
#define DRAG_DUST       256
#define DRAG_SCRY       512
#define DRAG_MAGIC      1024
#define DRAG_READAURA   2048
#define DRAG_SHIELD     4096
#define DRAG_WINGSOUT   16384
#define DRAG_HORNSOUT   32768

/*
 * Elemental aspects — values for pcdata->elem_type.
 */
#define ELEM_CHOICE     0
#define ELEM_FIRE       1
#define ELEM_WATER      2
#define ELEM_EARTH      3
#define ELEM_WIND       4
#define ELEM_SUN        5
#define ELEM_MOON       6
#define ELEM_TIME       7
#define ELEM_INFIN      8

/*
 * Mage sphere indices — pcdata->spheres[].
 */
#define SPHERE_CORRESPONDENCE   0
#define SPHERE_LIFE             1
#define SPHERE_PRIME            2
#define SPHERE_ENTROPY          3
#define SPHERE_MIND             4
#define SPHERE_SPIRIT           5
#define SPHERE_FORCES           6
#define SPHERE_MATTER           7
#define SPHERE_TIME             8

/* Short aliases for sphere indices (rot_wiz.c / magic.c) */
#define MCOR    SPHERE_CORRESPONDENCE
#define MLIF    SPHERE_LIFE
#define MPRI    SPHERE_PRIME
#define MENT    SPHERE_ENTROPY
#define MMIN    SPHERE_MIND
#define MSPI    SPHERE_SPIRIT
#define MFOR    SPHERE_FORCES
#define MMAT    SPHERE_MATTER
#define MTIM    SPHERE_TIME

/*
 * Combat Stances — pcdata->stance[0].
 */
#define STANCE_NONE      0
#define STANCE_VIPER     1
#define STANCE_CRANE     2
#define STANCE_CRAB      3
#define STANCE_MONGOOSE  4
#define STANCE_BULL      5
#define STANCE_MANTIS    6
#define STANCE_DRAGON    7
#define STANCE_TIGER     8
#define STANCE_MONKEY    9
#define STANCE_SWALLOW   10

/*
 * Combat Weapon Styles — ch->style[0].
 */
#define STYLE_CURRENT   0
#define STYLE_UNARMED   1
#define STYLE_NATURAL   2
#define STYLE_ONEHAND   3
#define STYLE_SHIELD    4
#define STYLE_TWOHAND   5
#define STYLE_TWOWEAPON 6

/*
 * Immunities.
 */
#define IMM_SLASH       1
#define IMM_STAB        2
#define IMM_SMASH       4
#define IMM_ANIMAL      8
#define IMM_MISC        16
#define IMM_CHARM       32
#define IMM_HEAT        64
#define IMM_COLD        128
#define IMM_LIGHTNING   256
#define IMM_ACID        512
#define IMM_SUMMON      1024
#define IMM_VOODOO      2048
#define IMM_VAMPIRE     4096
#define IMM_STAKE       8192
#define IMM_SUNLIGHT    16384
#define IMM_SHIELDED    32768
#define IMM_HURL        65536
#define IMM_BACKSTAB    131072
#define IMM_KICK        262144
#define IMM_DISARM      524288
#define IMM_STEAL       1048576
#define IMM_SLEEP       2097152
#define IMM_DRAIN       4194304
#define IMM_DEMON       8388608
#define IMM_TRANSPORT   16777216

/*
 * act() output type constants.
 */
#define TO_ROOM         0
#define TO_NOTVICT      1
#define TO_VICT         2
#define TO_CHAR         3

/*
 * ACT bits for mobs.
 */
#define ACT_IS_NPC      1
#define ACT_SENTINEL    2
#define ACT_SCAVENGER   4
#define ACT_AGGRESSIVE  32
#define ACT_STAY_AREA   64
#define ACT_WIMPY       128
#define ACT_PET         256
#define ACT_TRAIN       512
#define ACT_PRACTICE    1024
#define ACT_MOUNT       2048
#define ACT_NOPARTS     4096
#define ACT_NOEXP       8192
#define ACT_DOG         16384
#define ACT_SKILLED     32768   /* Uses combat stances */
#define ACT_COMMANDED   65536

/*
 * PLR (player) bits — stored in ch->act.
 */
#define PLR_BRIEF           1
#define PLR_COMBINE         2
#define PLR_PROMPT          4
#define PLR_TELNET_GA       8
#define PLR_AUTOEXIT        16
#define PLR_AUTOLOOT        32
#define PLR_AUTOSAC         64
#define PLR_AUTOGOLD        128
#define PLR_AUTOSPLIT       256
#define PLR_BOUGHT_PET      512
#define PLR_NOINTERPRET     1024
#define PLR_CAMP            2048
#define PLR_NOSUICIDE       4096
#define PLR_HOLYLIGHT       8192
#define PLR_WIZINVIS        16384
#define PLR_DENY            32768
#define PLR_FREEZE          65536
#define PLR_THIEF           131072
#define PLR_KILLER          262144
#define PLR_GODLESS         524288
#define PLR_ANSI            1048576
#define PLR_NEWBIE          2097152
#define PLR_BUILDWALK       4194304
#define PLR_QUESTOR         8388608
#define PLR_ONMAP           16777216
#define PLR_CLASSELDER      131072  /* Non-CL privileged */
#define PLR_SILENCE         33554432
#define PLR_NO_EMOTE        67108864
#define PLR_NO_TELL         134217728
#define PLR_WOLFMAN         268435456
#define PLR_AUTOFOLLOW      536870912  /* autofollow ally flag */

/*
 * Affected_by bits.
 */
#define AFF_BLIND           1
#define AFF_INVISIBLE       2
#define AFF_DETECT_EVIL     4
#define AFF_DETECT_INVIS    8
#define AFF_DETECT_MAGIC    16
#define AFF_DETECT_HIDDEN   32
#define AFF_SHADOWPLANE     64
#define AFF_SANCTUARY       128
#define AFF_FAERIE_FIRE     256
#define AFF_INFRARED        512
#define AFF_CURSE           1024
#define AFF_FLAMING         2048
#define AFF_POISON          4096
#define AFF_PROTECT         8192
#define AFF_ETHEREAL        16384
#define AFF_SNEAK           32768
#define AFF_HIDE            65536
#define AFF_SLEEP           131072
#define AFF_CHARM           262144
#define AFF_FLYING          524288
#define AFF_PASS_DOOR       1048576
#define AFF_POLYMORPH       2097152
#define AFF_SHADOWSIGHT     4194304
#define AFF_WEBBED          8388608
#define AFF_CONTRACEPTION   16777216
#define AFF_SHADOW          33554432
#define AFF_ROT             67108864
#define AFF_FEAR            134217728
#define AFF_JAIL            268435456
#define AFF_TOTALBLIND      536870912
#define AFF_NINJAHIDE       1073741824
/* AFF_ANCHORED — used by CAN_SIT() to block sitting while anchored */
#define AFF_ANCHORED        2147483648u

/*
 * Item affect bits.
 */
#define ITEMA_SHOCKSHIELD   1
#define ITEMA_FIRESHIELD    2
#define ITEMA_ICESHIELD     4
#define ITEMA_ACIDSHIELD    8
#define ITEMA_DBPASS        16
#define ITEMA_CHAOSSHIELD   32
#define ITEMA_ARTIFACT      64
#define ITEMA_REGENERATE    128
#define ITEMA_SPEED         256
#define ITEMA_VORPAL        512
#define ITEMA_PEACE         1024
#define ITEMA_RIGHT_SILVER  2048
#define ITEMA_LEFT_SILVER   4096
#define ITEMA_REFLECT       8192
#define ITEMA_RESISTANCE    16384
#define ITEMA_VISION        32768
#define ITEMA_STALKER       65536
#define ITEMA_VANISH        131072
#define ITEMA_RAGER         262144
#define ITEMA_HIGHLANDER    524288
#define ITEMA_SHADES        1048576
#define ITEMA_SUNSHIELD     2097152
#define ITEMA_RIGHT_AGG     4194304
#define ITEMA_LEFT_AGG      8388608
#define ITEMA_BLISSFUL      16777216    /* Genesis: Blissful Ignorance */

/*
 * Polymorph bits — ch->polyaff.
 */
#define POLY_WOLFMAN        1
#define POLY_WOLF           2
#define POLY_WARWOLF        4
#define POLY_BAT            8
#define POLY_MIST           16
#define POLY_SERPENT        32
#define POLY_DRAGONFORM     64  /* Genesis dragon form */
#define POLY_ELEMFORM       128 /* Genesis elemental form */
#define POLY_FROG           256
#define POLY_RAVEN          512
#define POLY_ZULO           1024

/*
 * Vampire affect bits — ch->vampaff.
 */
#define VAM_FANGS           1
#define VAM_CLAWS           2
#define VAM_NIGHTSIGHT      4
#define VAM_DISGUISED       8
#define VAM_EARTHMELDED     16
#define VAM_VANISH          32
#define VAM_CHANGED         64
#define VAM_PASSDOOR        128
#define VAM_OBFUSCATE       256
#define VAM_BLUR            512
#define VAM_DREAD           1024
#define VAM_HORRID          2048
#define VAM_NIGHTMARE       4096

/*
 * Vampire pass (discipline passives) — ch->vamppass.
 */
#define VPASS_FORTITUDE     1
#define VPASS_POTENCE       2
#define VPASS_CELERITY      4
#define VPASS_PRESENCE      8

/*
 * MORE bits — ch->more.
 */
#define MORE_SIRE           1
#define MORE_PREGNANT       2
#define MORE_SPIRITGUARD    4
#define MORE_WOLFGUARD      8
#define MORE_VANQUISH       16
#define MORE_STAKE          32
#define MORE_EARTHMELD      64
#define MORE_OBFUSCATE      128
#define MORE_SPIRIT         256
#define MORE_HUNGRY         512
#define MORE_THIRSTY        1024
#define MORE_DRUNK          2048
#define MORE_HELIOS         4096
#define MORE_HEAD           8192
#define MORE_SHIELDED       16384
#define MORE_DISINT         65536
#define MORE_CLOAK          131072
#define MORE_THINBLOOD      262144
#define MORE_ANIMAL_MASTER  524288
#define MORE_BRISTLES       1048576
#define MORE_FAVOUR         2097152
#define MORE_SINGING        4194304
#define MORE_INFORM         8388608
#define MORE_SILENCE        16777216
#define MORE_DARKNESS       33554432
#define MORE_GUARDIAN       67108864
#define MORE_LOYAL          134217728
#define MORE_NEUTRAL        268435456
#define MORE_REINA          536870912

/*
 * XTRA bits — ch->extra (extended player bits).
 */
#define EXTRA_EARTHMELD     1
#define EXTRA_BURROWED      2
#define EXTRA_WOLF          4
#define EXTRA_WARWOLF       8
#define EXTRA_SPECIAL       16
#define EXTRA_AWE           32
#define EXTRA_BERSERK       64
#define EXTRA_FREEZE        128
#define EXTRA_TWOHANDED     256
#define EXTRA_AGGRESSIVE    512
#define EXTRA_SPIRAL        1024
#define EXTRA_MARRIED       2048
#define EXTRA_PREGNANT      4096
#define EXTRA_TENDON        8192
#define EXTRA_VANQUISH      16384
#define EXTRA_NIGHTSHADE    32768
#define EXTRA_SAFE          65536
#define EXTRA_MUTE          131072
#define EXTRA_GAGGED        262144
#define EXTRA_BOUND         524288
#define EXTRA_GOLEM         1048576
#define EXTRA_DEAD          2097152
#define EXTRA_PSAY          4194304

/* Aliases for backward compatibility */
#define GAGGED              EXTRA_GAGGED

/* Additional EXTRA bits */
#define EXTRA_AFK           8388608
#define EXTRA_ILLUSION      16777216
#define EXTRA_VICISSITUDE   33554432
#define EXTRA_PROMPT        67108864
#define EXTRA_SIRE          134217728
#define EXTRA_PRINCE        268435456
#define EXTRA_OSWITCH       536870912
#define EXTRA_INVIS_EMAIL   1073741824
#define EXTRA_FAKE_CON      (int)2147483647
#define EXTRA_STANCE        (int32_t)(-2147483648) /* bit 31 — auto-stance flag */

/* EXTRA_CLEARED_EQ — used to track whether starter eq has been removed */
#define EXTRA_CLEARED_EQ    EXTRA_FAKE_CON  /* alias: reuse bit */

/* extra2 overflow bits — use IS_EXTRA2(ch, sn) and ch->extra2 field */
#define EXTRA2_BORN         1       /* Character was properly born/initialized */
#define EXTRA2_NEWPASS      2       /* Player needs to set a new password */
#define EXTRA2_CAN_SEE      4       /* Immortal familiar-sight power */
/* Aliases used in old code */
#define EXTRA_BORN          EXTRA2_BORN

/* damcap[] array indices */
#define DAM_CAP     0   /* current damage cap value */
#define DAM_CHANGE  1   /* flag: damage cap needs recalculation */
#define EXTRA_NEWPASS       EXTRA2_NEWPASS
#define EXTRA_CAN_SEE       EXTRA2_CAN_SEE

/* Additional PLR bits */
#define PLR_BLANK           536870912
#define PLR_DEMON           1073741824
#define PLR_MAP             268435456
#define PLR_MOBSTUN         134217728
#define PLR_WATCHER         67108864
#define PLR_WIZINCOG        33554432

/* Additional MORE bits */
#define MORE_NOPAIN         1073741824
#define MORE_FLAMES         536870912
#define MORE_EVILEYE        268435456
#define MORE_HAND_FLAME     134217728
#define MORE_OBEAH          67108864
#define MORE_MARAUDER       33554432
#define MORE_OUTCAST        16777216

/*
 * Rune, Glyph and Sigil bits (Mage system).
 */
#define RUNE_FIRE       1
#define RUNE_AIR        2
#define RUNE_EARTH      4
#define RUNE_WATER      8
#define RUNE_DARK       16
#define RUNE_LIGHT      32
#define RUNE_LIFE       64
#define RUNE_DEATH      128
#define RUNE_MIND       256
#define RUNE_SPIRIT     512
#define RUNE_MASTER     1024

#define GLYPH_CREATION      1
#define GLYPH_DESTRUCTION   2
#define GLYPH_SUMMONING     4
#define GLYPH_TRANSFORMATION 8
#define GLYPH_TRANSPORTATION 16
#define GLYPH_ENHANCEMENT   32
#define GLYPH_REDUCTION     64
#define GLYPH_CONTROL       128
#define GLYPH_PROTECTION    256
#define GLYPH_INFORMATION   512

#define SIGIL_SELF      1
#define SIGIL_TARGETING 2
#define SIGIL_AREA      4
#define SIGIL_OBJECT    8

/*
 * Spell types for advanced mage system.
 */
#define ST_MOB_MOB  1
#define ST_OBJ_MOB  2
#define ST_OBJ_OBJ  3

#define ADV_DAMAGE  1
#define ADV_AFFECT  2
#define ADV_ACTION  4

/*
 * Embrace positions.
 */
#define ARE_NONE        0
#define ARE_EMBRACING   1
#define ARE_EMBRACED    2

/*
 * Sex.
 */
#define SEX_NEUTRAL 0
#define SEX_MALE    1
#define SEX_FEMALE  2

/*
 * Well-known object VNUMs.
 */
#define OBJ_VNUM_MONEY_ONE      2
#define OBJ_VNUM_MONEY_SOME     3
#define OBJ_VNUM_CORPSE_NPC     10
#define OBJ_VNUM_CORPSE_PC      11
#define OBJ_VNUM_SEVERED_HEAD   12
#define OBJ_VNUM_MUSHROOM       20
#define OBJ_VNUM_LIGHT_BALL     21
#define OBJ_VNUM_SPRING         22
#define OBJ_VNUM_SOULBLADE      30000
#define OBJ_VNUM_PORTAL         30001
#define OBJ_VNUM_SPILT_BLOOD    30007
#define OBJ_VNUM_VOODOO_DOLL    30010
#define OBJ_VNUM_PROTOPLASM     30037
#define OBJ_VNUM_QUESTCARD      30039
#define OBJ_VNUM_QUESTMACHINE   30040
/* Body-part severing objects */
#define OBJ_VNUM_SLICED_ARM     30013
#define OBJ_VNUM_SLICED_LEG     30014
#define OBJ_VNUM_SQUIDGY_EYEBALL 30015
#define OBJ_VNUM_SLICED_EAR     30016
#define OBJ_VNUM_SLICED_NOSE    30017
#define OBJ_VNUM_SEVERED_HAND   30018
#define OBJ_VNUM_SEVERED_FOOT   30019
#define OBJ_VNUM_SEVERED_THUMB  30022
#define OBJ_VNUM_SEVERED_INDEX  30023
#define OBJ_VNUM_SEVERED_MIDDLE 30024
#define OBJ_VNUM_SEVERED_RING   30025
#define OBJ_VNUM_SEVERED_LITTLE 30026
/* Additional body-part objects */
#define OBJ_VNUM_TORN_HEART     30027
#define OBJ_VNUM_SPILLED_ENTRAILS 30028
#define OBJ_VNUM_QUIVERING_BRAIN 30029
#define OBJ_VNUM_RIPPED_FACE    30030
#define OBJ_VNUM_TORN_WINDPIPE  30031
#define OBJ_VNUM_CRACKED_HEAD   30032
#define OBJ_VNUM_KNOCKED_TOOTH  30033
#define OBJ_VNUM_TORN_TONGUE    30034
#define OBJ_VNUM_SEVERED_TOE    30035

/*
 * Item types.
 */
#define ITEM_LIGHT      1
#define ITEM_SCROLL     2
#define ITEM_WAND       3
#define ITEM_STAFF      4
#define ITEM_WEAPON     5
#define ITEM_PTOKEN     6
#define ITEM_TREASURE   8
#define ITEM_ARMOR      9
#define ITEM_POTION     10
#define ITEM_FURNITURE  12
#define ITEM_TRASH      13
#define ITEM_CONTAINER  15
#define ITEM_DRINK_CON  17
#define ITEM_KEY        18
#define ITEM_FOOD       19
#define ITEM_MONEY      20
#define ITEM_BOAT       22
#define ITEM_CORPSE_NPC 23
#define ITEM_CORPSE_PC  24
#define ITEM_FOUNTAIN   25
#define ITEM_PILL       26
#define ITEM_PORTAL     27
#define ITEM_EGG        28
#define ITEM_VOODOO     29
#define ITEM_STAKE      30
#define ITEM_MISSILE    31
#define ITEM_QUESTITEM  32
#define ITEM_BOOK       33
#define ITEM_PAGE       34
#define ITEM_BOARD      35
#define ITEM_PEN        36  /* pen (sorcerer spellbook writing tool) */

/*
 * Wear locations.
 */
#define WEAR_NONE       -1
#define WEAR_LIGHT       0
#define WEAR_FINGER_L    1
#define WEAR_FINGER_R    2
#define WEAR_NECK_1      3
#define WEAR_NECK_2      4
#define WEAR_BODY        5
#define WEAR_HEAD        6
#define WEAR_LEGS        7
#define WEAR_FEET        8
#define WEAR_HANDS       9
#define WEAR_ARMS       10
#define WEAR_SHIELD     11
#define WEAR_ABOUT      12
#define WEAR_WAIST      13
#define WEAR_WRIST_L    14
#define WEAR_WRIST_R    15
#define WEAR_WEAPON     16
#define WEAR_HOLD       17
#define WEAR_EYES       18
#define WEAR_FACE       19
#define WEAR_HORNS      20
#define WEAR_TAIL       21
#define WEAR_ABOUT_H    22
#define MAX_WEAR        23

/*
 * Wear flags.
 */
#define ITEM_TAKE           1
#define ITEM_WEAR_FINGER    2
#define ITEM_WEAR_NECK      4
#define ITEM_WEAR_BODY      8
#define ITEM_WEAR_HEAD      16
#define ITEM_WEAR_LEGS      32
#define ITEM_WEAR_FEET      64
#define ITEM_WEAR_HANDS     128
#define ITEM_WEAR_ARMS      256
#define ITEM_WEAR_SHIELD    512
#define ITEM_WEAR_ABOUT     1024
#define ITEM_WEAR_WAIST     2048
#define ITEM_WEAR_WRIST     4096
#define ITEM_WIELD          8192
#define ITEM_HOLD           16384
#define ITEM_WEAR_HOLD      ITEM_HOLD   /* alias: hold slot wear flag */
#define ITEM_WEAR_EYES      32768
#define ITEM_WEAR_FACE      65536
#define ITEM_WEAR_HORNS     131072
#define ITEM_WEAR_TAIL      262144
#define ITEM_WEAR_ABOUT_H   524288

/*
 * Extra flags for objects.
 */
#define ITEM_GLOW           1
#define ITEM_HUM            2
#define ITEM_DARK           4
#define ITEM_EVIL           8
#define ITEM_INVIS          16
#define ITEM_MAGIC          32
#define ITEM_NODROP         64
#define ITEM_BLESS          128
#define ITEM_ANTI_GOOD      256
#define ITEM_ANTI_EVIL      512
#define ITEM_ANTI_NEUTRAL   1024
#define ITEM_NOREMOVE       2048
#define ITEM_INVENTORY      4096
#define ITEM_VAMPIRE        8192
#define ITEM_NOPURGE        16384
#define ITEM_QUEST          32768
#define ITEM_LOYAL          65536
#define ITEM_DONATE         131072
#define ITEM_NOSAC          262144
#define ITEM_MELT_DROP      524288
#define ITEM_HAD_TIMER      1048576
#define ITEM_SELL_EXTRACT   2097152
#define ITEM_NOSAVE         4194304
#define ITEM_SHADOWPLANE    8388608
#define ITEM_BURIED         16777216
#define ITEM_WEREWOLF       33554432
#define ITEM_MAGE           67108864

/*
 * Apply types (for affects).
 */
#define APPLY_NONE          0
#define APPLY_STR           1
#define APPLY_DEX           2
#define APPLY_INT           3
#define APPLY_WIS           4
#define APPLY_CON           5
#define APPLY_SEX           6
#define APPLY_CLASS         7
#define APPLY_LEVEL         8
#define APPLY_AGE           9
#define APPLY_HEIGHT        10
#define APPLY_WEIGHT        11
#define APPLY_MANA          12
#define APPLY_HIT           13
#define APPLY_MOVE          14
#define APPLY_GOLD          15
#define APPLY_EXP           16
#define APPLY_AC            17
#define APPLY_HITROLL       18
#define APPLY_DAMROLL       19
#define APPLY_SAVING_PARA   20
#define APPLY_SAVING_ROD    21
#define APPLY_SAVING_PETRI  22
#define APPLY_SAVING_BREATH 23
#define APPLY_SAVING_SPELL  24
#define APPLY_BLOOD_MAX     25
#define APPLY_BLOOD_POT     26

/*
 * Positions.
 */
#define POS_DEAD        0
#define POS_MORTAL      1
#define POS_INCAP       2
#define POS_STUNNED     3
#define POS_SLEEPING    4
#define POS_RESTING     5
#define POS_SITTING     6
#define POS_FIGHTING    7
#define POS_STANDING    8

/*
 * Damage types.
 */
#define TYPE_HIT        0
#define TYPE_UNARMED    1
#define TYPE_SLASH      2
#define TYPE_PIERCE     3
#define TYPE_BLUNT      4
#define TYPE_SPELL      5

/*
 * Channel bits.
 */
#define CHANNEL_SORCTALK    1
#define CHANNEL_CHAT        2
#define CHANNEL_HONOR       4
#define CHANNEL_IMMTALK     8
#define CHANNEL_MUSIC       16
#define CHANNEL_MAGETALK    32
#define CHANNEL_SHOUT       64
#define CHANNEL_YELL        128
#define CHANNEL_VAMPTALK    256
#define CHANNEL_HOWL        512
#define CHANNEL_LOG         1024
#define CHANNEL_HIGHTALK    2048
#define CHANNEL_INFO        4096
#define CHANNEL_NEWBIE      8192
#define CHANNEL_TELL        16384
#define CHANNEL_MIKTALK     65536
#define CHANNEL_ELEM        131072  /* Genesis elemental channel */
#define CHANNEL_OOC         262144
#define CHANNEL_DRAGONROAR  524288  /* Genesis dragon channel */
#define CHANNEL_DROWSPEAK   1048576 /* Genesis drow channel */
#define CHANNEL_DEMON       2097152
#define CHANNEL_ANGEL       4194304
#define CHANNEL_MONK        8388608
#define CHANNEL_FLAME       16777216
#define CHANNEL_KTALK       33554432
#define CHANNEL_WINDWHISPER 67108864
#define CHANNEL_BUILD       134217728
#define CHANNEL_QUESTION    268435456

/*
 * Connection states.
 */
#define CON_PLAYING             0
#define CON_GET_NAME            1
#define CON_GET_OLD_PASSWORD    2
#define CON_CONFIRM_NEW_NAME    3
#define CON_GET_NEW_PASSWORD    4
#define CON_CONFIRM_NEW_PASSWORD 5
#define CON_GET_NEW_SEX         6
#define CON_GET_NEW_CLASS       7
#define CON_READ_MOTD           8
#define CON_BREAK_CONNECT       9
#define CON_EDITING             11  /* OLC */

/*
 * Room flags.
 */
#define ROOM_DARK           1
#define ROOM_NO_MOB         4
#define ROOM_INDOORS        8
#define ROOM_PRIVATE        32
#define ROOM_SAFE           64
#define ROOM_SOLITARY       128
#define ROOM_PET_SHOP       256
#define ROOM_NO_RECALL      512
#define ROOM_IMP_ONLY       1024
#define ROOM_GODS_ONLY      2048
#define ROOM_HEROES_ONLY    4096
#define ROOM_NEWBIES_ONLY   8192
#define ROOM_SHADOWPLANE    16384
#define ROOM_ARENA          32768
#define ROOM_NOPORTAL       65536
#define ROOM_CLAN           131072
#define ROOM_VAMPIRE        262144
#define ROOM_WEREWOLF       524288
#define ROOM_MAGE           1048576
#define ROOM_UNDERWATER     2097152
#define ROOM_NOHOME         4194304
#define ROOM_TOTAL_DARKNESS 8388608

/*
 * Secondary room flags (stored in added_flags field).
 */
#define ROOM2_SILENCE       1
#define ROOM2_TIMESLOW      2       /* Room flag: time moves slower here */

/*
 * Exit flags.
 */
#define EX_ISDOOR           1
#define EX_CLOSED           2
#define EX_LOCKED           4
#define EX_PICKPROOF        32
#define EX_PASSPROOF        64
#define EX_SECRET           256
#define EX_TRAPPED          512     /* Genesis trap system */
#define EX_BASHED           128     /* Door was bashed open */
#define EX_BASHPROOF        256     /* Door cannot be bashed */

/* Area flags */
#define AREA_NONE           0       /* No area flags set */

/* Area flags for OLC */
#define AREA_LOADING        1       /* Area is being loaded (boot phase) */
#define AREA_CHANGED        2       /* Area has unsaved OLC changes */
/* Directions */
#define MAX_DIR             6       /* N, E, S, W, U, D */
/* File paths */
#define AREA_LIST           "../area/area.lst"
#define NOTE_FILE           "../area/notes.not"

/*
 * Sector types.
 */
#define SECT_INSIDE         0
#define SECT_CITY           1
#define SECT_FIELD          2
#define SECT_FOREST         3
#define SECT_HILLS          4
#define SECT_MOUNTAIN       5
#define SECT_WATER_SWIM     6
#define SECT_WATER_NOSWIM   7
#define SECT_AIR            9
#define SECT_DESERT         10
#define SECT_UNDERWATER     11
#define SECT_MAX            12

/*
 * Door directions.
 */
#define DIR_NORTH   0
#define DIR_EAST    1
#define DIR_SOUTH   2
#define DIR_WEST    3
#define DIR_UP      4
#define DIR_DOWN    5

/*
 * Diablerie fields (index into pcdata->diableries[]).
 */
#define DIAB_TIME   0
#define DIAB_EVER   1

/*
 * Liquids.
 */
#define LIQ_WATER   0
#define LIQ_MAX     16

/*
 * Body location damage flags (stored in loc_hp[]).
 * loc_hp[0] = head bits, loc_hp[1] = body bits,
 * loc_hp[2] = left arm bits, loc_hp[3] = right arm bits,
 * loc_hp[4] = left leg bits, loc_hp[5] = right leg bits,
 * loc_hp[6] = bleeding bits.
 */
/* Head bits (loc_hp[0]) */
#define LOST_HEAD       1
#define LOST_TONGUE     2
#define CUT_THROAT      4
#define LOST_EAR_L      8
#define LOST_EAR_R      16
#define LOST_EYE_L      32
#define LOST_EYE_R      64
#define BROKEN_JAW      128
#define BROKEN_NOSE     256
#define BROKEN_SKULL    512
#define BROKEN_NECK     1024
#define LOST_TOOTH_1    2048
#define LOST_TOOTH_2    4096
#define LOST_TOOTH_4    8192
#define LOST_TOOTH_8    16384
#define LOST_TOOTH_16   32768
/* Head bits additional */
#define LOST_NOSE       512

/* Backwards-compat aliases */
#define LOST_EAR        LOST_EAR_L
#define LOST_EYE        LOST_EYE_L

/* Body bits (loc_hp[1]) */
#define CUT_STOMACH     1
#define CUT_CHEST       2
#define BROKEN_RIBS_1   4
#define BROKEN_RIBS_2   8
#define BROKEN_RIBS_4   16
#define BROKEN_RIBS_8   32
#define BROKEN_RIBS_16  64
#define BROKEN_SPINE    128
#define SLIT_WRIST      256

/* Arm/hand bits (loc_hp[2/3]) */
#define LOST_ARM        1
#define LOST_HAND       2
#define BROKEN_ARM      4
#define LOST_FINGER_I   8
#define LOST_FINGER_L   16
#define LOST_FINGER_M   32
#define LOST_FINGER_R   64
#define LOST_THUMB      128
#define BROKEN_FINGER_I 256
#define BROKEN_FINGER_L 512
#define BROKEN_FINGER_M 1024
#define BROKEN_FINGER_R 2048
#define BROKEN_THUMB    4096

/* Leg/foot bits (loc_hp[4/5]) */
#define LOST_LEG        1
#define LOST_FOOT       2
#define BROKEN_LEG      4
#define LOST_TOE_BIG    8
#define LOST_TOE_A      16
#define LOST_TOE_B      32
#define LOST_TOE_C      64
#define LOST_TOE_D      128
#define BROKEN_TOE_BIG  256
#define BROKEN_TOE_A    512
#define BROKEN_TOE_B    1024
#define BROKEN_TOE_C    2048
#define BROKEN_TOE_D    4096

/* Bleeding bits (loc_hp[6]) */
#define BLEEDING_HEAD   1
#define BLEEDING_THROAT 2
#define BLEEDING_ARM_L  4
#define BLEEDING_ARM_R  8
#define BLEEDING_LEG_L  16
#define BLEEDING_LEG_R  32
#define BLEEDING_CHEST  64
#define BLEEDING_STOMACH 128
#define BLEEDING_HAND_L 256
#define BLEEDING_HAND_R 512
#define BLEEDING_FOOT_L 1024
#define BLEEDING_FOOT_R 2048
#define BLEEDING_WRIST_L 4096
#define BLEEDING_WRIST_R 8192

/*
 * Condition indices (pcdata->condition[]).
 */
#define COND_DRUNK      0
#define COND_FULL       1
#define COND_THIRST     2

/*
 * Lie states (pcdata->lie).
 */
#define LIE_NORMAL      0
#define LIE_TRUE        1
#define LIE_FALSE       2

/*
 * Exercise indices (pcdata->exercise[]).
 */
#define EXE_STRENGTH    0
#define EXE_DEXTERITY   1
#define EXE_CONSTITUTION 2
#define EXE_COUNT       3
#define EXE_RECOVER     4

/*
 * Language values (pcdata->language[0]).
 */
#define LANG_COMMON     0
#define LANG_DARK       1
#define DIA_OLDE        2
#define DIA_BAD         3

#define IS_SPEAKING(ch, lang)   (!IS_NPC(ch) && (ch)->pcdata->language[0] == (lang))
#define CAN_SPEAK(ch, lang)     (!IS_NPC(ch) && (ch)->pcdata->language[1] & (1 << (lang)))

/*
 * Room text type constants (ROOMTEXT_DATA->type).
 * Low bits (% RT_RETURN) define the action type;
 * high bits are flags (RT_TIMER, RT_PERSONAL, RT_RETURN, RT_ACTION).
 */
#define RT_SAY          1
#define RT_LIGHTS       2
#define RT_LIGHT        3
#define RT_DARK         4
#define RT_OBJECT       5
#define RT_MOBILE       6
#define RT_SPELL        7
#define RT_PORTAL       8
#define RT_TELEPORT     9
#define RT_OPEN_LIFT    10
#define RT_CLOSE_LIFT   11
#define RT_MOVE_LIFT    12
#define RT_RETURN       64   /* Flag: stop processing after trigger  */
#define RT_TIMER        128  /* Flag: set object timer               */
#define RT_PERSONAL     256  /* Flag: send only to triggering char   */
#define RT_ACTION       512  /* Flag: mob command action             */

/*
 * Well-known room VNUMs.
 */
#define ROOM_VNUM_IN_OBJECT     30003
#define ROOM_VNUM_DISCONNECTION 30004

/*
 * File paths.
 */
#define BUG_FILE    "../area/bugs.txt"
#define IDEA_FILE   "../area/ideas.txt"
#define TYPO_FILE   "../area/typos.txt"

/*
 * World affect bits (int world_affects in comm.c).
 */
#define WORLD_NICE      1
#define WORLD_MIST      2
#define WORLD_FOG       4
#define WORLD_ECLIPSE   8
#define WORLD_RAIN      16

/*
 * Resist flags (pcdata->resist[]).
 */
#define RES_DOMINATE    1
#define RES_TOGGLE      2

/*
 * Willpower flags (pcdata->willpower[1]).
 */
#define WILL_DOMINATE   1

/*
 * Wiznet flags.
 */
#define WIZ_LOGINS      1

/*
 * Freeze Tag / game mode flags (ch->tag_flags).
 */
#define TAG_PLAYING     1

/*
 * Miscellaneous constants.
 */
#define BLUE_MAGIC      32768   /* Magic type for blue magic spells */
#define RED_MAGIC       65536
#define GREEN_MAGIC     131072
#define YELLOW_MAGIC    262144
#define BLACK_MAGIC     524288
#define RANDOM_MAGIC    (-1)    /* sentinel: pick random magic color */
#define PURPLE_MAGIC    1048576

/*
 * Additional vampire discipline indices.
 */
#define DISC_DAIMOINON  18
#define DISC_OBEAH      19
#define DISC_VISCERATIKA 20

/*
 * Vampire affect bits (additional).
 */
#define VAM_FLYING      4096

/*
 * Demon body modification bits (pcdata->bodymods).
 */
#define MOD_WINGS       1
#define MOD_HORNS       2
#define MOD_TUSKS       4
#define MOD_TAIL        8
#define MOD_EXOSKELETON 16
#define MOD_FINS        32
#define MOD_UNFOLDED    64

/*
 * Demon special affects (ch->spectype for demons).
 */
#define DEM_WINGS       1
#define DEM_UNFOLDED    2
#define DEM_SHADOWSIGHT 4

/*
 * Werewolf form constants.
 */
#define FORM_HOMID      0
#define FORM_GLABRO     1
#define FORM_CRINOS     2
#define FORM_HISPO      3
#define FORM_LUPUS      4

/*
 * Quest item special type flags.
 */
#define QUEST_ARTIFACT      1
#define QUEST_RELIC         2
#define QUEST_IMPROVED      4
#define QUEST_BLOODAGONY    8

/*
 * Quest score indices (pcdata->qstats[]).
 */
/* QS_NN constants are stance-count indices (used as switch case values) */
#define QS_11           0   /* first superstance */
#define QS_12           1
#define QS_13           2
#define QS_14           3
#define QS_15           4   /* fifth superstance */

/* QS_ bit flags used in pcdata->qstats[] */
#define QS_HIT_LESSER   1
#define QS_HIT_GREATER  2
#define QS_HIT_SUPREME  4
#define QS_DAM_LESSER   8
#define QS_DAM_GREATER  16
#define QS_DAM_SUPREME  32
#define QS_ARM_LESSER   64
#define QS_ARM_GREATER  128
#define QS_ARM_SUPREME  256
#define QS_POW_LESSER   512
#define QS_POW_GREATER  1024
#define QS_POW_SUPREME  2048
#define QS_RES_LESSER   4096
#define QS_RES_GREATER  8192
#define QS_RES_SUPREME  16384
#define QS_SUPER_PARRY  32768
#define QS_SUPER_DODGE  65536
#define QS_SUPER_SPEED  131072
#define QS_SUPER_BYPASS 262144
#define QS_DONE         524288

/*
 * Score stat indices (pcdata->score[]).
 */
#define SCORE_NUM_QUEST 5

/*
 * Item flags (additional).
 */
#define ITEM_VANISH     134217728
#define ITEM_NOTEPAPER  268435456
#define ITEM_WGATE      536870912
#define SITEM_SILVER    1

/*
 * Container flags.
 */
#define CONT_CLOSED     1
#define CONT_LOCKED     2
#define CONT_PUTLOCK    4
#define CONT_PICKPROOF  8
#define CONT_NOPUT      16

/*
 * Ninja/Monk constants.
 */
#define NINJAKI         1
#define NPOWER_CHIKYU   1
#define HARA_KIRI       2

/*
 * Exercise indices (additional).
 */
#define EXE_TYPE        0
#define EXE_PUSHUPS     1
#define EXE_SKIPPING    2
#define EXE_LEGRAISES   3

/*
 * Position constants (additional).
 */
#define POS_MEDITATING  9

/*
 * Stance constants (additional).
 */
#define STANCE_NORMAL   0

/*
 * Freeze Tag bit flags.
 */
#define TAG_RED         2
#define TAG_BLUE        4
#define TAG_FROZEN      8

/*
 * Mage embrace states and mflags bits.
 */
#define MAGE_EMBRACED1      1
#define MAGE_EMBRACED2      2
#define MAGE_TELEKINETICS   4       /* Mage power: telekinesis */
#define MAGE_POLYAPPEAR     8       /* Mage power: polymorphic appearance */
#define MAGE_BETTERBODY     16      /* Mage power: enhanced body */
#define MFLAGS_DIMMAK       32      /* Mflags: dim mak (death touch) enabled */
#define MAGE_PERCEPTION     64      /* Mage power: enhanced perception */
#define MAGE_SHAPED         128     /* Mage power: shapeshifted (spirit) */
#define MAGE_INROOM_GATE    1       /* Ward gate obj value: in-room side */
#define MAGE_TOROOM_GATE    2       /* Ward gate obj value: to-room side */

/* Mage damage type constants (passed to mage_damage()) */
#define MAGEDAM_ELECTRIC    1
#define MAGEDAM_ICESTORM    2
#define MAGEDAM_QUINT       3
#define MAGEDAM_OTHER       4

/* Immunity level indices (ch->imms[]) */
#define IMMUNITY    0
#define RESISTANCE  1
#define VULNERABLE  2

/* Object vnum: ward gate (warlock portal) */
#define OBJ_VNUM_WGATE      100

/* Quest enchantment flag (mage item enchanting) */
#define QUEST_MAGEENCHANT   (1 << 13)
#define QUEST_SORCENCHANT   (1 << 14)  /* sorcerer enchant applied */

/* Sphere indices (shortcuts matching mage.h SPHERE_* for convenience) */
#define MENT    4   /* SPHERE_MIND */
#define MTIM    8   /* SPHERE_TIME */

/* Koe class power indices (pcdata->powers[] array indices) */
#define KOE_CURRENT     0
#define KOE_PYIN        1
#define KOE_PYANG       2
#define KOE_HUN         3
#define KOE_PO          4
#define KOE_DYIN        5
#define KOE_DYANG       6
#define KOE_BLACKWIND   7
#define KOE_BLOOD       8
#define KOE_BONE        9
#define KOE_FLESH       10
#define KOE_JADE        11
#define KOE_GHOST       12
#define KOE_DHARMA      13
#define KOE_INTERNALIZE 14
#define KOE_DEMONART    15
#define KOE_GOBLINFACE  16
#define KOE_CHIMUH      17
#define KOE_PELESIT     18
#define KOE_TAPESTRY    19
#define KOE_EQUILIBRUM  20
#define KOE_DRAGONSCALES 21
#define KOE_BONEARMOR   22
#define KOE_BONEBLADE   23
#define KOE_BLOODLASH   24
#define KOE_CORPSESKIN  25
#define KOE_TUSK        26
#define KOE_FROSTAURA   27
#define KOE_FLAMEAURA   28
#define KOE_LIGHTNINGAURA 29

/*
 * Misc binding/condition flags.
 */
#define BLINDFOLDED     1
#define TIED_UP         2
#define IN_CRIB         4

/*
 * Evil eye action types (pcdata->evileye).
 */
#define EYE_SELFACTION  1
#define EYE_ACTION      2
#define EYE_SPELL       4

/*
 * Channel for MIKT (Mikto talk).
 */
#define CHANNEL_MIKT    CHANNEL_MIKTALK

/*
 * Wear location aliases.
 */
#define WEAR_WIELD      WEAR_WEAPON

/*
 * Power set constants.
 */
#define C_POWERS        20

/*
 * Blood pool indices (ch->blood[]).
 */
#define BLOOD_POOL      0
#define BLOOD_CURRENT   1
#define BLOOD_POTENCY   2

/*
 * Mounted state flags (ch->mounted field).
 */
#define IS_RIDING   1
#define IS_MOUNT    2
#define IS_ON_FOOT  0

/*
 * affected_by2 bits (ch->affected_by2).
 */
#define AF2_GARGOYLE        1
#define AF2_PERMEATE        2       /* koe permeate state active */
#define AF2_PLACATE         4       /* koe placate state active */
#define AF2_TOTALBLIND      8       /* total blindness effect */

/* APPLY_AFF2: affect location type for affected_by2 modifications */
#define APPLY_AFF2          100

/* Special character flags (ch->special) */
#define SPC_PRINCE          1       /* character is a Kindred prince */
#define SPC_AKUMA           2       /* character is Akuma (infernal pact) */

/* Universal affect index (pcdata->stats[] slot for affect flags) */
#define UNI_AFF             2       /* stats[2] = universal affect bitfield */

/* Additional POLY form bits */
#define POLY_SHADOWDEMON    131072
#define POLY_SHADOWBERSERKER 262144
#define POLY_SHADOWMONKEY   524288
#define POLY_BLACKTORTOISE  1048576
#define POLY_WHITETIGER     2097152
#define POLY_SCARLETPHOENIX 4194304
#define POLY_HIMA           8388608
#define POLY_CHINGSHIH      16777216

/* SITEM_WOLFWEAPON — weapon is usable in wolf form */
#define SITEM_WOLFWEAPON    2

/*
 * Exit wall/ward flags (exit_data->exit_info additions).
 */
#define EX_WALL_WATER   1024
#define EX_WALL_GRANITE 2048
#define EX_WALL_WIND    4096
#define EX_WALL_FLAME   8192
#define EX_WARD_GHOULS  16384
#define EX_WARD_LUPINES 32768
#define EX_WARD_KINDRED 65536
#define EX_WARD_SPIRITS 131072

/*
 * Additional polymorph bits.
 */
#define POLY_FISH       2048

/*
 * Additional demon spectype bits.
 */
#define DEM_LEAP        8
#define DEM_MOVE        16

/*
 * Container flags (additional).
 */
#define CONT_CLOSEABLE  32

/*
 * Item type constants (additional).
 */
#define ITEM_TOOL       36
#define ITEM_SIT_SINGLE 37
#define ITEM_SIT_DOUBLE 38
#define ITEM_SIT_TRIPLE 39

/*
 * Tool sub-types.
 */
#define TOOL_PEN        1

/*
 * Furniture/sitting max constants.
 */
#define ITEM_SIT_MAX    ITEM_SIT_TRIPLE

/*
 * Werewolf totem constants.
 */
#define TOTEM_BOAR      1
#define TOTEM_SNAKE     2

/*
 * Adventure system state flags.
 */
#define ADV_STARTED         1
#define ADV_FINISHED        2
#define ADV_FAILED          4

/*
 * Adventure system apply flags (for mage ADV spell targeting).
 */
#define ADV_AREA_AFFECT     8
#define ADV_VICTIM_TARGET   16
#define ADV_OBJECT_TARGET   32
#define ADV_GLOBAL_TARGET   64
#define ADV_NEXT_PAGE       128
#define ADV_PARAMETER       256
#define ADV_SPELL_FIRST     512
#define ADV_NOT_CASTER      1024
#define ADV_NO_PLAYERS      2048
#define ADV_SECOND_VICTIM   4096
#define ADV_SECOND_OBJECT   8192
#define ADV_REVERSED        16384

/*
 * Adventure apply attribute codes (what the spell applies to).
 */
#define ADV_STR             1
#define ADV_DEX             2
#define ADV_INT             4
#define ADV_WIS             8
#define ADV_CON             16
#define ADV_MANA            32
#define ADV_HIT             64
#define ADV_MOVE            128
#define ADV_AC              256
#define ADV_HITROLL         512
#define ADV_DAMROLL         1024
#define ADV_SAVING_SPELL    2048

/*
 * Adventure message slots.
 */
#define ADV_MESSAGE_1   0
#define ADV_MESSAGE_2   1
#define ADV_MESSAGE_3   2

/*
 * Action types for OLC/roomtext actions.
 */
#define ACTION_NONE     0
#define ACTION_MOVE     1
#define ACTION_MOB      2
#define ACTION_OBJECT   3

/*
 * Ninja power constants (additional).
 */
#define NPOWER_SORA     2

/*
 * Power array sizes.
 */
#define C_POWERS        20
#define O_POWERS        20

/*
 * PLR additional bits.
 */
#define PLR_CHAMPION    16777216

/*
 * MORE additional bits.
 */
#define MORE_CHAMPION   8388608

/*
 * Quest rune constant.
 */
#define QUEST_MASTER_RUNE   16
#define QUEST_SPELLPROOF    (1 << 0)
#define OBJ_VNUM_ASHES      2

/* Object quest bitflags for naming/enchanting (kav_wiz.c / magic.c) */
#define QUEST_NAME          (1 << 9)    /* item has been named */
#define QUEST_SHORT         (1 << 10)   /* short desc customised */
#define QUEST_LONG          (1 << 11)   /* long desc customised */
#define QUEST_FREENAME      (1 << 12)   /* free renaming token */

/* qstats[] array indices */
#define QS_HIT      0
#define QS_DAM      1
#define QS_AC       2
#define QS_TOTAL    3   /* total qstat bonus index */

/* Armor color constant (additional) */
#define ARM_PURPLE  4

/* Object vnum: blank notepaper */
#define OBJ_VNUM_NOTEPAPER  77

/*
 * Quest task type constants (questmaster.c).
 */
#define QTYPE_NONE          0
#define QTYPE_KILL          1   /* Kill a specific mob vnum  */
#define QTYPE_FETCH         2   /* Return a specific obj vnum to questmaster */

#define QUEST_COOLDOWN      5   /* Ticks to wait between quests              */
#define QUEST_DURATION      10  /* Ticks before quest expires (10 min)       */
#define QUEST_KEYWORD       "questmaster"  /* Keyword NPCs must have         */

/*
 * Auction house constants and structures (auction.c).
 */
#define AUCTION_FILE        "../area/auction.dat"
#define AUCTION_DURATION    30          /* Ticks an auction runs (30 min)    */
#define AUCTION_MIN_BID     100         /* Absolute floor for opening bid    */
#define AUCTION_MAX         10          /* Max simultaneous lots             */
#define AUCTION_BID_STEP    10          /* Minimum raise over current bid    */

typedef struct auction_data AUCTION_DATA;
struct auction_data
{
    AUCTION_DATA   *next;
    OBJ_DATA       *obj;            /* item being auctioned (detached)   */
    char            seller[50];     /* name of the seller                */
    char            bidder[50];     /* name of current high bidder       */
    int32_t         min_bid;        /* opening / minimum bid in QP       */
    int32_t         cur_bid;        /* current high bid in QP            */
    int32_t         timer;          /* ticks remaining                   */
    int32_t         lot;            /* sequential lot number             */
};

/*
 * COL_SCALE(buf, ch, cur, max) — write a color-coded numeric value into buf
 * based on what percentage cur is of max (green/yellow/orange/red).
 */
#define COL_SCALE(buf, ch, cur, max)                                    \
    do {                                                                  \
        int _pct = ((max) > 0) ? ((cur) * 100 / (max)) : 0;             \
        if      (_pct >= 75) sprintf((buf), "#G%d#n", (cur));            \
        else if (_pct >= 50) sprintf((buf), "#Y%d#n", (cur));            \
        else if (_pct >= 25) sprintf((buf), "#y%d#n", (cur));            \
        else                 sprintf((buf), "#R%d#n", (cur));            \
    } while (0)

/*
 * Armor color constants.
 */
#define ARM_RED             1
#define ARM_BLACK           2
#define ARM_MIDNIGHT        3
#define ARM_BLUE            5
#define ARM_IVORY           6
#define ARM_WHITE           7
#define ARM_SILVER          8

/* Demon special power flags (ch->pcdata->powers[0]) — additional */
#define DEM_DEMONFORM       (1 << 17)  /* demon's true form active */
#define DEM_IMP             (1 << 18)  /* demon imp power unlocked */
#define DEM_LIFESPAN        (1 << 19)  /* demon extended lifespan */

/* Room vnum: Heaven */
#define ROOM_VNUM_HEAVEN    30001

/*
 * Worn/wear bit constants.
 */
#define WEAR_STAKE          128

/*
 * Special item constants.
 */
#define SITEM_HIGHLANDER    1

/*
 * Extra flag constants for interp.c/db.c.
 */
#define EXTRA_DONE          1
#define EXTRA_EXP           2
#define AREA_ADDED          1
#define DISABLED_FILE       "disabled.txt"
#define COPYOVER_FILE       "copyover.data"
#define EXE_FILE            "../src/merc"

/*
 * More bits for vampire coterie/power system.
 */
#define MORE_LEADER         (1 << 20)

/*
 * String case conversion helper macros.
 */
#define UPPER(c)            ((c) >= 'a' && (c) <= 'z' ? (c) - 'a' + 'A' : (c))
#define LOWER(c)            ((c) >= 'A' && (c) <= 'Z' ? (c) - 'A' + 'a' : (c))

/*
 * Vampire discipline power constants (indexed by discipline).
 */
#define POW_NONE            0
#define POW_ANIMALISM       1
#define POW_AUSPEX          2
#define POW_CELERITY        3
#define POW_CHIMERSTRY      4
#define POW_DAIMOINON       5
#define POW_DOMINATE        6
#define POW_FORTITUDE       7
#define POW_MELPOMINEE      8
#define POW_NECROMANCY      9
#define POW_OBEAH           10
#define POW_OBFUSCATE       11
#define POW_OBTENEBRATION   12
#define POW_POTENCE         13
#define POW_PRESENCE        14
#define POW_PROTEAN         15
#define POW_QUIETUS         16
#define POW_SERPENTIS       17
#define POW_THANATOSIS      18
#define POW_THAUMATURGY     19
#define POW_VICISSITUDE     20

/*
 * Coterie (vampire group) power level and special tracking.
 */
#define COT_BANNED          0
#define COT_BASIC           1
#define COT_INTERMEDIATE    2
#define COT_ADVANCED        3
#define COT_EXPERT          4
#define COT_HP              5
#define COT_GENERATION      6
#define COT_SKILL           7
#define COT_STANCE          8
#define COT_EXTRA           9
#define COT_MAX             10

/* Coterie extra flags */
#define COT_ADV_STANCE      1
#define COT_GOLCONDA        2
#define COT_NO_DIABLERIE    4
#define COT_NO_KILL_SECT    8   /* coterie members cannot kill sect members */
#define COT_NO_KILL_COTERIE 16  /* coterie members cannot kill each other */

/*
 * Celerity resistance constant.
 */
#define RES_CELERITY        1

/*
 * Room VNUMs (additional).
 */
#define ROOM_VNUM_TEMPLE    30006
#define ROOM_VNUM_LIMBO     2
#define ROOM_VNUM_ARTIFACTS 30020
#define ROOM_VNUM_SCHOOL    30001   /* Newbie school starting room */
#define ROOM_VNUM_CHAT      30002   /* Chat/OOC room */

/*
 * Item type constants (additional).
 * (These are item_type values, not extra_flags bits.)
 */
#define ITEM_QUESTCARD      42
#define ITEM_QUESTMACHINE   43
#define ITEM_SYMBOL         44
#define ITEM_WALL           45
#define ITEM_WARD           46
#define ITEM_KEEP           47

/*
 * Power sub-indices.
 */
#define POWER_CURRENT   0
#define POWER_TOTAL     1
#define FOCUS_CURRENT   0

/*
 * Additional discipline indices.
 */
#define DISC_MELPOMINEE  21
#define DISC_THANATOSIS  22

/*
 * Discipline-specific APPLY_ constants (start at 27, after APPLY_BLOOD_POT=26).
 */
#define APPLY_POLY          27
#define APPLY_GENERATION    28
#define APPLY_ANIMALISM     29
#define APPLY_AUSPEX        30
#define APPLY_CELERITY      31
#define APPLY_CHIMERSTRY    32
#define APPLY_DAIMOINON     33
#define APPLY_DOMINATE      34
#define APPLY_FORTITUDE     35
#define APPLY_MELPOMINEE    36
#define APPLY_NECROMANCY    37
#define APPLY_OBEAH         38
#define APPLY_OBFUSCATE     39
#define APPLY_OBTENEBRATION 40
#define APPLY_POTENCE       41
#define APPLY_PRESENCE      42
#define APPLY_PROTEAN       43
#define APPLY_QUIETUS       44
#define APPLY_SERPENTIS     45
#define APPLY_THANATOSIS    46
#define APPLY_THAUMATURGY   47
#define APPLY_VICISSITUDE   48

/*
 * Apply set constants — set-discipline temporary boosts.
 * Values must not conflict with APPLY_NONE..APPLY_VICISSITUDE (0-48).
 */
#define APPLY_SET_ANIMALISM     49
#define APPLY_SET_AUSPEX        50
#define APPLY_SET_CELERITY      51
#define APPLY_SET_CHIMERSTRY    52
#define APPLY_SET_DAIMOINON     53
#define APPLY_SET_DOMINATE      54
#define APPLY_SET_FORTITUDE     55
#define APPLY_SET_MELPOMINEE    56
#define APPLY_SET_NECROMANCY    57
#define APPLY_SET_OBEAH         58
#define APPLY_SET_OBFUSCATE     59
#define APPLY_SET_OBTENEBRATION 60
#define APPLY_SET_POTENCE       61
#define APPLY_SET_PRESENCE      62
#define APPLY_SET_PROTEAN       63
#define APPLY_SET_QUIETUS       64
#define APPLY_SET_SERPENTIS     65
#define APPLY_SET_THANATOSIS    66
#define APPLY_SET_THAUMATURGY   67
#define APPLY_SET_VICISSITUDE   68

/*
 * Vampire affect bits (additional).
 */
#define VAM_SPIRITEYES  8192

/*
 * Polymorph bits (additional).
 */
#define POLY_SHADOW     2048
#define POLY_TRUE       4096
#define POLY_MASK       8192
#define POLY_FLESHCRAFT 16384
#define POLY_BONECRAFT  32768
#define POLY_HAGS       65536
#define POLY_CHANGELING 131072

/*
 * MORE bits (additional).
 */
#define MORE_ASHES      (int)268435456
#define MORE_MUMMIFIED  (int)134217728
#define MORE_PLASMA     (int)67108864
#define MORE_PROJECTION (int)33554432

/*
 * EXTRA bits (additional).
 */
#define EXTRA_NO_ACTION EXTRA_DEAD

/*
 * Spell target types.
 */
#define TAR_IGNORE          0
#define TAR_CHAR_OFFENSIVE  1
#define TAR_CHAR_DEFENSIVE  2
#define TAR_CHAR_SELF       3
#define TAR_DEATH           4

/*
 * Special item power type flags (obj->spectype).
 */
#define SITEM_ACTIVATE      1
#define SITEM_PRESS         2
#define SITEM_PULL          4
#define SITEM_TWIST         8
#define SITEM_TARGET        16
#define SITEM_SPELL         32
#define SITEM_DELAY1        64
#define SITEM_DELAY2        128
#define SITEM_TRANSPORTER   256
#define SITEM_TELEPORTER    512
#define SITEM_OBJECT        1024
#define SITEM_MOBILE        2048
#define SITEM_ACTION        4096
#define SITEM_DEMONIC       8192

/*
 * Weapon damage type (undefined for non-weapons).
 */
#define TYPE_UNDEFINED  -1

/*
 * Trade shop constants.
 */
#define MAX_TRADE       5

/*
 * Room flags (additional).
 */
#define ROOM_NO_TELEPORT    ROOM_NOPORTAL

/*
 * Score/stat indices (additional).
 */
#define SCORE_QUEST     0

/*
 * Wear location constants (additional).
 */
#define WEAR_SCABBARD_L     23
#define WEAR_SCABBARD_R     24

/*
 * Connection state constants (additional).
 * Note: CON_NOTE_* are also defined in board.h (values 10-14).
 * CON_GET_NEW_ANSI MUST NOT use 10-14 (those are CON_NOTE_* from board.h).
 */
/* CON_NOTE_TO=10 through CON_NOTE_FINISH=14 are defined in board.h */
#define CON_GET_INVIS           15
#define CON_COPYOVER_RECOVER    16
#define CON_GET_NEW_ANSI        17  /* ANSI/color prompt during login */
/* OLC editor states */
#define CON_AEDITOR             18  /* Area editor */
#define CON_REDITOR             19  /* Room editor */
#define CON_OEDITOR             20  /* Object editor */
#define CON_MEDITOR             21  /* Mobile editor */

/*
 * Wiznet flag bits.
 */
#define WIZ_ON          1
#define WIZ_PREFIX      2
#define WIZ_QUEST       4
/* WIZ_LOGINS is defined near wiznet flags above as 1 (alias for WIZ_ON) */
#define WIZ_GIVE        16
#define WIZ_SPAM        32

/*
 * Quest stat indices (pcdata->qstats[]).
 */
#define QUEST_STR       0
#define QUEST_DEX       1
#define QUEST_INT       2
#define QUEST_WIS       3
#define QUEST_CON       4
#define QUEST_HIT       5
#define QUEST_MANA      6
#define QUEST_MOVE      7
#define QUEST_HITROLL   8
#define QUEST_DAMROLL   9
#define QUEST_AC        10
#define QUEST_SONG      11
#define QUEST_VANISH    12

/*
 * File paths.
 */
#define SHUTDOWN_FILE   "../area/shutdown.txt"
#define FILE_PLAYER     1   /* Integer code for player file type (path = PLAYER_DIR) */

/*
 * EXTRA bits (additional).
 */
#define EXTRA_SWITCH    EXTRA_OSWITCH
#define EXTRA_TRUSTED   EXTRA_PRINCE
#define EXTRA_CALL_ALL  EXTRA_ILLUSION
#define EXTRA_NON_NEWBIE EXTRA_AFK
#define EXTRA_DYING     EXTRA_DEAD     /* Alias: character is in dying state */
#define EXTRA_LABOUR    EXTRA_BOUND    /* Alias: character is in labour */

/*
 * PLR bits (additional).
 */
#define PLR_LOG         PLR_WATCHER
#define PLR_LAST_AGG    PLR_WOLFMAN    /* Alias: last agressor flag */

/*
 * Quest state flags (pcdata->quest[] array or similar).
 */
#define QUEST_FOLLOWING 1   /* Quest: following a target */

/*
 * Score indices (pcdata->score[]).
 */
#define SCORE_HIGH_LEVEL    6
#define SCORE_HIGH_XP       7
#define SCORE_TOTAL_LEVEL   8
#define SCORE_TOTAL_XP      9

/*
 * Attack type constants.
 */
#define TYPE_ADD_AGG    1000    /* Offset to convert skill# to aggressor attack type */

/*
 * Demon special power bits (additional).
 */
#define DEM_INFERNO     256     /* Demon power: Inferno (hellfire) */

/*
 * File paths (additional).
 */
#define IDEA_FILE       "../area/ideas.txt"
#define DECAP_FILE      "../area/decaps.txt"

/*
 * Lost body-part bits (additional).
 */
#define LOST_EAR_L      8
#define LOST_ENTRAILS   (1 << 15)   /* Entrails ripped out */

/*
 * MORE bits (additional).
 */
#define MORE_STAKED     MORE_STAKE

/*
 * Room VNUMs (additional).
 */
#define ROOM_VNUM_ALTAR     30021

/*
 * Item types (additional).
 */
#define ITEM_AMMO       40
#define ITEM_THROWN     41

/*
 * SITEM additional flags.
 */
#define SITEM_MORPH     16384
#define SITEM_MESSAGE   32768

/*
 * GSN declarations — global skill numbers, defined in db.c.
 */
extern int gsn_backstab;
extern int gsn_berserk;
extern int gsn_blindness;
extern int gsn_charm_person;
extern int gsn_circle;
extern int gsn_claws;
extern int gsn_curse;
extern int gsn_dark_poison;
extern int gsn_darkness;
extern int gsn_demon;
extern int gsn_disarm;
extern int gsn_elbow;
extern int gsn_fastdraw;
extern int gsn_fear;
extern int gsn_firstaid;
extern int gsn_fury;
extern int gsn_headbutt;
extern int gsn_hide;
extern int gsn_hurl;
extern int gsn_invis;
extern int gsn_jail;
extern int gsn_jumpkick;
extern int gsn_kick;
extern int gsn_knee;
extern int gsn_lightningkick;
extern int gsn_mass_invis;
extern int gsn_monksweep;
extern int gsn_palmstrike;
extern int gsn_paradox;
extern int gsn_peek;
extern int gsn_pick_lock;
extern int gsn_poison;
extern int gsn_polymorph;
extern int gsn_potency;
extern int gsn_punch;
extern int gsn_repair;
extern int gsn_rescue;
extern int gsn_rot;
extern int gsn_shinkick;
extern int gsn_shiroken;
extern int gsn_sleep;
extern int gsn_sneak;
extern int gsn_spines;
extern int gsn_spinkick;
extern int gsn_spit;
extern int gsn_steal;
extern int gsn_sweep;
extern int gsn_tail;
extern int gsn_tendrils;
extern int gsn_thrustkick;
extern int gsn_tongue;
extern int gsn_tornadokick;
extern int gsn_track;
extern int gsn_wattack;
extern int gsn_web;
extern int gsn_backfist;

/*
 * Stat indices.
 */
#define STAT_STR    0
#define STAT_INT    1
#define STAT_WIS    2
#define STAT_DEX    3
#define STAT_CON    4

/*
 * Power array indices.
 */
#define C_CURRENT   0

/*
 * Additional demon special type bits.
 */
#define DEM_FANGS   32
#define DEM_CLAWS   64
#define DEM_EYES    128
#define DEM_EYESPY  256
#define DEM_HOOVES  512
#define DEM_SPEED   1024
#define DEM_TOUGH   2048
#define DEM_MIGHT   4096
#define DEM_TRAVEL  8192
#define DEM_SCRY    16384
#define DEM_FIREWALL 32768

/*
 * Vampire affect bits (additional).
 */
#define VAM_SONIC   16384

/*
 * POLY bits (additional).
 */
#define POLY_SPIRIT     262144

/*
 * MORE bits (additional).
 */
#define MORE_ANTITRIBU  134217728
#define MORE_ANGER      67108864
#define MORE_CALM       33554432
#define MORE_COURAGE    16777216
#define MORE_DEFLECTION 8388608
#define MORE_NPC        4194304

/*
 * EXTRA bits (additional).
 */
#define EXTRA_LABOUR    EXTRA_BOUND

/*
 * PLR bits (additional).
 */
#define PLR_WEREWOLF    PLR_WOLFMAN

/*
 * Resist flags (pcdata->resist[] - additional).
 */
#define RES_ANIMALISM   4
#define RES_AUSPEX      8
#define RES_PRESENCE    16

/*
 * Willpower flags (pcdata->willpower[1] - additional).
 */
#define WILL_ANIMALISM  2
#define WILL_AUSPEX     4
#define WILL_PRESENCE   8

/*
 * Location/body part constants (aliases).
 */
#define LOC_HEAD    0
#define LOC_BODY    1
#define LOC_ARM_L   2
#define LOC_ARM_R   3
#define LOC_LEG_L   4
#define LOC_LEG_R   5

/* AWARE — combat awareness spectype flag AND monk awareness-path array index */
#define AWARE       1

/*
 * Zombie NPC AI state constants (ch->spectype for zombies).
 */
#define ZOMBIE_NOTHING      0
#define ZOMBIE_REST         1
#define ZOMBIE_TRACKING     2
#define ZOMBIE_ANIMATE      3
#define ZOMBIE_CAST         4

/* SPIRIT — monk spirit-path array index (IS_SPIRIT() uses MORE_SPIRIT directly) */
#define SPIRIT      2

/*
 * WAS_STAKED constant.
 */
#define WAS_STAKED  MORE_STAKE

/*
 * Werewolf/Ninja constants.
 */
#define NEW_MONKSKIN    1
#define NEW_MONKFLAME   2   /* monk: flame aura */
#define NEW_MONKADAM    4   /* monk: adamantine skin */
#define NEW_POWER       8   /* monk: power-up newbit */
#define NPOWER_NINGENNO 3

/* Monk power index in powers[] */
#define PMONK   20          /* index into ch->pcdata->powers[] for monk level */

/* Focus / chi array indices (ch->focus[], ch->chi[]) */
#define CURRENT 0           /* focus[CURRENT] / chi[CURRENT] = current points */
#define MAXFOC  1           /* focus[MAXFOC]  = max focus */
/* MAXIMUM=1 already defined below for chi */

/*
 * Monk fighting-system technique bits (ch->monkstuff — IS_SET bitfield).
 */
#define TECH_KNEE       1
#define TECH_SWEEP      2
#define TECH_ELBOW      4
#define TECH_THRUST     8
#define TECH_SPIN       16
#define TECH_BACK       32
#define TECH_PALM       64
#define TECH_SHIN       128

/* Free-stance sub-technique bits (monkstuff, higher bits) */
#define FS_TRIP         256
#define FS_KICK         512
#define FS_BASH         1024
#define FS_ELBOW        2048
#define FS_KNEE         4096
#define FS_DISARM       8192
#define FS_BITE         16384
#define FS_DIRT         32768
#define FS_GRAPPLE      65536
#define FS_PUNCH        131072
#define FS_RIP          262144
#define FS_STAMP        524288
#define FS_BACKFIST     1048576
#define FS_JUMPKICK     2097152
#define FS_SPINKICK     4194304
#define FS_CHARGE       8388608
#define FS_HURL         16777216
#define FS_GOUGE        33554432
#define FS_HEADBUTT     67108864
#define FS_SWEEP        134217728

/*
 * Monk combo-step bits (ch->monkcrap — IS_SET bitfield).
 */
#define COMB_SHIN       1
#define COMB_KNEE       2
#define COMB_THRUST1    4
#define COMB_THRUST2    8
#define TOTEM_SPIDER    3
#define TOTEM_OWL       4   /* werewolf totem: owl */
#define TOTEM_BEAR      5   /* werewolf totem: bear */

/* Array index constants shared across multiple struct arrays:
 *   quint[4]:  BODY=0, AVATAR=1, PRIME=2, MTOTAL=3
 *   monkab[4]: BODY=0, AWARE=1, SPIRIT=2, COMBAT=3
 *   chi[2]:    CURRENT=0, MAXIMUM=1
 *   focus[2]:  CURRENT=0, MAXIMUM=1
 */
#define BODY    0
#define AVATAR  1
#define PRIME   2
#define MTOTAL  3   /* quint[3] = total mage quintessence */
#define COMBAT  3   /* monkab[3] = combat monk path (shares value with MTOTAL; different arrays) */
/* AWARE=1, SPIRIT=2 are already defined above */

/* chi[]/focus[] */
#define MAXIMUM 1

/* Room type flag: deathtrap */
#define ROOM_DEATHTRAP  (1 << 20)

/* Object vnum: empty dragon egg */
#define OBJ_VNUM_EMPTY_EGG  55

/* Missing colour constant */
#define GREY            7   /* terminal grey / dark-white colour index */

/* Missing demon power flag */
#define DEM_MAGIC       65536   /* demon special power: magic resistance */

/* Object vnums (additional) */
#define OBJ_VNUM_BLOOD_SPRING   77  /* blood-pool spring object */

/* Quest-related object flags (magic enchanting) */
#define QUEST_ENCHANTED     256     /* item has been magically enchanted */
#define QUEST_AGGRAVATED    512     /* item deals aggravated damage */

/* Extra character flags (additional) */
#define EXTRA_SUMMON        EXTRA_AFK      /* char can be summoned (alias) */
#define EXTRA_MARK          (1 << 25)      /* char is marked for tracking */

/* IS_XTRA — test a bit in ch->xtra */
#define IS_XTRA(ch, bit)    (IS_SET((ch)->xtra, (bit)))

/* Xtra bits (ch->xtra bitfield) */
#define XTRA_HEARTS_BLOOD   (1 << 0)   /* mage: hearts blood active */
#define XTRA_TENEBROUS      (1 << 10)  /* vampire: tenebrous form */
#define XTRA_ATONING        (1 << 11)  /* elemental: atoning winds */
#define XTRA_LIQUIFY        (1 << 12)  /* elemental: liquify form */
#define XTRA_SLOW           (1 << 13)  /* elemental: slow effect */
#define XTRA_DEEPUMBRA      (1 << 14)  /* mage: in the deep umbra */
#define XTRA_MAGE_CARESS    (1 << 15)  /* mage: spirit caress active */
#define XTRA_HOLY_SMOTE     (1 << 16)  /* mage: holy smote active */
#define XTRA_MUTATE_EPHEMERA (1 << 17) /* mage: mutate ephemera active */
#define XTRA_BETTER_STR     (1 << 18)  /* mage: better body STR */
#define XTRA_BETTER_DEX     (1 << 19)  /* mage: better body DEX */
#define XTRA_BETTER_CON     (1 << 20)  /* mage: better body CON */
#define XTRA_BETTER_INT     (1 << 21)  /* mage: better body INT */
#define XTRA_BETTER_WIS     (1 << 22)  /* mage: better body WIS */

/* Poly bits for polyaff field (higher range to avoid conflicts) */
#define POLY_CHIMERA        (1 << 25)  /* monk chimera polymorph */
#define POLY_PIG            (1 << 26)  /* mage pig polymorph */

/* Mage craft bits (stub — malleable is a bodycraft ability) */
#define IS_CRAFT(ch, bit)   (0)
#define CRAFT_MALLEABLE     1

/* Level constants */
#define LEV_SLE             (LEVEL_MORTAL)  /* sleepwalker: mortal-level witness */
#define LEV_AWA             (LEVEL_MORTAL)  /* awakened: minimum level after wipe */
#define LEV_IMP             (LEVEL_IMPLEMENTOR) /* implementor level threshold */

/* Ninja array indices for pcdata->ninja[5] */
#define NINJA_KI            0
#define NINJA_CHIKYU        1   /* earth element */
#define NINJA_NINGENNO      2   /* human relation / ninjutsu */
#define NINJA_TAIJUTSU      3
#define NINJA_BUJUTSU       4

/* Accessor macros for class-specific skill arrays */
#define get_ninja(ch, idx)    (!IS_NPC(ch) ? (ch)->pcdata->ninja[(idx)] : 0)
#define get_highland(ch, idx) (!IS_NPC(ch) ? (ch)->pcdata->highland[(idx)] : 0)

/* Player backup directory */
#define BACKUP_DIR          "../backup/"

/* pcdata->highland[4] array indices */
#define HIGH_ATTACK         0
#define HIGH_DEFENSE        1
#define HIGH_WEAPON         2
#define HIGH_ABILITY        3

/* pcdata->flag bitfield constants (Genesis sorcerer / player flags) */
#define IS_FLAG(ch, bit)    (!IS_NPC(ch) && IS_SET((ch)->pcdata->flag, (bit)))
#define FLAG_RP             1
#define FLAG_PK             2
#define FLAG_TEST           4
#define FLAG_AFK            8
#define FLAG_PIMP           16
#define FLAG_KINKY          32
#define FLAG_CCC            64
#define FLAG_LAZY           128

/* ITEMA_ bits (additional) */
#define ITEMA_FLAMING       (1 << 25)  /* weapon aflame */
#define ITEMA_AQUABARRIER   (1 << 26)  /* item has aqua barrier */
#define ITEMA_AURA          (1 << 27)  /* item has elemental aura */

/* POLY form bits (additional) */
#define POLY_PHOENIXFORM    (1 << 19)  /* elemental phoenix form */

/*
 * Spell target types (additional).
 */
#define TAR_OBJ_INV     5

/*
 * Room VNUMs (additional).
 */
#define ROOM_VNUM_HELL  30050
#define ROOM_VNUM_BANK      3700    /* bank room for account transactions */
#define ROOM_VNUM_FREEZE    30003   /* freeze-tag game staging room */
#define GAME_ON             TAG_PLAYING  /* freeze-tag: player is in active game */
#define ABS_MOB_VNUM    30000

/* absorb[] array indices (pcdata->absorb[6]) */
#define ABS_MOB_HP      4   /* absorb[4] = stored mob HP for possession */
#define ABS_MOB_MAX_HP  5   /* absorb[5] = stored mob max HP */

/*
 * Mob VNUMs (additional).
 */
#define MOB_VNUM_HOUND  30020

/*
 * File paths (additional).
 */
#define NULL_FILE   "/dev/null"
#define PLAYER_DIR  "../player/"
#define HOME_AREA   "../area/home.dat"  /* player home data file */

/*
 * Macro for minimum mob VNUM.
 */
#define MMIN        1

/*
 * =========================================================================
 * DATA STRUCTURES
 * =========================================================================
 */

struct liq_type
{
    char *  liq_name;
    char *  liq_color;
    int     liq_affect[3];
};

struct extra_descr_data
{
    EXTRA_DESCR_DATA   *next;
    char               *keyword;
    char               *description;
};

struct affect_data
{
    AFFECT_DATA *next;
    int16_t      type;
    int16_t      duration;
    int16_t      location;
    int32_t      modifier;
    int32_t      bitvector;
};

struct obj_index_data
{
    OBJ_INDEX_DATA     *next;
    EXTRA_DESCR_DATA   *extra_descr;
    AFFECT_DATA        *affected;
    AREA_DATA          *area;
    PROG_LIST          *progs;          /* ObjProg program list */
    int32_t             progtypes;      /* Bitmask of trigger types present */
    char               *name;
    char               *short_descr;
    char               *description;
    char               *chpoweron;
    char               *chpoweroff;
    char               *chpoweruse;
    char               *victpoweron;
    char               *victpoweroff;
    char               *victpoweruse;
    char               *actcommands;
    int32_t             vnum;
    int32_t             item_type;
    int32_t             extra_flags;
    int32_t             wear_flags;
    int32_t             count;
    int32_t             weight;
    int32_t             spectype;
    int32_t             specpower;
    int32_t             condition;
    int32_t             toughness;
    int32_t             resistance;
    int32_t             quest;
    int32_t             points;
    int32_t             cost;
    int32_t             value[4];
};

struct obj_data
{
    OBJ_DATA           *next;
    OBJ_DATA           *next_content;
    OBJ_DATA           *contains;
    OBJ_DATA           *in_obj;
    CHAR_DATA          *carried_by;
    CHAR_DATA          *chobj;
    EXTRA_DESCR_DATA   *extra_descr;
    AFFECT_DATA        *affected;
    OBJ_INDEX_DATA     *pIndexData;
    ROOM_INDEX_DATA    *in_room;
    ROOM_INDEX_DATA    *to_room;
    char               *name;
    char               *short_descr;
    char               *description;
    char               *chpoweron;
    char               *chpoweroff;
    char               *chpoweruse;
    char               *victpoweron;
    char               *victpoweroff;
    char               *victpoweruse;
    char               *actcommands;
    char               *questmaker;
    char               *questowner;
    int32_t             item_type;
    int32_t             extra_flags;
    int32_t             wear_flags;
    int32_t             wear_loc;
    int32_t             weight;
    int32_t             spectype;
    int32_t             specpower;
    int32_t             toughness;
    int32_t             quest;
    int32_t             points;
    int32_t             cost;
    int32_t             temp_timer;
    int32_t             level;
    int32_t             timer;
    int32_t             condition;      /* Object condition (0-100) */
    int32_t             resistance;     /* Damage resistance of object */
    int32_t             sitnum;         /* Max characters that can sit on this */
    CHAR_DATA          *sat;            /* First character sitting on this obj  */
    int32_t             value[4];
};

struct exit_data
{
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA       *next;
    TRAP_DATA       *trap;          /* Genesis: trap attached to exit */
    int32_t          vnum;
    int32_t          exit_info;
    int32_t          key;
    char            *keyword;
    char            *description;
    int32_t          rs_flags;
};

typedef struct roomtext_data
{
    int32_t                   type;
    int32_t                   power;
    int32_t                   mob;
    char                     *input;
    char                     *output;
    char                     *choutput;
    char                     *name;
    struct roomtext_data     *next;
} ROOMTEXT_DATA;

struct reset_data
{
    RESET_DATA *next;
    char        command;
    int32_t     arg1;
    int32_t     arg2;
    int32_t     arg3;
};

struct kill_data
{
    int16_t     number;
    int16_t     killed;
};

struct area_data
{
    AREA_DATA  *next;
    RESET_DATA *reset_first;
    RESET_DATA *reset_last;
    char       *filename;
    char       *name;
    int32_t     age;
    int32_t     nplayer;
    int32_t     lvnum;
    int32_t     uvnum;
    bool        empty;
    char       *builders;   /* OLC */
    int32_t     security;   /* OLC */
    int32_t     vnum;       /* OLC */
    int32_t     area_flags; /* OLC */
};

struct room_index_data
{
    ROOM_INDEX_DATA    *next;
    CHAR_DATA          *people;
    OBJ_DATA           *contents;
    EXTRA_DESCR_DATA   *extra_descr;
    ROOMTEXT_DATA      *roomtext;
    AREA_DATA          *area;
    EXIT_DATA          *exit[6];
    PROG_LIST          *progs;          /* RoomProg program list */
    int32_t             progtypes;      /* Bitmask of trigger types present */
    char               *name;
    char               *description;
    char               *owner;
    int32_t             vnum;
    int32_t             room_flags;
    int32_t             added_flags;    /* Secondary room flags (ROOM2_*) */
    int32_t             blood;          /* Blood pool in room */
    int32_t             light;
    int32_t             sector_type;
    int32_t             tunnel;
    char               *track[5];       /* Names of tracked characters (ring buffer) */
    int32_t             track_dir[5];   /* Directions of tracked characters          */
    OBJ_DATA           *to_obj;        /* Object this room is inside (if any)       */
    int32_t             passed;         /* Pathfinding / visited flag               */
    TRAP_DATA          *trap;           /* Genesis: trap attached to room           */
};

struct shop_data
{
    SHOP_DATA  *next;
    int32_t     keeper;
    int32_t     buy_type[5];
    int32_t     profit_buy;
    int32_t     profit_sell;
    int32_t     open_hour;
    int32_t     close_hour;
};

struct mob_index_data
{
    MOB_INDEX_DATA     *next;
    SPEC_FUN           *spec_fun;
    SHOP_DATA          *pShop;
    PROG_LIST          *progs;          /* MobProg program list */
    int32_t             progtypes;      /* Bitmask of trigger types present */
    char               *player_name;
    char               *short_descr;
    char               *long_descr;
    char               *description;
    int32_t             vnum;
    int32_t             count;
    int32_t             killed;
    int32_t             sex;
    int32_t             level;
    int32_t             act;
    int32_t             affected_by;
    int32_t             alignment;
    int32_t             hitroll;
    int32_t             ac;
    int32_t             hitnodice;
    int32_t             hitsizedice;
    int32_t             hitplus;
    int32_t             damnodice;
    int32_t             damsizedice;
    int32_t             damplus;
    int32_t             gold;
    int32_t             exp;
    int32_t             position;
    int32_t             default_pos;
    AREA_DATA          *area;           /* Area this mob belongs to (OLC)      */
    int32_t             itemaffect;     /* Item affect flags                   */
    char               *spec_name;     /* Special function name string (OLC)  */
};

/*
 * Genesis TRAP_DATA structure.
 */
struct trap_data
{
    TRAP_DATA  *next;
    int32_t     type;       /* TRAP_* flag bits */
    int32_t     level;
    int32_t     charges;
    int32_t     trigger;
    int32_t     value;
    int32_t     diff;       /* dodge difficulty check */
    int32_t     numhits;    /* damage: number of hits */
    int32_t     lodam;      /* damage: low damage */
    int32_t     hidam;      /* damage: high damage */
    int32_t     spellsn;    /* spell skill number to apply */
    int32_t     movevnum;   /* teleport: destination room vnum */
    int32_t     summonvnum; /* summon: mob vnum to summon */
    char       *name;
    char       *charmess;   /* message shown to trapped char */
    char       *roommess;   /* message shown to room */
    char       *dammess;    /* damage message prefix */
};

/* TRAP_DATA flags (trap->type bitmask) */
#define IS_TRAP(trap, bit)  (IS_SET((trap)->type, (bit)))
#define TRAP_SILENT         1   /* no avoidance message */
#define TRAP_ALARM          2   /* send alarm to info channel */
#define TRAP_AREA           4   /* affects all in room, not just triggerer */

/*
 * Time & weather.
 */
struct time_info_data
{
    int32_t hour;
    int32_t day;
    int32_t month;
    int32_t year;
};

struct weather_data
{
    int32_t mmhg;
    int32_t change;
    int32_t sky;
    int32_t sunlight;
};

/*
 * Leaderboard (Freeze Tag / score).
 */
struct l_board
{
    char    first_place[64];
    char    second_place[64];
    char    third_place[64];
    int32_t score_1;
    int32_t score_2;
    int32_t score_3;

    /* Extended leaderboard categories */
    char   *pk_name;
    int32_t pk_number;
    char   *pd_name;
    int32_t pd_number;
    char   *mk_name;
    int32_t mk_number;
    char   *md_name;
    int32_t md_number;
    char   *tt_name;
    int32_t tt_number;
    char   *qc_name;
    int32_t qc_number;
    char   *bestpk_name;
    int32_t bestpk_number;
    char   *worstpk_name;
    int32_t worstpk_number;
};

/*
 * Double-experience data.
 */
struct dxp_data
{
    int32_t flag;
    int32_t dxp;    /* Double-XP mode (0=off, 1=double, 2=half) */
    int32_t timer;
    int32_t time;   /* Alias for timer (legacy field name) */
};

/*
 * Automap.
 */
typedef struct map_type
{
    int32_t  vnum;
    int32_t  depth;
    int32_t  info;      /* room_flags or exit_info for this cell */
    bool     can_see;   /* whether the cell is visible */
    char     tegn;
} MAP_TYPE;

/*
 * Board/note data.
 */
struct note_data
{
    NOTE_DATA  *next;
    char       *sender;
    char       *date;
    char       *to_list;
    char       *subject;
    char       *text;
    int32_t     date_stamp;
    int32_t     expire;     /* Expiration date as time_t */
};

/* BOARD_DATA is fully defined in board.h, included at the bottom. */
/* Forward-declare only so PC_DATA can hold a BOARD_DATA pointer.  */
typedef struct board_data BOARD_DATA;

/*
 * =========================================================================
 * CHAR_DATA  — the central character structure.
 * Merges EM fields with Genesis additions (style[], dragskills, elem_type).
 * =========================================================================
 */
struct char_data
{
    /* Linked-list links */
    CHAR_DATA          *next;
    CHAR_DATA          *next_in_room;
    CHAR_DATA          *next_fight;
    CHAR_DATA          *prev_fight;

    /* Relationship pointers */
    CHAR_DATA          *master;
    CHAR_DATA          *leader;
    CHAR_DATA          *fighting;
    CHAR_DATA          *reply;
    CHAR_DATA          *mount;
    CHAR_DATA          *wizard;
    CHAR_DATA          *embrace;

    OBJ_DATA           *sat;
    SPEC_FUN           *spec_fun;
    MOB_INDEX_DATA     *pIndexData;
    DESCRIPTOR_DATA    *desc;
    AFFECT_DATA        *affected;
    OBJ_DATA           *carrying;
    ROOM_INDEX_DATA    *in_room;
    ROOM_INDEX_DATA    *was_in_room;
    PC_DATA            *pcdata;

    /* String fields */
    char               *hunting;
    char               *name;
    char               *short_descr;
    char               *long_descr;
    char               *description;
    char               *lord;
    char               *madness;
    char               *clan;
    char               *side;
    char               *morph;
    char               *createtime;
    char               *lasttime;
    char               *lasthost;
    char               *poweraction;
    char               *powertype;
    char               *pload;
    char               *prompt;
    char               *cprompt;
    char               *contingency;    /* Genesis mage contingency */

    /* Integer attributes */
    int32_t             sex;
    int32_t             race;
    int32_t             class;
    int32_t             immune;
    int32_t             polyaff;
    int32_t             vampaff;
    int32_t             uniaff;
    int32_t             itemaffect;
    int32_t             vamppass;
    int32_t             form;
    int32_t             body;
    int32_t             vampgen;
    int32_t             spectype;
    int32_t             specpower;

    /* Combat location HP */
    int32_t             loc_hp[7];

    /* Weapon, spell, combat, stance arrays */
    int32_t             wpn[13];
    int32_t             spl[6];
    int32_t             cmbt[8];
    int32_t             stance[16];

    /* Genesis: weapon style tracking */
    int32_t             style[7];

    int32_t             blood[3];
    int32_t             beast;
    int32_t             truebeast;
    int32_t             mounted;
    int32_t             embraced;
    int32_t             home;
    int32_t             init;
    int32_t             level;
    int32_t             trust;
    int32_t             played;

    time_t              logon;
    time_t              save_time;

    int32_t             timer;
    int32_t             wait;
    int32_t             pkill;
    int32_t             pdeath;
    int32_t             mkill;
    int32_t             mprog_delay;    /* MobProg delay counter        */
    CHAR_DATA          *mprog_target;   /* MobProg delayed target       */
    int32_t             mdeath;
    int32_t             agg;
    int32_t             hit;
    int32_t             max_hit;
    int32_t             mana;
    int32_t             max_mana;
    int32_t             move;
    int32_t             max_move;
    int32_t             gold;
    int32_t             exp;
    int32_t             act;
    int32_t             extra;
    int32_t             extra2;         /* Overflow extra bits (EXTRA_BORN, etc.) */
    int32_t             more;
    int32_t             special;
    int32_t             xtra;           /* Genesis extra bits */
    int32_t             affected_by;
    int32_t             affected_by2;

    long                wiznet;

    int32_t             position;
    int32_t             practice;
    int32_t             carry_weight;
    int32_t             carry_number;
    int32_t             saving_throw;
    int32_t             alignment;
    int32_t             hitroll;
    int32_t             damroll;
    int32_t             armor;
    int32_t             wimpy;
    int32_t             deaf;

    int32_t             paradox[3];
    int32_t             damcap[2];

    /* Monk fields */
    int32_t             monkstuff;
    int32_t             monkcrap;
    int32_t             monkab[4];
    int32_t             chi[2];

    /* Mage fields */
    int32_t             flag2;
    int32_t             focus[2];
    int32_t             spheres[9];
    int32_t             quint[4];           /* BODY, AVATAR, PRIME, MTOTAL */

    /* Misc fields */
    int32_t             newbits;
    int32_t             fight_timer;
    int32_t             death_timer;
    int32_t             save_timer;
    int32_t             shadowtimer;
    int32_t             enrage_timer;       /* berserk/enrage countdown */
    int32_t             liquify_timer;      /* genesis liquify effect */
    int32_t             atoningwinds_timer; /* genesis atoning winds */
    int32_t             slow_timer;         /* genesis slow effect */
    int32_t             better_body_timer;  /* genesis better body */
    int32_t             ephemera_timer;     /* genesis ephemeral state */
    int32_t             polymorph_timer;    /* mage polymorph countdown */
    int32_t             doublecast_timer;   /* mage double-cast cooldown */
    int32_t             beginners_luck;     /* mage beginners luck active */
    int32_t             goblinspark;
    int32_t             imms[3];
    int32_t             mflags;

    /* Genesis: dragon skills bitfield */
    int32_t             dragskills;

    /* Genesis: elemental type */
    int32_t             elem_type;

    /* Freeze Tag */
    long                tag_flags;
    int32_t             bank;

    /* Genesis: primal stat */
    int32_t             primal;

    /* Genesis: mage timers */
    int32_t             arete;
    int32_t             paradox_ward;
};

/* Compatibility alias: stance.c uses ch->weapon[] while the field is wpn[] */
#define weapon wpn


/*
 * =========================================================================
 * PC_DATA — data only player characters possess.
 * Fully merged from EM and Genesis.
 * =========================================================================
 */
struct pc_data
{
    PC_DATA        *next;
    BUFFER         *buffer;             /* Genesis pager */
    CHAR_DATA      *familiar;
    CHAR_DATA      *partner;
    CHAR_DATA      *propose;
    OBJ_DATA       *chobj;
    OBJ_DATA       *memorised;
    BOARD_DATA     *board;
    time_t          last_note[MAX_BOARD];
    NOTE_DATA      *in_progress;

    /* Authentication / identity */
    char           *pwd;
    char           *cfat;
    char           *bamfin;
    char           *bamfout;
    char           *title;
    char           *pretit;             /* pretitle prefix shown before name */
    char           *conception;
    char           *parents;
    char           *cparents;
    char           *marriage;
    char           *email;
    char           *conding;
    char           *love;
    char           *coterie;
    char           *cot_time;
    char           *cot_clans;
    char           *cot_sides;
    char           *covenant;
    char           *cov_time;
    char           *leader;
    char           *vleader;
    char           *decap_char;         /* Genesis */
    char           *cdest;              /* Genesis copyover destination */
    char           *stancename;
    char           *stancelist;
    char           *stancework;

    /* Stance move slots */
    int32_t         stancemove[5];

    /* Base stats */
    int32_t         perm_str;
    int32_t         perm_int;
    int32_t         perm_wis;
    int32_t         perm_dex;
    int32_t         perm_con;
    int32_t         mod_str;
    int32_t         mod_int;
    int32_t         mod_wis;
    int32_t         mod_dex;
    int32_t         mod_con;

    /* Admin */
    int32_t         denydate;
    int32_t         quest;
    int32_t         bounty;
    int32_t         wolf;
    int32_t         rank;
    int32_t         regenerate;
    int32_t         demonic;
    int32_t         atm;
    int32_t         homecv;             /* Genesis home CV */
    int32_t         song;
    int32_t         necromancy;
    int32_t         obeah;
    int32_t         darkness;
    int32_t         temporis;
    int32_t         reina;
    int32_t         pain;
    int32_t         silence;
    int32_t         bodymods;
    int32_t         bodycrafts;         /* Genesis */
    int32_t         bpoints;
    int32_t         dpoints;
    int32_t         gpoints;
    int32_t         rpoints;
    int32_t         wpoints;
    int32_t         spirit;
    int32_t         conjure;
    int32_t         celerity;
    int32_t         speed;
    int32_t         firstaid;
    int32_t         repair;
    int32_t         teach;
    int32_t         learn;
    int32_t         evileye;
    int32_t         notes;
    int32_t         followers;
    int32_t         dragcol;            /* Genesis dragon colour */
    int32_t         dragskills;         /* Genesis dragon ability bits */
    int32_t         weather;            /* Genesis weather control */

    /* Array fields */
    int32_t         gnosis[2];
    int32_t         absorb[6];
    int32_t         qstats[5];
    int32_t         language[2];
    int32_t         stage[3];
    int32_t         status[3];
    int32_t         wolfform[2];
    int32_t         score[6];
    int32_t         runes[4];
    int32_t         colors[6];

    /* Discipline/power arrays */
    int32_t         disc[DISC_MAX];
    int32_t         rituals[10];
    int32_t         cland[3];
    int32_t         tgift[3];
    int32_t         diableries[2];
    int32_t         resist[6];

    /* Werewolf subsystems (Genesis) */
    int32_t         breed[3];
    int32_t         auspice[5];
    int32_t         tribes[20];

    /* Powers arrays */
    int32_t         powers[21];
    int32_t         powers_mod[21];
    int32_t         powers_set[21];
    int32_t         powers_set_wear;
    int32_t         power[3];
    int32_t         genes[10];
    int32_t         cot_min[12];
    int32_t         willpower[2];

    /* Genesis class-specific arrays */
    int32_t         highland[4];
    int32_t         ninja[5];
    int32_t         monk[4];
    int32_t         drow[2];
    int32_t         classpoints[2];

    /* Fake stat display (score spoofing) */
    int32_t         fake_skill;
    int32_t         fake_stance;
    int32_t         fake_hit;
    int32_t         fake_dam;
    int32_t         fake_hp;
    int32_t         fake_mana;
    int32_t         fake_move;
    int32_t         fake_max_hp;
    int32_t         fake_max_mana;
    int32_t         fake_max_move;
    int32_t         fake_ac;
    int32_t         fake_stats[5];

    /* Misc */
    int32_t         obj_vnum;
    int32_t         majesty;
    int32_t         condition[3];
    int32_t         learned[MAX_SKILL];
    int32_t         exhaustion;
    int32_t         exercise[4];
    int32_t         lie;
    int32_t         class;
    int32_t         aflame;
    int32_t         achat;
    int32_t         custom;
    int32_t         security;           /* OLC */
    int32_t         stats[12];
    int32_t         safetimer;
    int32_t         bank;
    int32_t         imtimer;            /* Icy/Majesty timer */
    int32_t         autostance;

    /* Genesis mage advanced fields */
    int32_t         arete;
    int32_t         paradox_val;
    int32_t         beginners_luck;
    int32_t         paradox_ward;

    /* Genesis combat timers */
    int32_t         fight_timer;
    int32_t         polymorph_timer;
    int32_t         doublecast_timer;
    int32_t         enrage_timer;
    int32_t         liquify_timer;
    int32_t         atoningwinds_timer;
    int32_t         slow_timer;
    int32_t         better_body_timer;
    int32_t         ephemera_timer;

    /* EM elemental fields (kept for save compat) */
    int32_t         ELEMENTAL_ATTACK;
    int32_t         ELEMENTAL_DEFENCE;

    /* Genesis sorcerer flag */
    int32_t         flag;

    /* Body modification / demon affinity bitfields */
    int32_t         modifiers;          /* IS_MOD() — MOD_* body mod flags */
    int32_t         demaff;             /* IS_DEMAFF() — DEM_* demon aff flags */

    /* Quest task system (questmaster.c) */
    int32_t         quest_type;     /* QTYPE_* — active quest type, 0 = none */
    int32_t         quest_mob;      /* mob vnum for kill tasks */
    int32_t         quest_obj;      /* obj vnum for fetch tasks */
    int32_t         quest_timer;    /* ticks remaining before expiry */
    int32_t         quest_reward;   /* QP reward on completion */
    int32_t         quest_count;    /* lifetime quests completed */
    int32_t         quest_failed;   /* lifetime quests failed */
    int32_t         quest_wait;     /* cooldown ticks after last quest */
};


/*
 * Descriptor structure.
 */
struct descriptor_data
{
    DESCRIPTOR_DATA    *next;
    DESCRIPTOR_DATA    *snoop_by;
    CHAR_DATA          *character;
    CHAR_DATA          *original;
    BUFFER             *output;         /* Genesis pager buffer          */
    char               *outbuf;         /* Output buffer (raw)           */
    int32_t             outtop;         /* Current top of output buffer  */
    int32_t             outsize;        /* Allocated size of outbuf      */
    char               *host;
    int16_t             descriptor;
    int16_t             connected;
    bool                fcommand;
    char                inbuf[4 * MAX_INPUT_LENGTH];
    char                incomm[MAX_INPUT_LENGTH];
    char                inlast[MAX_INPUT_LENGTH];
    int32_t             repeat;
    char               *showstr_head;
    char               *showstr_point;
    void               *pEdit;         /* OLC */
    char              **pString;        /* OLC */
    int32_t             editor;         /* OLC */
    /* KaVir Protocol Snippet fields */
    bool                bMSDP;          /* Client supports MSDP          */
    bool                bMSSP;          /* Client supports MSSP          */
    bool                bMXP;           /* Client supports MXP           */
    bool                bATCP;          /* Client supports ATCP          */
    bool                bMSP;           /* Client supports MSP           */
    bool                bTTYPE;         /* Client sent terminal type     */
    bool                bNAWS;          /* Client sent window size       */
    bool                bNegotiated;    /* Handshake complete            */
    char                terminal_type[64]; /* e.g. "Mudlet", "MUSHclient" */
    int16_t             screen_width;   /* NAWS reported width           */
    int16_t             screen_height;  /* NAWS reported height          */
    /* MobProg support */
    bool                bMobProg;       /* Currently running a mobprog  */
};


/*
 * Editing data (OLC).
 */
struct editing_data
{
    int32_t   flags;
};


/*
 * BUFFER type (Genesis pager).
 */
struct buf_type
{
    BUFFER     *next;
    bool        valid;
    int32_t     state;
    int32_t     size;
    char       *string;
};


/*
 * Help data.
 */
struct help_data
{
    HELP_DATA  *next;
    int16_t     level;
    char       *keyword;
    char       *text;
};


/*
 * Ban data.
 */
struct ban_data
{
    BAN_DATA   *next;
    char       *name;
    int32_t     level;
};


/*
 * Disabled command node.
 */
struct disabled_data
{
    DISABLED_DATA          *next;
    struct cmd_type const  *command;
    char                   *disabled_by;
    int32_t                 level;
};


/*
 * Skill / spell table entry.
 */
typedef struct skill_type
{
    char       *name;
    int16_t     skill_level;
    SPELL_FUN  *spell_fun;
    int16_t     target;
    int16_t     minimum_position;
    int        *pgsn;
    int16_t     slot;
    int16_t     min_mana;
    int16_t     beats;
    char       *noun_damage;
    char       *msg_off;
} SKILL_TYPE;

/*
 * Cmd table entry.
 */
struct cmd_type
{
    char * const name;
    DO_FUN      *do_fun;
    int32_t      position;
    int32_t      level;
    int32_t      log;
};

/*
 * Social table entry.
 */
struct social_type
{
    char * const    name;
    char * const    char_no_arg;
    char * const    others_no_arg;
    char * const    char_found;
    char * const    others_found;
    char * const    vict_found;
    char * const    char_auto;
    char * const    others_auto;
};

struct nsocial_type
{
    char * const    name;
    char * const    char_no_arg;
    char * const    others_no_arg;
    char * const    char_found;
    char * const    others_found;
    char * const    vict_found;
    char * const    char_auto;
    char * const    others_auto;
};

struct xsocial_type
{
    char * const    name;
    char * const    char_no_arg;
    char * const    others_no_arg;
    char * const    char_found;
    char * const    others_found;
    char * const    vict_found;
    char * const    char_auto;
    char * const    others_auto;
    int32_t         gender;
    int32_t         stage;
    int32_t         position;
    int32_t         self;
    int32_t         other;
    bool            chance;
};

typedef struct str_app_type { int16_t tohit; int16_t todam; int16_t carry; int16_t wield; } STR_APP_TYPE;
typedef struct int_app_type { int16_t learn; }                                               INT_APP_TYPE;
typedef struct wis_app_type { int16_t practice; }                                            WIS_APP_TYPE;
typedef struct dex_app_type { int16_t defensive; }                                           DEX_APP_TYPE;
typedef struct con_app_type { int16_t hitp; int16_t shock; }                                 CON_APP_TYPE;
typedef struct wiznet_type  { char *name; long flag; int16_t level; }                        WIZNET_TYPE;


/*
 * =========================================================================
 * CONVENIENCE MACROS
 * =========================================================================
 */

/* Safe null check */
#define IS_NPC(ch)          (IS_SET((ch)->act, ACT_IS_NPC))
#define IS_IMMORTAL(ch)     (get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)         (get_trust(ch) >= LEVEL_HERO)
#define IS_AFFECTED(ch,sn)  (IS_SET((ch)->affected_by, (sn)))
#define IS_POLYAFF(ch,sn)   (IS_SET((ch)->polyaff, (sn)))
#define IS_VAMPAFF(ch,sn)   (IS_SET((ch)->vampaff, (sn)))
#define IS_ITEMAFF(ch,sn)   (IS_SET((ch)->itemaffect, (sn)))
#define IS_EXTRA(ch,sn)     (IS_SET((ch)->extra, (sn)))
#define IS_EXTRA2(ch,sn)    (IS_SET((ch)->extra2, (sn)))
#define IS_ACT(ch,sn)       (IS_SET((ch)->act, (sn)))
#define IS_MORE(ch,sn)      (IS_SET((ch)->more, (sn)))
#define IS_STANCE(ch,sn)    ((ch)->stance[0] == (sn))
#define IS_STYLE(ch,sn)     ((ch)->style[0] == (sn))   /* Genesis */

/* Class identity macros */
#define IS_VAMPIRE(ch)      (IS_SET((ch)->class, CLASS_VAMPIRE))
#define IS_GARGOYLE(ch)     ((ch)->class == CLASS_GARGOYLE)
#define IS_GHOUL(ch)        ((ch)->class == CLASS_GHOUL)
#define IS_WEREWOLF(ch)     (IS_SET((ch)->class, CLASS_WEREWOLF))
#define IS_DEMON(ch)        (IS_SET((ch)->class, CLASS_DEMON))
#define IS_ANGEL(ch)        ((ch)->class == CLASS_ANGEL)
#define IS_HIGHLANDER(ch)   ((ch)->class == CLASS_HIGHLANDER)
#define IS_SPIRIT(ch)       (IS_SET((ch)->more, MORE_SPIRIT))
#define IS_ABOMINATION(ch)  ((ch)->class == CLASS_ABOMINATION)
#define IS_LICH(ch)         ((ch)->class == CLASS_LICH)
#define IS_SKINDANCER(ch)   ((ch)->class == CLASS_SKINDANCER)
#define IS_BAALI(ch)        ((ch)->class == CLASS_BAALI)
#define IS_BLACK_SPIRAL_DANCER(ch) ((ch)->class == CLASS_BLACK_SPIRAL_DANCER)
#define IS_NEPHANDI(ch)     ((ch)->class == CLASS_NEPHANDI)
#define IS_NINJA(ch)        ((ch)->class == CLASS_NINJA)
#define IS_MONK(ch)         ((ch)->class == CLASS_MONK)
#define IS_KOE(ch)          ((ch)->class == CLASS_KOE)
#define IS_FAE(ch)          ((ch)->class == CLASS_FAE)
/* Genesis classes */
#define IS_MAGE(ch)         (IS_SET((ch)->class, CLASS_MAGE))
#define IS_SORCERER(ch)     ((ch)->class == CLASS_SORCERER)
#define IS_DROW(ch)         ((ch)->class == CLASS_DROW)
#define IS_DRAGON(ch)       ((ch)->class == CLASS_DRAGON)
#define IS_ELEMENTAL(ch)    ((ch)->class == CLASS_ELEMENTAL)

/* Character permission/status macros */
#define IS_JUDGE(ch)        (get_trust(ch) >= LEVEL_JUDGE)

/* Staff level shorthand macros */
#define IS_IMP(ch)          (!IS_NPC(ch) && (ch)->level >= LEVEL_IMPLEMENTOR)
#define IS_ST(ch)           (!IS_NPC(ch) && (ch)->level >= LEVEL_ENFORCER)
#define IS_IMM(ch)          (!IS_NPC(ch) && (ch)->level >= LEVEL_IMMORTAL)
#define IS_BUI(ch)          (!IS_NPC(ch) && (ch)->level >= LEVEL_BUILDER)
#define IS_AWA(ch)          (!IS_NPC(ch) && IS_MAGE(ch))   /* Awakened = mage */

/* Class-specific affinity macros */
#define IS_ELEM(ch)         IS_ELEMENTAL(ch)
#define IS_AFF2(ch, bit)    (IS_SET((ch)->affected_by2, (bit)))
#define IS_KOEAFF(ch, bit)  (!IS_NPC(ch) && IS_SET((ch)->pcdata->powers[KOE_CURRENT], (bit)))
#define IS_DRAGAFF(ch, bit) (!IS_NPC(ch) && IS_SET((ch)->pcdata->dragskills, (bit)))
#define get_drow(ch, bit)   (!IS_NPC(ch) ? IS_SET((ch)->pcdata->drow[0], (bit)) : 0)

/* PK eligibility check — single char */
#define CAN_PK(ch)          (!IS_NPC(ch) && IS_FLAG((ch), FLAG_PK))

/* Auto-map boundary check */
#define BOUNDARY(x, y)      ((x) < 0 || (x) > MAPX || (y) < 0 || (y) > MAPY)

/* Stat accessor macro */
#define char_stat(ch, stat) \
    ((stat) == STAT_STR ? get_curr_str(ch) : \
     (stat) == STAT_INT ? get_curr_int(ch) : \
     (stat) == STAT_WIS ? get_curr_wis(ch) : \
     (stat) == STAT_DEX ? get_curr_dex(ch) : \
     (stat) == STAT_CON ? get_curr_con(ch) : 0)
#define IS_QUESTMAKER(ch)   (get_trust(ch) >= LEVEL_QUESTMAKER)
#define IS_MOD(ch, bit)     (IS_SET((ch)->pcdata->modifiers, (bit)))
#define IS_IMMUNE(ch, bit)  (IS_SET((ch)->immune, (bit)))
#define IS_WILLPOWER(ch, bit) (!IS_NPC(ch) && IS_SET((ch)->pcdata->resist[0], (bit)))
#define IS_NEWBIE(ch)       (IS_SET((ch)->act, PLR_NEWBIE))
#define IS_FORM(ch, bit)    (IS_SET((ch)->form, (bit)))
#define IS_DEMPOWER(ch, bit) (IS_SET((ch)->pcdata->powers[0], (bit)))
#define IS_DEMAFF(ch, bit)  (IS_SET((ch)->pcdata->demaff, (bit)))
#define CAN_SIT(ch)         (!IS_SET((ch)->affected_by, AFF_ANCHORED))

/* Body part macros */
#define IS_HEAD(ch,sn)      (IS_SET((ch)->loc_hp[0], (sn)))
#define IS_BODY(ch,sn)      (IS_SET((ch)->loc_hp[1], (sn)))
#define IS_ARM_L(ch,sn)     (IS_SET((ch)->loc_hp[2], (sn)))
#define IS_ARM_R(ch,sn)     (IS_SET((ch)->loc_hp[3], (sn)))
#define IS_LEG_L(ch,sn)     (IS_SET((ch)->loc_hp[4], (sn)))
#define IS_LEG_R(ch,sn)     (IS_SET((ch)->loc_hp[5], (sn)))
#define IS_BLEEDING(ch,sn)  (IS_SET((ch)->loc_hp[6], (sn)))

#define IN_CLAN(ch)         (IS_NPC(ch) ? 0 : (ch)->clan)
#define IS_PLAYING(d)       ((d)->connected == CON_PLAYING)

#define IS_GOOD(ch)         ((ch)->alignment >=  350)
#define IS_EVIL(ch)         ((ch)->alignment <= -350)
#define IS_NEUTRAL(ch)      (!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)        ((ch)->position > POS_SLEEPING)

#define PC(ch, field)       ((ch)->pcdata->field)
#define GET_PC(ch,field,nopc) ((ch)->pcdata ? (ch)->pcdata->field : (nopc))

#define GET_AC(ch) \
    ((ch)->armor + (IS_AWAKE(ch) ? dex_app[get_curr_dex(ch)].defensive : 0))

#define GET_HITROLL(ch) ((ch)->hitroll + str_app[get_curr_str(ch)].tohit)
#define GET_DAMROLL(ch) ((ch)->damroll + str_app[get_curr_str(ch)].todam)

#define IS_OUTSIDE(ch) (!IS_SET((ch)->in_room->room_flags, ROOM_INDOORS))

#define WAIT_STATE(ch, npulse) ((ch)->wait = UMAX((ch)->wait, (npulse)))

#define CAN_WEAR(obj, part)     (IS_SET((obj)->wear_flags, (part)))
#define IS_OBJ_STAT(obj, stat)  (IS_SET((obj)->extra_flags, (stat)))

#define SET_BIT(var, bit)       ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
#define IS_SET(flag, bit)       ((flag) & (bit))
#define TOGGLE_BIT(var, bit)    ((var) ^= (bit))

/* STR — null-safe struct field accessor returning "" if field is NULL */
#define STR(ptr, field) ((ptr) && (ptr)->field ? (ptr)->field : "")

#define UMIN(a, b)  ((a) < (b) ? (a) : (b))
#define UMAX(a, b)  ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c) ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))

#define PERS(ch, looker) ( can_see((looker),(ch)) \
    ? (IS_NPC(ch) ? (ch)->short_descr \
       : (IS_AFFECTED((ch),AFF_POLYMORPH) ? (ch)->morph : (ch)->name)) \
    : "someone" )

/* send_to_char shorthand */
#define stc send_to_char


/*
 * =========================================================================
 * GLOBAL CONSTANTS
 * =========================================================================
 */
extern const struct str_app_type    str_app[26];
extern const struct int_app_type    int_app[26];
extern const struct wis_app_type    wis_app[26];
extern const struct dex_app_type    dex_app[26];
extern const struct con_app_type    con_app[26];
extern const struct wiznet_type     wiznet_table[];
extern const struct cmd_type        cmd_table[];
extern const struct liq_type        liq_table[LIQ_MAX];
extern const struct skill_type      skill_table[MAX_SKILL];
extern KILL_DATA                    kill_table[MAX_LEVEL];
extern const struct social_type     social_table[];
extern const struct nsocial_type    nsocial_table[];
extern const struct xsocial_type    xsocial_table[];
extern const char                  *sphere_name[9];
extern char * const                 dir_name[];
extern const int                    rev_dir[];


/*
 * =========================================================================
 * GLOBAL VARIABLES
 * =========================================================================
 */
extern HELP_DATA            *help_first;
extern SHOP_DATA            *shop_first;
extern SHOP_DATA            *shop_last;
extern BAN_DATA             *ban_list;
extern BAN_DATA             *ban_first;
extern CHAR_DATA            *char_list;
extern DESCRIPTOR_DATA      *descriptor_list;
extern NOTE_DATA            *note_list;
extern OBJ_DATA             *object_list;
extern AREA_DATA            *area_first;
extern AREA_DATA            *area_last;
extern DISABLED_DATA        *disabled_first;

/* Free lists */
extern AFFECT_DATA          *affect_free;
extern BAN_DATA             *ban_free;
extern CHAR_DATA            *char_free;
extern DESCRIPTOR_DATA      *descriptor_free;
extern EXTRA_DESCR_DATA     *extra_descr_free;
extern ROOMTEXT_DATA        *roomtext_free;
extern NOTE_DATA            *note_free;
extern OBJ_DATA             *obj_free;
extern PC_DATA              *pcdata_free;

/* Misc globals */
extern char             bug_buf[];
extern time_t           current_time;
extern bool             fLogAll;
extern FILE            *fpReserve;
extern char             log_buf[];
extern TIME_INFO_DATA   time_info;
extern WEATHER_DATA     weather_info;
extern DXP_DATA         dxp_info;
extern MAP_TYPE         map[MAPX + 1][MAPY + 1];

/* Score tracking globals (defined in comm.c) */
extern char             first_place[40];
extern char             second_place[40];
extern char             third_place[40];
extern int              score_1;
extern int              score_2;
extern int              score_3;

/* Fight list */
extern CHAR_DATA        *first_fight;
extern CHAR_DATA        *last_fight;

/* Leaderboard */
extern LEADER_BOARD      leader_board;

/* OLC counters */
extern int  top_affect;
extern int  top_area;
extern int  top_ed;
extern int  top_exit;
extern int  top_help;
extern int  top_mob_index;
extern int  top_obj_index;
extern int  top_reset;
extern int  top_room;
extern int  top_shop;
extern int  top_vnum_mob;
extern int  top_vnum_obj;
extern int  top_vnum_room;
extern char str_empty[1];

/* Index hash tables */
extern MOB_INDEX_DATA  *mob_index_hash[MAX_KEY_HASH];
extern OBJ_INDEX_DATA  *obj_index_hash[MAX_KEY_HASH];
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];

/* Signal / command tracking */
extern char     last_command[MAX_STRING_LENGTH];
extern char     last_command2[MAX_INPUT_LENGTH];
extern bool     command_finished;
extern bool     pfile;
extern char    *pfile_name;
extern char     mud_time[MSL];
extern char    *last_user;
#define SIGPFILE 77

/* World affect flags (declared in comm.c) */
extern int      world_affects;
extern int      bootcount;
extern long     chainspell;

/* GSN skill numbers (declared in db.c) */
extern int      gsn_charm_person;
extern int      gsn_backstab;
extern int      gsn_hide;
extern int      gsn_peek;
extern int      gsn_pick_lock;
extern int      gsn_sneak;
extern int      gsn_steal;
extern int      gsn_disarm;
extern int      gsn_berserk;
extern int      gsn_punch;
extern int      gsn_kick;
extern int      gsn_invis;
extern int      gsn_mass_invis;
extern int      gsn_polymorph;
extern int      gsn_track;
extern int      gsn_poison;
extern int      gsn_web;
extern int      gsn_potency;


/*
 * =========================================================================
 * FUNCTION DECLARATIONS
 * =========================================================================
 */
#define CD  CHAR_DATA
#define MID MOB_INDEX_DATA
#define OD  OBJ_DATA
#define OID OBJ_INDEX_DATA
#define RID ROOM_INDEX_DATA
#define SF  SPEC_FUN

/* act_comm.c */
void    add_follower    ( CD *ch, CD *master );
void    stop_follower   ( CD *ch );
void    die_follower    ( CD *ch );
bool    is_same_group   ( CD *ach, CD *bch );
void    room_text       ( CD *ch, char *argument );
char   *strlower        ( char *ip );
void    excessive_cpu   ( int blx );
bool    check_parse_name( char *name );
bool    is_in           ( char *, char * );
bool    all_in          ( char *, char * );

/* act_info.c */
void    set_title           ( CD *ch, char *title );
void    show_list_to_char   ( OD *list, CD *ch, bool fShort, bool fShowNothing );
bool    check_blind         ( CD *ch );
int     char_hitroll        ( CD *ch );
int     char_damroll        ( CD *ch );
int     char_ac             ( CD *ch );
int     char_str            ( CD *ch );
int     char_dex            ( CD *ch );
int     char_sta            ( CD *ch );
int     char_cha            ( CD *ch );
int     char_int            ( CD *ch );
int     char_per            ( CD *ch );
int     stat_cost           ( CD *ch, int whichstat );
int     get_celerity        ( CD *ch );
int     get_disc            ( CD *ch, int disc );
int     get_truedisc        ( CD *ch, int disc );
int     get_breed           ( CD *ch, int breed );
int     get_auspice         ( CD *ch, int auspice );
int     get_tribe           ( CD *ch, int tribe );

/* act_move.c */
void    move_char   ( CD *ch, int door );
void    open_lift   ( CD *ch );
void    close_lift  ( CD *ch );
void    move_lift   ( CD *ch, int to_room );
void    move_door   ( CD *ch );
void    thru_door   ( CD *ch, int doorexit );
void    open_door   ( CD *ch, bool be_open );
bool    is_open     ( CD *ch );
bool    same_floor  ( CD *ch, int cmp_room );
void    check_hunt  ( CD *ch );

/* act_obj.c */
bool    is_ok_to_wear   ( CD *ch, bool wolf_ok, char *argument );
void    quest_object    ( CD *ch, OD *obj );
bool    remove_obj      ( CD *ch, int iWear, bool fReplace );
void    wear_obj        ( CD *ch, OD *obj, bool fReplace );
bool    fifty_objects   ( CD *ch );
bool    hundred_objects ( CD *ch, OD *container );
void    set_disc        ( CD *ch );
void    strip_disc      ( CD *ch );
void    mortal_vamp     ( CD *ch );

/* act_wiz.c */
void    wiznet          ( char *string, CD *ch, OD *obj,
                          long flag, long flag_skip, int min_level );
void    bind_char       ( CD *ch );
void    logchan         ( char *argument );
void    sysmes          ( char *argument );
void    copyover_recover( void );
RID    *find_location   ( CD *ch, char *arg );

/* signals.c */
void    init_signals    ( void );
void    attempt_copyover( void );

/* random.c (Genesis — replaces M2.c leaderboard/utility) */
void    mageupkeep          ( CD *ch );
void    check_leaderboard   ( CD *ch, char *argument );
void    load_leaderboard    ( void );
void    save_leaderboard    ( void );
long    strlen2             ( const char *s );
long    get_ratio           ( CHAR_DATA *ch );

/* M2.c (EM utility functions) */
void    append_decap    ( CD *ch, char *file, char *str );
void    divide_to_char  ( CD *ch );
void    centre_text     ( char *text, CD *ch );
void    line            ( CD *ch );
void    line2           ( CD *ch );
void    cent_to_char    ( char *txt, CD *ch );

/* auto_map.c */
void    draw_map    ( CD *ch, char *desc );
int     get_line    ( char *desc, int max_len );
char   *get_exits   ( CD *ch );

/* comm.c */
void    close_socket    ( DESCRIPTOR_DATA *dclose );
void    close_socket2   ( DESCRIPTOR_DATA *dclose );
void    write_to_buffer ( DESCRIPTOR_DATA *d, const char *txt, int length );
void    send_to_char    ( const char *txt, CD *ch );
void    page_to_char    ( const char *txt, CD *ch );
void    send_to_all_char( const char *text );
void    act             ( const char *format, CD *ch,
                          const void *arg1, const void *arg2, int type );
void    act2            ( const char *format, CD *ch,
                          const void *arg1, const void *arg2, int type );
void    kavitem         ( const char *format, CD *ch,
                          const void *arg1, const void *arg2, int type );
/* db.c */
char   *print_flags     ( int flag );
void    boot_db         ( void );
void    area_update     ( void );
CD     *create_mobile   ( MID *pMobIndex );
OD     *create_object   ( OID *pObjIndex, int level );
void    clear_char      ( CD *ch );
void    free_char       ( CD *ch );
char   *get_extra_descr ( const char *name, EXTRA_DESCR_DATA *ed );
char   *get_roomtext    ( const char *name, ROOMTEXT_DATA *rt );
MID    *get_mob_index   ( int vnum );
OID    *get_obj_index   ( int vnum );
RID    *get_room_index  ( int vnum );
char    fread_letter    ( FILE *fp );
int     fread_number    ( FILE *fp );
long    fread_flag      ( FILE *fp );
char   *fread_string    ( FILE *fp );
void    fread_to_eol    ( FILE *fp );
char   *fread_word      ( FILE *fp );
void   *alloc_mem       ( int sMem );
void   *alloc_perm      ( int sMem );
long    flag_convert    ( char letter );
void    free_mem        ( void *pMem, int sMem );
char   *str_dup         ( const char *str );
void    free_string     ( char *pstr );
int     number_fuzzy    ( int number );
int     number_range    ( int from, int to );
int     number_percent  ( void );
int     number_door     ( void );
int     number_bits     ( int width );
int     number_mm       ( void );
int     dice            ( int number, int size );
int     interpolate     ( int level, int value_00, int value_32 );
void    smash_tilde     ( char *str );
bool    str_cmp         ( const char *astr, const char *bstr );
bool    str_prefix      ( const char *astr, const char *bstr );
bool    str_infix       ( const char *astr, const char *bstr );
bool    str_suffix      ( const char *astr, const char *bstr );
char   *capitalize      ( const char *str );
char   *upper_cap       ( const char *str );
void    append_file     ( CD *ch, char *file, char *str );
void    bug             ( const char *str, int param );
void    log_string      ( const char *str );
void    tail_chain      ( void );
void    reset_area      ( AREA_DATA *pArea );
void    load_disabled   ( void );
void    save_disabled   ( void );
void    load_bans       ( void );
void    save_bans       ( void );

/* fight.c */
void    violence_update ( void );
void    multi_hit       ( CD *ch, CD *victim, int dt );
void    damage          ( CD *ch, CD *victim, int dam, int dt );
void    group_gain      ( CD *ch, CD *victim );
void    adv_damage      ( CD *ch, CD *victim, int dam );
void    update_pos      ( CD *victim );
void    set_fighting    ( CD *ch, CD *victim );
void    stop_fighting   ( CD *ch, bool fBoth );
bool    no_attack       ( CD *ch, CD *victim );
bool    is_safe         ( CD *ch, CD *victim );
bool    auto_safe       ( CD *ch, CD *victim );
void    hurt_person     ( CD *ch, CD *victim, int dam, bool agg_dam );
void    killperson      ( CD *ch, CD *victim );
void    death_message   ( CD *ch, CD *victim );
int     dambonus        ( CD *ch, CD *victim, int dam, int stance );
void    demon_gain      ( CD *ch, CD *victim );
bool    has_timer       ( CD *ch );

/* handler.c */
long    wiznet_lookup   ( const char *name );
int     get_trust       ( CD *ch );
int     get_age         ( CD *ch );
int     get_hours       ( CD *ch );
int     get_curr_str    ( CD *ch );
int     get_curr_int    ( CD *ch );
int     get_curr_wis    ( CD *ch );
int     get_curr_dex    ( CD *ch );
int     get_curr_con    ( CD *ch );
int     can_carry_n     ( CD *ch );
int     can_carry_w     ( CD *ch );
bool    is_name         ( const char *str, char *namelist );
bool    is_abbr         ( const char *str, char *namelist );
int     loc_name        ( const char *str, char *namelist );
void    affect_to_char  ( CD *ch, AFFECT_DATA *paf );
void    affect_remove   ( CD *ch, AFFECT_DATA *paf );
void    affect_strip    ( CD *ch, int sn );
bool    is_affected     ( CD *ch, int sn );
void    affect_join     ( CD *ch, AFFECT_DATA *paf );
void    char_from_room  ( CD *ch );
void    char_to_room    ( CD *ch, RID *pRoomIndex );
void    obj_to_char     ( OD *obj, CD *ch );
void    obj_from_char   ( OD *obj );
int     apply_ac        ( OD *obj, int iWear );
OD     *get_eq_char     ( CD *ch, int iWear );
void    equip_char      ( CD *ch, OD *obj, int iWear );
void    unequip_char    ( CD *ch, OD *obj );
int     count_obj_list  ( OID *obj, OD *list );
void    obj_from_room   ( OD *obj );
void    obj_to_room     ( OD *obj, RID *pRoomIndex );
void    obj_to_obj      ( OD *obj, OD *obj_to );
void    obj_from_obj    ( OD *obj );
void    extract_obj     ( OD *obj );
void    extract_char    ( CD *ch, bool fPull );
CD     *get_char        ( CD *ch );
CD     *get_char_room   ( CD *ch, char *argument );
CD     *get_char_world  ( CD *ch, char *argument );
CD     *get_char_world2 ( CD *ch, char *argument );
CD     *get_char_finger ( CD *ch, char *argument );
OD     *get_obj_type    ( OID *pObjIndexData );
OD     *get_obj_list    ( CD *ch, char *argument, OD *list );
OD     *get_obj_in_obj  ( CD *ch, char *argument );
OD     *get_obj_carry   ( CD *ch, char *argument );
OD     *get_obj_wear    ( CD *ch, char *argument );
OD     *get_obj_here    ( CD *ch, char *argument );
OD     *get_obj_room    ( CD *ch, char *argument );
OD     *get_obj_world   ( CD *ch, char *argument );
OD     *get_obj_world2  ( CD *ch, char *argument );
OD     *create_money    ( int amount );
int     get_obj_number  ( OD *obj );
int     get_obj_weight  ( OD *obj );
bool    room_is_dark    ( RID *pRoomIndex );
bool    room_is_private ( RID *pRoomIndex );
bool    can_see         ( CD *ch, CD *victim );
bool    can_see_obj     ( CD *ch, OD *obj );
bool    can_drop_obj    ( CD *ch, OD *obj );
char   *affect_loc_name ( int location );
char   *affect_bit_name ( int vector );
void    affect_modify   ( CD *ch, AFFECT_DATA *paf, bool fAdd );
int     str_close       ( char *a, char *b );
void    mage_format     ( char *arg );

/* interp.c */
void    interpret       ( CD *ch, char *argument );
bool    is_number       ( char *arg );
int     number_argument ( char *argument, char *arg );
char   *one_argument    ( char *argument, char *arg_first );
void    stage_update    ( CD *ch, CD *victim, int stage );

/* magic.c */
int     skill_lookup    ( const char *name );
int     slot_lookup     ( int slot );
int     sn_lookup       ( int sn );
bool    saves_spell     ( int level, CD *victim );
void    obj_cast_spell  ( int sn, int level, CD *ch, CD *victim, OD *obj );
void    enhance_stat    ( int sn, int level, CD *ch, CD *victim,
                          int apply_bit, int bonuses, int affect_bit );

/* save.c */
void    save_char_obj           ( CD *ch );
void    save_char_obj_backup    ( CD *ch );
bool    load_char_obj           ( DESCRIPTOR_DATA *d, char *name );
bool    load_char_short         ( DESCRIPTOR_DATA *d, char *name );

/* special.c */
SF     *spec_lookup     ( const char *name );

/* update.c */
void    gain_exp        ( CD *ch, int gain );
void    gain_condition  ( CD *ch, int iCond, int value );
void    update_handler  ( void );

/* kav_fight.c */
void    special_move    ( CD *ch, CD *victim );
int     highest_skill   ( CD *ch );
int     highest_stance  ( CD *ch, bool advanced );
void    beast_rage      ( CD *ch );
bool    is_undying      ( CD *ch );

/* kav_info.c */
void    birth_date      ( CD *ch, bool is_self );
void    other_age       ( CD *ch, int extra, bool is_preg, char *argument );
int     years_old       ( CD *ch );
int     get_totalscore  ( CD *ch );
void    clear_eq        ( CD *ch );
void    sing_song       ( CD *ch, OD *obj );
bool    play_yet        ( CD *ch );
int     date_to_int     ( char *argument );

/* kav_wiz.c */
void    oset_affect     ( CD *ch, OD *obj, int value, int affect, bool is_quest );

/* clan.c */
void    werewolf_regen  ( CD *ch );
void    part_reg        ( CD *ch );
void    reg_mend        ( CD *ch );
void    vamp_rage       ( CD *ch );
bool    char_exists     ( int file_dir, char *argument );
OD     *get_page        ( OD *book, long page_num );
void    update_gen      ( CD *ch );
int     true_generation ( CD *ch, char *argument );
void    mummify_corpse  ( CD *ch, OD *obj );
void    staked_corpse   ( CD *ch, OD *obj );
void    power_ghoul     ( CD *ch, CD *victim );
void    poison_weapon   ( CD *ch, OD *obj );
void    ritual_ward     ( CD *ch, int direction, int ward );
char   *disc_name       ( int disc_no );

/* more.c */
void    guardian_message( CD *ch );

/* string.c */
char   *format_string   ( char *oldstring );
void    string_edit     ( CD *ch, char **pString );
void    string_add      ( CD *ch, char *argument );
void    string_append   ( CD *ch, char **pString );
char   *first_arg       ( char *argument, char *arg_first, bool fCase );
char   *string_replace  ( char *orig, char *old, char *newstr );
char   *string_proper   ( char *argument );
char   *string_unpad    ( char *argument );
int     arg_count       ( char *argument );

/* olc.c */
void    aedit           ( CD *ch, char *argument );
void    redit           ( CD *ch, char *argument );
void    medit           ( CD *ch, char *argument );
void    oedit           ( CD *ch, char *argument );
void    add_reset       ( AREA_DATA *area, RESET_DATA *pReset, int index );

/* core.c */
bool    regenerate_part ( OD *obj );

/* bit.c */
char   *room_bit_name       ( int room_flags );
int     room_name_bit       ( char *buf );
int     sector_number       ( char *argument );
char   *sector_name         ( int sect );
int     item_name_type      ( char *name );
char   *item_type_name      ( int item_type );
int     extra_name_bit      ( char *buf );
char   *extra_bit_name      ( int extra_flags );
int     wear_name_bit       ( char *buf );
char   *wear_bit_name       ( int wear );
int     act_name_bit        ( char *buf );
int     affect_name_bit     ( char *buf );
int     affect_name_loc     ( char *name );
int     wear_name_loc       ( char *buf );
char   *wear_loc_name       ( int wearloc );
char   *act_bit_name        ( int act );
int     get_weapon_type     ( char *arg );
char   *get_type_weapon     ( int arg );
int     get_container_flags ( char *arg );
char   *get_flags_container ( int arg );
int     get_liquid_type     ( char *arg );
char   *get_type_liquid     ( int arg );

/* util.c */
void    cptimer_check   ( void );
void    auto_copyover   ( void );

/* flag.c (Genesis) */
void    do_setflag      ( CD *ch, char *argument );

/* stance.c (Genesis) */
void    update_style    ( CD *ch );
void    improve_stance  ( CD *ch );
void    improve_style   ( CD *ch );
void    improve_weapon  ( CD *ch, long skill );

/* trap.c (Genesis) */
void    trigger_trap    ( CD *ch, TRAP_DATA *trap );
void    tdamage         ( CD *ch, TRAP_DATA *trap );
void    tspell          ( CD *ch, TRAP_DATA *trap );
void    tmove           ( CD *ch, TRAP_DATA *trap );
void    tsummon         ( CD *ch, TRAP_DATA *trap );

/* dragon.c (Genesis) */
void    do_dragonform   ( CD *ch, char *argument );
void    do_dragonskill  ( CD *ch, char *argument );

/* drow.c (Genesis) */
void    do_drowarmor    ( CD *ch, char *argument );
void    do_drowupkeep   ( CD *ch, char *argument );

/* elemental.c (Genesis) */
void    do_element      ( CD *ch, char *argument );
void    do_elemental    ( CD *ch, char *argument );
void    do_elemarmor    ( CD *ch, char *argument );

/* sorcerer.c (Genesis) */
void    do_sorctalk     ( CD *ch, char *argument );
void    do_sorccreate   ( CD *ch, char *argument );

/* wwauspice.c (Genesis) */
void    do_blur         ( CD *ch, char *argument );
void    do_blissful     ( CD *ch, char *argument );

/* wwbreed.c (Genesis) */
void    do_persuasion   ( CD *ch, char *argument );
void    do_jamtech      ( CD *ch, char *argument );

/* wwtribe.c (Genesis) */
void    do_wwpowers     ( CD *ch, char *argument );

/* highland.c (Genesis) */
void    do_highlander   ( CD *ch, char *argument );

/* mage.c (Genesis) */
void    do_magecast     ( CD *ch, char *argument );

/* disc.c (Genesis) — vampire earth-shift and other discipline extras */
void    do_earthshift   ( CD *ch, char *argument );


/*
 * =========================================================================
 * DECLARE_SPELL_FUN declarations for all spells (magic.c).
 * These allow skill_table[] in const.c to reference spell function pointers.
 * =========================================================================
 */
DECLARE_SPELL_FUN( spell_null            );
DECLARE_SPELL_FUN( spell_acid_blast      );
DECLARE_SPELL_FUN( spell_acid_breath     );
DECLARE_SPELL_FUN( spell_armor           );
DECLARE_SPELL_FUN( spell_bake            );
DECLARE_SPELL_FUN( spell_bless           );
DECLARE_SPELL_FUN( spell_blindness       );
DECLARE_SPELL_FUN( spell_brew            );
DECLARE_SPELL_FUN( spell_burning_hands   );
DECLARE_SPELL_FUN( spell_call_lightning  );
DECLARE_SPELL_FUN( spell_carve           );
DECLARE_SPELL_FUN( spell_cause_critical  );
DECLARE_SPELL_FUN( spell_cause_light     );
DECLARE_SPELL_FUN( spell_cause_serious   );
DECLARE_SPELL_FUN( spell_change_sex      );
DECLARE_SPELL_FUN( spell_chaos_blast     );
DECLARE_SPELL_FUN( spell_charm_person    );
DECLARE_SPELL_FUN( spell_chill_touch     );
DECLARE_SPELL_FUN( spell_clot            );
DECLARE_SPELL_FUN( spell_colour_spray    );
DECLARE_SPELL_FUN( spell_continual_light );
DECLARE_SPELL_FUN( spell_contraception   );
DECLARE_SPELL_FUN( spell_control_weather );
DECLARE_SPELL_FUN( spell_copy            );
DECLARE_SPELL_FUN( spell_create_food     );
DECLARE_SPELL_FUN( spell_create_spring   );
DECLARE_SPELL_FUN( spell_create_water    );
DECLARE_SPELL_FUN( spell_cure_blindness  );
DECLARE_SPELL_FUN( spell_cure_critical   );
DECLARE_SPELL_FUN( spell_cure_light      );
DECLARE_SPELL_FUN( spell_cure_poison     );
DECLARE_SPELL_FUN( spell_cure_serious    );
DECLARE_SPELL_FUN( spell_curse           );
DECLARE_SPELL_FUN( spell_darkblessing    );
DECLARE_SPELL_FUN( spell_detect_evil     );
DECLARE_SPELL_FUN( spell_detect_hidden   );
DECLARE_SPELL_FUN( spell_detect_invis    );
DECLARE_SPELL_FUN( spell_detect_magic    );
DECLARE_SPELL_FUN( spell_detect_poison   );
DECLARE_SPELL_FUN( spell_dispel_evil     );
DECLARE_SPELL_FUN( spell_dispel_magic    );
DECLARE_SPELL_FUN( spell_earthquake      );
DECLARE_SPELL_FUN( spell_enchant_armor   );
DECLARE_SPELL_FUN( spell_enchant_weapon  );
DECLARE_SPELL_FUN( spell_energy_drain    );
DECLARE_SPELL_FUN( spell_energyflux      );
DECLARE_SPELL_FUN( spell_engrave         );
DECLARE_SPELL_FUN( spell_faerie_fire     );
DECLARE_SPELL_FUN( spell_faerie_fog      );
DECLARE_SPELL_FUN( spell_fear            );
DECLARE_SPELL_FUN( spell_find_familiar   );
DECLARE_SPELL_FUN( spell_fire_breath     );
DECLARE_SPELL_FUN( spell_fireball        );
DECLARE_SPELL_FUN( spell_fix             );
DECLARE_SPELL_FUN( spell_flamestrike     );
DECLARE_SPELL_FUN( spell_fly             );
DECLARE_SPELL_FUN( spell_frenzy          );
DECLARE_SPELL_FUN( spell_frost_breath    );
DECLARE_SPELL_FUN( spell_gas_breath      );
DECLARE_SPELL_FUN( spell_gate            );
DECLARE_SPELL_FUN( spell_general_purpose );
DECLARE_SPELL_FUN( spell_giant_strength  );
DECLARE_SPELL_FUN( spell_guardian        );
DECLARE_SPELL_FUN( spell_harm            );
DECLARE_SPELL_FUN( spell_heal            );
DECLARE_SPELL_FUN( spell_high_explosive  );
DECLARE_SPELL_FUN( spell_identify        );
DECLARE_SPELL_FUN( spell_improve         );
DECLARE_SPELL_FUN( spell_infravision     );
DECLARE_SPELL_FUN( spell_insert_page     );
DECLARE_SPELL_FUN( spell_invis           );
DECLARE_SPELL_FUN( spell_jail            );
DECLARE_SPELL_FUN( spell_know_alignment  );
DECLARE_SPELL_FUN( spell_lightning_bolt  );
DECLARE_SPELL_FUN( spell_lightning_breath);
DECLARE_SPELL_FUN( spell_locate_object   );
DECLARE_SPELL_FUN( spell_magic_missile   );
DECLARE_SPELL_FUN( spell_major_creation  );
DECLARE_SPELL_FUN( spell_mana            );
DECLARE_SPELL_FUN( spell_mass_invis      );
DECLARE_SPELL_FUN( spell_mend            );
DECLARE_SPELL_FUN( spell_minor_creation  );
DECLARE_SPELL_FUN( spell_mount           );
DECLARE_SPELL_FUN( spell_pass_door       );
DECLARE_SPELL_FUN( spell_poison          );
DECLARE_SPELL_FUN( spell_polymorph       );
DECLARE_SPELL_FUN( spell_portal          );
DECLARE_SPELL_FUN( spell_preserve        );
DECLARE_SPELL_FUN( spell_protection      );
DECLARE_SPELL_FUN( spell_quest           );
DECLARE_SPELL_FUN( spell_refresh         );
DECLARE_SPELL_FUN( spell_regenerate      );
DECLARE_SPELL_FUN( spell_remove_curse    );
DECLARE_SPELL_FUN( spell_remove_page     );
DECLARE_SPELL_FUN( spell_resistance      );
DECLARE_SPELL_FUN( spell_sanctuary       );
DECLARE_SPELL_FUN( spell_scan            );
DECLARE_SPELL_FUN( spell_scribe          );
DECLARE_SPELL_FUN( spell_shield          );
DECLARE_SPELL_FUN( spell_shocking_grasp  );
DECLARE_SPELL_FUN( spell_sleep           );
DECLARE_SPELL_FUN( spell_soulblade       );
DECLARE_SPELL_FUN( spell_spellproof      );
DECLARE_SPELL_FUN( spell_spiritkiss      );
DECLARE_SPELL_FUN( spell_stone_skin      );
DECLARE_SPELL_FUN( spell_summon          );
DECLARE_SPELL_FUN( spell_teleport        );
DECLARE_SPELL_FUN( spell_tendrils        );
DECLARE_SPELL_FUN( spell_transport       );
DECLARE_SPELL_FUN( spell_ventriloquate   );
DECLARE_SPELL_FUN( spell_voodoo          );
DECLARE_SPELL_FUN( spell_weaken          );
DECLARE_SPELL_FUN( spell_web             );
DECLARE_SPELL_FUN( spell_word_of_recall  );

/*
 * =========================================================================
 * DECLARE_DO_FUN declarations for all commands.
 * These allow cmd_table[] to reference function pointers portably.
 * =========================================================================
 */
DECLARE_DO_FUN( do_addlag       );
DECLARE_DO_FUN( do_accept       );
DECLARE_DO_FUN( do_activate     );
DECLARE_DO_FUN( do_aeolus       );
DECLARE_DO_FUN( do_affects      );
DECLARE_DO_FUN( do_alignment    );
DECLARE_DO_FUN( do_afk          );
DECLARE_DO_FUN( do_allow        );
DECLARE_DO_FUN( do_alterstate   );
DECLARE_DO_FUN( do_alterweight  );
DECLARE_DO_FUN( do_analyze      );
DECLARE_DO_FUN( do_anesthetic   );
DECLARE_DO_FUN( do_angelarmour  );
DECLARE_DO_FUN( do_angelform    );
DECLARE_DO_FUN( do_anger        );
DECLARE_DO_FUN( do_animal       );
DECLARE_DO_FUN( do_ansi         );
DECLARE_DO_FUN( do_answer       );
DECLARE_DO_FUN( do_anull        );
DECLARE_DO_FUN( do_areas        );
DECLARE_DO_FUN( do_arms         );
DECLARE_DO_FUN( do_artifact     );
DECLARE_DO_FUN( do_ashes        );
DECLARE_DO_FUN( do_at           );
DECLARE_DO_FUN( do_autoclass    );
DECLARE_DO_FUN( do_autoexit     );
DECLARE_DO_FUN( do_autoloot     );
DECLARE_DO_FUN( do_autosac      );
DECLARE_DO_FUN( do_autosave     );
DECLARE_DO_FUN( do_autostance   );
DECLARE_DO_FUN( do_awe          );
DECLARE_DO_FUN( do_baal         );
DECLARE_DO_FUN( do_backstab     );
DECLARE_DO_FUN( do_balance      );
DECLARE_DO_FUN( do_bamfin       );
DECLARE_DO_FUN( do_bamfout      );
DECLARE_DO_FUN( do_ban          );
DECLARE_DO_FUN( do_beastlike    );
DECLARE_DO_FUN( do_beastwithin  );
DECLARE_DO_FUN( do_beckon       );
DECLARE_DO_FUN( do_berserk      );
DECLARE_DO_FUN( do_betterbody   );
DECLARE_DO_FUN( do_bind         );
DECLARE_DO_FUN( do_birth        );
DECLARE_DO_FUN( do_bite         );
DECLARE_DO_FUN( do_blank        );
DECLARE_DO_FUN( do_blindfold    );
DECLARE_DO_FUN( do_bloodagony   );
DECLARE_DO_FUN( do_bloodline    );
DECLARE_DO_FUN( do_bloodpact    );
DECLARE_DO_FUN( do_bloodrage    );
DECLARE_DO_FUN( do_blur         );
DECLARE_DO_FUN( do_bodywrack    );
DECLARE_DO_FUN( do_bonecraft    );
DECLARE_DO_FUN( do_brandish     );
DECLARE_DO_FUN( do_breach       );
DECLARE_DO_FUN( do_breakup      );
DECLARE_DO_FUN( do_breath       );
DECLARE_DO_FUN( do_brief        );
DECLARE_DO_FUN( do_bug          );
DECLARE_DO_FUN( do_buildtalk    );
DECLARE_DO_FUN( do_burrow       );
DECLARE_DO_FUN( do_buy          );
DECLARE_DO_FUN( do_call         );
DECLARE_DO_FUN( do_call_lightning );
DECLARE_DO_FUN( do_callspirit   );
DECLARE_DO_FUN( do_calm         );
DECLARE_DO_FUN( do_camouflage   );
DECLARE_DO_FUN( do_cast         );
DECLARE_DO_FUN( do_cauldronblood);
DECLARE_DO_FUN( do_celerity     );
DECLARE_DO_FUN( do_speed        );
DECLARE_DO_FUN( do_champions    );
DECLARE_DO_FUN( do_change       );
DECLARE_DO_FUN( do_changelight  );
DECLARE_DO_FUN( do_changeling   );
DECLARE_DO_FUN( do_channels     );
DECLARE_DO_FUN( do_chant        );
DECLARE_DO_FUN( do_chat         );
DECLARE_DO_FUN( do_cherub       );
DECLARE_DO_FUN( do_claim        );
DECLARE_DO_FUN( do_clandisc     );
DECLARE_DO_FUN( do_clanname     );
DECLARE_DO_FUN( do_clap         );
DECLARE_DO_FUN( do_classrecall  );
DECLARE_DO_FUN( do_claw         );
DECLARE_DO_FUN( do_claws        );
DECLARE_DO_FUN( do_clearclass   );
DECLARE_DO_FUN( do_clearclas    );
DECLARE_DO_FUN( do_clearstats   );
DECLARE_DO_FUN( do_clist        );
DECLARE_DO_FUN( do_cloneobject  );
DECLARE_DO_FUN( do_close        );
DECLARE_DO_FUN( do_command      );
DECLARE_DO_FUN( do_commands     );
DECLARE_DO_FUN( do_compare      );
DECLARE_DO_FUN( do_complete     );
DECLARE_DO_FUN( do_conditioning );
DECLARE_DO_FUN( do_config       );
DECLARE_DO_FUN( do_consent      );
DECLARE_DO_FUN( do_consider     );
DECLARE_DO_FUN( do_control      );
DECLARE_DO_FUN( do_controlbeast );
DECLARE_DO_FUN( do_controlweather );
DECLARE_DO_FUN( do_contort      );
DECLARE_DO_FUN( do_copyover     );
DECLARE_DO_FUN( do_coterie      );
DECLARE_DO_FUN( do_covenant     );
DECLARE_DO_FUN( do_courage      );
DECLARE_DO_FUN( do_cprompt      );
DECLARE_DO_FUN( do_crack        );
DECLARE_DO_FUN( do_create       );
DECLARE_DO_FUN( do_credits      );
DECLARE_DO_FUN( do_ctalk        );
DECLARE_DO_FUN( do_cvalk        );
DECLARE_DO_FUN( do_curse        );
DECLARE_DO_FUN( do_darkheart    );
DECLARE_DO_FUN( do_dazzle       );
DECLARE_DO_FUN( do_decapitate   );
DECLARE_DO_FUN( do_deleteme     );
DECLARE_DO_FUN( do_demonarmour  );
DECLARE_DO_FUN( do_demonform    );
DECLARE_DO_FUN( do_demote       );
DECLARE_DO_FUN( do_deny         );
DECLARE_DO_FUN( do_deposit      );
DECLARE_DO_FUN( do_description  );
DECLARE_DO_FUN( do_diagnose     );
DECLARE_DO_FUN( do_dimmak       );
DECLARE_DO_FUN( do_disable      );
DECLARE_DO_FUN( do_disarm       );
DECLARE_DO_FUN( do_discharge    );
DECLARE_DO_FUN( do_discipline   );
DECLARE_DO_FUN( do_humble       );
DECLARE_DO_FUN( do_disconnect   );
DECLARE_DO_FUN( do_dismount     );
DECLARE_DO_FUN( do_disquiet     );
DECLARE_DO_FUN( do_divorce      );
DECLARE_DO_FUN( do_donate       );
DECLARE_DO_FUN( do_doubleexp    );
DECLARE_DO_FUN( do_halfexp      );
DECLARE_DO_FUN( do_timedexp     );
DECLARE_DO_FUN( do_expoff       );
DECLARE_DO_FUN( do_down         );
DECLARE_DO_FUN( do_drainlife    );
DECLARE_DO_FUN( do_draw         );
DECLARE_DO_FUN( do_dreadgaze    );
DECLARE_DO_FUN( do_drink        );
DECLARE_DO_FUN( do_drop         );
DECLARE_DO_FUN( do_guardian     );
DECLARE_DO_FUN( do_earthembrace );
DECLARE_DO_FUN( do_earthmeld    );
DECLARE_DO_FUN( do_earthshield  );
DECLARE_DO_FUN( do_earthshift   );  /* Genesis disc.c */
DECLARE_DO_FUN( do_east         );
DECLARE_DO_FUN( do_eat          );
DECLARE_DO_FUN( do_echo         );
DECLARE_DO_FUN( do_classelder   );
DECLARE_DO_FUN( do_fall         );
DECLARE_DO_FUN( do_faeequipment );
DECLARE_DO_FUN( do_eclipse      );
DECLARE_DO_FUN( do_elemarmor    );
DECLARE_DO_FUN( do_element      );
DECLARE_DO_FUN( do_elemental    );
DECLARE_DO_FUN( do_empty        );
DECLARE_DO_FUN( do_email        );
DECLARE_DO_FUN( do_emote        );
DECLARE_DO_FUN( do_enchant      );
DECLARE_DO_FUN( do_equipment    );
DECLARE_DO_FUN( do_examine      );
DECLARE_DO_FUN( do_exits        );
DECLARE_DO_FUN( do_fangs        );
DECLARE_DO_FUN( do_finger       );
DECLARE_DO_FUN( do_flee         );
DECLARE_DO_FUN( do_fly          );
DECLARE_DO_FUN( do_follow       );
DECLARE_DO_FUN( do_force        );
DECLARE_DO_FUN( do_freeze       );
DECLARE_DO_FUN( do_get          );
DECLARE_DO_FUN( do_give         );
DECLARE_DO_FUN( do_goto         );
DECLARE_DO_FUN( do_group        );
DECLARE_DO_FUN( do_gtell        );
DECLARE_DO_FUN( do_help         );
DECLARE_DO_FUN( do_hide         );
DECLARE_DO_FUN( do_highfive     );
DECLARE_DO_FUN( do_highlander   );  /* Genesis */
DECLARE_DO_FUN( do_hitch        );
DECLARE_DO_FUN( do_home         );
DECLARE_DO_FUN( do_horns        );
DECLARE_DO_FUN( do_howl         );
DECLARE_DO_FUN( do_idea         );
DECLARE_DO_FUN( do_immtalk      );
DECLARE_DO_FUN( do_invis        );
DECLARE_DO_FUN( do_inventory    );
DECLARE_DO_FUN( do_jamtech      );  /* Genesis */
DECLARE_DO_FUN( do_jab          );
DECLARE_DO_FUN( do_joinherd     );
DECLARE_DO_FUN( do_kick         );
DECLARE_DO_FUN( do_kill         );
DECLARE_DO_FUN( do_kneel        );
DECLARE_DO_FUN( do_land         );
DECLARE_DO_FUN( do_lay          );
DECLARE_DO_FUN( do_leaderboard  );
DECLARE_DO_FUN( do_list         );
DECLARE_DO_FUN( do_load         );
DECLARE_DO_FUN( do_lock         );
DECLARE_DO_FUN( do_look         );
DECLARE_DO_FUN( do_magecast     );  /* Genesis */
DECLARE_DO_FUN( do_map          );
DECLARE_DO_FUN( do_marry        );
DECLARE_DO_FUN( do_medit        );
DECLARE_DO_FUN( do_mfind        );
DECLARE_DO_FUN( do_mload        );
DECLARE_DO_FUN( do_mob          );
DECLARE_DO_FUN( do_mstat        );
DECLARE_DO_FUN( do_mpstat       );
DECLARE_DO_FUN( do_mprog        );
DECLARE_DO_FUN( do_mwhere       );
DECLARE_DO_FUN( do_news         );
DECLARE_DO_FUN( do_nightsight   );
DECLARE_DO_FUN( do_nod          );
DECLARE_DO_FUN( do_north        );
DECLARE_DO_FUN( do_note         );
DECLARE_DO_FUN( do_obeah        );
DECLARE_DO_FUN( do_oedit        );
DECLARE_DO_FUN( do_ofind        );
DECLARE_DO_FUN( do_oload        );
DECLARE_DO_FUN( do_open         );
DECLARE_DO_FUN( do_order        );
DECLARE_DO_FUN( do_ostat        );
DECLARE_DO_FUN( do_owhere       );
DECLARE_DO_FUN( do_peek         );
DECLARE_DO_FUN( do_persuasion   );  /* Genesis */
DECLARE_DO_FUN( do_pick         );
DECLARE_DO_FUN( do_pkill        );
DECLARE_DO_FUN( do_polyaff      );
DECLARE_DO_FUN( do_pour         );
DECLARE_DO_FUN( do_powers       );
DECLARE_DO_FUN( do_practice     );
DECLARE_DO_FUN( do_pretitle     );
DECLARE_DO_FUN( do_prone        );
DECLARE_DO_FUN( do_protect      );
DECLARE_DO_FUN( do_pull         );
DECLARE_DO_FUN( do_purge        );
DECLARE_DO_FUN( do_push         );
DECLARE_DO_FUN( do_put          );
DECLARE_DO_FUN( do_quest        );
DECLARE_DO_FUN( do_quit         );
DECLARE_DO_FUN( do_read         );
DECLARE_DO_FUN( do_redit        );
DECLARE_DO_FUN( do_reload       );
DECLARE_DO_FUN( do_remove       );
DECLARE_DO_FUN( do_rename       );
DECLARE_DO_FUN( do_repair       );
DECLARE_DO_FUN( do_reply        );
DECLARE_DO_FUN( do_rescue       );
DECLARE_DO_FUN( do_rest         );
DECLARE_DO_FUN( do_restore      );
DECLARE_DO_FUN( do_return       );
DECLARE_DO_FUN( do_rfind        );
DECLARE_DO_FUN( do_rset         );
DECLARE_DO_FUN( do_rstat        );
DECLARE_DO_FUN( do_sacrifice    );
DECLARE_DO_FUN( do_save         );
DECLARE_DO_FUN( do_say          );
DECLARE_DO_FUN( do_scan         );
DECLARE_DO_FUN( do_score        );
DECLARE_DO_FUN( do_sell         );
DECLARE_DO_FUN( do_setflag      );  /* Genesis */
DECLARE_DO_FUN( do_shout        );
DECLARE_DO_FUN( do_sit          );
DECLARE_DO_FUN( do_skills       );
DECLARE_DO_FUN( do_sleep        );
DECLARE_DO_FUN( do_slit         );
DECLARE_DO_FUN( do_sneak        );
DECLARE_DO_FUN( do_snoop        );
DECLARE_DO_FUN( do_sorctalk     );  /* Genesis */
DECLARE_DO_FUN( do_sorccreate   );  /* Genesis */
DECLARE_DO_FUN( do_south        );
DECLARE_DO_FUN( do_spells       );
DECLARE_DO_FUN( do_split        );
DECLARE_DO_FUN( do_stance       );
DECLARE_DO_FUN( do_stand        );
DECLARE_DO_FUN( do_steal        );
DECLARE_DO_FUN( do_style        );
DECLARE_DO_FUN( do_switch       );
DECLARE_DO_FUN( do_tell         );
DECLARE_DO_FUN( do_time         );
DECLARE_DO_FUN( do_title        );
DECLARE_DO_FUN( do_track        );
DECLARE_DO_FUN( do_transfer     );
DECLARE_DO_FUN( do_typo         );
DECLARE_DO_FUN( do_unban        );
DECLARE_DO_FUN( do_unhitch      );
DECLARE_DO_FUN( do_unlock       );
DECLARE_DO_FUN( do_up           );
DECLARE_DO_FUN( do_value        );
DECLARE_DO_FUN( do_visible      );
DECLARE_DO_FUN( do_wake         );
DECLARE_DO_FUN( do_wear         );
DECLARE_DO_FUN( do_weather      );
DECLARE_DO_FUN( do_west         );
DECLARE_DO_FUN( do_who          );
DECLARE_DO_FUN( do_wield        );
DECLARE_DO_FUN( do_wimpy        );
DECLARE_DO_FUN( do_wings        );
DECLARE_DO_FUN( do_wizhelp      );
DECLARE_DO_FUN( do_wizinvis     );
DECLARE_DO_FUN( do_wizlist      );
DECLARE_DO_FUN( do_wizlock      );
DECLARE_DO_FUN( do_wiznet       );
DECLARE_DO_FUN( do_write        );
DECLARE_DO_FUN( do_wwpowers     );  /* Genesis */
DECLARE_DO_FUN( do_yell         );
DECLARE_DO_FUN( do_zap          );
DECLARE_DO_FUN( do_drowarmor    );  /* Genesis */
DECLARE_DO_FUN( do_drowupkeep   );  /* Genesis */
DECLARE_DO_FUN( do_dragonform   );  /* Genesis */
DECLARE_DO_FUN( do_dragonskill  );  /* Genesis */

/* EM-specific commands */
DECLARE_DO_FUN( do_bank         );
DECLARE_DO_FUN( do_withdraw     );
DECLARE_DO_FUN( do_deposit      );
DECLARE_DO_FUN( do_koe          );
DECLARE_DO_FUN( do_fae          );
DECLARE_DO_FUN( do_demon        );
DECLARE_DO_FUN( do_warlock      );
DECLARE_DO_FUN( do_freezetag    );
DECLARE_DO_FUN( do_bonus        );
DECLARE_DO_FUN( do_pretitle     );
DECLARE_DO_FUN( do_ritual       );

/* Missing do_* declarations referenced in interp.c (alphabetical) */
DECLARE_DO_FUN( do_adamantium        );
DECLARE_DO_FUN( do_adjustbalance     );
DECLARE_DO_FUN( do_adult             );
DECLARE_DO_FUN( do_aedit             );
DECLARE_DO_FUN( do_akuma             );
DECLARE_DO_FUN( do_arealist          );
DECLARE_DO_FUN( do_asave             );
DECLARE_DO_FUN( do_ashow             );
DECLARE_DO_FUN( do_atemi             );
DECLARE_DO_FUN( do_awaken            );
DECLARE_DO_FUN( do_backfist          );
DECLARE_DO_FUN( do_blinding          );
DECLARE_DO_FUN( do_bloodawaken       );
DECLARE_DO_FUN( do_bloodlash         );
DECLARE_DO_FUN( do_blue              );
DECLARE_DO_FUN( do_bomuzite          );
DECLARE_DO_FUN( do_bonearmor         );
DECLARE_DO_FUN( do_boneblade         );
DECLARE_DO_FUN( do_bounty            );
DECLARE_DO_FUN( do_celestial         );
DECLARE_DO_FUN( do_cfat              );
DECLARE_DO_FUN( do_chi               );
DECLARE_DO_FUN( do_chingshih         );
DECLARE_DO_FUN( do_chirift           );
DECLARE_DO_FUN( do_chiweapon         );
DECLARE_DO_FUN( do_circle            );
DECLARE_DO_FUN( do_clearreply        );
DECLARE_DO_FUN( do_consume           );
DECLARE_DO_FUN( do_corpseskin        );
DECLARE_DO_FUN( do_crib              );
DECLARE_DO_FUN( do_custom            );
DECLARE_DO_FUN( do_deathtouch        );
DECLARE_DO_FUN( do_delevel           );
DECLARE_DO_FUN( do_demonfangs        );
DECLARE_DO_FUN( do_detatchment       );
DECLARE_DO_FUN( do_dharma            );
DECLARE_DO_FUN( do_dismember         );
DECLARE_DO_FUN( do_ebondragon        );
DECLARE_DO_FUN( do_elbow             );
DECLARE_DO_FUN( do_elementalIncrease );
DECLARE_DO_FUN( do_embrace           );
DECLARE_DO_FUN( do_empower           );
DECLARE_DO_FUN( do_enchantarmor      );
DECLARE_DO_FUN( do_engulf            );
DECLARE_DO_FUN( do_enlighten         );
DECLARE_DO_FUN( do_enter             );
DECLARE_DO_FUN( do_entrancement      );
DECLARE_DO_FUN( do_escape            );
DECLARE_DO_FUN( do_exlist            );
DECLARE_DO_FUN( do_extendedfingers   );
DECLARE_DO_FUN( do_eyespy            );
DECLARE_DO_FUN( do_faestorm          );
DECLARE_DO_FUN( do_fakekill          );
DECLARE_DO_FUN( do_familiar          );
DECLARE_DO_FUN( do_favour            );
DECLARE_DO_FUN( do_fcommand          );
DECLARE_DO_FUN( do_fear              );
DECLARE_DO_FUN( do_feed              );
DECLARE_DO_FUN( do_fetishdoll        );
DECLARE_DO_FUN( do_fightclub         );
DECLARE_DO_FUN( do_fightstyle        );
DECLARE_DO_FUN( do_fill              );
DECLARE_DO_FUN( do_firstaid          );
DECLARE_DO_FUN( do_flamebolt         );
DECLARE_DO_FUN( do_flames            );
DECLARE_DO_FUN( do_flametalk         );
DECLARE_DO_FUN( do_flaminghands      );
DECLARE_DO_FUN( do_fleshbond         );
DECLARE_DO_FUN( do_fleshcraft        );
DECLARE_DO_FUN( do_flex              );
DECLARE_DO_FUN( do_for               );
DECLARE_DO_FUN( do_formillusion      );
DECLARE_DO_FUN( do_frenzy            );
DECLARE_DO_FUN( do_ftag              );
DECLARE_DO_FUN( do_gag               );
DECLARE_DO_FUN( do_generation        );
DECLARE_DO_FUN( do_gift              );
DECLARE_DO_FUN( do_gifts             );
DECLARE_DO_FUN( do_goblinface        );
DECLARE_DO_FUN( do_goblinscorch      );
DECLARE_DO_FUN( do_goblinspark       );
DECLARE_DO_FUN( do_godsheal          );
DECLARE_DO_FUN( do_godstrike         );
DECLARE_DO_FUN( do_guardianangel     );
DECLARE_DO_FUN( do_guide             );
DECLARE_DO_FUN( do_hagswrinkles      );
DECLARE_DO_FUN( do_halosun           );
DECLARE_DO_FUN( do_hand              );
DECLARE_DO_FUN( do_hara_kiri         );
DECLARE_DO_FUN( do_harness           );
DECLARE_DO_FUN( do_heal              );
DECLARE_DO_FUN( do_healingtouch      );
DECLARE_DO_FUN( do_hima              );
DECLARE_DO_FUN( do_holylight         );
DECLARE_DO_FUN( do_hooves            );
DECLARE_DO_FUN( do_hotienchi         );
DECLARE_DO_FUN( do_hover             );
DECLARE_DO_FUN( do_huh               );
DECLARE_DO_FUN( do_humanform         );
DECLARE_DO_FUN( do_hunt              );
DECLARE_DO_FUN( do_huntingsnake      );
DECLARE_DO_FUN( do_hurl              );
DECLARE_DO_FUN( do_icy               );
DECLARE_DO_FUN( do_immune            );
DECLARE_DO_FUN( do_imp               );
DECLARE_DO_FUN( do_impulse           );
DECLARE_DO_FUN( do_incant            );
DECLARE_DO_FUN( do_incog             );
DECLARE_DO_FUN( do_induct            );
DECLARE_DO_FUN( do_inferno           );
DECLARE_DO_FUN( do_infirmity         );
DECLARE_DO_FUN( do_inform            );
DECLARE_DO_FUN( do_inpart            );
DECLARE_DO_FUN( do_introduce         );
DECLARE_DO_FUN( do_invisibility      );
DECLARE_DO_FUN( do_jadeservant       );
DECLARE_DO_FUN( do_jail              );
DECLARE_DO_FUN( do_kakusu            );
DECLARE_DO_FUN( do_kanzuite          );
DECLARE_DO_FUN( do_kdiscipline       );
DECLARE_DO_FUN( do_knee              );
DECLARE_DO_FUN( do_koryou            );
DECLARE_DO_FUN( do_kotearmor         );
DECLARE_DO_FUN( do_ktalk             );
DECLARE_DO_FUN( do_lamprey           );
DECLARE_DO_FUN( do_layhands          );
DECLARE_DO_FUN( do_leader            );
DECLARE_DO_FUN( do_leaderclear       );
DECLARE_DO_FUN( do_leap              );
DECLARE_DO_FUN( do_learn             );
DECLARE_DO_FUN( do_level             );
DECLARE_DO_FUN( do_lick              );
DECLARE_DO_FUN( do_lifesight         );
DECLARE_DO_FUN( do_lifespan          );
DECLARE_DO_FUN( do_lightning         );
DECLARE_DO_FUN( do_littledeath       );
DECLARE_DO_FUN( do_locate            );
DECLARE_DO_FUN( do_log               );
DECLARE_DO_FUN( do_lordname          );
DECLARE_DO_FUN( do_lotuscloud        );
DECLARE_DO_FUN( do_love              );
DECLARE_DO_FUN( do_loyalty           );
DECLARE_DO_FUN( do_madness           );
DECLARE_DO_FUN( do_magearmor         );
DECLARE_DO_FUN( do_magetalk          );
DECLARE_DO_FUN( do_majesty           );
DECLARE_DO_FUN( do_mask              );
DECLARE_DO_FUN( do_masssummons       );
DECLARE_DO_FUN( do_mclear            );
DECLARE_DO_FUN( do_meditate          );
DECLARE_DO_FUN( do_memory            );
DECLARE_DO_FUN( do_mgate             );
DECLARE_DO_FUN( do_michi             );
DECLARE_DO_FUN( do_mienaku           );
DECLARE_DO_FUN( do_miktalk           );
DECLARE_DO_FUN( do_mindblast         );
DECLARE_DO_FUN( do_mindflex          );
DECLARE_DO_FUN( do_mindshield        );
DECLARE_DO_FUN( do_mirrorimage       );
DECLARE_DO_FUN( do_mists             );
DECLARE_DO_FUN( do_mitsukeru         );
DECLARE_DO_FUN( do_mizotamouyo       );
DECLARE_DO_FUN( do_monktalk          );
DECLARE_DO_FUN( do_moongate          );
DECLARE_DO_FUN( do_morph             );
DECLARE_DO_FUN( do_mother            );
DECLARE_DO_FUN( do_mount             );
DECLARE_DO_FUN( do_move              );
DECLARE_DO_FUN( do_mset              );
DECLARE_DO_FUN( do_mshadow           );
DECLARE_DO_FUN( do_mshow             );
DECLARE_DO_FUN( do_multicheck        );
DECLARE_DO_FUN( do_mummify           );
DECLARE_DO_FUN( do_mupkeep           );
DECLARE_DO_FUN( do_music             );
DECLARE_DO_FUN( do_neutral           );
DECLARE_DO_FUN( do_newbie            );
DECLARE_DO_FUN( do_newbielock        );
DECLARE_DO_FUN( do_nice              );
DECLARE_DO_FUN( do_ningentra         );
DECLARE_DO_FUN( do_ninjaarmor        );
DECLARE_DO_FUN( do_noble             );
DECLARE_DO_FUN( do_noemote           );
DECLARE_DO_FUN( do_nonewbie          );
DECLARE_DO_FUN( do_notell            );
DECLARE_DO_FUN( do_nsocials          );
DECLARE_DO_FUN( do_nuke              );
DECLARE_DO_FUN( do_obedience         );
DECLARE_DO_FUN( do_objectgate        );
DECLARE_DO_FUN( do_oclone            );
DECLARE_DO_FUN( do_offersoul         );
DECLARE_DO_FUN( do_omnipotence       );
DECLARE_DO_FUN( do_ooc               );
DECLARE_DO_FUN( do_orb               );
DECLARE_DO_FUN( do_oreturn           );
DECLARE_DO_FUN( do_orimasu           );
DECLARE_DO_FUN( do_oset              );
DECLARE_DO_FUN( do_oshow             );
DECLARE_DO_FUN( do_oswitch           );
DECLARE_DO_FUN( do_otransfer         );
DECLARE_DO_FUN( do_outcast           );
DECLARE_DO_FUN( do_pact              );
DECLARE_DO_FUN( do_palmstrike        );
DECLARE_DO_FUN( do_panacea           );
DECLARE_DO_FUN( do_paralyzing        );
DECLARE_DO_FUN( do_pardon            );
DECLARE_DO_FUN( do_parttheveil       );
DECLARE_DO_FUN( do_password          );
DECLARE_DO_FUN( do_path              );
DECLARE_DO_FUN( do_pathdisciplineshintai );
DECLARE_DO_FUN( do_pchat             );
DECLARE_DO_FUN( do_pcrename          );
DECLARE_DO_FUN( do_peace             );
DECLARE_DO_FUN( do_pelesit           );
DECLARE_DO_FUN( do_perception        );
DECLARE_DO_FUN( do_permeate          );
DECLARE_DO_FUN( do_peruse            );
DECLARE_DO_FUN( do_piercetheshroud   );
DECLARE_DO_FUN( do_pigeon            );
DECLARE_DO_FUN( do_placate           );
DECLARE_DO_FUN( do_plasmaform        );
DECLARE_DO_FUN( do_pload             );
DECLARE_DO_FUN( do_poison_cloud      );
DECLARE_DO_FUN( do_polyappear        );
DECLARE_DO_FUN( do_porcupine         );
DECLARE_DO_FUN( do_portal            );
DECLARE_DO_FUN( do_possess           );
DECLARE_DO_FUN( do_possession        );
DECLARE_DO_FUN( do_potency           );
DECLARE_DO_FUN( do_pray              );
DECLARE_DO_FUN( do_preservation      );
DECLARE_DO_FUN( do_preturn           );
DECLARE_DO_FUN( do_principles        );
DECLARE_DO_FUN( do_projection        );
DECLARE_DO_FUN( do_promote           );
DECLARE_DO_FUN( do_prompt            );
DECLARE_DO_FUN( do_propose           );
DECLARE_DO_FUN( do_psay              );
DECLARE_DO_FUN( do_pset              );
DECLARE_DO_FUN( do_ptalk             );
DECLARE_DO_FUN( do_ptoken            );
DECLARE_DO_FUN( do_punch             );
DECLARE_DO_FUN( do_purification      );
DECLARE_DO_FUN( do_purifyspirit      );
DECLARE_DO_FUN( do_putrefaction      );
DECLARE_DO_FUN( do_pwhere            );
DECLARE_DO_FUN( do_qblast            );
DECLARE_DO_FUN( do_qmake             );
DECLARE_DO_FUN( do_qset              );
DECLARE_DO_FUN( do_qstat             );
DECLARE_DO_FUN( do_qtrust            );
DECLARE_DO_FUN( do_quaff             );
DECLARE_DO_FUN( do_question          );
DECLARE_DO_FUN( do_qui               );
DECLARE_DO_FUN( do_quintessence      );
DECLARE_DO_FUN( do_rage              );
DECLARE_DO_FUN( do_rain              );
DECLARE_DO_FUN( do_randomload        );
DECLARE_DO_FUN( do_readaura          );
DECLARE_DO_FUN( do_reboo             );
DECLARE_DO_FUN( do_reboot            );
DECLARE_DO_FUN( do_recall            );
DECLARE_DO_FUN( do_recharge          );
DECLARE_DO_FUN( do_recho             );
DECLARE_DO_FUN( do_recite            );
DECLARE_DO_FUN( do_red               );
DECLARE_DO_FUN( do_regenerate        );
DECLARE_DO_FUN( do_reina             );
DECLARE_DO_FUN( do_relax             );
DECLARE_DO_FUN( do_release           );
DECLARE_DO_FUN( do_relevel           );
DECLARE_DO_FUN( do_renew             );
DECLARE_DO_FUN( do_rent              );
DECLARE_DO_FUN( do_report            );
DECLARE_DO_FUN( do_resets            );
DECLARE_DO_FUN( do_reshape           );
DECLARE_DO_FUN( do_ridethedragon     );
DECLARE_DO_FUN( do_ritualitem        );
DECLARE_DO_FUN( do_roll              );
DECLARE_DO_FUN( do_rshow             );
DECLARE_DO_FUN( do_rubbing           );
DECLARE_DO_FUN( do_safe              );
DECLARE_DO_FUN( do_scarletqueen      );
DECLARE_DO_FUN( do_scentbeyond       );
DECLARE_DO_FUN( do_scream            );
DECLARE_DO_FUN( do_scribe            );
DECLARE_DO_FUN( do_scry              );
DECLARE_DO_FUN( do_seal              );
DECLARE_DO_FUN( do_serenity          );
DECLARE_DO_FUN( do_serpent           );
DECLARE_DO_FUN( do_servant           );
DECLARE_DO_FUN( do_setauspice        );
DECLARE_DO_FUN( do_setbreed          );
DECLARE_DO_FUN( do_setdisc           );
DECLARE_DO_FUN( do_settribe          );
DECLARE_DO_FUN( do_shadowbody        );
DECLARE_DO_FUN( do_shadowcompanion   );
DECLARE_DO_FUN( do_shadownature      );
DECLARE_DO_FUN( do_shadowplane       );
DECLARE_DO_FUN( do_shadowsight       );
DECLARE_DO_FUN( do_shadowstep        );
DECLARE_DO_FUN( do_shapechange       );
DECLARE_DO_FUN( do_sharespirit       );
DECLARE_DO_FUN( do_sharpensenses     );
DECLARE_DO_FUN( do_sheath            );
DECLARE_DO_FUN( do_shield            );
DECLARE_DO_FUN( do_shift             );
DECLARE_DO_FUN( do_shinkick          );
DECLARE_DO_FUN( do_shintai           );
DECLARE_DO_FUN( do_shoot             );
DECLARE_DO_FUN( do_shroudedmoon      );
DECLARE_DO_FUN( do_shroudnight       );
DECLARE_DO_FUN( do_shutdow           );
DECLARE_DO_FUN( do_shutdown          );
DECLARE_DO_FUN( do_silence           );
DECLARE_DO_FUN( do_silencedeath      );
DECLARE_DO_FUN( do_sing              );
DECLARE_DO_FUN( do_skill             );
DECLARE_DO_FUN( do_sla               );
DECLARE_DO_FUN( do_slay              );
DECLARE_DO_FUN( do_slookup           );
DECLARE_DO_FUN( do_smother           );
DECLARE_DO_FUN( do_soar              );
DECLARE_DO_FUN( do_socials           );
DECLARE_DO_FUN( do_soulmask          );
DECLARE_DO_FUN( do_soultwist         );
DECLARE_DO_FUN( do_speak             );
DECLARE_DO_FUN( do_special           );
DECLARE_DO_FUN( do_spell             );
DECLARE_DO_FUN( do_spheres           );
DECLARE_DO_FUN( do_spinkick          );
DECLARE_DO_FUN( do_spiritblast       );
DECLARE_DO_FUN( do_spiriteyes        );
DECLARE_DO_FUN( do_spiritform        );
DECLARE_DO_FUN( do_spiritgate        );
DECLARE_DO_FUN( do_spiritguardian    );
DECLARE_DO_FUN( do_spiritkiss        );
DECLARE_DO_FUN( do_spiritpower       );
DECLARE_DO_FUN( do_spiritshare       );
DECLARE_DO_FUN( do_spit              );
DECLARE_DO_FUN( do_spy               );
DECLARE_DO_FUN( do_sset              );
DECLARE_DO_FUN( do_stake             );
DECLARE_DO_FUN( do_staredown         );
DECLARE_DO_FUN( do_strengthen        );
DECLARE_DO_FUN( do_stun              );
DECLARE_DO_FUN( do_succulence        );
DECLARE_DO_FUN( do_summon            );
DECLARE_DO_FUN( do_summoning         );
DECLARE_DO_FUN( do_sweep             );
DECLARE_DO_FUN( do_tail              );
DECLARE_DO_FUN( do_tasteblood        );
DECLARE_DO_FUN( do_teach             );
DECLARE_DO_FUN( do_teachdisc         );
DECLARE_DO_FUN( do_teachgift         );
DECLARE_DO_FUN( do_tear              );
DECLARE_DO_FUN( do_telekinetics      );
DECLARE_DO_FUN( do_tempest           );
DECLARE_DO_FUN( do_thanatopsis       );
DECLARE_DO_FUN( do_theftvitae        );
DECLARE_DO_FUN( do_thousandforms     );
DECLARE_DO_FUN( do_throw             );
DECLARE_DO_FUN( do_thrustkick        );
DECLARE_DO_FUN( do_tie               );
DECLARE_DO_FUN( do_timer_check       );
DECLARE_DO_FUN( do_token             );
DECLARE_DO_FUN( do_tongue            );
DECLARE_DO_FUN( do_totalscore        );
DECLARE_DO_FUN( do_train             );
DECLARE_DO_FUN( do_transport         );
DECLARE_DO_FUN( do_travel            );
DECLARE_DO_FUN( do_tribe             );
DECLARE_DO_FUN( do_tribegifts        );
DECLARE_DO_FUN( do_trip              );
DECLARE_DO_FUN( do_truedisguise      );
DECLARE_DO_FUN( do_truefear          );
DECLARE_DO_FUN( do_trueform          );
DECLARE_DO_FUN( do_truesight         );
DECLARE_DO_FUN( do_tsume             );
DECLARE_DO_FUN( do_turn              );
DECLARE_DO_FUN( do_unburden          );
DECLARE_DO_FUN( do_unleash           );
DECLARE_DO_FUN( do_unload            );
DECLARE_DO_FUN( do_unpolymorph       );
DECLARE_DO_FUN( do_unseal            );
DECLARE_DO_FUN( do_unshadownature    );
DECLARE_DO_FUN( do_untie             );
DECLARE_DO_FUN( do_unveil            );
DECLARE_DO_FUN( do_upkeep            );
DECLARE_DO_FUN( do_uplift            );
DECLARE_DO_FUN( do_users             );
DECLARE_DO_FUN( do_vamparmor         );
DECLARE_DO_FUN( do_vampire           );
DECLARE_DO_FUN( do_vamptalk          );
DECLARE_DO_FUN( do_vanish            );
DECLARE_DO_FUN( do_vclan             );
DECLARE_DO_FUN( do_visage            );
DECLARE_DO_FUN( do_vlist             );
DECLARE_DO_FUN( do_voodoo            );
DECLARE_DO_FUN( do_vopen             );
DECLARE_DO_FUN( do_wail              );
DECLARE_DO_FUN( do_wall              );
DECLARE_DO_FUN( do_wallflames        );
DECLARE_DO_FUN( do_wallgranite       );
DECLARE_DO_FUN( do_watcher           );
DECLARE_DO_FUN( do_wattack           );
DECLARE_DO_FUN( do_weakness          );
DECLARE_DO_FUN( do_weaponform        );
DECLARE_DO_FUN( do_web               );
DECLARE_DO_FUN( do_where             );
DECLARE_DO_FUN( do_whisper           );
DECLARE_DO_FUN( do_whitetigercorpse  );
DECLARE_DO_FUN( do_willpower         );
DECLARE_DO_FUN( do_windwalker        );
DECLARE_DO_FUN( do_windwhisper       );
DECLARE_DO_FUN( do_wingslice         );
DECLARE_DO_FUN( do_withering         );
DECLARE_DO_FUN( do_witherlimb        );
DECLARE_DO_FUN( do_wizdox            );
DECLARE_DO_FUN( do_wolfarmor         );
DECLARE_DO_FUN( do_worldaff          );
DECLARE_DO_FUN( do_wyld              );
DECLARE_DO_FUN( do_xemote            );
DECLARE_DO_FUN( do_xsocials          );
DECLARE_DO_FUN( do_yangheal          );
DECLARE_DO_FUN( do_yangmantle        );
DECLARE_DO_FUN( do_zombie            );
DECLARE_DO_FUN( do_ztag              );
DECLARE_DO_FUN( do_zuloform          );

/* Missing declarations added for compilation */
DECLARE_DO_FUN( do_board             );
DECLARE_DO_FUN( do_evileye           );
DECLARE_DO_FUN( do_ghostcall         );
DECLARE_DO_FUN( do_improve           );
DECLARE_DO_FUN( do_login             );
DECLARE_DO_FUN( do_pcwipe            );
DECLARE_DO_FUN( do_setstance         );
DECLARE_DO_FUN( do_side              );
DECLARE_DO_FUN( do_status            );
DECLARE_DO_FUN( do_tempering         );
DECLARE_DO_FUN( do_yangvanish        );

/* EM/Genesis merged missing prototypes */
DECLARE_DO_FUN( do_info              );
DECLARE_DO_FUN( do_forceauto         );
DECLARE_DO_FUN( do_watching          );
DECLARE_DO_FUN( do_noutcast          );
DECLARE_DO_FUN( do_moutcast          );
DECLARE_DO_FUN( do_tradition         );
DECLARE_DO_FUN( do_fatality          );
DECLARE_DO_FUN( do_unwerewolf        );
DECLARE_DO_FUN( do_mortalvamp        );

/* Bounty system */
DECLARE_DO_FUN( do_bountylist        );

/* Trivia system (trivia.c) */
DECLARE_DO_FUN( do_answer            );
DECLARE_DO_FUN( do_trivia            );
extern void     trivia_load          ( void );
extern void     trivia_update        ( void );

/* auto_copyover and cptimer_check (util.c) */
extern void     auto_copyover        ( void );
extern void     cptimer_check        ( void );

/* Quest task system (questmaster.c) */
DECLARE_DO_FUN( do_questmaster       );
DECLARE_DO_FUN( do_complete          );
DECLARE_DO_FUN( do_questinfo         );
extern void     quest_update         ( void );

/* Auction house (auction.c) */
DECLARE_DO_FUN( do_auction           );
extern void     auction_load         ( void );
extern void     auction_save         ( void );
extern void     auction_update       ( void );
extern void     auction_deliver      ( CHAR_DATA *ch );

/* OLC extended (olc.c) */
DECLARE_DO_FUN( do_mlist             );
DECLARE_DO_FUN( do_olist             );
DECLARE_DO_FUN( do_rlist             );

/* Missing utility functions (defined in stubs.c) */
extern long     get_color            ( CHAR_DATA *ch, long color );
extern bool     hurt_fighting        ( CHAR_DATA *ch, CHAR_DATA *victim );
extern void     quest_kill_check     ( CHAR_DATA *ch, CHAR_DATA *victim );
extern long     mod_damcap           ( CHAR_DATA *ch, CHAR_DATA *victim );
extern LEADER_BOARD leader_board;

/* Non-DO_FUN function prototypes */
extern void clear_stats( CHAR_DATA *ch );
extern void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype );
extern void improve_wpn( CHAR_DATA *ch, int dtype, bool right_hand );
extern void disarm( CHAR_DATA *ch, CHAR_DATA *victim );
extern void raw_kill( CHAR_DATA *ch, CHAR_DATA *victim );
extern void critical_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam );
extern void crack_head( CHAR_DATA *ch, OBJ_DATA *obj, char *argument );
extern void make_part( CHAR_DATA *ch, char *argument );
extern void take_item( CHAR_DATA *ch, OBJ_DATA *obj );
extern void mud_logf( char *fmt, ... );

#undef CD
#undef MID
#undef OD
#undef OID
#undef RID
#undef SF

/*
 * These headers use types defined above, so they are included last.
 */
#include "protocol.h"
#include "mobprog.h"
#include "board.h"

#endif /* MERC_H */
