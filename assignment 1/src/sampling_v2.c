#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define COMPLETED 1
#define NOT_COMPLETED 0

unsigned long number_of_samples;
short int done = NOT_COMPLETED;
struct timeval startwtime, endwtime;
double *timestamp;

/*
	writeLogFile() writes timestamps to a file
*/
void writeLogFile()
{
	FILE *fp;
	fp = fopen("../logs/timestamps_v2.txt", "w");

	if (!fp)
	{
		perror("Could not write to file");
		exit(EXIT_FAILURE);
	}

	for (unsigned long i = 0; i < number_of_samples; i++) {
		fprintf(fp, "%ld, %.8lf\n", i+1, timestamp[i]);
	}

	fclose(fp);
}

/*
	intHandler() handles the signal SIGINT
*/
void intHandler(int sig)
{
	printf("\n[*] Quitting sampling ...\n");

	// write timestamps collected so far to a file
	printf("Creating log file ...\n");
	writeLogFile();
	printf("[*] DONE\n");

	exit(EXIT_SUCCESS);
}

/*
	timerHandler() handles the signal SIGALRM
*/
void timerHandler(int sig)
{
	// sample_num contains the current sample number
	static unsigned long sample_num = 1;

	// get timestamp and save it to the array timestamp
	gettimeofday(&endwtime, NULL);
	timestamp[sample_num - 1] = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
				      + endwtime.tv_sec - startwtime.tv_sec);

	// if the required number of samples has been collected
	// then stop sampling
	if (sample_num < number_of_samples) sample_num++;
	else done = COMPLETED;
}


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s t dt\n", argv[0]);
		printf("\t- t: total sampling time (seconds)\n");
		printf("\t- dt: time interval between samples (seconds)\n");
		exit(EXIT_FAILURE);
	}

  float t = atof(argv[1]);
  float dt = atof(argv[2]);

	if (t <= 0 || dt <= 0)
	{
		printf("Error: t, dt arguments must be positive\n");
		exit(EXIT_FAILURE);
	}

	// calculate the number of samples required given t and dt
  number_of_samples = t / dt;

	// allocate memory for timestamps
	timestamp = (double *)malloc(number_of_samples * sizeof(double));

	// initialize timestamps array
	memset(timestamp, 0, sizeof(timestamp));

	// initialize timer parameters
	struct itimerval timer;

	unsigned long dt_to_usec = (unsigned long) (dt * 1000000);
	timer.it_value.tv_sec = dt_to_usec / 1000000;
	timer.it_value.tv_usec = dt_to_usec % 1000000;

	memcpy(&(timer.it_interval), &(timer.it_value), sizeof(timer.it_value));

	printf("\n===== PERIODIC SAMPLING =====\n");
	printf("Parameters:\n");
	printf("- Total sampling time: %f sec\n", t);
	printf("- Sampling interval: %f sec\n", dt);
	printf("- Total number of samples: %ld\n", number_of_samples);
	printf("- Process id: %d\n\n", getpid());

  // Set handler for SIGINT signal in case that the process is interrupted
	if (signal(SIGINT, intHandler) == SIG_ERR) {
    perror("Unable to catch SIGINT");
    exit(EXIT_FAILURE);
  }

	// Set handler for SIGALRM signal (required for the timer operation)
	if (signal(SIGALRM, timerHandler) == SIG_ERR) {
    perror("Unable to catch SIGALRM");
    exit(EXIT_FAILURE);
  }

	// get the date and time that the sampling started
	time_t rawtime;
	time(&rawtime);
	struct tm *time_info = localtime(&rawtime);

	printf("[*] Sampling started: %s\n", asctime(time_info));

	// start timer
	if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
    perror("error calling setitimer()");
    exit(EXIT_FAILURE);
  }
	gettimeofday(&startwtime, NULL);

	/*
		By using pause() the process sleeps between successive
		SIGALRM signals for low power consumption. This loop will
		be executed until the required number of samples has been
		collected and the sampling has completed.
	*/
  while (!done) pause();

	printf("[*] Sampling completed\n");

	// write the collected timestamps to a file
	printf("Creating log file ...\n");
	writeLogFile();
	printf("[*] DONE\n");

	// free the memory used for timestamps
	free(timestamp);

  return 0;
}
