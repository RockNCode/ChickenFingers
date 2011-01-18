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

void printListKey(appPrivateSt * appPrvt){
    int i;
    for(i=0;i<=appPrvt->max_index;i++){
        printf("arr[%d] = %c col = %d \n",i,
               appPrvt->letarr[i]->letra,appPrvt->letarr[i]->col);
    }
}

int isValidCol(appPrivateSt * appPrvt, int col){
    int i;
    for(i=0;i<=appPrvt->max_index;i++){
        if(appPrvt->letarr[i]->col == col){
            return 1;
        }
    }
    return 0;
}

int isValidLetter(appPrivateSt * appPrvt, char letter){
    int i;
    for(i=0;i<=appPrvt->max_index;i++){
        if(appPrvt->letarr[i]->letra == letter){
            return 1;
        }
    }
    return 0;
}

void addLetter(appPrivateSt * appPrvt){
    int col = rand() % 25 + 2;
    char letter = getNewLetter();
    appPrvt->max_index++;
    appPrvt->letarr[appPrvt->max_index]->row = 0;
    /*prevent overlapping columns*/
    while(isValidCol(appPrvt, col)){
        col = rand() % 30 + 2;
    }
    appPrvt->letarr[appPrvt->max_index]->col=col;
    /*prevent letter repetition, for now*/
    while(isValidLetter(appPrvt,letter)){
        letter = getNewLetter();
    }
    appPrvt->letarr[appPrvt->max_index]->letra = letter;

    appPrvt->letarr[appPrvt->max_index]->myTimeOut = rand() % 3 + 1;
    appPrvt->arr[appPrvt->letarr[appPrvt->max_index]->row][appPrvt->letarr[appPrvt->max_index]->col]=
        appPrvt->letarr[appPrvt->max_index]->letra;
}

void removeLetter(appPrivateSt * appPrvt,int index){
    appPrvt->arr[appPrvt->letarr[index]->row][appPrvt->letarr[index]->col] = ' ';
    int i =0;
    for (i=index; i<= appPrvt->max_index;i++){
        appPrvt->letarr[i]->letra =appPrvt->letarr[i+1]->letra;
        appPrvt->letarr[i]->row =appPrvt->letarr[i+1]->row;
        appPrvt->letarr[i]->col =appPrvt->letarr[i+1]->col;
    }
    appPrvt->letarr[appPrvt->max_index]->letra =' ';
    appPrvt->letarr[appPrvt->max_index]->row = 0;
    appPrvt->letarr[appPrvt->max_index]->col = 0;
    appPrvt->max_index--;
}

void processFoundKey(appPrivateSt * appPrvt, int index){

    /*appPrvt->letarr[index]->letra = getNewLetter();
    appPrvt->letarr[index]->row = 0;
    appPrvt->letarr[index]->col = rand() % 10 + 2;
    appPrvt->arr[appPrvt->letarr[index]->row][appPrvt->letarr[index]->col]= appPrvt->letarr[index]->letra;*/
    removeLetter(appPrvt,index);
    appPrvt->points++;
    addLetter(appPrvt);
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
    me->lives=3;
    pthread_mutex_init(&me->mutex,NULL);
    return me;
}

void display(appPrivateSt *appPrvt){
    int i=0,j=0;
    system("clear");
    printf(" This is Menios Chicken Finger game !!!!!\n");
    printf(" ========================================= \n");
    printf(" == Points %d === Level %d == Lives %d ====== \n", appPrvt->points, appPrvt->level,appPrvt->lives);
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
#ifdef DEBUG
    printf("Current Keys in list \n");
    printListKey(appPrvt);
#endif
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
                pthread_mutex_lock(&appPrvt->mutex);
                index = searchKey(appPrvt,temp);
                if( NOTFOUND != index){
                    if(i>=28){
                        appPrvt->comthrdstop=1;
                        return;
                    }
                    appPrvt->letarr[index]->timeout++;
                    if(appPrvt->letarr[index]->timeout >= appPrvt->letarr[index]->myTimeOut){
                        appPrvt->letarr[index]->timeout=0;
                        appPrvt->letarr[index]->row = i+1;
                        appPrvt->letarr[index]->col = j;
                        appPrvt->arr[i+1][j]=temp;
                        appPrvt->arr[i][j]=' ';
                    }
                }
                pthread_mutex_unlock(&appPrvt->mutex);
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

void freeResources(appPrivateSt * appPrvt){
    int i=0;
    for(i=0; i<10; i++)
        free(appPrvt->letarr[i]);
    pthread_mutex_destroy(&appPrvt->mutex);
}
