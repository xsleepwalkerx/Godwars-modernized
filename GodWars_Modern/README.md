# GodWars Modernized

A modernized GodWars MUD codebase merging two codebases:
- **EM (Eternal Malice)** — the primary base (~135K lines)
- **Genesis** — source of new classes and subsystems (~80K lines)

## New Features (from Genesis)

| System | Files | Description |
|--------|-------|-------------|
| Dragon class | `dragon.c` | Shapeshifting dragons with breath weapons, wings, horns, claws |
| Drow class | `drow.c` | Dark elves with spider/shadow/web abilities |
| Sorcerer class | `sorcerer.c` | Chaos mages with dynamic spell creation |
| Elemental class | `elemental.c` | Beings of pure elemental energy (Fire/Water/Earth/Wind/etc.) |
| WW Auspice | `wwauspice.c` | Ragabash/Theurge/Philodox/Galliard/Ahroun moon gifts |
| WW Breed | `wwbreed.c` | Homid/Lupus/Metis breed-specific powers |
| WW Tribe | `wwtribe.c` | 13 tribe gift trees |
| Weapon Style | `stance.c` | Unarmed/Natural/One-hand/Shield/Two-hand/Dual-wield styles |
| Trap System | `trap.c` | Traps on room exits |
| Highlander | `highland.c` | Enhanced highlander quickening |
| Flag System | `flag.c` | Granular immortal flag control |

## Building

```bash
cd src/
make
```

Requires `gcc` with C99 support and `libcrypt`.

## Source Tree

```
GodWars_Modern/
├── README.md
├── CHANGES.md          ← Detailed integration notes
├── area/               ← Area files (EM + Genesis additions)
└── src/
    ├── Makefile
    ├── merc.h          ← Master header (merged, modernized)
    ├── monk.h
    ├── warlock.h
    ├── mage.h
    ├── memwatch.h
    ├── colordef.h
    ├── board.h
    │
    │   [EM base files]
    ├── act_comm.c  act_info.c  act_move.c  act_obj.c  act_wiz.c
    ├── auto_map.c  bank.c      bit.c       board.c     bonus.c
    ├── clan.c      clearclass.c comm.c     const.c     core.c
    ├── db.c        demon.c     fae.c       fight.c     freeze.c
    ├── fun.c       handler.c   home.c      interp.c    kav_fight.c
    ├── kav_info.c  kav_wiz.c   koe.c       M2.c        magic.c
    ├── memwatch.c  monk.c      monk2.c     monk3.c     more.c
    ├── ninja.c     olc.c       pretitle.c  rename.c    rituals.c
    ├── rot_wiz.c   save.c      signals.c   special.c   string.c
    ├── update.c    util.c      warlock.c   werewolf.c  wizutil.c
    │
    │   [Genesis additions]
    ├── disc.c          ← Extra vampire discipline powers
    ├── dragon.c        ← Dragon class
    ├── drow.c          ← Drow class
    ├── elemental.c     ← Elemental class
    ├── flag.c          ← Flag management
    ├── highland.c      ← Enhanced highlander
    ├── mage.c          ← Enhanced mage system
    ├── random.c        ← Utility / leaderboard (replaces parts of M2.c)
    ├── sorcerer.c      ← Sorcerer class
    ├── stance.c        ← Weapon style system
    ├── trap.c          ← Trap system
    ├── wwauspice.c     ← Werewolf auspice powers
    ├── wwbreed.c       ← Werewolf breed powers
    └── wwtribe.c       ← Werewolf tribe gifts
```

## License

This code is derived from DIKU Mud (1990), Merc Diku Mud (1992-1993), and
GodWars (1994-1996, Richard Woolcock). See `area/license.doc` for terms.

