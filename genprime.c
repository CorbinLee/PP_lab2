#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char* argv[]) 
{
	if (argc < 3)
		exit(1);

	double tstart = 0.0, ttaken;

	printf("0\n");

	/* Parse arguments */
	int N = strtol(argv[1], NULL, 10);
	int t = strtol(argv[2], NULL, 10);

	/* Check that 2 < N <= 1,000,000 and 0 < t <= 100 */
	if (!(N > 2 && N <= 1000000) || !(t > 0 && t <= 100))
		exit(1);

	printf("1\n");

	int stop = (N+1) / 2;
	int* primes = (int *)calloc(N, sizeof(int));

	tstart = omp_get_wtime();

	/* Generate prime numbers */
	int i, j;

	printf("2\n");
	printf("N: %d, t: %d\n", N, t);

	#pragma omp parallel for num_threads(t) default(none) shared(stop, N, primes) private(i, j) schedule(guided, 3)
	for (i = 2; i <= stop; i++) {
		j = 2;
		while (primes[i-1] == 0 && j <= N/i) {
			primes[(i * j) - 1] = 1;
			j++;
		}
		// if (primes[i-1] == 0) {
		// 	for (j = 2; j <= N / i; j++) {
		// 		primes[(i * j) - 1] = 1;
		// 		if (primes[i-1] != 0)
		// 			break;
		// 	}
		// }
	}

	ttaken = omp_get_wtime() - tstart;
	printf("Time take for the main part: %f\n", ttaken);

	/* Write output file and exit */
	char filename[12];
	sprintf(filename, "%d.txt", N);
	
	FILE *output = fopen(filename, "w");
	if (output == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	j = 1;
	int prev = 2;
	for (i = 2; i <= N; i++) {
		if (primes[i-1] == 0) {
			fprintf(output, "%d, %d, %d\n", j, i, i-prev);
			j++;
			prev = i;
		}
	}

	fclose(output);

	free(primes);

	exit(0);
}
