#define ORGMODEPARSERC_IMPLEMENTATION

#include "../src/lib/orgmodeparser.h"

int main(int argc, char** argv) {

    mpc_result_t r;
    
    if (argc != 2) {
      printf("Usage: ./org-begin <orgmode.org>\n");
      exit(0);
    }
    
    mpc_parser_t* Ident          = mpc_new("ident");
    mpc_parser_t* Number         = mpc_new("number");
    mpc_parser_t* Character      = mpc_new("character");
    mpc_parser_t* String         = mpc_new("string");
    mpc_parser_t* Blanklines     = mpc_new("blanklines");
    mpc_parser_t* Stars          = mpc_new("stars");
    mpc_parser_t* Elements       = mpc_new("elements");
    mpc_parser_t* Expr           = mpc_new("expr");
    
    mpc_parser_t* OrgMode        = mpc_new("orgmode");
    
    
    mpc_err_t* err = mpca_lang(MPCA_LANG_PREDICTIVE,
    " ident     : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                           \n"
    " number    : /[0-9]+/ ;                                           \n"
    " character : /'.'/ ;                                              \n"
    " string    : /\"(\\\\.|[^\"])*\"/ ;                               \n"
    " blanklines: \t | \n | \r ;                                       \n"			     
    " stars     : /\\*{1,}\\s/ ;                                       \n"			       

    " elements  : <star> | blanklines ;                                \n"

    " expr      : <number> | <ident> | <character> | <string>          \n"
    "           | <elements> ;                                         \n"
			       
    " orgmode   : /^/ <expr> /$/ ;                                     \n"
    , Ident, Number, Character, String, Blanklines, Stars, Elements, Expr, OrgMode, NULL);

    
    if (mpc_parse("<stdin>", argv[1], OrgMode, &r)) {
        mpc_ast_print(r.output);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }
    
    mpc_cleanup(1, OrgMode);
    
    return 0;
}

