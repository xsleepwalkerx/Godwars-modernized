/*
 * monk.h — Monk subsystem header (GodWars Modernized)
 * Updated to use C99 standard types.
 */
#ifndef MONK_H
#define MONK_H

/* Monk ability indices into ch->monkab[] */
#define MONK_SPEED          0
#define MONK_STRENGTH       1
#define MONK_AGILITY        2
#define MONK_SPIRIT         3

/* Monk stance bits in ch->monkstuff */
#define MONK_VANISH         1
#define MONK_DEATH          2
#define MONK_BRACE          4
#define MONK_COUNTER        8
#define MONK_HEAL           16
#define MONK_DODGE          32
#define MONK_MEDITATE       64
#define MONK_IRON_SKIN      128
#define MONK_AWARE          256
#define MONK_CONCENTRATE    512
#define MONK_DEATHDODGE     1024

/* Chi array indices */
#define CHI_CURRENT         0
#define CHI_MAX             1

/* Monk mantra levels */
#define MANTRA_NONE         0
#define MANTRA_1            1
#define MANTRA_2            2
#define MANTRA_3            3
#define MANTRA_4            4
#define MANTRA_5            5
#define MANTRA_6            6
#define MANTRA_7            7
#define MANTRA_8            8
#define MAX_MANTRA          8

#endif /* MONK_H */
