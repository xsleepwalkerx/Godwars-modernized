/*
 * mage.h — Mage/Sorcerer subsystem header (GodWars Modernized, from Genesis)
 * Updated to use C99 standard types.
 */
#ifndef MAGE_H
#define MAGE_H

/* Mage sphere indices — also defined in merc.h, kept here for clarity */
#define SPHERE_CORRESPONDENCE   0
#define SPHERE_LIFE             1
#define SPHERE_PRIME            2
#define SPHERE_ENTROPY          3
#define SPHERE_MIND             4
#define SPHERE_SPIRIT           5
#define SPHERE_FORCES           6
#define SPHERE_MATTER           7
#define SPHERE_TIME             8
#define MAX_SPHERE              9

/* Paradox severity */
#define PARADOX_BACKLASH        1
#define PARADOX_WARP            2
#define PARADOX_SPIRIT          3
#define PARADOX_FLAW            4
#define PARADOX_STORM           5

/* Quintessence (prime) indices into ch->quint[] */
#define AVATAR                  0
#define BODY                    1
#define QUINT_CURRENT           0
#define QUINT_MAX               1
#define QUINT_STORED            2

/* Advanced mage spell types */
#define MAGE_DAMAGE             1
#define MAGE_AFFECT             2
#define MAGE_SUMMON             3
#define MAGE_PORTAL             4
#define MAGE_WARD               5

#endif /* MAGE_H */
