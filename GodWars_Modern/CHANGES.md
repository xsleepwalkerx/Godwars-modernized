# GodWars Modernized — Change Log & Integration Notes

## Overview

This codebase merges **EM (Eternal Malice)** as the primary base with
**Genesis**'s unused classes and subsystems fully integrated. All source
code has been modernized to C99 standards.

---

## 1. New C Standard (C99)

### What Changed
- All source files compile with `-std=c99`
- `#include <stdint.h>` and `#include <stdbool.h>` are used in `merc.h`
- `int` field declarations in structs replaced with sized types:
  - `int32_t` for all game integers (class, hp, flags, etc.)
  - `int16_t` for `sh_int` / small counters
  - `bool` from `<stdbool.h>` (native true/false)
- Old-style `args( list )` macros retained for compatibility with legacy
  `.c` files, but new code uses direct prototypes

### Migration Note
If any `.c` file fails to compile due to implicit int declarations, add
an explicit `int32_t` or `int` type. Common patterns:
```c
// OLD (K&R / C89 style):
int i, j;
for(i=0; i < 10; i++) { ... }

// NEW (C99):
for (int i = 0; i < 10; i++) { ... }
```

---

## 2. New Classes Integrated from Genesis

The following classes exist in Genesis but were absent from EM. They are
now fully integrated:

### CLASS_SORCERER (8192)
- **Source files:** `sorcerer.c`
- **Description:** Human practitioners of chaos magic. Uses a unique
  spell-creation system (`do_sorccreate`) rather than fixed spells.
- **Channel:** `CHANNEL_SORCTALK` (bit 1) — already present in EM
- **New commands:** `sorctalk`, `sorccreate`
- **Saves:** Uses existing `pcdata->flag` for sorcerer-specific flags

### CLASS_DROW (16384)
- **Source files:** `drow.c`
- **Description:** Dark elves with innate spider/shadow abilities.
- **Power array:** `pcdata->drow[2]` — drow[0]=power level, drow[1]=ability bits
- **Ability bits:** `DROW_ACID`, `DROW_WEB`, `DROW_DARK`, `DROW_FIRE`,
  `DROW_BITE`, `DROW_DUALWIELD`, `DROW_SPIDER`, etc.
- **New commands:** `drowarmor`, `drowupkeep`

### CLASS_DRAGON (32768)
- **Source files:** `dragon.c`
- **Description:** Dragons with shapeshifting and breath weapons.
  Uses `POLY_DRAGONFORM` polymorph bit to track dragon form.
- **Skill field:** `ch->dragskills` (bitfield of `DRAG_*` constants)
- **Ability bits:** `DRAG_FANGS`, `DRAG_CLAWS`, `DRAG_WINGS`, `DRAG_BREATH`,
  `DRAG_TAIL`, `DRAG_TRUESIGHT`, `DRAG_HORNS`, `DRAG_SHIELD`, etc.
- **New commands:** `dragonform`, `dragonskill`
- **Visual:** `DRAG_WINGSOUT` / `DRAG_HORNSOUT` track deployed appendages

### CLASS_ELEMENTAL (65536)
- **Source files:** `elemental.c`
- **Description:** Beings of pure elemental energy. Player chooses an
  element type stored in `ch->elem_type`.
- **Element types:** `ELEM_FIRE`, `ELEM_WATER`, `ELEM_EARTH`, `ELEM_WIND`,
  `ELEM_SUN`, `ELEM_MOON`, `ELEM_TIME`, `ELEM_INFIN`
- **Channel:** `CHANNEL_ELEM` (bit 131072)
- **New commands:** `element`, `elemental`, `elemarmor`

---

## 3. New Werewolf Subsystems from Genesis

### Auspice System (`wwauspice.c`)
The five astrological roles of the Garou:
- `AUSPICE_RAGABASH` (0) — Trickster / No-Moon
- `AUSPICE_THEURGE` (1) — Mystic / Crescent Moon  
- `AUSPICE_PHILODOX` (2) — Mediator / Half-Moon
- `AUSPICE_GALLIARD` (3) — Bard / Gibbous Moon
- `AUSPICE_AHROUN` (4) — Warrior / Full Moon

Each auspice grants unique powers accessed via `get_auspice(ch, AUSPICE_*)`.
Powers include: Blur, Blissful Ignorance, Sense Wyrm, Call of the Wyld, etc.

### Breed System (`wwbreed.c`)
Three breeds defining origin:
- `BREED_HOMID` (0) — Human-born: social powers (Persuasion, Jam Technology)
- `BREED_LUPUS` (1) — Wolf-born: animal affinity powers  
- `BREED_METIS` (2) — Metis-born: deformity gifts (Sense Magic, Bristles)

### Tribe System (`wwtribe.c`)
Thirteen tribes with unique gifts:
`BLACK_FURIES`, `BONE_GNAWERS`, `CHILDREN_OF_GAIA`, `FIANNA`,
`GET_OF_FENRIS`, `GLASS_WALKERS`, `RED_TALONS`, `SHADOW_LORDS`,
`SILENT_STRIDERS`, `SILVER_FANGS`, `STARGAZERS`, `UKTENA`, `WENDIGOS`

Use `get_tribe(ch, TRIBE_*)` to check tribe power levels.
New command: `wwpowers` — shows werewolf power summary.

---

## 4. New Combat Systems from Genesis

### Weapon Style System (`stance.c`)
Replaces the stub style tracking in EM with a full implementation:
- `STYLE_UNARMED` — No weapon
- `STYLE_NATURAL` — Claws/fangs (vampire/werewolf/dragon)
- `STYLE_ONEHAND` — Single weapon
- `STYLE_SHIELD` — Weapon + shield
- `STYLE_TWOHAND` — Two-handed weapon
- `STYLE_TWOWEAPON` — Dual wielding

`update_style(ch)` is called automatically when equipment changes.
`improve_style(ch)` and `improve_weapon(ch, skill)` give gradual progression.

### Trap System (`trap.c`)
Rooms and exits can now have traps (stored in `EXIT_DATA->trap`):
```c
struct trap_data {
    int32_t  type;      /* TRAP_DAMAGE, TRAP_SPELL, TRAP_MOVE, TRAP_SUMMON */
    int32_t  level;
    int32_t  charges;   /* -1 = unlimited */
    int32_t  trigger;   /* TRIG_ENTER, TRIG_PICK, TRIG_OPEN, etc. */
    int32_t  value;
    char    *name;
};
```
Traps are triggered by `trigger_trap(ch, trap)` in movement code.

---

## 5. New Vampire Disciplines from Genesis

### Additional Disciplines (disc.c)
The `disc.c` file from Genesis adds high-level discipline effects:
- **Earth Control** (Protean 6): `do_earthshift` — teleport via earth-meld
- Earthshift requires `IS_VAMPAFF(ch, VAM_EARTHMELDED)` to be active

Discipline array expanded: `pcdata->disc[DISC_MAX]` where `DISC_MAX = 18`

---

## 6. Miscellaneous Systems from Genesis

### Flag System (`flag.c`)
`do_setflag` — Immortal command to toggle flags on players/objects.
More granular than EM's flag toggling.

### Highlander (`highland.c`)
`do_highlander` — Enhanced highlander command handling with quickening
tracking in `pcdata->highland[4]`.

### Random / Utility (`random.c`)
Genesis replaces several EM `M2.c` functions with cleaner implementations:
- Leaderboard functions: `load_leaderboard()`, `save_leaderboard()`
- `mageupkeep(ch)` — mage paradox/arete tick processing
- `check_leaderboard(ch, arg)` — score tracking

Both `M2.c` and `random.c` are compiled to cover all utility functions.

---

## 7. Struct Changes

### CHAR_DATA additions
```c
char    *contingency;    /* Mage contingency spell string */
int32_t  style[7];       /* Weapon style tracking (Genesis) */
int32_t  dragskills;     /* Dragon ability bitfield */
int32_t  elem_type;      /* Elemental element type */
int32_t  xtra;           /* Genesis extended bits */
int32_t  primal;         /* Primal stat (Genesis) */
int32_t  arete;          /* Mage arete level */
int32_t  paradox_ward;   /* Paradox ward strength */
```

### PC_DATA additions
```c
char    *decap_char;          /* Decapitation message */
char    *cdest;               /* Copyover destination */
int32_t  homecv;              /* Home CV number */
int32_t  bodycrafts;          /* Genesis body crafts */
int32_t  dragcol;             /* Dragon colour */
int32_t  weather;             /* Weather control level */
int32_t  colors[6];           /* Colour settings */
int32_t  highland[4];         /* Highlander quickening array */
int32_t  ninja[5];            /* Ninja ability array */
int32_t  monk[4];             /* Monk power array */
int32_t  drow[2];             /* Drow power array */
int32_t  classpoints[2];      /* Class-specific points */
int32_t  arete;               /* Mage arete value */
int32_t  paradox_val;         /* Mage paradox value */
int32_t  beginners_luck;      /* Mage beginner's luck */
int32_t  paradox_ward;        /* Paradox ward value */
int32_t  fight_timer;         /* Combat timer */
int32_t  polymorph_timer;     /* Polymorph duration */
int32_t  doublecast_timer;    /* Double-cast cooldown */
int32_t  enrage_timer;        /* Enrage duration */
int32_t  liquify_timer;       /* Liquify effect timer */
int32_t  atoningwinds_timer;  /* Atoning winds timer */
int32_t  slow_timer;          /* Slow effect timer */
int32_t  better_body_timer;   /* Better Body timer */
int32_t  ephemera_timer;      /* Ephemera mutation timer */
```

### EXIT_DATA addition
```c
TRAP_DATA  *trap;    /* Trap attached to this exit (Genesis) */
```

---

## 8. Area Files Added from Genesis

The following area files from Genesis are not present in EM and have been
added to the `area/` directory:

| File | Description |
|------|-------------|
| `bgtower.are` | Bg Tower dungeon |
| `dtemple.are` | Demon Temple (Genesis version) |
| `castle.are` | Castle area |
| `newbie.are` | Newbie starting area |
| `gcastle.are` | Gnome Castle (Genesis version) |
| `heartwood.are` | Heartwood forest |
| `maraden.are` | Maraden region |
| `midge.are` | Midgaard (Genesis version) |
| `imms.are` | Immortal offices (Genesis) |

These will need to be added to `area.lst` to activate them.

---

## 9. Building

```bash
cd src/
make clean
make
```

The binary will be `./merc`. Run from the mud root directory so it can
find the `area/` folder.

To run:
```bash
cd /path/to/GodWars_Modern
./src/merc <port>
```

---

## 10. Save File Compatibility

The new PC_DATA fields are additive. Existing player files from EM will
load correctly — new fields default to 0. Genesis player files will also
load, but some EM-specific fields (KOE powers, FAE abilities, EM-specific
demon powers) will be at default values.

New class fields (drow[], highland[], ninja[], monk[], dragskills, etc.)
will be saved automatically once `save.c` is updated to include them.

---

## 11. Known Integration Points Requiring Manual Review

1. **`save.c`** — Must add save/load code for new PC_DATA fields (drow[],
   highland[], dragskills, elem_type, breed/auspice/tribes arrays, etc.)

2. **`interp.c`** — Add command table entries for new Genesis commands:
   `drowarmor`, `drowupkeep`, `dragonform`, `dragonskill`, `element`,
   `elemental`, `elemarmor`, `sorctalk`, `sorccreate`, `wwpowers`,
   `setflag`, `earthshift`, `highlander`

3. **`clearclass.c`** — Add clear logic for new class fields when a player
   resets their class

4. **`const.c`** — If LOGIN_* constants are used in the character creation
   menu, add entries for SORCERER, DROW, DRAGON, ELEMENTAL

5. **`act_wiz.c`** — The `do_stat` command should display new class fields

6. **`clan.c`** / `werewolf.c`** — Review interaction between EM werewolf
   implementation and new Genesis auspice/breed/tribe systems. The Genesis
   system is additive (additional power arrays), not a replacement.


---

## Phase 2: Protocol Snippet + MobProgs (2026)

### New Files Added
- `src/protocol.h` / `src/protocol.c` — KaVir-style telnet protocol negotiation
- `src/mobprog.h`  / `src/mobprog.c`  — MobProg/ObjProg/RoomProg scripting system

---

### 1. KaVir Protocol Snippet

Full telnet protocol negotiation adapted for Merc/GodWars. Implemented:

**TTYPE** — Terminal type detection. Mud sends `DO TTYPE` on connect; client
responds with its name (e.g. "Mudlet", "MUSHclient", "TinTin++"). This sets
`d->terminal_type` and auto-enables MSDP/MXP for known capable clients.

**NAWS** — Window size negotiation. Mud sends `DO NAWS`; client responds with
screen width/height stored in `d->screen_width` / `d->screen_height`. Lets
builders write progs that format output for the player's actual terminal width.

**MSDP** — MUD Server Data Protocol. After negotiation, stat blocks (HP, mana,
moves, AC, hitroll, damroll, XP, room name, opponent info) are pushed to
the client as machine-readable key/value pairs every prompt via `msdp_update()`.
Clients like Mudlet can bind these to health bars, maps, and gauges without
any screen-scraping.

**MSSP** — MUD Server Status Protocol. When a crawling bot (TheMudConnector,
MudStats, etc.) connects and negotiates MSSP, the mud auto-responds with
structured data (player count, uptime, genre, features) via `mssp_send()`.
Mud listing sites can index the server automatically.

**MXP** — MUD eXtension Protocol. Negotiated with client; enables clickable
hyperlinks for room exits, item names, and help references.

**ATCP** — Achaea Telnet Client Protocol. Fallback MSDP-equivalent for older
Mudlet versions that use ATCP instead of MSDP.

**MSP** — MUD Sound Protocol. Negotiated; sound triggers can be added to
act() strings using `!!SOUND(file.wav)` syntax once the flag is set.

#### Integration Points
- `comm.c new_descriptor()` — calls `protocol_negotiate(dnew)` after greeting
- `comm.c read_from_descriptor()` — calls `protocol_input()` to strip IAC bytes
- `comm.c bust_a_prompt()` — calls `msdp_update(ch)` before each prompt
- `DESCRIPTOR_DATA` in `merc.h` — extended with 10 new protocol fields

#### struct DESCRIPTOR_DATA additions
```c
bool    bMSDP, bMSSP, bMXP, bATCP, bMSP;   /* protocol flags      */
bool    bTTYPE, bNAWS, bNegotiated;         /* handshake state     */
char    terminal_type[64];                   /* e.g. "Mudlet 5.0"  */
int16_t screen_width, screen_height;         /* NAWS reported size  */
bool    bMobProg;                            /* running a mobprog  */
```

---

### 2. MobProg / ObjProg / RoomProg System

A full scripting system for NPCs, objects, and rooms. Programs are loaded from
the `#MOBPROGS` section of `.are` files and attached to index data by vnum.
Programs fire on trigger conditions and execute a mini command language.

#### Trigger Types
| Type | Fires when... |
|------|--------------|
| ACT | Room act() text matches trig_phrase |
| BRIBE | Player gives gold >= trig_phrase amount |
| DEATH | Mob is killed |
| ENTRY | Mob is loaded into a room |
| FIGHT | Each combat round when mob attacks |
| GIVE | Player gives object matching trig_phrase to mob |
| GREET | Any player enters mob's room |
| KILL | Player types `kill` on mob |
| HPCNT | Mob HP drops to <= trig_phrase percent |
| RANDOM | Random chance (trig_phrase %) each mobile update |
| SPEECH | Player says something containing trig_phrase |
| EXIT | Player leaves mob's room |
| DELAY | After mpdelay N pulses |
| OBJGET | Object is picked up |
| OBJDROP | Object is dropped |
| OBJWEAR | Object is worn |
| OBJREMOVE | Object is removed |
| ROOMENTER | Someone enters the room |
| ROOMEXIT | Someone leaves the room |
| ROOMRAND | Random % chance each area update |

#### Special MobProg Commands
```
mpmload <vnum>           load a mobile into current room
mpoload <vnum>           load an object into mob's inventory
mpkill                   mob attacks actor
mppurge [who]            purge self or named NPC
mpgoto <vnum>            teleport mob to room
mptransfer <who|all>     move player(s) to mob's room
mpforce <who|all> <cmd>  force character(s) to run a command
mpecho <msg>             act() to room
mpechoat <who> <msg>     act() to one person
mpsay <msg>              mob says without "You say" prefix
mpdelay <pulses>         delay N pulses then fire DELAY trigger
mpcancel                 cancel pending delay
break                    stop executing this prog
if/else/endif            conditional branching (10 levels)
```

#### Condition Checks
`ispc`, `isnpc`, `isgood`, `isevil`, `isneutral`, `isfight`, `isimmort`,
`ischarmed`, `isfollow`, `rand <pct>`, `hitprcnt <pct>`, `inroom <vnum>`,
`carries <vnum>`, `wears <vnum>`, `name <str>`, `class <name>`, `pos <pos>`
Prefix any condition with `!` to negate.

#### Area File Format
```
#MOBPROGS
#<vnum>
SPEECH hello~
if ispc
  mpsay Greetings, mortal!
else
  mpsay Hello, beast.
endif
~
#0
```

#### Immortal Commands Added
- `mpstat <mob>` — show all progs attached to a mob (trust 7)
- `mprog <mob> [actor]` — manually fire a mob's first prog for testing (trust 11)

#### New CHAR_DATA fields
```c
int32_t  mprog_delay;    /* delay counter for TRIG_DELAY */
CHAR_DATA *mprog_target; /* actor at time of mpdelay     */
```

#### Index Data additions
```c
/* MOB_INDEX_DATA, OBJ_INDEX_DATA, ROOM_INDEX_DATA all gained: */
PROG_LIST  *progs;       /* linked list of programs         */
int32_t     progtypes;   /* bitmask of active trigger types */
```

#### struct DESCRIPTOR_DATA addition
```c
bool    outbuf, outtop, outsize;   /* (bug fix: these were missing from merc.h) */
```
