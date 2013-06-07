#define whitespaceskip s += strspn(s, "\n\t \r")
#define elementskip *s=='"' ? (++s, s += strcspn(s, "\"\n\0"), s += *s=='"') : s += strcspn(s, "\n\t \0")
enum { cmdmain, cmdotskok, cmdimpuls };
enum { testing, optimizing, debuging };
enum { withtime, settitle };
enum { whwnd,whlog,whpro,whper,whcmd };
#define MODE_TRADES 0
#define MODE_HISTORY 1
#define OP_BUY 0
#define OP_SELL 1
#define OP_BUYLIMIT 2
#define OP_SELLLIMIT 3
#define OP_BUYSTOP 4
#define OP_SELLSTOP 5
#define OP_SL 0
#define OP_TP 1
#define OP_CLOSE 2
#define OP_DELETE 3
#define PRICE_CLOSE 0
#define PRICE_OPEN 1
#define PRICE_HIGH 2
#define PRICE_LOW 3
#define PRICE_MEDIAN 4
#define PRICE_TYPICAL 5
#define PRICE_WEIGHTED 6
#define MODE_MAIN 0
#define MODE_PLUSDI 1
#define MODE_MINUSDI 2
#define OTSKOK 1
#define IMPULS 2
