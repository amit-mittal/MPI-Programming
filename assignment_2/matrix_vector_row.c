#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Taking input the weight matrix
void take_input(int **mat, int *b, int n)
{
	int i, j;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			int t = scanf("%d", &mat[i][j]);
		}
	}

	for (i = 0; i < n; ++i)
	{
		int t = scanf("%d", &b[i]);
	}
}

void generate_input(int **mat, int *b, int n)
{
	int i, j;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			int temp = rand()%10;
			mat[i][j] = temp;
		}
	}

	for (j = 0; j < n; ++j)
	{
		int temp = rand()%10;
		b[j] = temp;
	}
}

// printing matrix
void print_matrix(int **mat, int n)
{
	int i, j;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			printf("%d\t", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// printing vector
void print_vector(int *b, int n)
{
	int i, j;

	for (j = 0; j < n; ++j)
	{
		printf("%d\n", b[j]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	int **mat;
	int *buffer, *b, *c, *myray, *myc;
	int n, i, j, k;
	int source, each_row;
	int numprocs, myid;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	n = 1000;
	source = 0;

	// TODO instead use scatterv and gatherv
	// allocating memory to matrix
	buffer = (int *)malloc(2*n*n*sizeof(int));// allocating extra memory
	b = (int *)malloc(n*sizeof(int));
	c = (int *)malloc(2*n*sizeof(int));// allocating extra memory
	mat = (int **)malloc(n*sizeof(int *));
	for (i = 0; i < n; ++i)
		mat[i] = buffer + (n*i);

	if(myid == source)
	{
		printf("INPUT\n");
		generate_input(mat, b, n);
		//print_matrix(mat, n);
		//print_vector(b, n);	
	}

	// broadcasting the vector
	MPI_Bcast(b, n, MPI_INT, source, MPI_COMM_WORLD);

	each_row = (int)ceil((double)n/numprocs);
	
	// scattering the matrix rows
	myray = (int *)malloc(n*each_row*sizeof(int));
	myc = (int *)malloc(each_row*sizeof(int));
	MPI_Scatter(buffer, n*each_row, MPI_INT, myray, n*each_row, MPI_INT, source, MPI_COMM_WORLD);
	
	// Multiplying vector and respective matrix rows
	for (i = 0; i < each_row; ++i)
	{
		int val = 0;
		for (j = 0; j < n; ++j)
			val+=(myray[(i*n)+j]*b[j]);
		myc[i] = val;
	}

	// Gathering the final vector rows
	MPI_Gather(myc, each_row, MPI_INT, c, each_row, MPI_INT, source, MPI_COMM_WORLD);

	if(myid == source)
	{
		printf("OUTPUT\n");
		//print_vector(c, n);
	}

	MPI_Finalize();

	return 0;
}