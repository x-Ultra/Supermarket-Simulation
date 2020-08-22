//
// Created by ezio on 19/08/20.
//

#include "strutture_dati.h"
#include <math.h>
#include <string.h>

void stampa_num_eventi(int num);

//-------------------------------------------------------
//funzioni utili
//-------------------------------------------------------

double Exponential(double media){

    double rand = (double)random()/RAND_MAX;

    return -media*log(1.0-rand);
}


char *secondi_ora(int secondi){

    int ora = secondi/(60*60);
    int minuti = (secondi-ora*60*60)/60;
    int secs = secondi-ora*60*60-minuti*60;


    char *ora_str = malloc(3);
    char *min_str = malloc(3);
    char *sec_str = malloc(3);
    char *all_str = malloc(10);

    if(minuti == 0){
        sprintf(min_str, "00");
    }else if(minuti < 10){
        sprintf(min_str, "0%d", minuti);
    }else{
        sprintf(min_str, "%d", minuti);
    }

    if(secs == 0){
        sprintf(sec_str, "00");
    }else if(secs < 10){
        sprintf(sec_str, "0%d", secs);
    }else{
        sprintf(sec_str, "%d", secs);
    }

    if(ora < 10){
        sprintf(ora_str, "0%d", ora);
    }else{
        sprintf(ora_str, "%d", ora);
    }

    sprintf(all_str, "%s:%s:%s", ora_str, min_str, sec_str);

    return all_str;
}



//-------------------------------------------------------
//funzioni per la gestione dei clienti
//-------------------------------------------------------

//genera il numero di oggetti secondo un esponenziale
//a media.. vedere relazione
int get_num_oggetti(){

    return (int)Exponential(Xn);
}

//un cliente è generato a partire da un evento di tipo arrivo,
//per la generazione è passata l'ora associato all'evento di arrivo.
struct cliente* genera_cliente(int ora_evento_arrivo){

    struct cliente *cli = (struct cliente *)malloc(sizeof(struct cliente));
    cli->num_oggetti = get_num_oggetti();
    cli->in_fila = ora_evento_arrivo;

    return cli;
}


//-------------------------------------------------------
//funzione per la gestione delle file (fila_cassa)
//-------------------------------------------------------

struct fila_cassa *crea_fila(){

    struct fila_cassa *fc = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
    fc->cliente_in_fila = NULL;
    fc->next = NULL;

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
    struct fila_cassa *f = fila;

    while(f->cliente_in_fila != NULL){
        l++;
        if(f->next == NULL){
            break;
        }
        f = f->next;
    }

    return l;
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


//possibilità di aggiungere una cassa ad una configurazione esistente
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
    for(struct casse *c = casse; c != NULL; c = c->next ){
        l++;
    }
    return l;
}


//possibilità di rimuovere una cassa ad una configurazione esistente
void rimuovi_cassa(struct casse *casse){
    //necessario ?
}


//-------------------------------------------------------
//funzione per la gestione delle configurazioni di cassa
//-------------------------------------------------------

//aggiunge una configurazione di un dato tipo alle configurazioni attive (VARIABILE GLOBALE )
//casse_casuali_in_mista rappresenta quante casse delle 'numero_casse' sono dedicate alla
//configurazione pseudo casuale, il resto andranno alla selettiva.
//TODO aggiungere alla relazione ?
//usando tale funzione possiamo 'eliminare la configurazione di cassa mista', in quanto basterebbe aggiungere
//una configurazione pseudocasuale e poi una selettiva. Potremmo addirittura fare configurazioni
//non esposte nella relazione, come pseudocasuale + selettiva, pseudo casuale + random.....
int aggiungi_configurazione_cassa(int tipo, int numero_casse, int casse_casuali_in_mista){

    if((tipo != condivisa && tipo != pseudo_casuale && tipo != selettiva && tipo != mista) || numero_casse == 0){
        printf("Input errati\n");
        return -1;
    }

    if( tipo == mista ){

        if(numero_casse-casse_casuali_in_mista <= 0){
            printf("Input errati\n");
            return -1;
        }

        aggiungi_configurazione_cassa(pseudo_casuale, casse_casuali_in_mista, -1);
        aggiungi_configurazione_cassa(selettiva, numero_casse-casse_casuali_in_mista, -1);

        printf("Configurazione di cassa mista aggiunta\n");

        return 0;

    }

    struct config_cassa_attive **current = &config_attive;

    if(*current != NULL) {
        do{
            current = &(((struct config_cassa_attive *)*current)->next);
        }while(*current != NULL);
    }

    *current = (struct config_cassa_attive *)malloc(sizeof(struct config_cassa_attive));

    ((struct config_cassa_attive *)*current)->next = NULL;
    ((struct config_cassa_attive *)*current)->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
    struct config_cassa **nuova_configurazione = &((struct config_cassa_attive *)*current)->configurazione_cassa;


    int legg = 0, med = 0, pes = 0;

    switch(tipo){
        case condivisa:

            ((struct config_cassa *)*nuova_configurazione)->tipo = condivisa;
            ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(numero_casse);
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = crea_fila();

            printf("Configurazione di cassa condivisa aggiunta\n");

            break;
        case pseudo_casuale:

            //gestire poi in modo che ogni cassa generata gestisca un cliente alla volta,
            //prelevandolo dalla fila condivisa
            ((struct config_cassa *)*nuova_configurazione)->tipo = pseudo_casuale;
            ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(numero_casse);
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = NULL;

            printf("Configurazione di cassa pseudo-casuale aggiunta\n");

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

            ((struct config_cassa *)*nuova_configurazione)->tipo = selettiva;
            ((struct config_cassa *)*nuova_configurazione)->sotto_tipo = selettiva_leggera;
            ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(legg);
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = NULL;

            printf("Configurazione di cassa selettiva-leggera aggiunta\n");

            if(med > 0) {
                (((struct config_cassa_attive *)*current)->next) = (struct config_cassa_attive *) malloc(sizeof(struct config_cassa_attive));
                current = &(((struct config_cassa_attive *)*current)->next);

                ((struct config_cassa_attive *)*current)->next = NULL;
                ((struct config_cassa_attive *)*current)->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
                nuova_configurazione = &((struct config_cassa_attive *)*current)->configurazione_cassa;

                ((struct config_cassa *)*nuova_configurazione)->tipo = selettiva;
                ((struct config_cassa *)*nuova_configurazione)->sotto_tipo = selettiva_media;
                ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(med);
                ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = NULL;

                printf("Configurazione di cassa selettiva-media aggiunta\n");
            }

            if(pes > 0){
                (((struct config_cassa_attive *)*current)->next) = (struct config_cassa_attive *) malloc(sizeof(struct config_cassa_attive));
                current = &(((struct config_cassa_attive *)*current)->next);

                ((struct config_cassa_attive *)*current)->next = NULL;
                ((struct config_cassa_attive *)*current)->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
                nuova_configurazione = &((struct config_cassa_attive *)*current)->configurazione_cassa;

                ((struct config_cassa *)*nuova_configurazione)->tipo = selettiva;
                ((struct config_cassa *)*nuova_configurazione)->sotto_tipo = selettiva_pesante;
                ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(med);
                ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = NULL;

                printf("Configurazione di cassa selettiva-pesante aggiunta\n");
            }
            break;

    }

    return 0;

}


//ESPERIMENTO, se non funge, eliminare
//Funzione che permette di introdurre una cassa selettiva con specifiche a grana fina, ovvero (eg.) possibilità di inserire
//3 casse leggere con una fila condivisa e 2 pesanti con file separate ecc....
//qui ci si può sbizzarrire nella fase di sperimentazione
int aggiungi_configurazione_selettiva_custom(int num_casse_leggere, int num_casse_medie, int num_casse_pesanti,
                                                int fila_leggera_condivisa, int fila_media_condivisa, int fila_pesante_condivisa){

    struct config_cassa_attive **current = &config_attive;

    if(*current != NULL) {
        do{
            current = &(((struct config_cassa_attive *)*current)->next);
        }while(*current != NULL);
    }

    *current = (struct config_cassa_attive *)malloc(sizeof(struct config_cassa_attive));

    ((struct config_cassa_attive *)*current)->next = NULL;
    ((struct config_cassa_attive *)*current)->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
    struct config_cassa **nuova_configurazione = &((struct config_cassa_attive *)*current)->configurazione_cassa;

    if(num_casse_leggere != 0){

        ((struct config_cassa *)*nuova_configurazione)->tipo = selettiva;
        ((struct config_cassa *)*nuova_configurazione)->sotto_tipo = selettiva_leggera;
        ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(num_casse_leggere);
        if(fila_leggera_condivisa) {
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = crea_fila();
        }else {
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = NULL;
        }
    }

    if(num_casse_medie != 0){
        (((struct config_cassa_attive *)*current)->next) = (struct config_cassa_attive *) malloc(sizeof(struct config_cassa_attive));
        current = &(((struct config_cassa_attive *)*current)->next);

        ((struct config_cassa_attive *)*current)->next = NULL;
        ((struct config_cassa_attive *)*current)->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
        nuova_configurazione = &((struct config_cassa_attive *)*current)->configurazione_cassa;

        ((struct config_cassa *)*nuova_configurazione)->tipo = selettiva;
        ((struct config_cassa *)*nuova_configurazione)->sotto_tipo = selettiva_media;
        ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(num_casse_medie);
        if(fila_media_condivisa) {
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = crea_fila();
        }else {
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = NULL;
        }
    }

    if(num_casse_pesanti != 0){
        (((struct config_cassa_attive *)*current)->next) = (struct config_cassa_attive *) malloc(sizeof(struct config_cassa_attive));
        current = &(((struct config_cassa_attive *)*current)->next);

        ((struct config_cassa_attive *)*current)->next = NULL;
        ((struct config_cassa_attive *)*current)->configurazione_cassa = (struct config_cassa *) malloc(sizeof(struct config_cassa));
        nuova_configurazione = &((struct config_cassa_attive *)*current)->configurazione_cassa;

        ((struct config_cassa *)*nuova_configurazione)->tipo = selettiva;
        ((struct config_cassa *)*nuova_configurazione)->sotto_tipo = selettiva_pesante;
        ((struct config_cassa *)*nuova_configurazione)->casse = genera_set_casse(num_casse_pesanti);
        if(fila_pesante_condivisa) {
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = crea_fila();
        }else {
            ((struct config_cassa *)*nuova_configurazione)->fila_condivisa = NULL;
        }
    }

    return 0;

}



//-------------------------------------------------------
//funzione per la gestione degli eventi
//-------------------------------------------------------
#include <unistd.h>
int aggiungi_evento(int tipo, int ora_evento, struct fila_cassa *fila){

    struct lista_eventi **current = &eventi;

    struct evento *nuovo_evento = (struct evento *)malloc(sizeof(struct evento));
    nuovo_evento->tipo = tipo;
    if(tipo == servito){
        nuovo_evento->fila = fila;
    }
    nuovo_evento->tempo = ora_evento;

    //se la lista di eventi è vuota, mettilo per primo.
    if(eventi == NULL){
        eventi = (struct lista_eventi *)malloc(sizeof(struct lista_eventi));
        eventi->evento = nuovo_evento;
        eventi->next = NULL;
        eventi->prev = NULL;

        return 0;
    }

    //altrimenti cerca dove metterlo.
    struct lista_eventi *new_el = (struct lista_eventi *)malloc(sizeof(struct lista_eventi));
    new_el->next = NULL;
    new_el->prev = NULL;
    new_el->evento = nuovo_evento;

    do{

        //printf("Nel do\n");

        if(((struct lista_eventi *)*current)->evento->tempo <= ora_evento){

            //printf("Ora del nuovo evento maggiore del corrente: (corrente) %s vs %s\n", secondi_ora(ora_evento), secondi_ora(((struct lista_eventi *)*current)->evento->tempo));

            if(((struct lista_eventi *)*current)->next == NULL){

                //printf("Arrivato all'ultimo evento\n");

                ((struct lista_eventi *)*current)->next = new_el;
                new_el->prev = ((struct lista_eventi *)*current);

                return 0;
            }else{

                current = &((struct lista_eventi *)*current)->next;
                //printf("Continuo\n");

                continue;
            }
        }else{

            //printf("Ora del nuovo evento minore del corrente: (corrente) %s vs %s\n", secondi_ora(ora_evento), secondi_ora(((struct lista_eventi *)*current)->evento->tempo));


            //caso in cui lo devo inserire in testa.
            if(((struct lista_eventi *)*current)->prev == NULL){

                //printf("Inserisco in testa\n");

                new_el->next = ((struct lista_eventi *)*current);
                ((struct lista_eventi *)*current)->prev = new_el;

                eventi = new_el;

                return 0;
            //caso in cui lo devo inserire in mezzo.
            }else{

                //printf("Inserisco in mezzo\n");
                //TODO in realta' non li inserisce, aggiusta qui.

                new_el->prev = ((struct lista_eventi *)*current)->prev;
                new_el->next = ((struct lista_eventi *)*current);

                ((struct lista_eventi *)*current)->prev = new_el;

                stampa_num_eventi(10);
                sleep(1);

                return 0;
            }
        }

    }while(1);



}

//gerera ora di arrivo, usato per generare a monte tutti gli arrivi dei clienti,
//e dunque tutti gli eventi di tipo 'arrivo'. Ciascun evento sarà associato ad un orario
//che verrà usato per la creazione del cliente.
double genera_arrivo(int ora, int giorno_settimana){

    int media_arr = 0;

    //TODO finire giorni

    switch (giorno_settimana){

        case lun:
            if(ore_6 <= ora && ora <= ore_7){
                media_arr = arrivi_lun_6_7;

            }else if(ore_7 < ora && ora <= ore_8){
                media_arr = arrivi_lun_7_8;

            }else if(ore_8 < ora && ora <= ore_9){
                media_arr = arrivi_lun_8_9;

            }else if(ore_9 < ora && ora <= ore_10){
                media_arr = arrivi_lun_9_10;

            }else if(ore_10 < ora && ora <= ore_11){
                media_arr = arrivi_lun_10_11;

            }else if(ore_11 < ora && ora <= ore_12){
                media_arr = arrivi_lun_11_12;

            }else if(ore_12 < ora && ora <= ore_13){
                media_arr = arrivi_lun_12_13;

            }else if(ore_13 < ora && ora <= ore_14){
                media_arr = arrivi_lun_13_14;

            }else if(ore_14 < ora && ora <= ore_15){
                media_arr = arrivi_lun_14_15;

            }else if(ore_15 < ora && ora <= ore_16){
                media_arr = arrivi_lun_15_16;

            }else if(ore_16 < ora && ora <= ore_17){
                media_arr = arrivi_lun_16_17;

            }else if(ore_17 < ora && ora <= ore_18){
                media_arr = arrivi_lun_17_18;

            }else if(ore_18 < ora && ora <= ore_19){
                media_arr = arrivi_lun_18_19;

            }else if(ore_19 < ora && ora <= ore_20){
                media_arr = arrivi_lun_19_20;

            }else if(ore_20 < ora && ora <= ore_21){
                media_arr = arrivi_lun_20_21;

            }else if(ore_21 < ora && ora <= ore_22){
                media_arr = arrivi_lun_21_22;

            }
            break;


        case mar:

            break;
    }

    return Exponential(media_arr);
}

void genera_evento_servito(struct cliente *c){

    int n = c->num_oggetti;

    double tempo_di_servizio = A*n+B;


    printf("Genrando evento servito, %s\n", secondi_ora((int)tempo_di_servizio + c->iniziato_a_servire));

    //passo l'approssimazione intera in secondi del tempo di servizio
    aggiungi_evento(servito, (int)tempo_di_servizio + c->iniziato_a_servire , *(c->fila_scelta));
}



//-------------------------------------------------------
//funzione per la stampa degli eventi e configurazioni di cassa attive
//-------------------------------------------------------


char *tipo_config(int tipo){

    switch(tipo){
        case selettiva:
            return selettiva_str;
        case pseudo_casuale:
            return pseudo_casuale_str;
        case condivisa:
            return condivisa_str;
        case mista:
            return mista_str;
        case selettiva_leggera:
            return selettiva_leggera_str;
        case selettiva_media:
            return selettiva_media_str;
        case selettiva_pesante:
            return selettiva_pesante_str;
        default:
            return "Non Registrata";
    }

}

void info_su_configurazioni_attive(){

    int i = 1;
    for(struct config_cassa_attive *current = config_attive; current != NULL; current = current->next){
        printf("Configurazioni attive:\n");

        char *tipo_str = tipo_config(current->configurazione_cassa->tipo);

        printf("%d]-%s\n", i, tipo_str);

        if(strcmp(tipo_str, selettiva_str) == 0){
            printf("Sottotipo: %s\n", tipo_config(current->configurazione_cassa->sotto_tipo));
        }

        printf("Fila condivisa: ");
        if(current->configurazione_cassa->fila_condivisa == NULL) {
            printf("NO\n");
        }else{
            printf("SI\n");
            printf("Clienti in fila condivisa: %d\n", lunghezza_fila(current->configurazione_cassa->fila_condivisa));
        }

        int nc = numero_casse(current->configurazione_cassa->casse);
        printf("Numero di casse: %d\n", nc);

        int j = 0;
        for(struct casse *fc = current->configurazione_cassa->casse; fc != NULL; fc = fc->next){
            j++;
            printf("Clienti in fila alla cassa '%d': %d\n", j, lunghezza_fila(fc->fila_cassa));
        }

        i++;

    }
}


char* tipo_evento_str(int tipo){

    switch (tipo){

        case arrivo:
            return arrivo_str;
        case servito:
            return servito_str;
        default:
            return "Non gestito";
    }

}

void stampa_tutti_eventi(){

    int i = 0;
    for(struct lista_eventi *le = eventi; le != NULL; le = le->next){

        printf("[%d] - %s - %s\n", i, tipo_evento_str(le->evento->tipo), secondi_ora(le->evento->tempo));
        i++;
    }

}

void stampa_num_eventi(int num){

    int i = 0;
    for(struct lista_eventi *le = eventi; le != NULL && i < num; le = le->next){

        printf("[%d] - %s - %s\n", i, tipo_evento_str(le->evento->tipo), secondi_ora(le->evento->tempo));
        i++;
    }

}



















