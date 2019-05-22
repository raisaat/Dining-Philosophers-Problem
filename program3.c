/////////////////////////////////////////////////////////////////////////////////////////
// program3.c
// Author: Raisaat Atifa Rashid
//
// Procedures:
//      main				Creates 5 threads for 5 philosophers and initializes
//							5 semaphores for 5 forks
//		think				Lets a philosopher think
//		eat					Lets a philosopher eat 
//		philosopher			Allows each philosopher to think and eat. Allows
//							philosophers to eat only when the forks on both the 
//							philosopher's left and right are available. While the 
//							philosopher is eating, those two forks are locked.
/////////////////////////////////////////////////////////////////////////////////////////

/* Required libraries */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

/* Preprocessor macro to handle an error that has an error number */
#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0) // store the error number, print the error message to stderr and terminate the program with the value of EXIT_FAILURE

/* Preprocessor macro to handle an error that does not have an error number */
#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0) // print the error message to stderr and terminate the program with the value of EXIT_FAILURE

#define NUMB_THREADS 5 // Number of philosophers and hence, the number of threads

int philosopher_num[NUMB_THREADS] = {0, 1, 2, 3, 4}; // Array of philosophers' IDs
sem_t forks[NUMB_THREADS]; // Array of 5 semaphores for the 5 forks on the table

////////////////////////////////////////////////////////////////////////////////////////
// void think(int n)
// Author: Raisaat Atifa Rashid
// Date: 10 March 2019
// Description: Function that lets a philosopher think. Displays a message
//				saying which philosopher is thinking. Sleeps for a random number
//				of microseconds and after that, displays a message saying that
//				the philosopher has finished thinking.
//
// Parameters:
//     n		I/P		int		The philosopher number
////////////////////////////////////////////////////////////////////////////////////////
void think(int n)
{
	struct timeval time; // Structure to hold the time of the day
	gettimeofday(&time, NULL); // Get the time of the day

	printf("Philosopher %d is thinking.\n", n + 1); // Display which philosopher is thinking
	
	srand(time.tv_sec * time.tv_usec); // Seed the random number generator
	usleep(rand() % 10000000);	// Let the philosopher think for a random number of microseconds between 0 and 10000000 
								// microseconds by letting the function sleep for that number of seconds 
	
	printf("Philosopher %d has finished thinking.\n", n + 1); // Display a message saying that the philosopher has finished thinking

} // end of void think(int n)

/////////////////////////////////////////////////////////////////////////////////////////////
// void eat (int n)
// Author: Raisaat Atifa Rashid
// Date: 10 March 2019
// Description: Function that lets a philosopher eat. Displays a message
//				saying which philosopher is eating and the time at which the 
//				philosopher starts eating. Sleeps for a random number of microseconds
//				and after that, displays a message saying that the philosopher has 
//				finished eating and the time at which the philosopher finishes eating.
//
// Parameters:
//     n		I/P		int		The philosopher number
/////////////////////////////////////////////////////////////////////////////////////////////
void eat (int n)
{
	time_t rawTime; // Variable to hold the raw current time
	struct tm * timeInfo; // Structure to hold the current local time
	
	struct timeval time_1; // Structure to hold the time of the day
	gettimeofday(&time_1, NULL); // Get the time of the day

	time(&rawTime); // Get the raw current time
	timeInfo = localtime(&rawTime); // Get the current local time
	
	// Display which philosopher is eating and the time at which the philosopher starts eating
	printf("Philosopher %d is eating. The time now is %d:%d:%d.\n", n + 1, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
	
	srand(time_1.tv_sec * time_1.tv_usec); // Seed the random number generator
	usleep(rand() % 10000000);	// Let the philosopher eat for a random number of microseconds between 0 and 10000000
								// microseconds by letting the function sleep for that number of seconds

	time(&rawTime); // Get the raw current time
	timeInfo = localtime(&rawTime); // Get the current local time

	// Display a message saying that the philosopher has finished eating and also the time at which the philosopher finishes eating
	printf("Philosopher %d has finished eating. The time now is %d:%d:%d.\n", n + 1, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

} // end of void eat (int n)

///////////////////////////////////////////////////////////////////////////////////////////////////
// void * philosopher(void * n)
// Author: Raisaat Atifa Rashid
// Date: 10 March 2019
// Description: Function that allows a philosopher to think and then eat. The function first
//				allows the philosopher to think by calling function think(). It then
//				uses a semaphore to lock the fork on the philosopher's left and another
//				semaphore to lock the fork on the philosopher's right, and then allows the 
//				philosopher to eat by calling function eat(). After the philosopher
//				finishes eating, it unlocks the right fork's semaphore followed by the
//				left fork's semaphore.
//
// Parameters:
//		n				I/P		int			The philosopher number
//		philosopher		O/P		void *		NULL pointer
///////////////////////////////////////////////////////////////////////////////////////////////////
void * philosopher(void * n)
{
	int num = *(int *)n; // Get the philosopher number
	
	// Continuous loop in which the philosopher thinks and eats
	while (1)
	{
		think(num); // call function think() to let the philosopher think
		sem_trywait(&forks[num]); // Pick up the left fork (lock the left fork)
		sem_trywait(&forks[(num + 1) % 5]); // Pick up the right fork (lock the right fork)
		eat(num); // Call function eat() to let the philosopher eat
		sem_post(&forks[(num + 1) % 5]); // Put down the right fork (unlock the right fork)
		sem_post(&forks[num]); // Put down the left fork (unlock the left fork)
	}
	
	return (NULL); // Exit from function

} // end of void * philosopher(void * n)

////////////////////////////////////////////////////////////////////////////////////
// int main()
// Author: Raisaat Atifa Rashid
// Date: 10 March 2019
// Description: Initializes the semaphores for the 5 forks. Spawns 5 threads, 
//				one for each of the 5 philosophers. It then sleeps for 300 
//				seconds (5 minutes) to allow the threads to run for 300 seconds
//				and then kills the threads after 300 seconds. After that it 
//				joins the threads and finally, destroys the semaphores.
//
// Parameters:
//      main    O/P     int         status code
////////////////////////////////////////////////////////////////////////////////////
int main()
{
	pthread_t thread_id[NUMB_THREADS]; // Array to hold the thread IDs of the philosophers' threads
	int i, // Counter for for-loops
		s; // Variable to hold the value returned by function calls from the pthread library

	/* Loop NUMB_THREADS times to initialize the 5 semaphores used for the 5 forks*/
	for (i = 0; i < NUMB_THREADS; i++)
		sem_init(&forks[i], 0, 1); // Initialize each semaphore
	
	/* Loop NUMB_THREADS times to create 5 threads representing the 5 dining philosopher */
	for (i = 0; i < NUMB_THREADS; i++)
	{
		s = pthread_create(&thread_id[i], NULL, philosopher, &philosopher_num[i]);	// Create a thread for each of the 5 philosophers by calling 
																					// pthread_create(), and store the function's return value in s
		if (s != 0) // If s is not equal to 0, i.e. if the pthread_create function failed to execute
			handle_error_en(s, "pthread_create");	// Call error-handling macro with the error value in s 
													// and an error message as arguments to terminate the program
	}

	sleep(300); // Sleep for 300 seconds (5 minutes) to allow the threads to run for 300 seconds

	/* Loop NUMB_THREADS times to cancel (and hence, kill) the 5 threads */
	for (i = 0; i < NUMB_THREADS; i++)
	{
		s = pthread_cancel(thread_id[i]);	// Cancel (and hence, kill) each thread by calling pthread_cancel(), and store the function's return value in s
		if (s != 0) // If s is not equal to 0, i.e. if the pthread_cancel function failed to execute
			handle_error_en(s, "pthread_cancel");	// Call error-handling macro with the error value in s 
													// and an error message as arguments to terminate the program
	}

	/* Loop NUMB_THREADS times to wait for the 5 threads to join */
	for (i = 0; i < NUMB_THREADS; i++)
	{
		s = pthread_join(thread_id[i], NULL); // Join the thread by calling pthread_join(), and store the function's return value in s
		if (s != 0) // If s is not equal to 0, i.e. if the pthread_join function failed to execute
			handle_error_en(s, "pthread_join"); // Call error-handling macro with the error value in s and an error message as arguments in order to terminate the program
	}

	/* Loop NUMB_THREADS times to destroy the 5 semaphores used for the 5 forks*/
	for (i = 0; i < NUMB_THREADS; i++)
		sem_destroy(&forks[i]); // Destroy each semaphore

	printf("\nThe program will now exit.\n"); // Display a message before exiting the program

	return (0); // Exit the program

} // end of int main()