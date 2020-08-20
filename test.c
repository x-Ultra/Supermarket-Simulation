//
// Created by ezio on 20/08/20.
//

#include <stdio.h>
#include <stdlib.h>

struct list{
    int num;
    struct list *next;
};

struct list *l;

void test(){

    struct list *new = malloc(sizeof(struct list));

    new->num = 0;
    new->next = l;

    l = new;
}

int main(){

    l = malloc(sizeof(struct list));


    l->num = 1;
    l->next = malloc(sizeof(struct list));

    l->next->num = 2;
    l->next->next = NULL;

    test();

    for(struct list *c = l; l != NULL; c = c->next){
        printf("Num: %d\n", c->num);
    }

    return 0;
}