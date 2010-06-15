#include "chickenFinger.h"

int main(){
    int error = 0;
    appPrivateSt *appPrvt = NULL;
    appPrvt = alloc_app_resources();
    appPrvt->arr[0][2]=appPrvt->let1->letra;
    error = pthread_create (&appPrvt->commFunc, NULL, CommandListener, appPrvt);
    while(!appPrvt->comthrdstop){
        display(appPrvt);
        usleep(appPrvt->sleep_t);
        moveDown(appPrvt);
        if(appPrvt->comthrdstop){
            printf("Perdiste !\n");
            break;
        }
    }
    return 0;
}
