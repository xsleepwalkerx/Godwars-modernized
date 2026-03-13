/* Color definition for portable colors */

/* Change those to fit to your MUD */

/* if you do not have color at all, just change all to be "" */

/* Foreground colors */

#define RED		"#r"
#define BLUE		"#l"
#define GREEN		"#g"
#define BLACK		""
#define GREY		"#0" /* Merge repair: legacy dark grey token used by magic.c */
#define WHITE		"#w"
#define MAGENTA		"#p"
#define YELLOW		"#y"
#define CYAN		"#c"

/* Light (bright) foreground colors */
#define L_RED		"#R"
#define L_BLUE		"#L"
#define L_GREEN		"#G"
#define L_BLACK		""
#define L_WHITE		"#W"
#define L_MAGENTA	"#P"
#define L_YELLOW	"#Y"
#define L_CYAN		"#C"

/* Background colors */

#define RED_BG			"#R"
#define BLUE_BG			"#L"
#define GREEN_BG		"#G"
#define BLACK_BG		""
#define WHITE_BG		""
#define MAGENTA_BG		"#P"
#define YELLOW_BG		"#Y"
#define CYAN_BG			""

/* Dark (dim) foreground colors */
#define D_RED		"#r"
#define D_BLUE		"#l"
#define D_GREEN		"#g"
#define D_WHITE		"#w"
#define D_CYAN		"#c"
#define D_YELLOW	"#y"

/* Other codes */

#define BOLD			""
#define NO_COLOR		"" /* Default color */
#define BLINK       	        ""
#define REVERSE			""
#define TILDE         	        ""

/* Scale columns for color arrays */
#define SCALE_COLS	4

/* ADD_COLOUR macro: prepend color code to a buffer */
#define ADD_COLOUR(ch, buf, color)  \
    do { \
        char _tmp[MAX_STRING_LENGTH]; \
        snprintf(_tmp, sizeof(_tmp), "%s%s", (color), (buf)); \
        strncpy((buf), _tmp, sizeof(buf)-1); \
    } while(0)



/* Merge repair: restore status-bar color scaling macro expected by EM/Genesis UI code. */
#define COL_SCALE(_swh_str,_swh_ch,_swh_curr,_swh_max) \
    do { \
        if ((_swh_max) <= 0) snprintf((_swh_str), 16, "#w0#n"); \
        else { \
            int _pct = (((_swh_curr) * 100) / (_swh_max)); \
            const char *_col = (_pct >= 75) ? "#G" : (_pct >= 40) ? "#Y" : "#R"; \
            snprintf((_swh_str), 16, "%s%d#n", _col, (_swh_curr)); \
        } \
    } while (0)
