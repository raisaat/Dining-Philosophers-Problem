# Dining-Philosophers-Problem

A program that allows 5 philosophers to think and eat. Each philosopher eats using 2 out of the 5 available forks. The program uses semaphores for mutual exclusion amongst the philosophers, so that no two philosophers are using the same fork at the same time, and displays which philosopher is eating and when. The program also avoids deadlock and starvation and terminates 5 minutes after the first philosopher starts thinking.

### Dining Philosophers Problem Description:

The Dining Philosopher’s Problem is described as follows: Five philosophers live in a house, where a round table is laid for them. On the table is set a large serving bowl of spaghetti, five plates, one for each philosopher, and five forks. A philosopher wishing to eat goes to his or her assigned place at the table and, using the two forks on either side of the plate, takes and eats some spaghetti.

### Program Description:

The program implements the pthread.h library to create a thread for each of the 5 philosophers. The semaphore.h library was used to create 5 semaphores for each of the 5 available forks to ensure mutual exclusion, i.e. no two philosophers are using the same fork at the same time.

A “think” function prints which philosopher is thinking. It uses a random number generator to generate a random number of microseconds for which the function sleeps. This is the thinking time of the philosopher. After the philosopher’s thinking time is over, i.e. after the function sleeps for a random number of microseconds, the function displays a message saying that the philosopher has finished thinking and exits.

An “eat” function prints which philosopher is eating and when. The function first gets the current local time using the function localtime() defined in the library time.h. It then prints which philosopher is eating and the current local time, which is the time at which the philosopher starts eating. After that, it uses a random number generator to generate a random number of microseconds for which the function sleeps. This is the eating time of the philosopher. After the philosopher’s eating time is over, i.e. after the function sleeps for a random number of microseconds, it uses the function localtime() again to get the current local time. It displays a message saying that the philosopher has finished eating and also displays the current local time, which is the time at which the philosopher stops eating.

A “philosopher” function is created for the threads, which allows a philosopher to think and eat. The function implements a continuous loop to allow the philosopher to think and eat continuously. In the loop, the function first calls the think() function to allow the philosopher to think. After the philosopher is done thinking, the function prepares for the philosopher to eat. The fork on the philosopher’s left side is picked up by locking the semaphore assigned to that fork and then the fork on the philosopher’s right side is picked up by locking the semaphore assigned to that fork. The semaphores are locked only if they are currently not locked, i.e. if their values are currently positive. To implement this, the sem_trywait() function defined in the library semaphore.h is used. This also ensures that deadlocks and starvation are avoided. Once the two forks are locked, the eat() function is called to allow the philosopher to eat. After the philosopher finishes eating, the right fork’s semaphore is unlocked and then the left fork’s semaphore is unlocked.

The program starts by making the main() function initialize the semaphores for the 5 forks and create a thread for each of the 5 philosophers. After the threads are spawned, main() goes to sleep for 5 minutes to allow the threads to run for 5 minutes. After 5 minutes, main() kills the threads. It then joins the threads and then destroys the semaphores before finally exiting.

### Building and running the program:

The program is built and run in a Linux machine. It is built using GCC.

1. In the working directory, where the program source code is saved, enter the following command to build the code:

        gcc -lpthread program3.c -o program3

    where program3.c is the program source code and program3 is the executable.

2. Enter the following command to run the program:
  
        ./program3  
