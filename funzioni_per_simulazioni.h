//
// Created by ezio on 20/08/20.
//

#include "gestore_strutture_dati.h"
#include <unistd.h>

//genera tutti gli eventi di tipo arrivo (dei clienti)
void popola_arrivi(int giorno_settimana){

    int arr;
    printf("Popolamento degli arrivi\n");

    T = opening_time;
    while(T < closing_time){


        arr = (int)genera_arrivo(T, giorno_settimana);
        //aggiorno il tempo
        T = T + arr;

        //genero l'evento
        aggiungi_evento(arrivo, T, NULL);
    }
    printf("Popolamento degli arrivi terminato\n");
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

    //fino a 100 file selezionabili e altrettante configurazioni di cassa
    struct fila_cassa **file_selezionabili[100];
    struct config_cassa **config_selezionabili[100];

    int file_scelte = 0;

    for(struct config_cassa_attive *cfga = config_attive; cfga != NULL; cfga = cfga->next){

        if(cfga->configurazione_cassa->tipo == condivisa){
            //aggiungi a selezionabile
            file_selezionabili[file_scelte] = &(cfga->configurazione_cassa->fila_condivisa);
            config_selezionabili[file_scelte] = &(cfga->configurazione_cassa);
            file_scelte++;
            printf("Scelta fila condivisa\n");
        }

        if(cfga->configurazione_cassa->tipo == selettiva && cfga->configurazione_cassa->sotto_tipo == tipo_spesa){
            //aggiungi a selezionabile
            //se si tratta di una configurazione selettiva con cassa condivisa, aggiungila
            if(cfga->configurazione_cassa->fila_condivisa != NULL){
                file_selezionabili[file_scelte] = &(cfga->configurazione_cassa->fila_condivisa);
                config_selezionabili[file_scelte] = &(cfga->configurazione_cassa);
                file_scelte++;
                printf("Scelta fila condivisa selettiva\n");
            }else{
                //altrimenti tra le casse della configurazione scegli quella con meno fila
                int min = 100000000;
                for(struct casse *c = cfga->configurazione_cassa->casse; c != NULL; c = c->next){

                    if(lunghezza_fila(c->fila_cassa) < min){
                        file_selezionabili[file_scelte] = &(c->fila_cassa);
                        config_selezionabili[file_scelte] = &(cfga->configurazione_cassa);
                        file_scelte++;
                        break;
                    }
                }
                printf("Scelta fila selettiva...\n");
            }
        }

        if(cfga->configurazione_cassa->tipo == pseudo_casuale){
            //aggiungi a selezionabile
            int min = 100000000;
            for(struct casse *c = cfga->configurazione_cassa->casse; c != NULL; c = c->next){

                if(lunghezza_fila(c->fila_cassa) < min){
                    file_selezionabili[file_scelte] = &(c->fila_cassa);
                    config_selezionabili[file_scelte] = &(cfga->configurazione_cassa);
                    file_scelte++;
                    break;
                }
            }
            printf("Scelta fila pseudo casuale...\n");

        }

    }

    file_selezionabili[file_scelte] = NULL;
    config_selezionabili[file_scelte] = NULL;

    //scorri tutte le file selezionabili e scegli quella con il minor numero di persone in fila
    int i = 0;
    int min = lunghezza_fila(*file_selezionabili[0]);
    int scelta = 0;

    for(struct fila_cassa **fc = file_selezionabili[i]; fc != NULL; fc = file_selezionabili[i], ++i){

        if(lunghezza_fila(*fc) <= min){
            //cli->fila_scelta = fc;
            //((struct fila_cassa *)*fc)->cliente_in_fila = cli;
            scelta = i;
        }

    }

    cli->config_scelta = config_selezionabili[scelta-1];

    //TODO far avanzare il cliente dalla fila condivisa alla cassa, SE e' presente una libera.
    //  funge?
    if(((struct config_cassa *)*cli->config_scelta)->fila_condivisa != NULL){

        struct casse **cass = &((struct config_cassa *)*config_selezionabili[scelta-1])->casse;

        while(((struct casse *)*cass) != NULL){

            printf("Blablesco\n");

            if(lunghezza_fila(((struct casse *)*cass)->fila_cassa) == 0){
                printf("IUKYJTHRGF\n");
                cli->fila_scelta = &((struct casse *)*cass)->fila_cassa;
                ((struct casse *)*cass)->fila_cassa->cliente_in_fila = cli;

                ((struct casse *)*cass)->fila_cassa->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
                ((struct casse *)*cass)->fila_cassa->next->next = NULL;
                ((struct casse *)*cass)->fila_cassa->next->cliente_in_fila = NULL;




                return 0;
            }

            cass = &((struct casse *)*cass)->next;
            printf("OK\n");
        }

    }


    //TODO mettere cliente alla fine della fila
    // funge ?
    struct fila_cassa **fc = (struct fila_cassa **)file_selezionabili[scelta-1];


    while( ((struct fila_cassa *)*fc) != NULL){

        if(((struct fila_cassa *)*fc)->cliente_in_fila == NULL){

            cli->fila_scelta = fc;
            ((struct fila_cassa *)*fc)->cliente_in_fila = cli;

            ((struct fila_cassa *)*fc)->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
            ((struct fila_cassa *)*fc)->next->next = NULL;
            ((struct fila_cassa *)*fc)->next->cliente_in_fila = NULL;

            return 0;
        }

        fc = &((struct fila_cassa *)*fc)->next;
    }

    return 0;
}


//funzione che e' usata per 'servire' il primo cliente di una fila
//aggiungendolo poi in una lista di clienti serviti (clienti_serviti)
int servi_prossimo_cliente(struct cliente *cli, struct evento *e){

    //servi il cliente, aggiungendolo nei clienti serviti
    struct fila_cassa **fc = &clienti_serviti;
    while(*fc != NULL){

        fc = &((struct fila_cassa *)*fc)->next;
    }

    *fc = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
    ((struct fila_cassa *)*fc)->next = NULL;
    ((struct fila_cassa *)*fc)->cliente_in_fila = cli;

    //aggiorno i tempi del cliente, una volta servito
    cli->servito_alle = e->tempo;
    cli->attesa_in_fila = cli->iniziato_a_servire - cli->in_fila;

    //algoritmo di WELFORD per il calcolo dinamico del tempo medio di attesa
    attesa_media_corrente = attesa_media_corrente + (float)1/arrivi_totali*(cli->attesa_in_fila - attesa_media_corrente);

    //imposta al cliente successivo il tempo in cui e' iniziato ad essere servito, se presente
    if( ((struct fila_cassa *)*(cli->fila_scelta))->next->cliente_in_fila != NULL){
        ((struct fila_cassa *)*(cli->fila_scelta))->next->cliente_in_fila->iniziato_a_servire = e->tempo;
    }else{

        //se il cliente e' l'unico della fila, prima di avanzare, crea un posto libero.
        ((struct fila_cassa *)*(cli->fila_scelta))->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
        ((struct fila_cassa *)*(cli->fila_scelta))->next->next = NULL;
        ((struct fila_cassa *)*(cli->fila_scelta))->next->cliente_in_fila = NULL;
    }

    //Scorri la fila, deve cambiare per tutti !, cambiamento visibile a tutti.
    *(cli->fila_scelta) = ((struct fila_cassa *)*(cli->fila_scelta))->next;


    //TODO, il todo scritto in start()
    //il cliente ha liberato la cassa di una fila condivisa, se ha scelto una config. con fila condivisa.
    //bisogna dunque fas scorrere la fila condivisa, se non vuota.
    if(((struct config_cassa *)*(cli->config_scelta))->fila_condivisa != NULL){

        if(lunghezza_fila(((struct config_cassa *)*(cli->config_scelta))->fila_condivisa) > 0){

            //salvo il cliente che deve avanzare alle casse
            struct cliente *avanzante = ((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->cliente_in_fila;

            //segno quando il cliente nella fila condivisa verra' iniziato a servire
            //info_su_configurazioni_attive();
            //printf("Lunghezza della fila condivisa, quando si libera una cassa: %d\n", lunghezza_fila(((struct config_cassa *)*cli->config_scelta)->fila_condivisa));
            ((struct config_cassa *)*cli->config_scelta)->fila_condivisa->cliente_in_fila->iniziato_a_servire = e->tempo;

            //faccio avanzare la fila condivisa
            ((struct config_cassa *)*cli->config_scelta)->fila_condivisa = ((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->next;

            //metti il cliente avanzante alla prima cassa libera
            for(struct casse *cassa_condivisa = ((struct config_cassa *)*(cli->config_scelta))->casse;
                                                            cassa_condivisa->fila_cassa != NULL; cassa_condivisa = cassa_condivisa->next){

                //se trovo una cassa libera, acciongo il cliente.
                if(cassa_condivisa->fila_cassa->cliente_in_fila != NULL){

                    aggiungi_cliente_infila(cassa_condivisa->fila_cassa, avanzante);

                    //TODO aggiornare la sua nova fila, cosi' non funge mi sa
                    avanzante->fila_scelta = &(cassa_condivisa->fila_cassa);

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

    //stampa_tutti_eventi();

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

            printf("Nuovo cliente aggiunto in fila\n");

            //se il cliente ha deciso di abbandonare il negozio
            //pre la troppa fila, non fare nulla
            if(cliente->fila_scelta == NULL){
                abbandoni++;
                printf("Nuovo cliente ha abbandonato\n");
                continue;
            }

            //  Se e' il primo della fila, ma la fila e' condivisa,
            //  non fare nulla. Sara' il primo cliente che libera una cassa
            //  a falo avanzare a quella stessa cassa.
            //se il cliente e' il primo della fila, allora genera
            //l'evento del tempo di servizio
            if(((struct fila_cassa *)*(cliente->fila_scelta))->cliente_in_fila == cliente
                    && ((struct config_cassa *)*(cliente->config_scelta))->fila_condivisa != ((struct fila_cassa *)*(cliente->fila_scelta))){

                cliente->iniziato_a_servire = evento_corrente->tempo;
                genera_evento_servito(cliente);
            }

            //se il cliente non è il primo della fila, non far nulla.


        //se l'evento è il servizio di un cliente
        }else if(evento_corrente->tipo == servito){

            printf("Cliente in servizio\n");

            servi_prossimo_cliente(evento_corrente->fila->cliente_in_fila, evento_corrente);
        }


        stampa_evento(evento_corrente);
        stampa_num_eventi(20);
        printf("\n");
        info_su_configurazioni_attive();
        printf("\n");
        sleep(2);


    }

    printf("Fine\n");

}
