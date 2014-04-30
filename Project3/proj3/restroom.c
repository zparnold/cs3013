#include <stdio.h>
#include "restroom.h"

static void change_state ( restroom_data_t *data, int state ) {
	data->restroom_state = state;
}

static void increment_occupants ( restroom_data_t *data ) {
	data->uses++;
	data->current_occupants++;

	data->tot_occupied += data->current_occupants;
}

static void decrement_occupants ( restroom_data_t *data ) {
 	data->current_occupants--;
 }

int Enter ( restroom_data_t *data, gender g ) {
	/* test mutex for lock */
	int waited = 0; //did the thread wait in the queue?

	pthread_mutex_lock ( &(data->mutex) );

	if (data->restroom_state == VACANT)
	{	/* restroom is unoccupied*/

		/* get time of day to flag the end of unoccupied time */
		gettimeofday( &(data->unoccupied_Finalize), NULL );
		/* add to the total unoccupied time */
		data->time_unoccupied += get_elapsed_ms( &(data->unoccupied_start), &(data->unoccupied_Finalize));

		change_state( data, g );
		increment_occupants( data );
		
	} else if (data->restroom_state == g) {
		/* restroom is already occupied with this gender */

		/* gender diagnostic */
		// printf("Gender %d joined gender %d in the restroom.\n", g, data->restroom_state);
	
		increment_occupants( data ); // increment # of uses
	} else {
		/* restroom is occupied by opposite gender */
		data->wait_queue++; //increment wait queue;

		data->tot_queue += data->wait_queue; //add current queue to total
		data->queue_uses++; //number of uses of the queue has increased

		waited++;
		
		pthread_cond_wait(&(data->cond), &(data->mutex)); //wait for condition

		change_state( data, g ); //let opposite gender Enter
		increment_occupants( data );
	}

	/*unlock!*/
	pthread_mutex_unlock ( &(data->mutex) );
	return waited;
}

void Leave ( restroom_data_t *data ) {
	/* test mutex for lock */
	pthread_mutex_lock ( &(data->mutex) );

	/* remove restroom user */
	decrement_occupants( data );

	if (data->current_occupants <= 0) {
		/* no more occupants in restroom! */

		change_state( data, VACANT );

		/* start up unoccupied time again */
		gettimeofday( &(data->unoccupied_start), NULL);

		pthread_cond_broadcast( &(data->cond) ); //start up waiting threads

		data->wait_queue = 0; //reset wait queue
	}

	/* unlock! */
	pthread_mutex_unlock ( &(data->mutex) );
}

void Initialize ( restroom_data_t *data ) {
	/* initialize all variables, the mutex, and the semaphore */
	
	pthread_mutex_init( &(data->mutex), NULL );
	pthread_cond_init( &(data->cond), NULL );

	data->uses = 0; //total number of uses of the restroom
	data->current_occupants = 0; // # of occupants

	data->time_unoccupied = 0; //amount of time the restroom was empty
	data->time_occupied = 0; //amount of time the restroom was occupied 
	data->avg_queue_length = 0; //average queue length
	data->avg_occupancy = 0; //average number of people in the restroom

	data->restroom_state = VACANT; //restroom state

	/* get time for the start time and the unoccupied time */
	gettimeofday( &(data->start_time), NULL );

	gettimeofday( &(data->unoccupied_start), NULL );
}

void Finalize ( restroom_data_t *data ) {
	gettimeofday( &(data->Finalize_time), NULL ); // get the Finalize time
	
	/* manipulate time to get some final */
	data->time_running = get_elapsed_ms( &(data->start_time), &(data->Finalize_time) );

	data->time_occupied = data->time_running - data->time_unoccupied;

	/* manipulate collected data to get averages */
	//data->avg_queue_length = data->tot_queue / data->queue_uses;
	data->avg_queue_length = data->tot_queue / data->queue_uses;
	data->avg_occupancy = data->tot_occupied / data->uses;

	/* clean up */
	pthread_mutex_destroy ( &(data->mutex) );
	pthread_cond_destroy ( &(data->cond) );

	/* print all data relevant to restroom */
	printf("\nThe restroom simulation has completed!\n\n");
	printf("Stats:\n");
	printf("\tNum uses: %ld\n", data->uses);
	printf("\tTotal time vacant: %ld ms\n", data->time_unoccupied);
	printf("\tTotal time occupied: %ld ms\n", data->time_occupied);
	printf("\tAvg queue length: %d users\n", data->avg_queue_length);
	printf("\tAvg occupancy: %d users\n\n", data->avg_occupancy);
}

long get_elapsed_ms(struct timeval *before, struct timeval *after) {
	unsigned long long elapsed_ms = 0;
	unsigned long long seconds = 0;
	unsigned long long ms = 0;

	seconds = after->tv_sec - before->tv_sec; //get number of seconds
	elapsed_ms += seconds * 1000; //convert to milliseconds 

	if (before->tv_usec > after->tv_usec) { //if before ms > after ms
		//one or more seconds has elapsed, so

		ms = before->tv_usec - after->tv_usec; //microseconds
		elapsed_ms += ms / 1000; //convert to milliseconds
	} else {
		ms = after->tv_usec - before->tv_usec;
		elapsed_ms += ms / 1000; //conver to milliseconds 
	}

	return elapsed_ms;
}
