#include <multithreading.h>
#define N 2

void printf(char* str);
using namespace myos;

int tid1;
int tid2;
int test1;
int test2;

int empty = 256;
int full = 0;
char buffer[256] = {'0'};
int count = 0;
int turn = 0;
bool interested[N] = {false};
ThreadManager tm;

//Peterson algorithm
void enter_region(int process){
    int other;

    other = 1 - process;
    interested[process] = true;
    turn = process;
    while(turn == process && interested[other]);
}

void leave_region(int process){
    interested[process] = false;
}

void sleep(int n){
    for(int i = 0; i < n; i++){
        printf("");
    }
}

void producer(){
char *str = "0";
char item = 'a';
    for(int i = 0; i < 26; ++i){
        enter_region(0);

        full++;
        sleep(10000000);
           str[0]=item;
        buffer[full - 1] = item++;

        printf(str);
        leave_region(0);
    }
    sleep(10000000);
}

void consumer(){
    char *str = "0";
    for(int i = 0; i < 26; ++i){

    enter_region(1);
        if(full >= 0){

            str[0] = buffer[--full];
            printf(str);
            printf("\n");
            count++;

        }
    leave_region(1);
    }
    tm.TerminateThread(tid2);
}

void producer2(){
for(int i = 0; i < 10; ++i){
        printf("A");
        sleep(100000000);
    }
}

void consumer2(){
    for(int i = 0; i < 10; ++i){
        printf("B");
        sleep(100000000);
    }

    tm.TerminateThread(tid2);
}

void testConditions(){
    tid1 = tm.CreateThread(producer);
    tid2 = tm.CreateThread(consumer);
    tm.JoinThreads(tid2);

    printf("\nPrinted alphabet is regular output. '0' is race condition.\n");
    tm.TerminateThread(test1);
}

void testYield(){
    tid1 = tm.CreateThread(producer2);
    tid2 = tm.CreateThread(consumer2);
    
    tm.YieldThreads(tid1);
    printf("\n.\n");
    printf("\nPrinted alphabet is regular output. '0' is race condition.\n");
    tm.TerminateThread(test2);
}
