//
// Created by ezio on 19/08/20.
//

#include <stdlib.h>
#include <stdio.h>

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
#define max_acquisti 30

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

//TODO riempire con medie vere
//media (dell'esponenziale) arrivo clienti
#define arrivi_lun_6_7 20
#define arrivi_lun_7_8 20
#define arrivi_lun_8_9 20
#define arrivi_lun_9_10 20
#define arrivi_lun_10_11 20
#define arrivi_lun_11_12 20
#define arrivi_lun_12_13 20
#define arrivi_lun_13_14 20
#define arrivi_lun_14_15 20
#define arrivi_lun_15_16 20
#define arrivi_lun_16_17 20
#define arrivi_lun_17_18 20
#define arrivi_lun_18_19 20
#define arrivi_lun_19_20 20
#define arrivi_lun_20_21 20
#define arrivi_lun_21_22 20

//....

//giorno della settimana
#define lun 0
#define mar 1
#define mer 2
#define gio 3
#define ven 4
#define sab 5
#define dom 6

//configurazioni di cassa attive
struct config_cassa_attive *config_attive = NULL;

//lista di eventi
struct lista_eventi *eventi = NULL;

//lista di tutti i clienti serviti
struct fila_cassa *clienti_serviti = NULL;

//per monitorare l'arrivo e gli abbandonidei clienti
int arrivi_totali = 0;
int abbandoni = 0;

//massimo tempo di attesa (30 minuti ?)
int max_attesa = 30*60;
double attesa_media_corrente = 0;

//media distribuzione esponenziale del num ogg cliente
double Xn = 13.343702;

//inizio del giorno
int giorno_corrente = 0;

//semi
int SEED = 1234567;

//l'idea è sottrarre al seme 1, per ogni simulazione (settimanale)
//che viene fatta, in modo da applicare la replicazione
int num_simulazioni = 10;

//coefficienti estratti dalla retta di regressione
//per stimare il tempo di servizio in base al numero di oggetti
double A = 2.61;
double B = 20.56;

//id per file
int file_ids = 0;