#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Ciclista {
    int numero_pettorale;
    char *nome;
    int tempo_totale;
    int tempo_tappa;

    struct Ciclista *next;
} Ciclista;

void insert(int numero_pettorale, char *nome);
Ciclista *find(int numero_pettorale);
void aggiungi_tempo(int numero_pettorale, char *tempo, unsigned int posizione);


void stampa_classifica_giorno();
void stampa_classifica_parziale();

void parseData(char *data);
int tempo_in_secondi(char *tempo);
char *secondi_in_tempo(int secondi);
#endif