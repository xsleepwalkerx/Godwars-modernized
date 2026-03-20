/*
 * warlock.h — Warlock subsystem header (GodWars Modernized)
 * Updated to use C99 standard types.
 */
#ifndef WARLOCK_H
#define WARLOCK_H

/* Warlock power flags */
#define WAR_HELLFIRE        1
#define WAR_CORRUPTION      2
#define WAR_TORMENT         4
#define WAR_SHADOWBLAST     8
#define WAR_BLIGHT          16
#define WAR_CURSE           32
#define WAR_DREAD           64
#define WAR_DRAIN           128
#define WAR_WRAITHFORM      256
#define WAR_BIND            512

/* Warlock focus indices into ch->focus[] */
#define FOCUS_CURRENT       0
#define FOCUS_MAX           1

#endif /* WARLOCK_H */
