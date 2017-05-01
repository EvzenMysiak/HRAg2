#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "variables.h"

typedef int (*commands)(char*);

//-------Struktury----------
typedef struct central_store{
    int jedlo;
    int pivo;
    int ruda;
    int mince;
    int vodka;
    int palenka;
}store;

typedef struct my_player{
    char ID[3];
    char name[20];
    int lives;
    int hunger;
    int energy;
    int power;
    int stamina;
    int defence;
}player;

typedef struct wolf{
    int sila;
    int defence;
    int stamina;
}WOLF;
//-------END struktury------

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
    printf("\nCast MENU\n");
    char testtext;
    char *testtext2;
    int pocet_riadkov;
    player* myplayer;

    FILE *fdefault;
    fdefault=fopen("/home/mysiak/CLionProjects/CV11/HRAg2/default.txt","r");
    if (fdefault==NULL){
        printf("Subor s defaultnym nastavenim sa nepodarilo otvorit");
        return NULL;
    }
    pocet_riadkov=rows(fdefault);
    rewind(fdefault);
    myplayer=(player*)calloc(pocet_riadkov,sizeof(player));
    loadFromFile(fdefault,myplayer);

    for (int i = 0; i <pocet_riadkov; ++i) {
        printf("%s ",myplayer[i].ID);
        printf("%s ",myplayer[i].name);
        printf("%d ",myplayer[i].lives);
        printf("%d ",myplayer[i].hunger);
        printf("%d ",myplayer[i].energy);
        printf("%d ",myplayer[i].power);
        printf("%d ",myplayer[i].stamina);
        printf("%d \n",myplayer[i].defence);
    }

    fclose(fdefault);
    return 1;
}

void loadFromFile(FILE *fin, player myplayer[]) {

    char buffer[1000];


    //nacitame riadok, rozdelime, posunieme sa na dalsie mesto
    while (fgets(buffer, 1000, fin) != NULL)
    {
        puts(buffer);
        parse(buffer, myplayer);
        myplayer++;
    }
}
int parse(char *line, player *myplayer)
{
    //vyuzivame strchr - najde najblizsi vyskyt znaku v retazci
    char* next = strchr(line, ',');

    next[0] = '\0';
    next++;

    strcpy(myplayer->ID, line);
    line = next;

    next = strchr(line, ',');
    next[0] = '\0';
    next++;

    strcpy(myplayer->name, line);
    line = next;
    next = strchr(line, ',');
    next[0] = '\0';
    next++;

    myplayer->lives=*line;
    line = next;
    next = strchr(line, ',');
    next[0] = '\0';
    next++;

    myplayer->hunger=(int)*line;
    line = next;
    next = strchr(line, ',');
    next[0] = '\0';
    next++;

    myplayer->energy=(int)*line;
    line = next;
    next = strchr(line, ',');
    next[0] = '\0';
    next++;

    myplayer->power=(int)*line;
    line = next;
    next = strchr(line, ',');
    next[0] = '\0';
    next++;

    myplayer->stamina=(int)*line;
    line = next;


    myplayer->defence=(int)*line;
    line = next;

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
    return 1;
}
//koniec funkcii z prikazoveho riadku

int rows(FILE* myfile) {
    int pocet = 0;
    int c;
    while((c=getc(myfile)) != EOF)
    {
        if (c == '\n')
            pocet++;
    }
    return pocet+1;
}


commands find(char* command){
    int n;

    static struct {
        char *alias;
        commands fcn;
    } command_tab[]={
        {"menu", menu},
        {"quit",quit},
        {"print",print},
        {"change",change},
        {"load",load},
        {"save",save},
        {"make",make},
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
    int vyber_menu2=0;
    int potvrdkoniec=1;

    store my_items;
    player hrac;
    WOLF wolf_player;


    printf("\nVyberte si jednu z moznosti\n");
    printf("\nMENU:\n1.\tmenu\n2.\tprint\n3.\tchange\n4.\tsave\n5.\tload\n6.\tmake\n7.\tquit\nPre splnenie prikazu"
                   "napiste prikaz slovom a potvrdte enterom\n");

    command = get_command();
    make_command(command);

    while (potvrdkoniec) {
        printf("\nVyberte si jednu z moznosti:\n");
        printf("\n1.\tPrikazovy riadok\n2.\tKoniec hry");
        scanf("%d", &vyber_menu2);
        switch (vyber_menu2) {
            case 1:
                while (pokracovat) {
                    printf("\nVyberte si jednu z moznosti\n");
                    printf("\nMENU:\n1.\tmenu\n2.\tprint\n3.\tchange\n4.\tsave\n5.\tload\n6.\tmake\n7.\tquit\nPre splnenie prikazu"
                                   "napiste prikaz slovom a potvrdte enterom\n");
                    command = get_command();
                    pokracovat = make_command(command);
                }
                break;
            case 2:
                potvrdkoniec=0;
                break;
        }
    }
    return 0;
}

//TODO vo funkcii print dat ako parameter ze ktory trpaslik sa ma printovat a tiez ak pride v parametri poziadavka
// TODO prerobit zapisovanie do struktury

