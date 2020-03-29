#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "banker.h"
#include <stdbool.h>

/* thesee may be any values >= 0 */
#define NUMBEROFCUSTOMERS 5
#define NUMBEROFRESOURCES 3
/* the available amount of each resource */
int available[NUMBEROFRESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBEROFCUSTOMERS][NUMBEROFRESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBEROFCUSTOMERS][NUMBEROFRESOURCES];
/* the remaining need of each customer */
int need[NUMBEROFCUSTOMERS][NUMBEROFRESOURCES];

void* getResources(void *arg);
    pthread_t tid[NUMBEROFCUSTOMERS]; /* the thread identifiers */
int main(int argc, char *argv[])
{

    if (argc != NUMBEROFRESOURCES+1)
    {
        fprintf(stderr,"not enough resources\n");
        exit(1);
    }

    for(int i=1; i<argc; i++)
        if(atoi(argv[i]) < 0)
        {
            fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[i]));
            exit(1);
        }


    /*setting up the available vector */
    for(int i=0; i<NUMBEROFRESOURCES; i++)
    {
        available[i] = atoi(argv[i+1]);

        for(int j=0; j<NUMBEROFCUSTOMERS; j++)
        {
            maximum[j][i] = 10;    /* random initializations for maximum allowed requests*/
            need[j][i] = 10;  /*The need should equal the maximum*/
            allocation[j][i]=0;    /*By default the allocation matrix is zero*/
        }
    }

    /* create the thread */
    int process_num[] = {0,1,2,3,4};
    for(int k=0; k<NUMBER_OF_CUSTOMERS; k++)
        pthread_create(&(tid[k]),NULL,getResources,&process_num[k]);


    /* now wait for the thread to exit */
    for(int i=0; i<NUMBEROFCUSTOMERS; i++)
        pthread_join(tid[i],0);


    printf("Reached the end of the program!\n");
    pthread_mutex_destroy(&lock);
    return 0;

}

void* getResources(void *arg)
{

    bool released = false;
    int customer_num = *(int *)arg;  //typecasting

    int request_one[] = {6,7,8};     //prototype for a request
    int request_two[] = {2,1,1};     //prototype for a request
    int request_three[] = {2,2,1};     //prototype for a request

    while(request_resources_highlevel(request_one,customer_num)==false);
    while(request_resources_highlevel(request_two,customer_num)==false);
    while(request_resources_highlevel(request_three,customer_num)==false);

    release_resources_highlevel(customer_num);

    return 0;
}
