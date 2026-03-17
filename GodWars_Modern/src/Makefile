# ===========================================================================
#  GodWars Modernized — Makefile
#  Merged from EM (Eternal Malice) and Genesis codebases.
#  Requires: gcc with C99 support, libcrypt
# ===========================================================================

CC       = gcc
PROF     =
NOCRYPT  =

# C99 standard, all warnings, debug symbols, define MERC and linux compat.
# Remove -g and add -O2 for production builds.
C_FLAGS  = -std=c99 -Wall -Wextra -Wno-unused-parameter \
           -g $(PROF) $(NOCRYPT) \
           -DMERC -Dlinux -Dunix -DMEMWATCH

L_FLAGS  = -O $(PROF) -lcrypt

# ---------------------------------------------------------------------------
#  All object files.
#  EM base files + Genesis-exclusive additions.
# ---------------------------------------------------------------------------
O_FILES = \
	act_comm.o   act_info.o   act_move.o   act_obj.o    act_wiz.o   \
	bit.o        board.o      clan.o       clearclass.o comm.o      \
	const.o      core.o       db.o         fight.o      handler.o   \
	interp.o     magic.o      memwatch.o   more.o       olc.o       \
	rename.o     rot_wiz.o    save.o       signals.o    special.o   \
	string.o     update.o     wizutil.o                              \
	\
	kav_wiz.o    kav_info.o                                          \
	\
	monk.o       monk2.o      monk3.o                                \
	ninja.o      warlock.o    demon.o      fae.o        koe.o       \
	werewolf.o   rituals.o                                           \
	\
	fun.o        home.o       bank.o       freeze.o     bonus.o     \
	pretitle.o   auto_map.o   kav_fight.o                            \
	\
	M2.o         util.o                                              \
	\
	disc.o       dragon.o     drow.o       elemental.o  flag.o      \
	highland.o   mage.o       random.o     sorcerer.o   stance.o    \
	trap.o       wwauspice.o  wwbreed.o    wwtribe.o    \
	\
	protocol.o   mobprog.o  stubs.o                                   \
	trivia.o     auction.o  questmaster.o

# ---------------------------------------------------------------------------
TARGET = merc

$(TARGET): $(O_FILES)
	rm -f $(TARGET)
	$(CC) -o $(TARGET) $(O_FILES) $(L_FLAGS)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -rf *.o $(TARGET)

depend:
	makedepend -Y *.c 2>/dev/null

.PHONY: clean depend
