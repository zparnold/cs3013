#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "restroom.h"

restroom_data_t rr; // the restroom

/* struct to pass to new thread */
typedef struct {
	int thread_number;
	gender thread_gender;
} thread_info_t;

/* for individual threads to base their randomization */
double arrival_time; //average arrival time
double stay_time; //average stay time
double loop_count; //loop count

pthread_mutex_t print_mutex; //keep threads from printing at the same time

///////////////// Functions /////////////////

 /* thread function declaration */
 void *Individual( void *info );

/* function to get a random number based on the 
 * mean provided by the user. 
 */
 double get_distributed_rand ( double mean )
 {
 	double a = drand48(); //generate random numbers
 	double b = drand48();

 	/* box-muller distribution */
 	double z = sqrt( -2 * log(a) ) * cos( 2 * M_PI * b);

 	double result = (( mean / 2 ) * z) + mean;

 	//check bounds
 	if (result < 1) {
 		result = 1;
 	} else if (result > 100) {
 		result = 99;
 	}

 	return result;
 }
 
 int main(int argc, char const *argv[])
 {
 	Initialize( &rr ); //initialize the restroom
 	srand48((long)time(NULL)); //randomize more

 	pthread_mutex_init ( &print_mutex, NULL );

 	int i; //for loops

 	if ( argc == 5 )
 	{
 		/* get arguments */
 		int n_users = strtol( argv[1], (char **) NULL, 10);
 		arrival_time = strtof( argv[2], (char **) NULL);
 		stay_time = strtof( argv[3], (char **) NULL);
 		loop_count = strtof( argv[4], (char **) NULL);
		

 		if (arrival_time > 100 || stay_time > 100 || loop_count > 100 || n_users > 300) {
 			printf("Arguments are too large. Quitting\n");
 			exit(1);
 		} else if (arrival_time <= 0 || stay_time <= 0 || loop_count <= 0 || n_users <= 0) {
 			printf("Arguments must be greater than zero. Quitting\n");
 			exit(1);
 		}

 		/* threads */
 		pthread_t thread[n_users];
 		
 		printf("Simulation Starting...\n");
 		printf("Number of users: %d\n", n_users );

 		
 		for (i = 0; i < n_users; i++)
 		{
 			thread_info_t *info = malloc( sizeof( thread_info_t * )); 
			info->thread_number = i; //thread number of the thread

			double b_or_g = drand48();
 			if ( b_or_g < .5 ) { // girl or boy?
 				info->thread_gender = male;
 			} else {
 				info->thread_gender = female;
 			}

 			pthread_create(&thread[i], NULL/*&thread_attr*/, Individual, (void *) info);
 		}

 		for (i = 0; i < n_users; ++i) {
    		pthread_join(thread[i], NULL);
  		}
 	} else {
 		printf("Error: please use the correct number of arguments\n");
 		printf("\tnUsers meanArrival meanStay meanLoopCount\n");
 	}

 	Finalize( &rr );
 	return 0;
 }

/* function for individual threads */
 void *Individual( void *info ) {
	long min_queue_time = 999999999; //minimum time spent in queue
	long avg_queue_time = 0; //average time spent in queue
	long max_queue_time = 0; //maximum time spent in queue
	long tot_queue_time = 0; //total time spent in queue
	long tot_waits = 0; //total number of times waited in queue

	struct timeval before; //to keep track of time elapsed
	struct timeval after;

	thread_info_t * usr = (thread_info_t *) info;

	/* generate a random loop count based on average */
	int rand_loop_count = (int) floor( get_distributed_rand ( loop_count ) );
	// printf("Inside thread %d: Looping %d times\n", user->thread_number, rand_loop_count);

	int i; //for loop

 	for (i = 0; i < rand_loop_count; i++)
 	{
 		int rand_arrival_time = (int) floor(get_distributed_rand( arrival_time ));
 		int rand_stay_time = (int) floor(get_distributed_rand( stay_time ));

 		/* wait for a random length of time
 		   based on arrival_time */
 		
 		usleep( rand_arrival_time * MICRO_SEC );

 		gettimeofday(&before, NULL); //get time before
 		int waited = Enter( &rr, usr->thread_gender );
 		gettimeofday(&after, NULL); //get time after

 		/* wait for a random length of time
 		   based on stay time */
 		usleep( rand_stay_time * MICRO_SEC );

 		Leave( &rr );

 		//do some time calculations
 		long elapsed = get_elapsed_ms(&before, &after);

 		if (waited) {
	 		if (elapsed > max_queue_time) {
	 			max_queue_time = elapsed;
	 		} 

	 		if (elapsed < min_queue_time) {
	 			min_queue_time = elapsed;
	 		}

	 		tot_waits++;
	 		tot_queue_time += elapsed;
	 	}
 	}

 	// get average from the total / the loop count
 	if (tot_waits) {
		avg_queue_time = tot_queue_time / tot_waits;
	}

	//change value for printing
	if (min_queue_time == 999999999) {
		min_queue_time = 0;
	}

 	/* print final information regarding thread */

 	pthread_mutex_lock ( &print_mutex ); //lock so only this thread prints

 	printf("\nThread %d has Finalizeed.\n", usr->thread_number);
 	
 	if (usr->thread_gender == 0) {
 		printf("Gender: Male\n");
 	} else {
 		printf("Gender: Female\n");
 	}

 	printf("# Loops: %d\n", rand_loop_count);
 	printf("Waited in queue: %ld times\n", tot_waits);
 	printf("Total queue time: %ld ms\n", tot_queue_time);
 	printf("Min queue time: %ld ms\n", min_queue_time);
 	printf("Max queue time: %ld ms\n", max_queue_time);
 	printf("Avg queue time: %ld ms\n", avg_queue_time);

 	pthread_mutex_unlock( &print_mutex ); //unlock!

 	pthread_exit(NULL);
 }
 
 