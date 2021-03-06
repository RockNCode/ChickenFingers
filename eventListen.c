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
	int index = NOTFOUND;
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
		pthread_mutex_lock(&appPrvt->mutex);
		index = searchKey(appPrvt,key);
		if(index != NOTFOUND){
			processFoundKey(appPrvt,index);
		}
		else{
			appPrvt->lives--;
		}
		pthread_mutex_unlock(&appPrvt->mutex);
		if(appPrvt->lives<=0){
			appPrvt->comthrdstop=1;
			continue;
		}
		tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	}
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	return (void*)error;
}

