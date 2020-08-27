//
// Created by ezio on 20/08/20.
//

#include "funzioni_per_simulazioni.h"
#include "rngs.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//inizializza simulazione (eg. setta i semi, tipi di configurazioni di cassa in base ai cassieri....)
// num_simulazione ne definisca il tipo di cassa, num_casse il numero di casse

//ritorna il tipo di configurazione usato, in formato stringa
char *inizializza(int num_simulazione, int num_casse){

    PlantSeeds(SEED);

    int legg = 0, med = 0, pes = 0;
    char *str = malloc(100);

    switch (num_simulazione) {

        case validation_poche_casse:
            aggiungi_configurazione_cassa(pseudo_casuale, num_casse, 0);
            return "poche_casse";

        case incr_2_10_cond:
            aggiungi_configurazione_cassa(condivisa, num_casse, 0);
            sprintf(str, "Condivisa con %d casse", num_casse);
            return str;
        case incr_2_10_pc:
            aggiungi_configurazione_cassa(pseudo_casuale, num_casse, 0);
            sprintf(str, "Pseudo-Casuale con %d casse", num_casse);
            return str;
        case incr_3_10_sel:
            aggiungi_configurazione_cassa(selettiva, num_casse, 0);
            sprintf(str, "Selettiva con %d casse", num_casse);
            return str;
        case incr_3_10_sel_cond:
            //num_casse parte da 3
            do{
                legg++;
                if(num_casse-legg-med-pes == 0){
                    break;
                }
                med++;
                if(num_casse-legg-med-pes == 0){
                    break;
                }
                pes++;
                if(num_casse-legg-med-pes == 0) {
                    break;
                }
            }while(1);
            aggiungi_configurazione_selettiva_custom(legg, med, pes, 1, 1, 1);
            sprintf(str, "Selettiva con casse %d casse e fila condivise", num_casse);
            return str;
        case mista_3_x:
            //num_casse sono le condivise oltre le 3 selettive
            aggiungi_configurazione_cassa(selettiva, 3+num_casse, num_casse);
            sprintf(str, "3 selettive con %d casse a fila condivisa", num_casse);
            return str;
        case mista_1_x:
            aggiungi_configurazione_cassa(selettiva,1+ num_casse, num_casse);
            sprintf(str, "1 selettiva (leggera) con %d casse a fila condivisa", num_casse);
            return str;

        default:
            break;

    }


    //creazione delle configurazioni di cassa
    //Testato OK (singolarmente)
    //aggiungi_configurazione_cassa(condivisa, 3, 0);
    //aggiungi_configurazione_selettiva_custom(1, 1, 1, 0, 1, 0);
    //aggiungi_configurazione_cassa(condivisa, 10, 0);
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

    return "undefined";

}

void simulazioni_ezio();

int main() {


    FILE *ff = fopen("simulation_results_ezio.csv", "a");
    if (ff == NULL) {
        printf("Impossibile creare/aprire file!");
        exit(1);
    }

    super_supermarket = 1;
    super_factor = 10;

    simulazioni_ezio();
    return 0;

    //fare num_ismulazioni per ogni giorno, commentare i risultati in base al giorno
    for (int i = 0; i <= dom; ++i) {

        SEED = SEED - 1;

        eventi = NULL;
        config_attive = NULL;
        clienti_serviti = NULL;
        arrivi_totali = 0;
        abbandoni = 0;
        attesa_media_corrente = 0;
        varianza_tempo_attesa = 0;
        slowdown_medio_corrente = 0;


        inizializza(-123234, 0);

        //printf("Inizializzazione %d completa per gioro %d\n", i, giorno_corrente);
        start();

        printf("Simulazione giorno %s terminata\nMedia attesa registrata: %s\nSlowdown medio: %f\n",
               giorno_str(i), secondi_ora((int) attesa_media_corrente), slowdown_medio_corrente);
        printf("Deviazione std tempo d'attesa: %s\n", secondi_ora(sqrt(varianza_tempo_attesa)));
        printf("Abbandoni: %d\n", abbandoni);
        printf("Arrivi totali: %d\n", arrivi_totali);
    }
}

void simulazioni_ezio(){


    FILE *ff = fopen("simulation_results_ezio.csv","a");
    if(ff == NULL)
    {
        printf("Impossibile creare/aprire file!");
        exit(1);
    }

    super_supermarket = 1;
    super_factor = 10;

    int alpha = 5;

    //definire qui tipo di simulazione e aggiustare num_cassieri iniziali di conseguenza
    int tipo_simulazione = incr_2_10_cond;

    for(int num_cassieri = 2; num_cassieri <= 10; num_cassieri++) {

        giorno_corrente = lun;
        do {

            double medie_att[num_simulazioni];
            double slowdowns[num_simulazioni];
            int abbandoni_tot[num_simulazioni];
            int arrivi_tot[num_simulazioni];

            char *tipo_config_str;

            SEED = 94823498;

            //fare num_ismulazioni per ogni giorno, commentare i risultati in base al giorno
            for (int i = 0; i < num_simulazioni; ++i) {

                SEED = SEED - 1;

                eventi = NULL;
                config_attive = NULL;
                clienti_serviti = NULL;
                arrivi_totali = 0;
                abbandoni = 0;
                attesa_media_corrente = 0;
                varianza_tempo_attesa = 0;
                slowdown_medio_corrente = 0;


                tipo_config_str = inizializza(tipo_simulazione, num_cassieri);

                //printf("Inizializzazione %d completa per giorno %d\n", i, giorno_corrente);
                start();
                /*
                printf("Simulazione giorno %d terminata\nMedia attesa registrata: %s\nSlowdown medio: %f\n",
                       giorno_corrente, secondi_ora((int) attesa_media_corrente), slowdown_medio_corrente);
                printf("Deviazione std tempo d'attesa: %s\n", secondi_ora(sqrt(varianza_tempo_attesa)));
                printf("Abbandoni: %d\n", abbandoni);
                printf("Arrivi totali: %d\n", arrivi_totali);
                 */

                //appendo dati
                medie_att[i] = attesa_media_corrente;
                slowdowns[i] = slowdown_medio_corrente;
                arrivi_tot[i] = arrivi_totali;
                abbandoni_tot[i] = abbandoni;

            }

            double ic_att_l;
            double ic_att_r;
            double Xnatt = 0;
            double Snatt = 0;

            double ic_var_l;
            double ic_var_r;

            double ic_slow_l;
            double ic_slow_r;
            double Xnslow = 0;
            double Snslow = 0;

            double ic_arr_l;
            double ic_arr_r;
            double Xnarr = 0;
            double Snarr = 0;

            double ic_abb_l;
            double ic_abb_r;
            double Xnabb = 0;
            double Snabb = 0;

            double t_alpha_mezzi = get_stud(alpha);
            double z_alpha_mezzi = get_gauss(alpha);

            if(t_alpha_mezzi == -1 || z_alpha_mezzi == -1){
                printf("Alpha value not supported\n");
                return;
            }

            for(int i = 0; i < num_simulazioni; i++){
                Xnatt +=  medie_att[i];
                Xnslow += slowdowns[i];
                Xnarr +=  arrivi_tot[i];
                Xnabb += abbandoni_tot[i];
            }

            Xnatt = (double)Xnatt/num_simulazioni;
            Xnslow = (double)Xnslow/num_simulazioni;
            Xnarr = (double)Xnarr/num_simulazioni;
            Xnabb = (double)Xnabb/num_simulazioni;

            for(int i = 0; i < num_simulazioni; i++){
                Snatt += (medie_att[i] - Xnatt)*(medie_att[i] - Xnatt);
                Snslow += (slowdowns[i]-Xnslow)*(slowdowns[i]-Xnslow);
                Snarr += (arrivi_tot[i]- Xnarr)*(arrivi_tot[i]- Xnarr);
                Snabb += (abbandoni_tot[i]-Xnabb)*(abbandoni_tot[i]-Xnabb);
            }

            //varienze
            Snatt = (double)Snatt/(num_simulazioni -1);
            Snslow = (double)Snslow/(num_simulazioni -1);
            Snarr = (double)Snarr/(num_simulazioni -1);
            Snabb = (double)Snabb/(num_simulazioni -1);

            //deviazioni standard
            Snatt = sqrt(Snatt);
            Snarr = sqrt(Snarr);
            Snslow = sqrt(Snslow);
            Snabb = sqrt(Snabb);

            //varianza media attesa
            ic_var_r = ((num_simulazioni -1)*(Snatt*Snatt))/get_chi(alpha)[0];
            ic_var_l = ((num_simulazioni -1)*(Snatt*Snatt))/get_chi(alpha)[1];

            //media attesa
            ic_att_l = Xnatt - (double)get_stud(alpha)*Snatt/sqrt(num_simulazioni);
            ic_att_r = Xnatt + (double)get_stud(alpha)*Snatt/sqrt(num_simulazioni);

            //media slowdown
            ic_slow_l = Xnslow - (double)get_stud(alpha)*Snslow/sqrt(num_simulazioni);
            ic_slow_r = Xnslow + (double)get_stud(alpha)*Snslow/sqrt(num_simulazioni);

            //media arrivi
            ic_arr_l = Xnarr - (double)get_stud(alpha)*Snarr/sqrt(num_simulazioni);
            ic_arr_r = Xnarr + (double)get_stud(alpha)*Snarr/sqrt(num_simulazioni);

            //media abbandoni
            ic_abb_l = Xnabb - (double)get_stud(alpha)*Snabb/sqrt(num_simulazioni);
            ic_abb_r = Xnabb + (double)get_stud(alpha)*Snabb/sqrt(num_simulazioni);

            fprintf(ff, "%d, %s, %d, %s, [%s, %s], [%f, %f], [%s, %s], [%f, %f], [%f, %f], %d, %d, %d\n",
                   validation_poche_casse, tipo_config_str, num_cassieri, giorno_str(giorno_corrente), secondi_ora(ic_att_l),
                   secondi_ora(ic_att_r), ic_slow_l, ic_slow_r, secondi_ora(ic_var_l), secondi_ora(ic_var_r), ic_abb_l, ic_abb_r, ic_arr_l, ic_arr_r,
                   massima_lunghezza_fila_tollerata, num_simulazioni, alpha);
            /*
            printf("%d, %s, %d, %s, [%s, %s], [%f, %f], [%s, %s], [%f, %f], [%f, %f], %d, %d, %d\n",
                   validation_poche_casse, tipo_config_str, num_cassieri, giorno_str(giorno_corrente), secondi_ora(ic_att_l),
                   secondi_ora(ic_att_r), ic_slow_l, ic_slow_r, secondi_ora(ic_var_l), secondi_ora(ic_var_r), ic_abb_l, ic_abb_r, ic_arr_l, ic_arr_r,
                   massima_lunghezza_fila_tollerata, num_simulazioni, alpha);
            */

            printf("Giorno: %s fatto, con %d num_casse\n", giorno_str(giorno_corrente), num_cassieri);

            giorno_corrente++;

        } while (giorno_corrente <= dom);
    }

    fclose(ff);
}