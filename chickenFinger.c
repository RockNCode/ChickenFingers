#include "chickenFinger.h"

int main(){
    int error = 0;
    appPrivateSt *appPrvt = NULL;
    appPrvt = alloc_app_resources();
    appPrvt->arr[0][2]=appPrvt->letarr[0]->letra;
    error = pthread_create (&appPrvt->commFunc, NULL, CommandListener, appPrvt);
    error = pthread_create (&appPrvt->displayFunc, NULL, displayThread, appPrvt);
    while(!appPrvt->comthrdstop){
        usleep(appPrvt->sleep_t);
        moveDown(appPrvt);
        if(appPrvt->comthrdstop){
            break;
        }
    }
    for(error = 0; error < 4;error++){
        display(appPrvt);
        printf("\033[0;3%dm YOU LOOSE !!!\n",error);
        usleep(appPrvt->sleep_t);
    }
    printf("\033[0m\n");
    freeResources(appPrvt);
    return 0;
}
