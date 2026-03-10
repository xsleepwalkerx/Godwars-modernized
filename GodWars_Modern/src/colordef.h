/* Color definition for portable colors */

/* Change those to fit to your MUD */

/* if you do not have color at all, just change all to be "" */

/* Foreground colors */

#define RED		"#r"
#define BLUE		"#l"
#define GREEN		"#g"
#define BLACK		""
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

