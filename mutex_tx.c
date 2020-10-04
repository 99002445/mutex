#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

pthread_t my_thread[5];
pthread_mutex_t lock;
int j = 1;

struct msg_buff { 
	long mesg_type; 
	char mesg_text[100]; 
} message; 

void* mutex_trans(void *arg)
{

	key_t key; 
	int msgid;
	
	// ftok to generate unique key 
	key = ftok("progfile", 65); 

	// msgget creates a message queue and returns identifier 
	msgid = msgget(key, 0666 | IPC_CREAT); 
	message.mesg_type = 1; 
	
	//Locking
	pthread_mutex_lock(&lock);
/***********************************************************************/

//	printf("Write Data : "); 
	printf("car entry %d: \t ",j); 
	++j;
	fgets(message.mesg_text,100,stdin); 

	// msgsnd to send message 
	msgsnd(msgid, &message, sizeof(message), 0); 

	// display the message 
	//printf("Data send is : %s \n", message.mesg_text); 
	
/***********************************************************************/

	//Unlocking
	pthread_mutex_unlock(&lock);    
	return NULL;
}

int main(void)
{
    int i;
    int err;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    while(i < 5)
    {
        err = pthread_create(&(my_thread[i]), NULL, &mutex_trans, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(my_thread[0], NULL);
    pthread_join(my_thread[1], NULL);
    pthread_join(my_thread[2], NULL);
    pthread_join(my_thread[3], NULL);
    pthread_join(my_thread[4], NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}
