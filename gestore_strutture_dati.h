//
// Created by ezio on 19/08/20.
//

#include "strutture_dati.h"
#include <stdlib.h>
#include <stdio.h>


//-------------------------------------------------------
//funzioni per la gestione dei clienti
//-------------------------------------------------------

//genera il numero di oggetti secondo un esponenziale
//a media.. vedere relazione
int get_num_oggetti(){

    //TODO chiamare esponenziale con media campionaria della relazione

    return 5;
}

//gerera ora di arrivo del cliente, in base all'orario
int genera_arrivo(int ora){

    int media_arr = 0;

    if(ore_6 < ora && ora <= ore_7){
        media_arr = arrivi_6_7;

    }else if(ore_7 < ora && ora <= ore_8){
        media_arr = arrivi_7_8;

    }else if(ore_8 < ora && ora <= ore_9){
        media_arr = arrivi_8_9;

    }else if(ore_9 < ora && ora <= ore_10){
        media_arr = arrivi_9_10;

    }else if(ore_10 < ora && ora <= ore_11){
        media_arr = arrivi_10_11;

    }else if(ore_11 < ora && ora <= ore_12){
        media_arr = arrivi_11_12;

    }else if(ore_12 < ora && ora <= ore_13){
        media_arr = arrivi_12_13;

    }else if(ore_13 < ora && ora <= ore_14){
        media_arr = arrivi_13_14;

    }else if(ore_14 < ora && ora <= ore_15){
        media_arr = arrivi_14_15;

    }else if(ore_15 < ora && ora <= ore_16){
        media_arr = arrivi_15_16;

    }else if(ore_16 < ora && ora <= ore_17){
        media_arr = arrivi_16_17;

    }else if(ore_17 < ora && ora <= ore_18){
        media_arr = arrivi_17_18;

    }else if(ore_18 < ora && ora <= ore_19){
        media_arr = arrivi_18_19;

    }else if(ore_19 < ora && ora <= ore_20){
        media_arr = arrivi_19_20;

    }else if(ore_20 < ora && ora <= ore_21){
        media_arr = arrivi_20_21;

    }else if(ore_21 < ora && ora <= ore_22){
        media_arr = arrivi_21_22;

    }

    //TODO chiamare esponenziale con media_arr

    return 30;

}

struct cliente* genera_cliente(int ora){

    struct cliente *cli = (struct cliente *)malloc(sizeof(struct cliente));
    cli->num_oggetti = get_num_oggetti();
    cli->in_fila = ora + genera_arrivo(ora);

    return cli;
}


//-------------------------------------------------------
//funzione per la gestione delle file (fila_cassa)
//-------------------------------------------------------

struct fila_cassa *crea_fila(){

    struct fila_cassa *fc = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
    fc->cliente_in_fila = NULL;

    return fc;
}

int aggiungi_cliente_infila(struct fila_cassa *fila, struct cliente *cli){

    fila->cliente_in_fila = cli;
    fila->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
    fila->next->cliente_in_fila = NULL;

    return 0;
}

int lunghezza_fila(struct fila_cassa *fila){

    int l = 0;

    for(struct fila_cassa *f = fila; f->cliente_in_fila != NULL; f = f->next ){
        l++;
    }

    return l;
}

void rimuovi_cliente_infila(struct fila_cassa *fila){
    //necessario ?
}


//-------------------------------------------------------
//funzione per la gestione delle casse
//-------------------------------------------------------

//ritorna delle casse, ovvero un determinato numero di file
struct casse *genera_set_casse(int num_casse){

    struct casse *casse = (struct casse *)malloc(sizeof(struct casse));

    //crea le file per ogni cassa
    int i = 0;
    struct casse *c;
    for(c = casse; ;c = c->next) {
        c->fila_cassa = crea_fila();
        i++;
        if (i < num_casse) {
            c->next = (struct casse *) malloc(sizeof(struct casse));
        }else{
            c->next = NULL;
            break;
        }

    }

    return casse;
}

int aggiungi_cassa(struct casse *casse){

    struct casse *c = casse;
    while(1){

        if(c->next == NULL){
            c->next = genera_set_casse(1);
            break;
        }
    }

    casse->fila_cassa = crea_fila();
    casse->next = (struct casse *)malloc(sizeof(struct casse));

    return 0;
}

int numero_casse(struct casse *casse){

    int l = 0;
    for(struct casse *c = casse; c->next != NULL; c = c->next ){
        l++;
    }
    return l;
}

void rimuovi_cassea(struct casse *casse){
    //necessario ?
}


//-------------------------------------------------------
//funzione per la gestione delle configurazioni di cassa
//-------------------------------------------------------

//aggiunge una configurazione di un dato tipo alle configurazioni attive (VARIABILE GLOBALE in costanti.h)
int aggiungi_configurazione_cassa(int tipo, int numero_casse){

    if((tipo != condivisa && tipo != pseudo_casuale && tipo != selettiva && tipo != mista) || numero_casse == 0){
        printf("Input errati\n");
        return -1;
    }

    struct config_cassa_attive *current = config_attive;

    if(current != NULL) {
        do{
            current = current->next;
        }while(current != NULL);
    }

    current = (struct config_cassa_attive *)malloc(sizeof(struct config_cassa_attive));
    current->next = NULL;
    current->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
    struct config_cassa *nuova_configurazione = current->configurazione_cassa;

    int legg = 0, med = 0, pes = 0;

    switch(tipo){
        case condivisa:

            nuova_configurazione->tipo = condivisa;
            nuova_configurazione->casse = genera_set_casse(numero_casse);
            nuova_configurazione->fila_condivisa = NULL;

            break;
        case pseudo_casuale:

            //gestire poi in modo che ogni cassa generata gestisca un cliente alla volta,
            //prelevandolo dalla fila condivisa
            nuova_configurazione->tipo = condivisa;
            nuova_configurazione->casse = genera_set_casse(numero_casse);
            nuova_configurazione->fila_condivisa = crea_fila();

            break;

        case selettiva:
            //incremento in modo prioridatio dalla cassa leggera alla pesante
            //il numero di casse date loro a disposizione
            do{
                legg++;
                if(numero_casse-legg-med-pes == 0){
                    break;
                }
                med++;
                if(numero_casse-legg-med-pes == 0){
                    break;
                }
                pes++;
                if(numero_casse-legg-med-pes == 0) {
                    break;
                }
            }while(1);

            nuova_configurazione->tipo = selettiva_leggera;
            nuova_configurazione->casse = genera_set_casse(legg);
            nuova_configurazione->fila_condivisa = NULL;

            if(med > 0) {
                current->next = (struct config_cassa_attive *)malloc(sizeof(struct config_cassa_attive));
                current = current->next;

                current->next = NULL;
                current->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
                nuova_configurazione = current->configurazione_cassa;

                nuova_configurazione->tipo = selettiva_media;
                nuova_configurazione->casse = genera_set_casse(med);
                nuova_configurazione->fila_condivisa = NULL;
            }

            if(pes > 0){
                current->next = (struct config_cassa_attive *)malloc(sizeof(struct config_cassa_attive));
                current = current->next;

                current->next = NULL;
                current->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
                nuova_configurazione = current->configurazione_cassa;

                nuova_configurazione->tipo = selettiva_pesante;
                nuova_configurazione->casse = genera_set_casse(pes);
                nuova_configurazione->fila_condivisa = NULL;
            }

            break;

        case mista:

            //TODO

            break;
    }

    return 0;

}


//EXTRA, dopo eventi e test
void aggiungi_configurazione_selettiva_custom(){

}













//-------------------------------------------------------
//funzione per la gestione degli eventi
//-------------------------------------------------------



















