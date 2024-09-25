#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Ciclista *ciclisti = NULL;
char *data_voluta = NULL;

void swap(Ciclista *c1, Ciclista *c2){
    int tempPettorale = c1->numero_pettorale;
    char *tempNome = c1->nome;
    int tempTempoTotale = c1->tempo_totale;
    int tempTempoTappa = c1->tempo_tappa;

    c1->numero_pettorale = c2->numero_pettorale;
    c1->nome = c2->nome;
    c1->tempo_totale = c2->tempo_totale;
    c1->tempo_tappa = c2->tempo_tappa;

    c2->numero_pettorale = tempPettorale;
    c2->nome = tempNome;
    c2->tempo_totale = tempTempoTotale;
    c2->tempo_tappa = tempTempoTappa;
}

void insert(int numero_pettorale, char *nome) {

    if (find(numero_pettorale) != NULL) {
        return;
    }

    Ciclista *newCiclista = (Ciclista *) malloc(sizeof(Ciclista));
    newCiclista->numero_pettorale = numero_pettorale;
    newCiclista->nome = strdup(nome);
    newCiclista->tempo_totale = 0;
    newCiclista->tempo_tappa = 0;    
    newCiclista->next = ciclisti;
    ciclisti = newCiclista;
}

Ciclista *find(int numero_pettorale) {
    Ciclista *current = ciclisti;
    while (current != NULL) {
        if (current->numero_pettorale == numero_pettorale) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


void aggiungi_tempo(int numero_pettorale, char *tempo, unsigned int posizione) {
    Ciclista *ciclista = find(numero_pettorale);
    if (ciclista == NULL) {
        printf("Errore semantico: ciclista non trovato\n");
        exit(1);
    }

    int tempoSec = tempo_in_secondi(tempo);

    ciclista->tempo_tappa = tempoSec;
    ciclista->tempo_totale += tempoSec;
    switch (posizione) {
    case 1:
        ciclista->tempo_totale -= 60;
        break;
    case 2:
        ciclista->tempo_totale -= 30;
        break;
    case 3:
        ciclista->tempo_totale -= 10;
        break;
    default:
        break;
    }

    while(ciclista->next != NULL)
    {
        if (ciclista->tempo_totale > ciclista->next->tempo_totale)
        {
            swap(ciclista, ciclista->next);
        }
        ciclista = ciclista->next;
    }
}



void stampa_classifica_giorno() {
    Ciclista *sortedCiclisti = NULL;
    Ciclista *t = ciclisti;
    while(t != NULL)
    {
        
        Ciclista *tmp = (Ciclista *)malloc(sizeof(Ciclista));

        tmp->numero_pettorale = t->numero_pettorale;
        tmp->nome = t->nome;
        tmp->tempo_totale = t->tempo_totale;
        tmp->tempo_tappa = t->tempo_tappa;
        tmp->next = NULL;

        if (sortedCiclisti == NULL)
        {
            sortedCiclisti = tmp;
        } 
        else
        {
            for(Ciclista *t2 = sortedCiclisti; t2 != NULL; t2 = t2->next)
            {
                if (t2->next == NULL)
                {
                    t2->next = tmp;
                    break;
                }
                else if (t2->tempo_tappa > t2->next->tempo_tappa)
                {
                    swap(t2, t2->next);
                }
            }
        }

        t = t->next;
    }

    Ciclista *current = sortedCiclisti;
    printf("CLASSIFICA TAPPA %s\n", data_voluta);
    for (int i = 1; current != NULL; current = current->next, i++) {
        printf("%d. %-25s\t%s\n", i, current->nome, secondi_in_tempo(current->tempo_tappa));
    }
}

void stampa_classifica_parziale(){
    Ciclista *current = ciclisti;
        
    printf("\nCLASSIFICA PARZIALE\n");
    for (int i = 1; current != NULL; current = current->next, i++) {
        printf("%d. %-25s\t%s\n", i, current->nome, secondi_in_tempo(current->tempo_totale));
    }
}

void parseData(char *data) {
    data = strdup(data);
    char *giorno = strtok(data, "-/");
    char *mese = strtok(NULL, "-/");
    char *anno = strtok(NULL, "-");
    if (giorno != NULL && mese != NULL) {
        sprintf(data, "%s-%s", giorno, mese);
    }
    data_voluta = strdup(data);
}

int tempo_in_secondi(char *tempo) {
    int ore, minuti, secondi;
    sscanf(tempo, "%d:%d:%d", &ore, &minuti, &secondi);
    return ore * 3600 + minuti * 60 + secondi;
}

char *secondi_in_tempo(int secondi) {
    int ore = secondi / 3600;
    int minuti = (secondi % 3600) / 60;
    int sec = secondi % 60;
    char *tempo = (char *) malloc(9);
    sprintf(tempo, "%02d:%02d:%02d", ore, minuti, sec);
    return tempo;
}