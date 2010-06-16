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

int searchKey(appPrivateSt * appPrvt, char key){
    int i;
    for(i=0;i<=appPrvt->max_index;i++){
        if(appPrvt->letarr[i]->letra==key){
            return i;
        }
    }
    return NOTFOUND;
}

void addLetter(appPrivateSt * appPrvt){
    appPrvt->max_index++;
    appPrvt->letarr[appPrvt->max_index]->letra = getNewLetter();
    appPrvt->letarr[appPrvt->max_index]->row = 0;
    appPrvt->letarr[appPrvt->max_index]->col = 3*(rand() % 10 + 2);
    appPrvt->arr[appPrvt->letarr[appPrvt->max_index]->row][appPrvt->letarr[appPrvt->max_index]->col]=
        appPrvt->letarr[appPrvt->max_index]->letra;
}

void processFoundKey(appPrivateSt * appPrvt, int index){
    appPrvt->arr[appPrvt->letarr[index]->row][appPrvt->letarr[index]->col] = ' ';
    appPrvt->letarr[index]->letra = getNewLetter();
    appPrvt->letarr[index]->row = 0;
    appPrvt->letarr[index]->col = rand() % 10 + 2;
    appPrvt->arr[appPrvt->letarr[index]->row][appPrvt->letarr[index]->col]= appPrvt->letarr[index]->letra;
    appPrvt->points++;
    if(0==(appPrvt->points)%10){
        appPrvt->level+=1;
        appPrvt->sleep_t-=appPrvt->delta_t;
    }
    if(0==(appPrvt->level+1)%4){
        appPrvt->delta_t=appPrvt->delta_t/2;
    }

    if(appPrvt->level==1 && appPrvt->points==10){
        addLetter(appPrvt);
    }
    else if(appPrvt->level==2 && appPrvt->points==20){
        addLetter(appPrvt);
    }
    else if(appPrvt->level==3 && appPrvt->points==30){
        addLetter(appPrvt);
    }
    else if(appPrvt->level==4 && appPrvt->points==30){
        addLetter(appPrvt);
    }
}

appPrivateSt *alloc_app_resources(void){
    appPrivateSt* me = (appPrivateSt *) malloc (sizeof (appPrivateSt));
    int i=0;
    if (NULL == me) {
        perror("malloc-app_core_new");
        return me;
    }
    me->commFunc =NULL;
    me->comthrdstop=0;

    for(i=0; i<10; i++)
        me->letarr[i]=(letter*)malloc(sizeof(letter));
    me->letarr[0]->letra = getNewLetter();
    me->letarr[0]->row=0;
    me->letarr[0]->col=0;
    me->max_index=0;
    me->points=0;
    me->level=0;
    me->sleep_t=500000;
    me->delta_t=100000;
    fillArr(me->arr);
    me->max_index=0;
    printf("exiting alloca \n");
    return me;
}

void display(appPrivateSt *appPrvt){
    int i=0,j=0;
    system("clear");
    printf(" This is Menios Chicken Finger game !!!!!\n");
    printf(" ========================================= \n");
    printf(" ========= Points %d === Level %d ========== \n", appPrvt->points, appPrvt->level);
    printf(" ========================================= \n");
    for(i=0;i<30;i++){
        for(j=0;j<30;j++){
            if(0 == j && 29 != i)
                printf(" ||");
            if(29 == j && 29 != i)
                printf("         ||");
            if(i < 22 ){
                printf("%c",appPrvt->arr[i][j]);
            }
            else{
                printf("\033[0;31m%c\033[0m",appPrvt->arr[i][j]); // prints "hello, world" in red
            }
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
    int index = NOTFOUND;
    int i=0,j=0;
    for(i=29;i>=0;i--){
        for(j=29;j>=0;j--){
            temp=appPrvt->arr[i][j];
            if(i<29){
                appPrvt->arr[i+1][j]=temp;                
                appPrvt->arr[i][j]=' ';
                index = searchKey(appPrvt,temp);
                if( NOTFOUND != index){
                    if(i>=28){
                        appPrvt->comthrdstop=1;
                        return;
                    }
                    appPrvt->letarr[index]->row = i+1;
                    appPrvt->letarr[index]->col = j;
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
