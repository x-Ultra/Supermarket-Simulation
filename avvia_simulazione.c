//
// Created by ezio on 20/08/20.
//

#include "funzioni_per_simulazioni.h"
#include "rngs.h"
#include <unistd.h>
#include <stdio.h>

#define validation_poche_casse 0

//inizializza simulazione (eg. setta i semi, tipi di configurazioni di cassa in base ai cassieri....)
// num_simulazione ne definisca il tipo di cassa, num_casse il numero di casse
void inizializza(int num_simulazione, int num_casse){

    PlantSeeds(94823498);

    switch (num_simulazione) {

        case validation_poche_casse:
            aggiungi_configurazione_cassa(pseudo_casuale, num_casse, 0);
        break;


    }

    return;

    //creazione delle configurazioni di cassa
    //Testato OK (singolarmente)
    aggiungi_configurazione_cassa(condivisa, 1, 0);
    //aggiungi_configurazione_cassa(condivisa, 4, 0);
    //aggiungi_configurazione_cassa(pseudo_casuale, 1, 0);
    //aggiungi_configurazione_cassa(pseudo_casuale, 2, 0);
    //aggiungi_configurazione_cassa(mista, 2, 1);
    //aggiungi_configurazione_cassa(mista, 3, 1);
    //aggiungi_configurazione_cassa(mista, 4, 1);

    //Testato (a gruppi di configurazioni)
    //leggera media pesante + 1 condivisa
    //aggiungi_configurazione_cassa(mista, 3, 0);
    //aggiungi_configurazione_cassa(condivisa, 1, 0);

    //leggera media e pesante, di cui la media condivisa
    //aggiungi_configurazione_selettiva_custom(1, 1, 1, 0, 1, 0);

    //leggera media e pesante, di cui la leggera condivisa e con 2 casse
    //aggiungi_configurazione_selettiva_custom(2, 1, 1, 1, 0, 0);

    //sbizzarrirsi...e automatizzare

    //aggiungi_configurazione_cassa(condivisa, 3, 0);

    //aggiungi_configurazione_cassa(pseudo_casuale, 3, 0);

    //aggiungi_configurazione_selettiva_custom(3, 2, 1, 1, 0, 0);

    //aggiungi_configurazione_cassa(pseudo_casuale, 6, 0);

    //aggiungi_configurazione_cassa(condivisa, 6, 0);

    //aggiungi_configurazione_selettiva_custom(1, 3, 1, 0, 1, 0);

}


int main() {

    FILE *f = fopen("simulation_results.csv", "a");
    if(f == NULL)
    {
        printf("Impossibile creare/aprire file!");   
        exit(1);             
    }

    super_supermarket = 1;
    super_factor = 10;

    for(int i = 3; i < 4; ++i) {
        
        giorno_corrente = lun;
        do{

            eventi = NULL;
            config_attive = NULL;
            clienti_serviti = NULL;
            arrivi_totali = 0;
            abbandoni = 0;
            attesa_media_corrente = 0;
            slowdown_medio_corrente = 0;


            inizializza(validation_poche_casse, i);
            printf("Inizializzazione %d completa per giorno %d\n", i, giorno_corrente);

            start();

            //TODO aggiungere funzione che elabora i dati

            //per ora testo solo lunedi'
            giorno_corrente++;

            fprintf(f, "%d, pseudo-casuale, none, %d, %d, %f, %f, 0, %d, %d, %d, 1\n", validation_poche_casse, i, giorno_corrente, attesa_media_corrente, slowdown_medio_corrente, abbandoni,massima_lunghezza_fila_tollerata, arrivi_totali);

            printf("Simulazione giorno %d terminata\nMedia attesa registrata: %s\nSlowdown medio: %f\n", giorno_corrente, secondi_ora((int)attesa_media_corrente), slowdown_medio_corrente);
            printf("Abbandoni: %d\n", abbandoni);
            printf("Arrivi totali: %d\n", arrivi_totali);

        } while(giorno_corrente <= dom);

    }

    fclose(f);
}