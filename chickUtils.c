#include "chickenFinger.h"

void clrscr(void)
{
    static int init;
    //printf(" \033[2J");
    if (init == 0)
    {
        initscr();
        init = 1;
    }
    
    clear();
    refresh();
}

appPrivateSt *alloc_app_resources(void){
    printf("entering alloca \n");
    appPrivateSt* me = (appPrivateSt *) malloc (sizeof (appPrivateSt));
    if (NULL == me) {
        perror("malloc-app_core_new");
        return me;
    }
    me->commFunc =NULL;
    me->comthrdstop=0;
    me->let1=(letter*)malloc(sizeof(letter));
    me->let1->letra =getNewLetter();
    me->let1->row=0;
    me->let1->col=0;
    me->let2=(letter*)malloc(sizeof(letter));
    me->points=0;
    me->level=0;
    me->sleep_t=500000;
    me->delta_t=100000;
    fillArr(me->arr);
    printf("exiting alloca \n");
    return me;
}

void display(appPrivateSt *appPrvt){
    int i=0,j=0;
    system("clear");
    printf(" Este es el juego de Menios Chicken Finger \n");
    printf(" ========================================= \n");
    printf(" ========= Puntos %d === Nivel %d ========== \n", appPrvt->points, appPrvt->level);
    printf(" ========================================= \n");
    for(i=0;i<30;i++){
        for(j=0;j<30;j++){
            if(0 == j && 29 != i)
                printf(" ||");
            if(29 == j && 29 != i)
                printf("         ||");
            printf("%c",appPrvt->arr[i][j]);
            if(i==29 && 21>j)
                printf("=");
        }
        printf("\n");
    }
}

void fillArr(char arr[30][30]){
    //system("clear");
    int i=0,j=0;
    for(i=0;i<30;i++){
        for(j=0;j<30;j++){
            arr[i][j]=' ';
        }
    }
}
void moveDown(appPrivateSt *appPrvt){
    char temp=' ';
    int i=0,j=0;
    for(i=29;i>=0;i--){
        for(j=29;j>=0;j--){
            temp=appPrvt->arr[i][j];
            if(i<29){
                appPrvt->arr[i+1][j]=temp;                
                appPrvt->arr[i][j]=' ';
                if(appPrvt->arr[i+1][j] == appPrvt->let1->letra){
                    if(i>=28){
                        appPrvt->comthrdstop=1;
                        return;
                    }
                    appPrvt->let1->row = i+1;
                    appPrvt->let1->col = j;
                }
                else if(appPrvt->arr[i+1][j] == appPrvt->let2->letra){
                    if(i>=28){
                        appPrvt->comthrdstop=1;
                        return;
                    }
                    appPrvt->let2->row = i+1;
                    appPrvt->let2->col = j;
                }
            }
        }
    }
}

char getNewLetter(){
    char x='x';
    int rand_n = rand() % 10 + 1;
    switch(rand_n){
    case 1:
        x='a';
        break;
    case 2:
        x='s';
        break;
    case 3:
        x='d';
        break;
    case 4:
        x='f';
        break;
    case 5:
        x='g';
        break;
    case 6:
        x='h';
        break;
    case 7:
        x='j';
        break;
    case 8:
        x='k';
        break;
    case 9:
        x='l';
        break;
    case 10:
        x='q';
        break;
    }
    return x;
}
