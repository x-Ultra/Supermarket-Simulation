//
// Created by ezio on 20/08/20.
//

#include <stdio.h>
#include <stdlib.h>
#include "costanti.h"
#include "strutture_dati.h"
#include "gestore_strutture_dati.h"


//genera tutti gli eventi di tipo arrivo (dei clienti)
void popola_arrivi(int giorno_settimana){

    int arr;
    while(T < closing_time){

        arr = genera_arrivo(T, giorno_settimana);

        //aggiorno il tempo
        T = T + arr;

        //genero l'evento
        aggiungi_evento(arrivo, T, NULL);
    }
}


//funzoine che nel momento in cui viene generato un clinete, lo associa alla fila
//piu' favorevole (in base al suo numero di acquisti).
int scegli_fila(struct cliente *cli){

    //se il tempo medio di attesa e' maggiore di una certa quantita, il cliente abbandona
    if(attesa_media_corrente > max_attesa){
        cli->fila_scelta = NULL;

        return 0;
    }


    int acquisti = cli->num_oggetti;
    int tipo_spesa;
    if(acquisti <= acquisti_leggeri){
        tipo_spesa = selettiva_leggera;
    }else if(acquisti_medi < acquisti && acquisti <= max_acquisti){
        tipo_spesa = selettiva_media;
    }else{
        tipo_spesa = selettiva_pesante;
    }

    //fino a 100 file selezionabili...
    struct fila_cassa **file_selezionabili[100];
    int file_scelte = 0;

    for(struct config_cassa_attive *cfga = config_attive; cfga != NULL; cfga = cfga->next){

        if(cfga->configurazione_cassa->tipo == condivisa){
            //aggiungi a selezionabile
            file_selezionabili[file_scelte] = &(cfga->configurazione_cassa->fila_condivisa);
            file_scelte++;
        }

        if(cfga->configurazione_cassa->tipo == selettiva && cfga->configurazione_cassa->sotto_tipo == tipo_spesa){
            //aggiungi a selezionabile
            //se si tratta di una configurazione selettiva con cassa condivisa, aggiungila
            if(cfga->configurazione_cassa->fila_condivisa != NULL){
                file_selezionabili[file_scelte] = &(cfga->configurazione_cassa->fila_condivisa);
                file_scelte++;
            }else{
                //altrimenti tra le casse della configurazione scegli quella con meno fila
                int min = 100000000;
                for(struct casse *c = cfga->configurazione_cassa->casse; c != NULL; c = c->next){

                    if(lunghezza_fila(c->fila_cassa) < min){
                        file_selezionabili[file_scelte] = &(cfga->configurazione_cassa->fila_condivisa);
                        file_scelte++;
                        break;
                    }
                }

            }
        }

        if(cfga->configurazione_cassa->tipo == pseudo_casuale){
            //aggiungi a selezionabile
            int min = 100000000;
            for(struct casse *c = cfga->configurazione_cassa->casse; c != NULL; c = c->next){

                if(lunghezza_fila(c->fila_cassa) < min){
                    file_selezionabili[file_scelte] =&(cfga->configurazione_cassa->fila_condivisa);
                    file_scelte++;
                    break;
                }
            }

        }

    }

    file_selezionabili[file_scelte] = NULL;

    //scorri tutte le file selezionabili e scegli quella con il minor numero di persone in fila
    //CLI->FILA_SCELTA = puntatore all'indirizzo di memoria
    // che ha il puntatpre alla fila scelta dalcliente
    //(indirizzo di memoria di config_cassa->fila_cassa)

    int i = 0;
    int min = lunghezza_fila(*file_selezionabili[0]);

    for(struct fila_cassa **fc = file_selezionabili[i]; fc != NULL; fc = file_selezionabili[i], ++i){

        if(lunghezza_fila(*fc) < min){
            cli->fila_scelta = fc;
        }

    }



    return 0;
}


//funzione che e' usata per 'servire' il primo cliente di una fila
//aggiungendolo poi in una lista di clienti serviti (clienti_serviti)
int servi_prossimo_cliente(struct cliente *cli, struct evento *e){

    //servi il cliente, aggiungendolo nei clienti serviti
    struct fila_cassa *fc = clienti_serviti;
    while(fc != NULL){
        fc = fc->next;
    }

    fc = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
    fc->next = NULL;
    fc->cliente_in_fila = cli;

    //aggiorno i tempi del cliente, una volta servito
    cli->servito_alle = e->tempo;
    cli->attesa_in_fila = cli->iniziato_a_servire - cli->in_fila;

    //algoritmo di WELFORD per il calcolo dinamico del tempo medio di attesa
    attesa_media_corrente = attesa_media_corrente + (float)1/arrivi_totali*(cli->attesa_in_fila - attesa_media_corrente)

    //imposta al cliente successivo il tempo in cui e' iniziato ad essere servito
    ((struct fila_cassa *)(cli->fila_scelta))->next->cliente_in_fila->iniziato_a_servire = e->tempo;

    //Scorri la fila, TODO vedere se corretto
    *(cli->fila_scelta) = ((struct fila_cassa *)(cli->fila_scelta))->next;

    //il cliente ha liberato la cassa di una fila condivisa, se ha scelto una config. con fila condivisa.
    //bisogna dunque fas scorrere la fila condivisa, se non vuota.
    if(cli->config_scelta->fila_condivisa != NULL){
        if(lunghezza_fila(cli->config_scelta->fila_condivisa) > 0){
            //fai andare alla cassa appena liberata,il primo cliente nella fila

            struct cliente *avanzante = cli->config_scelta->fila_condivisa->cliente_in_fila;
            //TODO vedere se va bene, la fila condifisa deve cambiare per tutti, non solo per 'cli'
            cli->config_scelta->fila_condivisa = cli->config_scelta->fila_condivisa->next;

            //metti il cliente avanzante alla prima cassa libera

            for(struct fila_cassa *cassa_condivisa = cli->config_scelta->casse->fila_cassa; cassa_condivisa != NULL;
                                                                                cassa_condivisa = cassa_condivisa->next){
                if(lunghezza_fila(cassa_condivisa) == 0){

                    aggiungi_cliente_infila(cassa_condivisa, avanzante);

                    return 0;
                }
            }

            //se sono qui, qualcosa e' andato storto, dovrebbe esserci una cassa libera.
            return -1;
        }
    }

    return 0;
}


//inizia la simulazione
void start(){

    popola_arrivi(giorno_corrente);

    struct evento *evento_corrente;

    //scorro gli eventi
    for(struct lista_eventi *e = eventi; e != NULL;  e = e->next){
        evento_corrente = e->evento;


        //se l'evento è l'arrivo di un cliente, crealo
        //e aggiungilo nella fila piu' adeguata (che sceglierebbe)
        if(evento_corrente->tipo == arrivo){
            arrivi_totali++;

            struct cliente *cliente = genera_cliente(evento_corrente->tempo);
            scegli_fila(cliente);

            //se il cliente ha deciso di abbandonare il negozio
            //pre la troppa fila, non fare nulla
            if(cliente->fila_scelta == NULL){
                abbandoni++;
                continue;
            }

            //se il cliente e' il primo della fila, allora genera
            //l'evento del tempo di servizio
            if(((struct fila_cassa *)(cliente->fila_scelta))->cliente_in_fila == cliente){
                cliente->iniziato_a_servire = evento_corrente->tempo;

                genera_evento_servito(cliente);
            }

            //se il cliente non è il primo della fila, non far nulla.


        //se l'evento è il servizio di un cliente
        }else if(evento_corrente->tipo == servito){

            servi_prossimo_cliente(evento_corrente->fila->cliente_in_fila, evento_corrente);
        }

    }

}
