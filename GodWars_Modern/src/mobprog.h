/***************************************************************************
 * mobprog.h -- Mobile programs for GodWars_Modern                        *
 *                                                                         *
 * Based on the widely-used Merc MobProg snippet.                          *
 * Allows mobs, objects, and rooms to run scripted trigger programs.       *
 ***************************************************************************/

#ifndef MOBPROG_H
#define MOBPROG_H

/*
 * MobProg trigger types
 */
#define TRIG_ACT        (1 <<  0)   /* act() text matches               */
#define TRIG_BRIBE      (1 <<  1)   /* Player bribes mob                */
#define TRIG_DEATH      (1 <<  2)   /* Mob is killed                    */
#define TRIG_ENTRY      (1 <<  3)   /* Someone enters the room          */
#define TRIG_FIGHT      (1 <<  4)   /* Each combat round                */
#define TRIG_GIVE       (1 <<  5)   /* Player gives object to mob       */
#define TRIG_GREET      (1 <<  6)   /* Mob greets player entering room  */
#define TRIG_GRALL      (1 <<  7)   /* Greet all (including other mobs) */
#define TRIG_KILL       (1 <<  8)   /* Mob initiates a kill             */
#define TRIG_HPCNT      (1 <<  9)   /* HP percent threshold             */
#define TRIG_RANDOM     (1 << 10)   /* Random chance each pulse         */
#define TRIG_SPEECH     (1 << 11)   /* Player says something            */
#define TRIG_EXIT       (1 << 12)   /* Player leaves the room           */
#define TRIG_EXALL      (1 << 13)   /* Exit all (including mobs)        */
#define TRIG_DELAY      (1 << 14)   /* Timed delay                      */
#define TRIG_SURR       (1 << 15)   /* Mob is surrounded                */
/* ObjProg triggers */
#define TRIG_OBJ_GET    (1 << 16)   /* Object is picked up              */
#define TRIG_OBJ_DROP   (1 << 17)   /* Object is dropped                */
#define TRIG_OBJ_WEAR   (1 << 18)   /* Object is worn                   */
#define TRIG_OBJ_REMOVE (1 << 19)   /* Object is removed                */
#define TRIG_OBJ_SAC    (1 << 20)   /* Object is sacrificed             */
/* RoomProg triggers */
#define TRIG_ROOM_ENTER (1 << 21)   /* Someone enters the room          */
#define TRIG_ROOM_EXIT  (1 << 22)   /* Someone leaves the room          */
#define TRIG_ROOM_RAND  (1 << 23)   /* Random room trigger              */

/*
 * A single program block
 */
struct prog_list
{
    PROG_LIST  *next;
    int32_t     trig_type;      /* Which trigger fires this prog    */
    char       *trig_phrase;    /* Phrase/percent/etc for trigger   */
    char       *prog_text;      /* The script text itself           */
};

typedef struct prog_list PROG_LIST;

/*
 * Execution context — passed through the interpreter
 */
typedef struct prog_ctx
{
    CHAR_DATA  *mob;            /* The mob running the program       */
    CHAR_DATA  *actor;          /* The player who triggered it       */
    OBJ_DATA   *obj;            /* Object involved (if any)          */
    void       *vo;             /* Generic "victim" pointer          */
    char       *trigger_arg;    /* The argument that triggered       */
    int32_t     depth;          /* Nesting depth (max 10)            */
} PROG_CTX;

/* Function prototypes */

/* Execution */
void    mprog_driver        ( const char *prog, PROG_CTX *ctx );
void    mprog_do_ifchck     ( const char *ifchck, PROG_CTX *ctx, bool *cond );

/* Trigger dispatch functions */
void    mprog_act_trigger   ( const char *buf, CHAR_DATA *mob,
                              CHAR_DATA *ch, OBJ_DATA *obj, void *vo );
void    mprog_bribe_trigger ( CHAR_DATA *mob, CHAR_DATA *ch, int amount );
void    mprog_death_trigger ( CHAR_DATA *mob, CHAR_DATA *killer );
void    mprog_entry_trigger ( CHAR_DATA *mob );
void    mprog_fight_trigger ( CHAR_DATA *mob, CHAR_DATA *ch );
void    mprog_give_trigger  ( CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj );
void    mprog_greet_trigger ( CHAR_DATA *ch );
void    mprog_kill_trigger  ( CHAR_DATA *mob, CHAR_DATA *ch );
void    mprog_hpfall_trigger( CHAR_DATA *mob, CHAR_DATA *ch );
void    mprog_random_trigger( CHAR_DATA *mob );
void    mprog_speech_trigger( const char *txt, CHAR_DATA *ch );
void    mprog_exit_trigger  ( CHAR_DATA *ch, int door );
void    mprog_delay_trigger ( CHAR_DATA *mob );

/* ObjProg triggers */
void    oprog_get_trigger   ( CHAR_DATA *ch, OBJ_DATA *obj );
void    oprog_drop_trigger  ( CHAR_DATA *ch, OBJ_DATA *obj );
void    oprog_wear_trigger  ( CHAR_DATA *ch, OBJ_DATA *obj );
void    oprog_remove_trigger( CHAR_DATA *ch, OBJ_DATA *obj );

/* RoomProg triggers */
void    rprog_enter_trigger ( CHAR_DATA *ch );
void    rprog_exit_trigger  ( CHAR_DATA *ch, int door );
void    rprog_random_trigger( ROOM_INDEX_DATA *room );

/* OLC/load support */
PROG_LIST *new_prog         ( void );
void       free_prog        ( PROG_LIST *prg );

#endif /* MOBPROG_H */
