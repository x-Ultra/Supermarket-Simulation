//
// Created by ezio on 07/09/20.
//

#include <unistd.h>
#include <stdio.h>
#include "strutture_dati.h"
#include <sys/types.h>
#include <sys/wait.h>


char *carica_stato_generatore(){

    FILE *fp;
    char *stato = malloc(20);

    fp = fopen(filename_stato, "r");
    fscanf(fp, "%s", stato);
    fclose(fp);

    return stato;
}


int main(){

    int choice = 0;
    int exit = 0;

    printf("-------- SUPERMARKT SIMULATION --------\n\n");
    while(1){
        printf("0)Inizia tutte le simulazioni\n");
        printf("1)Avvia la simulazione manuale\n");
        printf("Cosa vuoi fare: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                if(execl("./simu", "./simu", "test-manuale", NULL) == -1){
                    printf("Errore nel lanciare ./simu");
                    return -1;
                }
                return 0;
            case 0:
                exit = 1;
                break;
        }

        if(exit){
            break;
        }
    }

    //qui lancio tutte le simulazioni
    char *tipo = malloc(10);
    char *min_cassieri = malloc(5);
    char *max_cassieri = malloc(5);

    int pid;
    int status;

    sprintf(tipo, "%d", sperimentale_60_20_20);
    sprintf(min_cassieri, "%d", 4);
    sprintf(max_cassieri, "%d", 10);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    //dopo la prima simulazinoe lo stato del generatore è stato salvato
    //bisogna prenderlo
    printf("qui\n");
    sprintf(tipo, "%d", sperimentale_20_60_20);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", sperimentale_20_20_60);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", sperimentale_leggera_condivisa);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", sperimentale_media_condivisa);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", sperimentale_pesante_condivisa);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", incr_2_10_cond);
    sprintf(min_cassieri, "%d", 2);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", incr_2_10_pc);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", incr_3_10_sel);
    sprintf(min_cassieri, "%d", 3);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", incr_3_10_sel_cond);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", mista_3_x);
    sprintf(min_cassieri, "%d", 1);
    sprintf(max_cassieri, "%d", 7);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }

    sprintf(tipo, "%d", mista_1_x);
    sprintf(max_cassieri, "%d", 9);
    pid = fork();
    if(pid == 0){
        if(execl("./simu", "./simu", tipo, min_cassieri, max_cassieri, carica_stato_generatore(), NULL) == -1){
            printf("Errore nel lanciare ./simu");
            return -1;
        }
    }else{
        wait(&status);
    }


}