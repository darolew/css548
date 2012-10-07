/*
 * CSS 548; Autumn 2012
 * Aaron Hoffer and Daniel Lewis
 */
#include <assert.h>
#include "tokenutil.h"

/* The minimum token value */
#define YMIN 257

/*
 * A list of all tokens. Listed in the same order as in tokenconsts.h, with
 * no omissions or duplications.
 */
static const char *token_map[] = {
	"yand", "yarray", "yassign", "ybegin", "ycaret", "ycase", "ycolon",
	"ycomma", "yconst", "ydispose", "ydiv", "ydivide", "ydo", "ydot", 
	"ydotdot", "ydownto", "yelse", "yend", "yequal", "yfalse", "yfor", 
	"yfunction", "ygreater", "ygreaterequal", "yident", "yif", "yin", 
	"yleftbracket", "yleftparen", "yless", "ylessequal", "yminus", 
	"ymod", "ymultiply", "ynew", "ynil", "ynot", "ynotequal", "ynumber",
	"yof", "yor", "yplus", "yprocedure", "yprogram", "yread", "yreadln", 
	"yrecord", "yrepeat", "yrightbracket", "yrightparen", "ysemicolon", 
	"yset", "ystring", "ythen", "yto", "ytrue", "ytype", "yuntil",
	"yvar", "ywhile", "ywrite", "ywriteln", "yunknown"
};

/*
 * Convert a token value into a token name.
 */
const char *token_name(int token)
{
	assert(token >= YMIN);
	return token_map[token - YMIN];
}
