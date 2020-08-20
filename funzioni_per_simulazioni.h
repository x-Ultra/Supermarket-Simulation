//
// Created by ezio on 20/08/20.
//

#include <stdio.h>
#include <stdlib.h>
#include "costanti.h"
#include "strutture_dati.h"
#include "gestore_strutture_dati.h"

//inizializza simulazione (eg. setta i semi, tipi di configurazioni di cassa in base ai cassieri....)
void inizializza(){


}


//genera tutti gli eventi di tipo arrivo (dei clienti)
void popola_arrivi(){

    int arr;
    while(T < closing_time){

        arr = genera_arrivo(T);

        //aggiorno il tempo
        T = T + arr;

        //genero l'evento
        aggiungi_evento(arrivo, T, NULL);
    }
}


//funzoine che nel momento in cui viene generato un clinete, lo associa alla fila
//piu' favorevole (in base al suo numero di acquisti).
int scegli_fila(struct cliente *cli){

    //CLI->FILA_SCELTA = puntatore all'indirizzo di memoria
    // che ha il puntatpre alla fila scelta dalcliente
    //(indirizzo di memoria di config_cassa->fila_cassa)

    //TODO
    //MAGIc here



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

    inizializza();

    popola_arrivi();

    struct evento *evento_corrente;

    //scorro gli eventi
    for(struct lista_eventi *e = eventi; e != NULL;  e = e->next){
        evento_corrente = e->evento;


        //se l'evento è l'arrivo di un cliente, crealo
        //e aggiungilo nella fila piu' adeguata (che sceglierebbe)
        if(evento_corrente->tipo == arrivo){
            struct cliente *cliente = genera_cliente(evento_corrente->tempo);
            scegli_fila(cliente);

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
