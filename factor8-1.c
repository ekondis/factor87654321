#include <stdio.h>
#include <sys/time.h>
#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_max_threads(void){ return 1; }
#endif

struct timeval time_check(void){
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

double time_done(struct timeval t){
	struct timeval t2;
	double elapsedTime;
	gettimeofday(&t2, NULL);

	elapsedTime = (t2.tv_sec - t.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (t2.tv_usec - t.tv_usec) / 1000.0;   // us to ms
	return elapsedTime;
}

int main(void){
	int i, j;
	struct timeval t1;
	double elapsedTime;

	printf("Searching for two factors in range [1, 15000] whose multiple is 87654321\n");

	t1 = time_check();

	#pragma omp parallel for private(j) schedule(dynamic,500)
	for(i=1; i<=15000; i++)
		for(j=i+1; j<=15000; j++)
			if( i*j==87654321 )
				printf("Found! %d %d\n", i, j);

	elapsedTime = time_done(t1);

	printf("kernel execution time = %.2f msecs (%d threads)\n", elapsedTime, omp_get_max_threads());

	return 0;
}
