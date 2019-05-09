#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define COMPLETED 1
#define NOT_COMPLETED 0

unsigned long number_of_samples;
short int done = NOT_COMPLETED;
struct timeval startwtime, endwtime;
double *timestamp;
struct timespec timer;
double dt;

/*
	writeLogFile() writes timestamps to a file
*/
void writeLogFile()
{
	FILE *fp;
	fp = fopen("../logs/timestamps_v1.txt", "w");

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
void timerHandler()
{
	// sample_num contains the current sample number
	static unsigned long sample_num = 1;

	// get timestamp and save it to the array timestamp
	gettimeofday(&endwtime, NULL);
	timestamp[sample_num - 1] = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
				      + endwtime.tv_sec - startwtime.tv_sec);

	// set next timer parameters
	float dif = fmodf(timestamp[sample_num - 1] - sample_num * dt, dt);
	float next_dt = dt - dif;

	unsigned long dt_to_nsec = (unsigned long) (next_dt * 1000000000);
	timer.tv_sec = dt_to_nsec / 1000000000;
	timer.tv_nsec = dt_to_nsec % 1000000000;

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
  dt = atof(argv[2]);

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
	unsigned long dt_to_nsec = (unsigned long) (dt * 1000000000);
	timer.tv_sec = dt_to_nsec / 1000000000;
	timer.tv_nsec = dt_to_nsec % 1000000000;


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

	// get the date and time that the sampling started
	time_t rawtime;
	time(&rawtime);
	struct tm *time_info = localtime(&rawtime);

	printf("[*] Sampling started: %s\n", asctime(time_info));

	// start sampling
	gettimeofday(&startwtime, NULL);

	/*
		By using nanosleep() the process sleeps between successive
		timerHandler() calls for low power consumption. This loop will
		be executed until the required number of samples has been
		collected and the sampling has completed.
	*/
  while (!done)
  {
    nanosleep(&timer, NULL);

    timerHandler();
  }

	printf("[*] Sampling completed\n");

	// write the collected timestamps to a file
	printf("Creating log file ...\n");
	writeLogFile();
	printf("[*] DONE\n");

	// free the memory used for timestamps
	free(timestamp);

  return 0;
}
