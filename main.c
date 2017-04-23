#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "variables.h"

typedef int (*commands)(char*);


/*
 * 1.funkcia oddych (naplni si inventar a znizi si unavu/nudu)
 * 2.Bannik (tazi rudu a ak nieje prilis unaveny pribudne unava a
 *      ak ma co jest ubudne mu z jedla)
 * 3.Remeselnik (zaraba premenou rudy zo skladu na minceak nieje prilis unaveny ubudne unava a ak ma co
 *      jest ubudne jedlo)
 * 4.Kuchar (vari jedlo ak nieje prilis unudeny pribuda nuda az kym ho to neprestane bavit
 *      ak nema peniaze vyrobi menej jedla a musi zarobit nieco na suroviny)
 * 5.Pivovarnik (vari pivo pritom sa nudi ale potrebbuje nejake suroviny tak ich vymeni za
 *      mince zo skladu)*/


//funkcie z prikazoveho riadku
int menu(char* params){
    printf("menu");
    return 1;
}

int quit(char* params){
    printf("quit");
    return 0;
}

int print(char* params){
    printf("print");
    return 1;
}

int change(char* params){
    printf("change");
    return 1;
}

int load(char* params){
    printf("load");
    return 1;
}

int save(char* params){
    printf("save");
    return 1;
}

int make(char* params){
    printf("make");
    return 1;
}

int nothing(char* params){
    printf("nothing");
    return 1;
}
//koniec funkcii z prikazoveho riadku


commands find(char* command){
    int n;

    static struct {
        char *alias;
        commands fcn;
    } command_tab[]={
        {"exit", menu},
        {"quit",quit},
        {"print",print},
        {"uprava",change},
        {"nacitaj",load},
        {"uloz",save},
        {"sprav",make},
    };
    n=sizeof(command_tab)/sizeof(command_tab[0]);

    for (int i = 0; i <n; ++i) {
        if (strcmp(command,command_tab[i].alias)==0)
            return command_tab[i].fcn;
    }
    return nothing;
};

commands translate(char** ptr_commandpromt){
    char *commandpromt=*ptr_commandpromt;
    char *params=commandpromt;
    commands make_it;

    while (!isspace(*params)){
        params++;
    }

    *params='\0';
    params++;
    make_it=find(commandpromt);
    *ptr_commandpromt=params;
    return make_it;
}

int make_command(char *commandpromt){
    commands make_it;
    if (commandpromt==NULL)
        return 0;

    make_it=translate(&commandpromt);
    return make_it(commandpromt);
}

char *get_command(){
    static char buffer[MAXBUF];
    printf("> ");
    fgets(buffer,MAXBUF,stdin);
    return buffer;
}

int main(){

    int pokracovat = 1;
    char *command;

    while (pokracovat)
    {
        command = get_command();
        pokracovat = make_command(command);
    }
    return 0;
}

//TODO vo funkcii print dat ako parameter ze ktory trpaslik sa ma printovat a tiez ak pride v parametri poziadavka
//TODO na vypis vsetkych tak v switch case sa zvoli ina funkcia