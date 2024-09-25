win_flex lexer.fl
win_bison -d parser.y
gcc lex.yy.c parser.tab.c symbol_table.c