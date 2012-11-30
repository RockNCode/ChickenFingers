#include "chickenFinger.h"

void * displayThread(void* pThreadData){

	int error = 0;
	appPrivateSt * appPrvt = (appPrivateSt *)pThreadData;
	while(!appPrvt->comthrdstop){
		display(appPrvt);
		usleep(100000);
	}
	return (void*)error;
}
