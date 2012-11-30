#include "chickenFinger.h"

#undef DEBUG

void clrscr(void)
{
	static int init;

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
	int last_index = NOTFOUND;
	for(i = appPrvt->max_index ;i >= 0; i--){
		if(key == appPrvt->letarr[i]->letra){
			last_index=i;
		}
	}

	return last_index;
}

void displayLetterArray(appPrivateSt *appPrvt){
	int i=0;
	for(i=0;i<=appPrvt->max_index;i++){
		printf(" arr[%d]= %c ",i,appPrvt->letarr[i]->letra);
	}
}

void deleteElement(appPrivateSt *appPrvt, int index){
	int i=index;
	char temp;
	int row,col;
	letter * delLetter = appPrvt->letarr[index];
	if(appPrvt->arr[delLetter->row][delLetter->col] != ' '){
		appPrvt->arr[delLetter->row][delLetter->col] = ' ';
	}else{
		row = delLetter->row;
		col = delLetter->col;
	}
	for(i=index;i < appPrvt->max_index;i++){
		temp = appPrvt->letarr[i+1]->letra;
		row = appPrvt->letarr[i+1]->row;
		col = appPrvt->letarr[i+1]->col;
		appPrvt->letarr[i]->letra = temp;
		appPrvt->letarr[i]->row = row;
		appPrvt->letarr[i]->col = col;
		appPrvt->letarr[i+1]->letra=' ';
	}
	appPrvt->max_index--;
}

int getColumn(appPrivateSt * appPrvt){
	int i=0,maxCol=0;
	int temp = 3*(rand() % 10 + 2);
	for(i=0;i<=appPrvt->max_index;i++){
		if(maxCol < appPrvt->letarr[i]->col){
			maxCol = appPrvt->letarr[i]->col;
		}
	}
	for(i=0;i<=appPrvt->max_index;i++){
		if(temp == appPrvt->letarr[i]->col){
			temp=maxCol+1;
		}
	}
	return temp;
}

void addLetter(appPrivateSt * appPrvt){
	int max = ++appPrvt->max_index;
	letter *newLetter = appPrvt->letarr[max];

	newLetter->letra = getNewLetter();
	newLetter->row = 0;
	newLetter->col = getColumn(appPrvt);

	appPrvt->arr[newLetter->row][newLetter->col]=
		newLetter->letra;
}

void processFoundKey(appPrivateSt * appPrvt, int index){
	deleteElement(appPrvt,index);
	addLetter(appPrvt);
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
	me->commFunc = (pthread_t)NULL;
	me->comthrdstop=0;

	for(i=0; i<10; i++)
		me->letarr[i]=(letter*)malloc(sizeof(letter));
	me->letarr[0]->letra = getNewLetter();
	me->letarr[0]->row=0;
	me->letarr[0]->col=2;
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
#ifdef DEBUG
	displayLetterArray(appPrvt);
	printf("\n");
#endif

	printf(" Chicken Fingers !!!!!\n");
	printf(" ================================================== \n");
	printf(" ======== Points %d === Level %d == Lives %d ========= \n",
		   appPrvt->points, appPrvt->level,appPrvt->lives);
	printf(" ================================================== \n");
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			if(0 == j && 29 != i)
				printf(" ||");
			if(29 == j && 29 != i)
				printf("		 ||");
			if(i < 22 ){
				printf("%c",appPrvt->arr[i][j]);
			}
			else{
				printf("\033[0;31m%c\033[0m",
					   appPrvt->arr[i][j]);
			}
			if(i==29 && 25>j)
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

void updateLetters(appPrivateSt *appPrvt){
	int i=0;
	for(i=0;i <= appPrvt->max_index; i++){
		appPrvt->letarr[i]->row += 1;
		if(appPrvt->letarr[i]->row >= 28){
			appPrvt->comthrdstop=1;
			return;
		}
	}
}

void moveDown(appPrivateSt *appPrvt){
	char temp=' ';
	int index = NOTFOUND;
	int i=0,j=0;
	pthread_mutex_lock(&appPrvt->mutex);
	updateLetters(appPrvt);
	pthread_mutex_unlock(&appPrvt->mutex);
	for(i=29;i>=0;i--){
		for(j=29;j>=0;j--){
			temp=appPrvt->arr[i][j];
			if(i<29){
				pthread_mutex_lock(&appPrvt->mutex);
				appPrvt->arr[i+1][j]=temp;
				appPrvt->arr[i][j]=' ';
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
