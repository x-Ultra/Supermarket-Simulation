//
// Created by ezio on 19/08/20.
//

#include <stdlib.h>
#include <stdio.h>

//#define DEBUG 1

#ifdef DEBUG
#  define D(x) x
#else
#  define D(x)
#endif

//cliente
struct cliente{

    //numerodi oggetti acquistati dal cliente
    int num_oggetti;

    //ora in secondi
    int in_fila;
    int iniziato_a_servire;
    int servito_alle;

    int attesa_in_fila;

    //la fila scelta dal cliente
    struct fila_cassa **fila_scelta;

    //la configurazione di cassa scelta
    struct config_cassa **config_scelta;

    //int id per debug
    int id;
};

//fila, lista collegata di clienti
struct fila_cassa{

    struct cliente *cliente_in_fila;
    struct fila_cassa *next;

    //id per debug
    int id;
};

//casse, lista collegate di più casse, ovvero di piu' file
struct casse{

    struct fila_cassa *fila_cassa;
    struct casse *next;
};

//configurazione di cassa
struct config_cassa{

    //tipo di configurazione di cassa
    int tipo;

    //sottotipo inizializzato se tipo = selettiva o mista
    int sotto_tipo;

    //una configurazione di cassa puo' essere costituita da
    //una singola fila condivisa associata a più casse
    //(in questo caso ogni cassa di 'casse' avrà una fila di
    // un solo cliente)
    //oppure puo' essere farra da piu' casse, ognina con la
    //sua fila di clienti, !! in questo caso fila_condivisa = null !!
    struct casse *casse;
    struct fila_cassa *fila_condivisa;
};

//configurazioni di cassa attive nel supermercato
struct config_cassa_attive{

    struct config_cassa *configurazione_cassa;
    struct config_cassa_attive *next;
};

//evento
struct evento{

    //tipo di evento
    int tipo;

    //tempo in secondi del momento in cui schedulare l'evento
    int tempo;

    //fila dalla quale 'smaltire' il cliente, se l'evento
    //e di tipo 'servito'.
    struct fila_cassa *fila;
};

//lista doppiamente collegata di eventi
struct lista_eventi{

    struct evento *evento;
    struct lista_eventi *next;
    struct lista_eventi *prev;
};


//---------------------------------
//altre variabili globali qui sotto
//---------------------------------


//tipi di configurazione di cassa
#define condivisa 0
#define condivisa_str "Condivisa"

#define pseudo_casuale 1
#define pseudo_casuale_str "Pseudo-Casuale"

#define selettiva 2
#define selettiva_leggera 3
#define selettiva_leggera_str "Leggera"
#define selettiva_media 4
#define selettiva_media_str "Media"
#define selettiva_pesante 5
#define selettiva_pesante_str "Pesante"
#define selettiva_str "Selettiva"

//margini destri di acquisti per casse selettive
#define acquisti_leggeri 10
#define acquisti_medi 20

#define mista 6
#define mista_str "Mista"

//tipi di evento
#define arrivo 0
#define arrivo_str "Arrivo"
#define servito 1
#define servito_str "Servito"

//ora di apertura e chiusura, in secondi
#define opening_time 6*60*60
#define closing_time 22*60*60

//tempo usato per scandire gli eventi
int T = opening_time;

//orari in secondi
#define ore_6 6*60*60
#define ore_7 7*60*60
#define ore_8 8*60*60
#define ore_9 9*60*60
#define ore_10 10*60*60
#define ore_11 11*60*60
#define ore_12 12*60*60
#define ore_13 13*60*60
#define ore_14 14*60*60
#define ore_15 15*60*60
#define ore_16 16*60*60
#define ore_17 17*60*60
#define ore_18 18*60*60
#define ore_19 19*60*60
#define ore_20 20*60*60
#define ore_21 21*60*60
#define ore_22 22*60*60
#define ore_23 22*60*60

//media (dell'esponenziale) arrivo clienti
#define arrivi_lun_6_7 3.250000
#define arrivi_lun_7_8 20.687500
#define arrivi_lun_8_9 23.650000
#define arrivi_lun_9_10 24.227273
#define arrivi_lun_10_11 28.928571
#define arrivi_lun_11_12 32.035714
#define arrivi_lun_12_13 31.612903
#define arrivi_lun_13_14 30.333333
#define arrivi_lun_14_15 27.862069
#define arrivi_lun_15_16 29.320000
#define arrivi_lun_16_17 25.962963
#define arrivi_lun_17_18 25.851852
#define arrivi_lun_18_19 26.814815
#define arrivi_lun_19_20 25.115385
#define arrivi_lun_20_21 23.200000
#define arrivi_lun_21_22 19.142857
#define arrivi_lun_22_23 7.000000

#define arrivi_mar_6_7 3.000000
#define arrivi_mar_7_8 19.500000
#define arrivi_mar_8_9 21.758621
#define arrivi_mar_9_10 23.454545
#define arrivi_mar_10_11 25.384615
#define arrivi_mar_11_12 29.250000
#define arrivi_mar_12_13 29.000000
#define arrivi_mar_13_14 29.825000
#define arrivi_mar_14_15 27.829268
#define arrivi_mar_15_16 27.250000
#define arrivi_mar_16_17 27.128205
#define arrivi_mar_17_18 26.526316
#define arrivi_mar_18_19 26.055556
#define arrivi_mar_19_20 25.687500
#define arrivi_mar_20_21 21.750000
#define arrivi_mar_21_22 20.640000
#define arrivi_mar_22_23 6.400000

#define arrivi_mer_6_7 2.750000
#define arrivi_mer_7_8 20.500000
#define arrivi_mer_8_9 25.571429
#define arrivi_mer_9_10 29.619048
#define arrivi_mer_10_11 28.296296
#define arrivi_mer_11_12 30.321429
#define arrivi_mer_12_13 32.379310
#define arrivi_mer_13_14 30.785714
#define arrivi_mer_14_15 28.066667
#define arrivi_mer_15_16 28.964286
#define arrivi_mer_16_17 28.814815
#define arrivi_mer_17_18 27.384615
#define arrivi_mer_18_19 23.821429
#define arrivi_mer_19_20 24.565217
#define arrivi_mer_20_21 23.315789
#define arrivi_mer_21_22 20.000000
#define arrivi_mer_22_23 4.142857

#define arrivi_gio_6_7 3.375000
#define arrivi_gio_7_8 24.526316
#define arrivi_gio_8_9 24.400000
#define arrivi_gio_9_10 31.120000
#define arrivi_gio_10_11 30.062500
#define arrivi_gio_11_12 31.266667
#define arrivi_gio_12_13 30.818182
#define arrivi_gio_13_14 27.843750
#define arrivi_gio_14_15 25.303030
#define arrivi_gio_15_16 26.806452
#define arrivi_gio_16_17 28.103448
#define arrivi_gio_17_18 26.612903
#define arrivi_gio_18_19 26.466667
#define arrivi_gio_19_20 23.137931
#define arrivi_gio_20_21 21.192308
#define arrivi_gio_21_22 22.600000
#define arrivi_gio_22_23 6.000000

#define arrivi_ven_6_7 3.875000
#define arrivi_ven_7_8 24.052632
#define arrivi_ven_8_9 24.160000
#define arrivi_ven_9_10 28.760000
#define arrivi_ven_10_11 28.272727
#define arrivi_ven_11_12 29.062500
#define arrivi_ven_12_13 30.787879
#define arrivi_ven_13_14 29.468750
#define arrivi_ven_14_15 25.600000
#define arrivi_ven_15_16 25.354839
#define arrivi_ven_16_17 29.607143
#define arrivi_ven_17_18 27.533333
#define arrivi_ven_18_19 25.758621
#define arrivi_ven_19_20 23.666667
#define arrivi_ven_20_21 23.041667
#define arrivi_ven_21_22 21.571429
#define arrivi_ven_22_23 6.000000

#define arrivi_sab_6_7 3.250000
#define arrivi_sab_7_8 17.363636
#define arrivi_sab_8_9 21.214286
#define arrivi_sab_9_10 23.800000
#define arrivi_sab_10_11 26.176471
#define arrivi_sab_11_12 29.062500
#define arrivi_sab_12_13 30.176471
#define arrivi_sab_13_14 29.055556
#define arrivi_sab_14_15 26.000000
#define arrivi_sab_15_16 25.888889
#define arrivi_sab_16_17 24.312500
#define arrivi_sab_17_18 27.600000
#define arrivi_sab_18_19 27.230769
#define arrivi_sab_19_20 26.000000
#define arrivi_sab_20_21 23.666667
#define arrivi_sab_21_22 16.750000
#define arrivi_sab_22_23 4.000000

#define arrivi_dom_6_7 1.500000
#define arrivi_dom_7_8 12.888889
#define arrivi_dom_8_9 16.636364
#define arrivi_dom_9_10 18.833333
#define arrivi_dom_10_11 20.769231
#define arrivi_dom_11_12 25.714286
#define arrivi_dom_12_13 27.428571
#define arrivi_dom_13_14 29.533333
#define arrivi_dom_14_15 24.647059
#define arrivi_dom_15_16 25.500000
#define arrivi_dom_16_17 28.583333
#define arrivi_dom_17_18 28.181818
#define arrivi_dom_18_19 28.500000
#define arrivi_dom_19_20 24.400000
#define arrivi_dom_20_21 21.200000
#define arrivi_dom_21_22 14.111111
#define arrivi_dom_22_23 4.000000

//giorno della settimana
#define lun 0
#define mar 1
#define mer 2
#define gio 3
#define ven 4
#define sab 5
#define dom 6

//tipo di sperimentazioni
#define incr_2_10_cond 10001
#define incr_2_10_pc 10002
#define incr_3_10_sel 10003
#define incr_3_10_sel_cond 10004
#define sperimentale_60_20_20 6
#define sperimentale_20_60_20 7
#define sperimentale_20_20_60 8
#define sperimentale_leggera_condivisa 9
#define sperimentale_media_condivisa 10
#define sperimentale_pesante_condivisa 11
//3 casse selettive e da 1 a 7 casse con fila condivisa
#define mista_3_x 10005
//da 1 a 9 casse ocn fila condivisa più una selettiva legera
#define mista_1_x 10006


#define validation_poche_casse 0

//configurazioni di cassa attive
struct config_cassa_attive *config_attive = NULL;

//lista di eventi
struct lista_eventi *eventi = NULL;

//per monitorare l'arrivo e gli abbandonidei clienti
int arrivi_totali = 0;
int abbandoni = 0;

//massimo lunghezza della fila
int massima_lunghezza_fila_tollerata = 10;
double attesa_media_corrente = 0;
double varianza_tempo_attesa = 0;

double slowdown_medio_corrente = 0;

//media distribuzione esponenziale del num ogg cliente
double Xn = 13.343702;

//inizio del giorno
int giorno_corrente = 0;

//semi
int SEED = 94823498;

//l'idea è sottrarre al seme 1, per ogni simulazione (settimanale)
//che viene fatta, in modo da applicare la replicazione
int num_simulazioni = 20;

//coefficienti estratti dalla retta di regressione
//per stimare il tempo di servizio in base al numero di oggetti
double A = 2.61;
double B = 20.56;

//prova per validazione
int validazione = 0;
//arrivi e smaltimenti al secondo
int mu_valid = 50;
int lambda_valid = 10;

//id per file
int file_ids = 0;

int super_supermarket;
int super_factor = 10;

//vasiabili usate per il calcolo del costo sostenuto dal

//ogni minuto sotto i 5min.....
int minuti_sopportati = 5*60;
//.... il supermercato quagagna 1$
int guadagno_attesa_cliente = 1;

int costo_abbandono_cliente = 20;

int guadagno_mensile_cassieri = 1300;

//file dove verra salvato lo stato del generatore
char *filename_stato = "stato_generatore.txt";
