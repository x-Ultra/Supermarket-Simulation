//
// Created by ezio on 19/08/20.
//

#include <stdlib.h>

//cliente
struct cliente{

    //numerodi oggetti acquistati dal cliente
    int num_oggetti;

    //ora in secondi
    int in_fila;
    int iniziato_a_servire;
    int servito;

    int attesa_in_fila;
};

//fila, lista collegata di clienti
struct fila_cassa{

    struct cliente *cliente_in_fila;
    struct fila_cassa *next;
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
    //oppure puo' essere farra d apiu' casse, ognina con la
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


};

//lista doppiamente collegata di eventi
struct lista_eventi{

    struct evento evento;
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
#define selettiva_media 4
#define selettiva_pesante 5
#define selettiva_str "Selettiva"

#define mista 6
#define mista_str "Mista"

//tipi di evento
#define arrivo 0
#define servizio 1

//ora di apertura e chiusura, in secondi
#define opening_time 6*60*60
#define closing_time 22*60*60

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
#define arrivi_6_7 20
#define arrivi_7_8 20
#define arrivi_8_9 20
#define arrivi_9_10 20
#define arrivi_10_11 20
#define arrivi_11_12 20
#define arrivi_12_13 20
#define arrivi_13_14 20
#define arrivi_14_15 20
#define arrivi_15_16 20
#define arrivi_16_17 20
#define arrivi_17_18 20
#define arrivi_18_19 20
#define arrivi_19_20 20
#define arrivi_20_21 20
#define arrivi_21_22 20

//configurazioni di cassa attive
struct config_cassa_attive *config_attive = NULL;