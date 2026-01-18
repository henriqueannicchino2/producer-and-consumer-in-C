#include  <stdio.h>
#include <pthread.h>
#include <time.h>

int x=0, y=1, estado=0, p=0, limite=10, vet[10];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *produtor(void *data){
    srand(time(0));
    while (x <= y){
    /* ... */
        if(p==0)
            while(estado==0 && p<limite){vet[p]=rand()%100; printf("%d ",vet[p]); p++;}
        printf("\n");
        if(p==limite){
            p--;
            pthread_mutex_lock(&mutex); //Pedir LOCK
            printf("<(2) PRODUTOR cheio, ativar CONSUMIDOR.>\n");
            pthread_cond_signal(&cond); //WAKE UP consumidor

            printf("<(3) PRODUTOR, antes de ser bloqueado.>\n");
            /* Interrompe a thread até receber um 'sinal'! */
            pthread_cond_wait(&cond, &mutex); //SLEEP
            estado=0;
        }
        /* ... */
    }
    printf("<(7) PRODUTOR, fim;>\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *consumidor(void *data){
    pthread_mutex_lock(&mutex); //Importante bloquear a mudança do estado e dormir,
    // senão pode acontecer de mudar o estado e antes de dormir, a outra
    // thread tentar acordar essa, e quando essa estiver dormindo, não ser acordada.
    printf("<(1) Consumidor, sleep>\n");
    pthread_cond_wait(&cond, &mutex);
    estado=1;

    printf("<(4) Consumidor, wake>\n");
    //pthread_mutex_lock(&mutex);
    while(estado==1 && p>0){vet[p]=-1; printf("%d ",vet[p]); p--;}
    printf("\n");
    vet[p]=-1;
    /* ... */
    printf("<(5) CONSUMIDOR, lock.>\n");
    x = y + 1;
    pthread_cond_signal(&cond); //WAKE UP do produtor

    printf("<(6) CONSUMIDOR, libera PRODUTOR.>\n");
    /* ... */

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void delay(int number_of_seconds) {
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

int main(){
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    pthread_create(&tid2, NULL, consumidor, NULL);
    pthread_create(&tid1, NULL, produtor, NULL);

    //pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    x=0;
    pthread_create(&tid4, NULL, consumidor, NULL);
    pthread_create(&tid3, NULL, produtor, NULL);

    printf("{\n");
    delay(9);
    printf("}\n");

    return 0;
}
