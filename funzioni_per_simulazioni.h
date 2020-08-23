//
// Created by ezio on 20/08/20.
//

#include "gestore_strutture_dati.h"
#include <unistd.h>

//genera tutti gli eventi di tipo arrivo (dei clienti)
void popola_arrivi(int giorno_settimana){

    int arr;
    D(printf("Popolamento degli arrivi\n"));

    T = opening_time;
    while(T < closing_time){


        arr = (int)genera_arrivo(T, giorno_settimana);
        //aggiorno il tempo
        T = T + arr;

        //genero l'evento
        aggiungi_evento(arrivo, T, NULL);
    }
    D(printf("Popolamento degli arrivi terminato\n"));
}


//funzoine che nel momento in cui viene generato un clinete, lo associa alla fila
//piu' favorevole (in base al suo numero di acquisti).
int scegli_fila(struct cliente *cli, struct evento *e){

    //se il tempo medio di attesa e' maggiore di una certa quantita, il cliente abbandona
    if(attesa_media_corrente > max_attesa){
        cli->fila_scelta = NULL;

        return 0;
    }

    //cli->in_fila_condivisa = 0;


    int acquisti = cli->num_oggetti;
    int tipo_spesa;
    if(acquisti <= acquisti_leggeri){
        tipo_spesa = selettiva_leggera;
    }else if(acquisti_leggeri < acquisti && acquisti <= max_acquisti){
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
            D(printf("Scelta fila condivisa\n"));
        }

        if(cfga->configurazione_cassa->tipo == selettiva && cfga->configurazione_cassa->sotto_tipo == tipo_spesa){
            //aggiungi a selezionabile
            //se si tratta di una configurazione selettiva con cassa condivisa, aggiungila
            if(cfga->configurazione_cassa->fila_condivisa != NULL){
                file_selezionabili[file_scelte] = &(cfga->configurazione_cassa->fila_condivisa);
                config_selezionabili[file_scelte] = &(cfga->configurazione_cassa);
                file_scelte++;
                D(printf("Scelta fila condivisa selettiva\n"));
            }else{
                //altrimenti tra le casse della configurazione scegli quella con meno fila
                int min = 100000000;
                for(struct casse *c = cfga->configurazione_cassa->casse; c != NULL; c = c->next){

                    if(lunghezza_fila(c->fila_cassa) < min){
                        file_selezionabili[file_scelte] = &(c->fila_cassa);
                        config_selezionabili[file_scelte] = &(cfga->configurazione_cassa);
                        file_scelte++;
                        continue;
                    }
                }
                D(printf("Scelta fila selettiva...\n"));
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
                    continue;
                }
            }
            D(printf("Scelta fila pseudo casuale...\n"));

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

    D(printf("Il cliente %d ha scelto la fila con id: %d\n",cli->id, ((struct fila_cassa *)*file_selezionabili[scelta-1])->id));

    cli->config_scelta = config_selezionabili[scelta-1];

    if(((struct config_cassa *)*cli->config_scelta)->fila_condivisa != NULL){

        //cli->in_fila_condivisa = 1;

        struct casse **cass = &((struct config_cassa *)*config_selezionabili[scelta-1])->casse;

        while(((struct casse *)*cass) != NULL){

            if(lunghezza_fila(((struct casse *)*cass)->fila_cassa) == 0){

                cli->fila_scelta = &((struct casse *)*cass)->fila_cassa;
                ((struct casse *)*cass)->fila_cassa->cliente_in_fila = cli;

                ((struct casse *)*cass)->fila_cassa->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
                ((struct casse *)*cass)->fila_cassa->next->next = NULL;
                ((struct casse *)*cass)->fila_cassa->next->id = ((struct casse *)*cass)->fila_cassa->id;
                ((struct casse *)*cass)->fila_cassa->next->cliente_in_fila = NULL;

                D(printf("Il cliente %d ha avanzato dalla condivisa alla cassa con id: %d\n",cli->id, ((struct casse *)*cass)->fila_cassa->id));
                //cli->in_fila_condivisa = 0;

                cli->iniziato_a_servire = e->tempo;
                genera_evento_servito(cli);
                D(printf("Generato evento servito per il cliente %d, il fila %d\n", cli->id, ((struct fila_cassa *)*(cli->fila_scelta))->id));



                return 0;
            }

            cass = &((struct casse *)*cass)->next;
        }

        D(printf("Il cliente %d non ha avanzato dalla condivisa alla cassa perche non libere\n", cli->id));

    }


    struct fila_cassa **fc = (struct fila_cassa **)file_selezionabili[scelta-1];

    D(printf("Il cliente %d si sta per accodare in fondo alla fila con id: %d, lunga %d\n",cli->id, ((struct fila_cassa *)*file_selezionabili[scelta-1])->id, lunghezza_fila(((struct fila_cassa *)*file_selezionabili[scelta-1]))));

    //se il c liente e' il primo di una fila 'normale' (non condivisa), genera evento servizio
    if( lunghezza_fila((struct fila_cassa *)*fc) == 0 && ((struct config_cassa *)*cli->config_scelta)->fila_condivisa == NULL){


        //inserisci il cliente
        cli->fila_scelta = fc;
        ((struct fila_cassa *)*fc)->cliente_in_fila = cli;

        ((struct fila_cassa *)*fc)->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
        ((struct fila_cassa *)*fc)->next->next = NULL;
        ((struct fila_cassa *)*fc)->next->id = ((struct fila_cassa *)*fc)->id;
        ((struct fila_cassa *)*fc)->next->cliente_in_fila = NULL;

        //cli->in_fila_condivisa = 0;
        cli->iniziato_a_servire = e->tempo;
        genera_evento_servito(cli);
        D(printf("Il cliente %d, ha trovato la fila %d libera\n", cli->id, ((struct fila_cassa *)*fc)->id));

        return 0;
    }

    while( ((struct fila_cassa *)*fc) != NULL){

        if(((struct fila_cassa *)*fc)->cliente_in_fila == NULL){

            cli->fila_scelta = fc;
            ((struct fila_cassa *)*fc)->cliente_in_fila = cli;

            ((struct fila_cassa *)*fc)->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
            ((struct fila_cassa *)*fc)->next->next = NULL;
            ((struct fila_cassa *)*fc)->next->id = ((struct fila_cassa *)*fc)->id;
            ((struct fila_cassa *)*fc)->next->cliente_in_fila = NULL;

            D(printf("Il cliente %d si e' accodato in fondo alla fila con id: %d\n",cli->id, ((struct fila_cassa *)*fc)->id));

            return 0;
        }

        fc = &((struct fila_cassa *)*fc)->next;
    }

    printf("!!!!!!!-> Il cliente '%d' non si e' accodato in fila\n", cli->id);

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

        //genero l'evento per il prossimo cliente
        //genero l'evento in quanto e' il primo della fila
        genera_evento_servito(((struct fila_cassa *)*(cli->fila_scelta))->next->cliente_in_fila);

        D(printf("1, %d\n", lunghezza_fila(((struct fila_cassa *)*(cli->fila_scelta)))));
        D(printf("Il cliente %d ha detto al cliente %d, nella fila %d che dopo e' il suo turno\n",cli->id,((struct fila_cassa *)*(cli->fila_scelta))->next->cliente_in_fila->id, ((struct fila_cassa *)*(cli->fila_scelta))->id));

    }else{
        D(printf("2\n"));
        //se il cliente e' l'unico della fila, prima di avanzare, crea un posto libero.
        ((struct fila_cassa *)*(cli->fila_scelta))->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
        ((struct fila_cassa *)*(cli->fila_scelta))->next->next = NULL;
        ((struct fila_cassa *)*(cli->fila_scelta))->next->id = ((struct fila_cassa *)*(cli->fila_scelta))->id;
        ((struct fila_cassa *)*(cli->fila_scelta))->next->cliente_in_fila = NULL;

        D(printf("Il cliente %d e' l'unico nella fila %d\n",cli->id, ((struct fila_cassa *)*(cli->fila_scelta))->id));

    }

    //Scorri la fila, deve cambiare per tutti !, cambiamento visibile a tutti.
    *(cli->fila_scelta) = ((struct fila_cassa *)*(cli->fila_scelta))->next;
    D(printf("La fila %d scorre in avanti\n",((struct fila_cassa *)*(cli->fila_scelta))->id));

    //il cliente ha liberato la cassa di una fila condivisa, se ha scelto una config. con fila condivisa.
    //bisogna dunque fas scorrere la fila condivisa, se non vuota.
    if(((struct config_cassa *)*(cli->config_scelta))->fila_condivisa != NULL){

        if(lunghezza_fila(((struct config_cassa *)*(cli->config_scelta))->fila_condivisa) > 0){

            D(printf("Il cliente %d fa avanzare la fila condivisa %d\n",cli->id, ((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->id));

            //salvo il cliente che deve avanzare alle casse
            struct cliente *avanzante = ((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->cliente_in_fila;

            D(printf("Il cliente %d avanza dalla condivisa alla cassa......\n",avanzante->id));

            //segno quando il cliente nella fila condivisa verra' iniziato a servire
            ((struct config_cassa *)*cli->config_scelta)->fila_condivisa->cliente_in_fila->iniziato_a_servire = e->tempo;

            D(printf("La fila condivisa %d, avanza\n",  ((struct config_cassa *)*cli->config_scelta)->fila_condivisa->id));

            //metti il cliente avanzante alla prima cassa libera
            for(struct casse **cassa_condivisa = &((struct config_cassa *)*(cli->config_scelta))->casse;
                    ((struct casse *)*cassa_condivisa) != NULL; cassa_condivisa = &((struct casse *)*cassa_condivisa)->next){

                //se trovo una cassa libera, acciongo il cliente.
                if(((struct casse *)*cassa_condivisa)->fila_cassa->cliente_in_fila == NULL){

                    D(printf("...... %d\n", ((struct casse *)*cassa_condivisa)->fila_cassa->id));

                    //aggiungi_cliente_infila(cassa_condivisa->fila_cassa, avanzante);

                    //cassa_condivisa->fila_cassa->cliente_in_fila = avanzante;
                    ((struct casse *)*cassa_condivisa)->fila_cassa->cliente_in_fila = avanzante;
                    ((struct casse *)*cassa_condivisa)->fila_cassa->next = (struct fila_cassa *)malloc(sizeof(struct fila_cassa));
                    ((struct casse *)*cassa_condivisa)->fila_cassa->next->next = NULL;
                    ((struct casse *)*cassa_condivisa)->fila_cassa->next->id = ((struct casse *)*cassa_condivisa)->fila_cassa->id;
                    ((struct casse *)*cassa_condivisa)->fila_cassa->next->cliente_in_fila = NULL;

                    //aggiorno la fila del cliente
                    ((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->cliente_in_fila->fila_scelta = &(((struct casse *)*cassa_condivisa)->fila_cassa);

                    //genero l'evento in quanto e' il primo della fila
                    D(printf("Iniziato a servire: %d\n", ((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->cliente_in_fila->iniziato_a_servire));
                    genera_evento_servito(((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->cliente_in_fila);

                    //faccio infine avanzare la fila condivisa
                    ((struct config_cassa *)*cli->config_scelta)->fila_condivisa = ((struct config_cassa *)*(cli->config_scelta))->fila_condivisa->next;

                    return 0;
                }
            }

            //se sono qui, qualcosa e' andato storto, dovrebbe esserci una cassa libera.
            printf("Qualcosa e andato storto!!!!!!!!!!!!!\n");
            return -1;
        }
    }else{

        //se il cliente era in una fila non condivisa non vuota, falla avanzare
        if(((struct fila_cassa *)*(cli->fila_scelta))->cliente_in_fila != NULL){
            //altrimenti aggiorna la sua posizione e basta
            ((struct fila_cassa *)*(cli->fila_scelta))->cliente_in_fila->fila_scelta = (cli->fila_scelta);
            D(printf("Ora il cliente %d si e' spostato nella fila %d lunga %d\n", ((struct fila_cassa *)*(cli->fila_scelta))->cliente_in_fila->id, (((struct fila_cassa *)*(cli->fila_scelta))->id), lunghezza_fila(*(((struct fila_cassa *)*(cli->fila_scelta)))->cliente_in_fila->fila_scelta)));

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
            scegli_fila(cliente, evento_corrente);

            D(printf("Nuovo cliente %d aggiunto in fila\n", cliente->id));

            //se il cliente ha deciso di abbandonare il negozio
            //pre la troppa fila, non fare nulla
            if(cliente->fila_scelta == NULL){
                abbandoni++;
                D(printf("Nuovo cliente ha abbandonato\n"));
                continue;
            }


        //se l'evento è il servizio di un cliente
        }else if(evento_corrente->tipo == servito){

            D(printf("Cliente in servizio nella coda %d\n", evento_corrente->fila->id));

            servi_prossimo_cliente(evento_corrente->fila->cliente_in_fila, evento_corrente);
        }


        D(printf("\n"));
        D(stampa_evento(evento_corrente));
        D(printf("Media attesa attuale: %f\n", attesa_media_corrente));
        //D(stampa_num_eventi(20));
        D(info_su_configurazioni_attive());
        D(printf("\n-----------------------------------------------------------------------\n"));
        D(sleep(2));


    }

    printf("Fine\n");

}
