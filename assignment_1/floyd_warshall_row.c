#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define INF 1000000000
#define min(a, b) (a)<(b) ? a : b

// Taking input the weight matrix
void take_input(int **mat, int n)
{
	int i, j;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			scanf("%d", &mat[i][j]);
			if(mat[i][j] == -1)
			{
				mat[i][j] = INF;
			}
		}
	}
}

void generate_matrix(int **mat, int n)
{
	int count = 0;
	int i, j;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			mat[i][j] = rand()%10000;
			if(mat[i][j] >= 4700 && mat[i][j] <= 5300)
			{
				mat[i][j] = INF;
				++count;
			}
		}
	}

//	printf("%d\n", count);
}

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

int main(int argc, char *argv[])
{
	srand(NULL);

	int **mat;
	int *buffer;
	int n, i, j, k;
	int myid, numprocs;
	int source, each_row;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	source = 0;
	n = 100;// size of graph
	
	// allocating memory to graph
	buffer = (int *)malloc(n*n*sizeof(int));
	mat = (int **)malloc(n*sizeof(int *));
	for (i = 0; i < n; ++i)
		mat[i] = buffer + (n*i);

	// Taking the weight matrix input
	if(myid == source)
	{
		printf("INPUT\n");
		// TODO can do generate specifically for processes
		generate_matrix(mat, n);
		print_matrix(mat, n);
	}

	// broadcasting the whole weight matrix
	MPI_Bcast(buffer, n*n, MPI_INT, source, MPI_COMM_WORLD);

	each_row = (int)ceil(n/numprocs);

	for(k = 0 ; k < n ; ++k)
	{
		// all processes computing respective rows
		for (i = (each_row*myid); i < (min(each_row*(myid+1), n)); ++i)
		{
			for (j = 0; j < n; ++j)
			{
				if((mat[i][k]+mat[k][j]) < mat[i][j])
				{
					mat[i][j] = mat[i][k] + mat[k][j];
				}
			}
		}

		// broadcasting everything which has been computed
		for (i = 0; i < numprocs; ++i)
		{
			MPI_Bcast(buffer + (i*each_row*n), each_row*n, MPI_INT, i, MPI_COMM_WORLD);
		}
	}

	if(myid == source)
	{
		printf("OUTPUT\n");
		print_matrix(mat, n);
	}

	MPI_Finalize();

	return 0;
}
