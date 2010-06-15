#include "chickenFinger.h"

void * CommandListener(void* pThreadData){
    printf("Entering Command listener \n");
    char key;
    int error=0;
    fd_set rfds;
    appPrivateSt * appPrvt = (appPrivateSt *)pThreadData;
    struct timeval tv;
    int retval;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    static struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    while(!error && !appPrvt->comthrdstop){
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        /*set this attribute to bypass newline character*/
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);
        retval = select(1, &rfds, NULL, NULL, &tv);
        
        if(0 == retval)
            continue;
        else
            scanf(" %c",&key);
        /*set normal stdin read type*/
        if(key == appPrvt->let1->letra || key == appPrvt->let2->letra ){
            if(key == appPrvt->let1->letra){
                appPrvt->arr[appPrvt->let1->row][appPrvt->let1->col] = ' ';
                appPrvt->let1->letra = getNewLetter();
                appPrvt->let1->row = 0;
                appPrvt->let1->col = rand() % 10 + 2;
                appPrvt->arr[appPrvt->let1->row][appPrvt->let1->col]= appPrvt->let1->letra;
                appPrvt->points++;
            }
            else if(key == appPrvt->let2->letra){
                appPrvt->arr[appPrvt->let2->row][appPrvt->let2->col] = ' ';
                appPrvt->let2->letra = getNewLetter();
                appPrvt->let2->row = 0;
                appPrvt->let2->col = rand() % 10 + 2;
                appPrvt->arr[appPrvt->let2->row][appPrvt->let2->col]= appPrvt->let2->letra;
                appPrvt->points++;
            }
            if(0==(appPrvt->points)%10){
                appPrvt->level+=1;
                appPrvt->sleep_t-=appPrvt->delta_t;
            }
            if(0==(appPrvt->level+1)%4){
                appPrvt->delta_t=appPrvt->delta_t/2;
            }

            if(appPrvt->level==3 && appPrvt->points==30){
                appPrvt->let2->letra = getNewLetter();
                appPrvt->let2->row = 0;
                appPrvt->let2->col = rand() % 10 + 2;
                appPrvt->arr[appPrvt->let2->row][appPrvt->let2->col]= appPrvt->let2->letra;
            }

        }
        
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
        switch (tolower(key)){
        case 'c':
            //clrscr();
            //system("clear");
            break;
        case 'p':
            //printf("This is thread 2 \n");

            break;
        case 'z':
            printf("Stopping \n");
            appPrvt->comthrdstop=1;
            break;
        default:
            //printf("Command not recognized \n");
            break;
            };
    }
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    printf("Exiting Command Listener \n");

    return (void*)error;
}

