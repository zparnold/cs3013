#ifndef RESTROOM_H
#define RESTROOM_H

#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

//vacant constant (so it's easier later) 
#define VACANT -1

/* time constant 10ms -> 10000 microseconds */
#define MICRO_SEC 10000

/* gender of thread */
typedef enum { male, female } gender;

/* data structure for a restroom */
typedef struct {
	pthread_mutex_t mutex; //define a mutex to control sync
	pthread_cond_t cond;

	long uses; //total number of uses of the restroom
	int queue_uses; // number of uses of the queue

	int current_occupants; // # of occupants
	int wait_queue; // users in queue
	
	int tot_queue; // total number of users that waited in the queue
	int tot_occupied; // total number of users that occupied the restroom

	/* time info */
	struct timeval start_time;
	struct timeval Finalize_time;
	struct timeval unoccupied_start;
	struct timeval unoccupied_Finalize;

	long time_unoccupied; //amount of time the restroom was empty
	long time_occupied; //amount of time that the restroom was occupied
	long time_running; //amount of time the restroom was running

	/* vars for average */
	int avg_queue_length; //average queue length
	int avg_occupancy; //average number of people in the restroom

	/* variables that the restroom uses to manage access */
	int restroom_state; // restroom state
} restroom_data_t;

/* Enter the restroom */
int Enter ( restroom_data_t *data, gender g );

/* Leave the restroom */
void Leave ( restroom_data_t *data );

/* initialize restroom */
void Initialize ( restroom_data_t *data );

// wrap-up restroom 
void Finalize ( restroom_data_t *data );

// time between two time intervals in (ms)
long get_elapsed_ms(struct timeval *before, struct timeval *after);

#endif
