//
// Created by ezio on 20/08/20.
//

#include "funzioni_per_simulazioni.h"
#include "rngs.h"
#include <math.h>

#define pseudo_casuale_s 0

#define sperimentale_60_20_20 6
#define sperimentale_20_60_20 7
#define sperimentale_20_20_60 8
#define sperimentale_leggera_condivisa 9
#define sperimentale_media_condivisa 10
#define sperimentale_pesante_condivisa 11

// Ricava numero di casse in base alla percentuale
int* get_split(int total, int first, int second, int third) {

    int* perc = malloc(3*sizeof(int));

    perc[0] = (int) total * (first*1.0 / 100);
    perc[1] = (int) total * (second*1.0 / 100);
    perc[2] = (int) total * (third*1.0 / 100);
    
    int curr = 2;


    while (perc[0]+perc[1]+perc[2] < total) {
        perc[curr]++;
        curr--;
        curr = curr % 3;
    }

    //printf("%d, %d, %d\n", perc[0], perc[1], perc[2]);
    return perc;
}

//inizializza simulazione (eg. setta i semi, tipi di configurazioni di cassa in base ai cassieri....)
// num_simulazione ne definisca il tipo di cassa, num_casse il numero di casse
//ritorna il tipo di configurazione usato, in formato stringa
char *inizializza(int num_simulazione, int num_casse, int seed){

    PlantSeeds(seed);

    int* nums;

    int legg = 0, med = 0, pes = 0;
    char *str = malloc(200);

    switch (num_simulazione) {


        case pseudo_casuale_s:
            aggiungi_configurazione_cassa(pseudo_casuale, num_casse, 0);
        return "pseudo_casuale";

        case sperimentale_60_20_20:
            nums = get_split(num_casse, 60, 20, 20);
            aggiungi_configurazione_selettiva_custom(nums[0], nums[1], nums[2], 1, 1, 1);
            free(nums);
        return "sperimentale_60_20_20";
            
        case sperimentale_20_60_20:
            nums = get_split(num_casse, 60, 20, 20);
            aggiungi_configurazione_selettiva_custom(nums[1], nums[0], nums[2], 1, 1, 1);
            free(nums);
        return "sperimentale_20_60_20";

        case sperimentale_20_20_60:
            nums = get_split(num_casse, 60, 20, 20);
            aggiungi_configurazione_selettiva_custom(nums[2], nums[1], nums[0], 1, 1, 1);
            free(nums);
        return "sperimentale_20_20_60";

        case sperimentale_leggera_condivisa:
            nums = get_split(num_casse, 60, 20, 20);
            aggiungi_configurazione_selettiva_custom(nums[0], 0, 0, 1, 1, 1);
            aggiungi_configurazione_cassa(condivisa, nums[1]+nums[2], 0);
            free(nums);
        return "sperimentale_leggera_condivisa";

        case sperimentale_media_condivisa:
            nums = get_split(num_casse, 60, 20, 20);
            info_su_configurazioni_attive();
            aggiungi_configurazione_selettiva_custom(0, nums[0], 0, 1, 1, 1);
            aggiungi_configurazione_cassa(condivisa, nums[1]+nums[2], 0);
            //info_su_configurazioni_attive();
            free(nums);
        return "sperimentale_media_condivisa";

        case sperimentale_pesante_condivisa:
            nums = get_split(num_casse, 60, 20, 20);
            aggiungi_configurazione_selettiva_custom(0, 0, nums[0], 1, 1, 1);
            aggiungi_configurazione_cassa(condivisa, nums[1]+nums[2], 0);
            free(nums);
        return "sperimentale_pesante_condivisa";

        case incr_2_10_cond:
            aggiungi_configurazione_cassa(condivisa, num_casse, 0);
            sprintf(str, "Condivisa");
            return str;
        case incr_2_10_pc:
            aggiungi_configurazione_cassa(pseudo_casuale, num_casse, 0);
            sprintf(str, "Pseudo-Casuale");
            return str;
        case incr_3_10_sel:
            aggiungi_configurazione_cassa(selettiva, num_casse, 0);
            sprintf(str, "Selettiva");
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
            sprintf(str, "Selettiva con fila condivisa");
            return str;
        case mista_3_x:
            //num_casse sono le condivise oltre le 3 selettive
            aggiungi_configurazione_cassa(selettiva, 3+num_casse, num_casse);
            sprintf(str, "3 selettive più casse a file condivisa");
            return str;
        case mista_1_x:
            aggiungi_configurazione_selettiva_custom(1, 0, 0, 1, 0, 0);
            aggiungi_configurazione_cassa(condivisa,num_casse, 0);
            sprintf(str, "1 selettiva (leggera) più casse a fila condivisa");
            return str;
    }


    //creazione delle configurazioni di cassa
    //Testato OK (singolarmente)
    //aggiungi_configurazione_cassa(condivisa, 3, 0);
    //aggiungi_configurazione_selettiva_custom(1, 1, 1, 0, 1, 0);
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

    return "undefined";

}

void simulazioni_ezio();

void tutte_simulazioni_ezio(){

    simulazioni_ezio(incr_2_10_cond, 2, 10);

    /*

    simulazioni_ezio(incr_2_10_pc, 2, 10);
    simulazioni_ezio(incr_3_10_sel, 3, 10);
    simulazioni_ezio(incr_3_10_sel_cond, 3, 10);
    simulazioni_ezio(mista_3_x, 1, 7);
    simulazioni_ezio(mista_1_x, 1, 9);
    */


}


int main() {

    tutte_simulazioni_ezio();
    return 0;


    FILE *ff = fopen("simulation_results_ezio.csv", "a");
    if(ff == NULL)
    {
        printf("Impossibile creare/aprire file!");
        exit(1);
    }

    super_supermarket = 1;
    super_factor = 10;

    double alpha = 0.05;

    int num_cassieri = 1;

    int simu_id = sperimentale_pesante_condivisa;

    for (int c = 4; c < 15; c++) {
        giorno_corrente = lun;
        
        printf("Number of cashiers: %d\n", c);
        do{

            double medie[num_simulazioni];
            double varianze[num_simulazioni];
            double slowdowns[num_simulazioni];
            int abbandoni_tot[num_simulazioni];
            int arrivi_tot[num_simulazioni];

            char *tipo_config_str;

            int seed = SEED;

            //fare num_ismulazioni per ogni giorno, commentare i risultati in base al giorno
            for(int i = 0; i < num_simulazioni; ++i) {

                eventi = NULL;
                config_attive = NULL;
                clienti_serviti = NULL;
                arrivi_totali = 0;
                abbandoni = 0;
                attesa_media_corrente = 0;
                varianza_tempo_attesa = 0;
                slowdown_medio_corrente = 0;

                tipo_config_str = inizializza(simu_id, c, seed);

                seed++;

                
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
                medie[i] = attesa_media_corrente;
                varianze[i] = varianza_tempo_attesa;
                slowdowns[i] = slowdown_medio_corrente;
                arrivi_tot[i] = arrivi_totali;
                abbandoni_tot[i] = abbandoni;

                //printf("Done with simulation %d\n", i);

            }

            double ic_att_l = 0;
            double ic_att_r = 0;

            double ic_var_l = 0;
            double ic_var_r = 0;

            double ic_slow_l = 0;
            double ic_slow_r = 0;

            double ic_arr_l = 0;
            double ic_arr_r = 0;

            double ic_abb_l = 0;
            double ic_abb_r = 0;

            //Calcolo medie
            for (int j = 0; j < num_simulazioni; j++) {

                ic_att_l += medie[j];
                ic_var_l += varianze[j];
                ic_slow_l += slowdowns[j];
                ic_arr_l += arrivi_tot[j];
                ic_abb_l += abbandoni_tot[j];

            }

            ic_att_l = ic_att_l / num_simulazioni;
            ic_var_l = ic_var_l / num_simulazioni;
            ic_slow_l = ic_slow_l / num_simulazioni;
            ic_arr_l = ic_arr_l / num_simulazioni;
            ic_abb_l = ic_abb_l / num_simulazioni;

            //Calcolo Deviazione standard
            for (int j = 0; j < num_simulazioni; j++) {
                ic_att_r += pow(medie[j] - ic_att_l, 2);     
                ic_var_r += pow(varianze[j] - ic_var_l, 2);
                ic_slow_r += pow(slowdowns[j] - ic_slow_l, 2);
                ic_arr_r += pow(arrivi_tot[j] - ic_arr_l, 2);
                ic_abb_r += pow(abbandoni_tot[j] - ic_abb_l, 2);
            }

            ic_att_r = sqrt(ic_att_r / num_simulazioni);
            ic_var_r = sqrt(ic_var_r / num_simulazioni);
            ic_slow_r = sqrt(ic_slow_r / num_simulazioni);
            ic_arr_r = sqrt(ic_arr_r / num_simulazioni);
            ic_abb_r = sqrt(ic_abb_r / num_simulazioni);

            fprintf(ff, "%d, %s, %d, %s, \"[%f, %f]\", \"[%f, %f]\", \"[%f, %f]\", \"[%f, %f]\", \"[%f, %f]\", %d, %d, %f\n", simu_id, tipo_config_str, num_cassieri, giorno_str(giorno_corrente), ic_att_l, ic_att_r, ic_slow_l, ic_slow_r, ic_var_l, ic_var_r, ic_abb_l, ic_abb_r, ic_arr_l, ic_arr_r, massima_lunghezza_fila_tollerata, num_simulazioni, alpha);

            printf("Done with day %d\n", giorno_corrente);
            giorno_corrente++;
        } while(giorno_corrente <= dom);
    }
    fclose(ff);
}

int minuti_sotto(int minuti){

    return (double)(minuti_sopportati-minuti)/60;
}


void simulazioni_ezio(int tipo_simulazione, int numero_iniziale_cassieri, int max_num_cassieri){


    FILE *ff = fopen("simulation_results_ezio.csv","a");
    if(ff == NULL)
    {
        printf("Impossibile creare/aprire file!");
        exit(1);
    }

    super_supermarket = 1;
    super_factor = 10;

    int alpha = 5;
    int seed;

    for(int num_cassieri = numero_iniziale_cassieri; num_cassieri <= max_num_cassieri; num_cassieri++) {

        giorno_corrente = lun;
        do {

            double medie_att[num_simulazioni];
            double slowdowns[num_simulazioni];
            int abbandoni_tot[num_simulazioni];
            int arrivi_tot[num_simulazioni];

            char *tipo_config_str;

            seed = 94823498;

            //fare num_ismulazioni per ogni giorno, commentare i risultati in base al giorno
            for (int i = 0; i < num_simulazioni; ++i) {

                seed = seed - 1;

                eventi = NULL;
                config_attive = NULL;
                clienti_serviti = NULL;
                arrivi_totali = 0;
                abbandoni = 0;
                attesa_media_corrente = 0;
                varianza_tempo_attesa = 0;
                slowdown_medio_corrente = 0;


                tipo_config_str = inizializza(tipo_simulazione, num_cassieri, seed);

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

            //TODO magari usare coefficienti diversi per calcolare costo mensile
            //calcolo del costo mensile del supermercato.
            double costo_mensile = num_cassieri*2*guadagno_mensile_cassieri - guadagno_attesa_cliente*minuti_sotto(Xnatt)*30*Xnarr + costo_abbandono_cliente*Xnabb*30;

            fprintf(ff, "%s, %d, %s, \"[%s, %s]\", \"[%f, %f]\", \"[%s, %s]\", \"[%f, %f]\", \"[%f, %f]\", %d, %d, %d\n",
                    tipo_config_str, num_cassieri, giorno_str(giorno_corrente), secondi_ora(ic_att_l),
                    secondi_ora(ic_att_r), ic_slow_l, ic_slow_r, secondi_ora(ic_var_l), secondi_ora(ic_var_r), ic_abb_l, ic_abb_r, ic_arr_l, ic_arr_r,
                    massima_lunghezza_fila_tollerata, num_simulazioni, num_cassieri);


            printf("%s, %d, %s, [%s, %s], [%f, %f], [%s, %s], [%f, %f], [%f, %f], %d, %d, %d, %f\n",
                   tipo_config_str, num_cassieri, giorno_str(giorno_corrente), secondi_ora(ic_att_l),
                   secondi_ora(ic_att_r), ic_slow_l, ic_slow_r, secondi_ora(ic_var_l), secondi_ora(ic_var_r), ic_abb_l, ic_abb_r, ic_arr_l, ic_arr_r,
                   massima_lunghezza_fila_tollerata, num_simulazioni, num_cassieri, costo_mensile);


            printf("Giorno: %s fatto, con %d num_casse\n", giorno_str(giorno_corrente), num_cassieri);

            giorno_corrente++;

        } while (giorno_corrente <= dom);
    }

    fclose(ff);
}