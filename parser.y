%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(const char *s);
extern int yylineno;
int posizione_attuale = 0;
%}

%union {
    char *str;
    int num;
}

%token <str> DATA CODICE NOME SQUADRA DATA_TAPPA CITTA TEMPO
%token <num> NUMERO_PETTORALE
%token <str> DATA_PREFIX CODICE_PREFIX NOME_PREFIX SQUADRA_PREFIX PETTORALE_PREFIX SEPARATORE_CICLISTI SEPARATORE_SEZIONE_1 SEPARATORE_SEZIONE_2 TRATTINO SEP_PETT_TEMPO PARENTESI_SX PARENTESI_DX FRECCIA
%type <str> intestazione ciclista tappa risultato
%%


start:
    intestazione SEPARATORE_SEZIONE_1 ciclisti SEPARATORE_SEZIONE_2 tappe
    ;

intestazione:
    DATA_PREFIX DATA {
        parseData($2);
    }
    ;

ciclisti: ciclista
        | ciclisti ciclista
    ;

ciclista:
    CODICE_PREFIX CODICE NOME_PREFIX NOME SQUADRA_PREFIX SQUADRA PETTORALE_PREFIX NUMERO_PETTORALE SEPARATORE_CICLISTI {
        insert($8, $4);
    }
    ;

tappe: tappa
      | tappe tappa
    ;

tappa:
    DATA_TAPPA TRATTINO CITTA TRATTINO CITTA TRATTINO risultati_tappa
    ;

risultati_tappa: risultato {posizione_attuale = 0;}
               | risultato FRECCIA risultati_tappa
    ;

risultato:
    PARENTESI_SX NUMERO_PETTORALE SEP_PETT_TEMPO TEMPO PARENTESI_DX {
        posizione_attuale++;
        aggiungi_tempo($2, $4, posizione_attuale);
    }
    ;

%%

void yyerror(const char *s) {
    printf("Errore: %s sulla riga %d\n", s, yylineno);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Impossibile aprire %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }

    if(yyparse()==0){
        stampa_classifica_giorno();
        stampa_classifica_parziale();
    }

    return 0;
}


