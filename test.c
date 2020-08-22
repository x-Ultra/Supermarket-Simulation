//
// Created by ezio on 20/08/20.
//

#include "funzioni_per_simulazioni.h"

#include <unistd.h>

//inizializza simulazione (eg. setta i semi, tipi di configurazioni di cassa in base ai cassieri....)
//chiamata all'inizio di ogni nuovo giorno (ma num_simulazion cambia ogni settimana)
void inizializza(int num_simulazione){

    srandom(SEED-num_simulazione);

    //creazione delle configurazioni di cassa
    aggiungi_configurazione_cassa(condivisa, 1, 0);
    //aggiungi_configurazione_cassa(pseudo_casuale, 4, 0);
    //aggiungi_configurazione_cassa(mista, 4, 3);

    //aggiungi_configurazione_selettiva_custom(1, 1, 1, 0, 1, 0);
}


int main() {

    for(int i = 0; i < num_simulazioni; ++i) {

        do{
            inizializza(i);
            printf("Inizializzazione %d completa per giorno %d\n", i, giorno_corrente);

            //info_su_configurazioni_attive();

            start();
            //TODO aggiungere funzione che elabora i dati

            //per ora testo solo lunedi'
            giorno_corrente++;

            break;

        }while(giorno_corrente < dom);

        //per ora testo solo 1 simulazione
        break;
    }
}