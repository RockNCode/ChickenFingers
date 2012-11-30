#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <pthread.h>
#include <sys/time.h>
#include <error.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO
#include <curses.h>

#define NOTFOUND 0xFF


/* Structures */
typedef struct{
    char letra;
    int row;
    int col;
}letter;
typedef struct{
    void* phandle;
    volatile int comthrdstop;
    pthread_t commFunc;
    pthread_t dispThr;
    letter *letarr[10];
    char arr[30][30];
    int points;
    int level;
    int sleep_t;
    int delta_t;
    int max_index;
    int lives;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}appPrivateSt;

/*Function prototypes*/
void * CommandListener(void* pThreadData);
void * DisplayThread(void* pThreadData);
appPrivateSt *alloc_app_resources(void);
void clrscr(void);
void display(appPrivateSt *appPrvt);
void fillArr(char arr[30][30]);
void moveDown(appPrivateSt *appPrvt);
char getNewLetter();
int searchKey(appPrivateSt * appPrvt, char key);
void addLetter(appPrivateSt * appPrvt);
void processFoundKey(appPrivateSt * appPrvt, int index);
void freeResources(appPrivateSt * appPrvt);
