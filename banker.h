#ifndef BANKER_H // to prevent multiple includes in the main file
#define BANKER_H

#define NUMBEROFCUSTOMERS 5
#define NUMBEROFRESOURCES 3
#include <stdbool.h>


/* these may be any values >= 0 */
#define NUMBEROFCUSTOMERS 5
#define NUMBEROFRESOURCES 3
/* the available amount of each resource */
extern int available[NUMBEROFRESOURCES];
/*the maximum demand of each customer */
extern int maximum[NUMBEROFCUSTOMERS][NUMBEROFRESOURCES];
/* the amount currently allocated to each customer */
extern int allocation[NUMBEROFCUSTOMERS][NUMBEROFRESOURCES];
/* the remaining need of each customer */
extern int need[NUMBEROFCUSTOMERS][NUMBEROFRESOURCES];

/* mutex lock */
//pthread_mutex_t lock;
pthread_mutex_t lock;



bool isSafe()
{


    bool finish[NUMBEROFCUSTOMERS];
    int work[NUMBEROFRESOURCES];
    //int finish_seq[NUMBEROFCUSTOMERS]; 
    bool process_can = true;
    int count = 0;
    int customer_count = 1;
    bool flag = true;

    for(int i=0; i<NUMBEROFRESOURCES; i++)
        work[i] = available[i];

    for(int i=0; i<NUMBEROFCUSTOMERS; i++)
        finish[i] = false;

    for(int k=0; k<NUMBEROFRESOURCES; k++)
        work[k] += allocation[customer_count][k];

    while(count<NUMBEROFCUSTOMERS)
    {

        if(finish[customer_count] == false)
        {
            /* check if the process can be granted the resources */
            for(int j=0; j<NUMBEROFRESOURCES; j++)
                if(need[customer_count][j]>work[j])  
                    process_can = false;

            /* process can finish */
            if(process_can)
            {
                for(int k=0; k<NUMBEROFRESOURCES; k++)
                    work[k] += allocation[customer_count][k];
                finish[customer_count] = true;
                count++;

                customer_count = (customer_count + 1) % NUMBEROFCUSTOMERS; 
                break;
            }
printf("STUCK HERE\n");

        }

        /* for all false states -if they cannot run- then we are in an unsafe state.*/

        for(int i=0; i<NUMBEROFCUSTOMERS; i++)
            if(finish[i]==false)
            {
                for(int j=0; j<NUMBEROFRESOURCES; j++)
                    if(work[j]>need[customer_count][j])
                        flag = false;
                break;
            }

        if(flag == true)
            return false;

    }

    return true;
}


bool release_resources(int release[],int customer_num)
{
    /*add resouces*/
    for(int i=0; i<  NUMBEROFRESOURCES; i++)
    {
        available[i] += release[i];
    }
    return true;
}

bool request_resources(int request[], int customer_num)
{

//    sleep(1);
    printf("\nCustomer %d is Requesting Resources:\n",customer_num);
    for(int i=0; i<NUMBEROFRESOURCES; i++)
    {
        printf("%d ", request[i]);
    }

    printf("\nAvailable Resources : \n");

    for(int i = 0; i < NUMBEROFRESOURCES; i++)
    {
        printf("%d ",available[i]);
    }

    printf("\nThe need : \n");
    for(int i = 0; i < NUMBEROFRESOURCES; i++)
    {
        printf("%d ",need[customer_num][i]);
    }

    printf("\n");
    for(int i=0; i<NUMBEROFRESOURCES; i++)
    {
        if(request[i] <= need[customer_num][i]) 
        {
            if(request[i]>available[i])
            {
                printf("NOT SAFE with this request (request > available) \n");
                return false;
            }
            else  //taking resourcees//
            {
 
                if(isSafe())
                {
                    printf("The System is Sfe \nResources Graned \n");

		       for(int i=0; i<NUMBEROFRESOURCES; i++)
		        {
		            available[i] -= request[i];
		            allocation[customer_num][i] += request[i];
		            need[customer_num][i] -= request[i];
		        }	
		            return true;
		}
                else
                {
                    printf("NOT SAFE ! CAN'T GRANT RESOURCES \n");
                    return false;
                }
            }
        }

        else if(request[i] > need[customer_num][i])
        {
            printf("The request is more than the need. Abort! \n");
            return false;
        }
    }

}



void release_resources_highlevel(int customer_num){
        pthread_mutex_lock(&lock);


        release_resources(maximum[customer_num],customer_num);
        pthread_mutex_unlock(&lock);
	printf( "Thread %d finished execution \n" , customer_num);
}

bool request_resources_highlevel(int request[],int customer_num){
    //CRITICAL SECTION //
    bool released = false;
    pthread_mutex_lock(&lock);
    released=request_resources(request,customer_num);
    pthread_mutex_unlock(&lock);

    return released;
}

#endif 
