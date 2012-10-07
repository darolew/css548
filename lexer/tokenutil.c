#include "tokenutil.h"

#define YMIN 257

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

const char *token_name(int token)
{
	return token_map[token - YMIN];
}
