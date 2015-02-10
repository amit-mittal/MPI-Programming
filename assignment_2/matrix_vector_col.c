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

// take transpose of the matrix
void transpose(int **mat, int n)
{
	int i, j;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < i; ++j)
		{
			int t = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = t;	
		}
	}
}

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	int **mat;
	int *buffer, *b, *c, *mybuffer, *myc, *myb;
	int n, i, j, k;
	int source, each_row;
	int numprocs, myid;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	n = 4;
	source = 0;

	// allocating memory to matrix
	buffer = (int *)malloc(2*n*n*sizeof(int));// allocating extra memory
	b = (int *)malloc(2*n*sizeof(int));// allocating extra memory
	c = (int *)malloc(2*n*sizeof(int));// allocating extra memory
	mat = (int **)malloc(n*sizeof(int *));
	for (i = 0; i < n; ++i)
		mat[i] = buffer + (n*i);

	if(myid == source)
	{
		printf("INPUT\n");
		generate_input(mat, b, n);
		print_matrix(mat, n);
		print_vector(b, n);
		// taking transpose so that columns are contiguous
		transpose(mat, n);
	}

	each_row = (int)ceil((double)n/numprocs);

	// scattering the vector
	myb = (int *)malloc(each_row*sizeof(int));
	MPI_Scatter(b, each_row, MPI_INT, myb, each_row, MPI_INT, source, MPI_COMM_WORLD);
	
	// scattering the matrix rows
	mybuffer = (int *)malloc(n*each_row*sizeof(int));
	MPI_Scatter(buffer, n*each_row, MPI_INT, mybuffer, n*each_row, MPI_INT, source, MPI_COMM_WORLD);

	// Multiplying vector and respective matrix rows
	for (i = 0; i < n; ++i)
	{
		int val = 0;
		for (j = 0; j < each_row; ++j)
			val+=(mybuffer[(j*n)+i]*myb[j]);
		c[i] = val;
	}

	// Gathering the final vector rows
	myc = (int *)malloc(n*numprocs*sizeof(int));
	MPI_Gather(c, n, MPI_INT, myc, n, MPI_INT, source, MPI_COMM_WORLD);

	if(myid == source)
	{
		printf("OUTPUT\n");
		// adding all the corresponding entries obtained
		for (i = 0; i < n; ++i)
		{
			int val = 0;
			for (j = 0; j < numprocs; ++j)
				val+=myc[(j*n)+i];
			c[i] = val;
		}
		
		print_vector(c, n);
	}

	MPI_Finalize();

	return 0;
}