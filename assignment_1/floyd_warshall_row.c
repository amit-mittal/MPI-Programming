// 1. Make data
// 2. buffer should be contiguous, mat is 2d matrix
// 3. 

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define INF 9999

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
	int **mat;
	int *buffer;
	int n, i, j, k;
	int myid, numprocs;
	int source;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	source = 0;

	if(myid == source)
	{
		scanf("%d", &n);
	}

	MPI_Bcast(&n, 1, MPI_INT, source, MPI_COMM_WORLD);

	buffer = (int *)malloc(n*n*sizeof(int));
	mat = (int **)malloc(n*sizeof(int *));
	for (i = 0; i < n; ++i)
		mat[i] = buffer + (n*i);

	if(myid == source)
	{
		printf("INPUT\n");
		take_input(mat, n);
		print_matrix(mat, n);
	}

	MPI_Bcast(buffer, n*n, MPI_INT, source, MPI_COMM_WORLD);

	int each_row = 1; // change it to n/numprocs

	for(k = 0 ; k < n ; ++k)
	{
		for (i = 0; i < numprocs; ++i)
		{
			MPI_Bcast(buffer + (i*each_row*n), each_row*n, MPI_INT, i, MPI_COMM_WORLD);
		}

		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				if((mat[i][k]+mat[k][j]) < mat[i][j])
				{
					mat[i][j] = mat[i][k] + mat[k][j];
				}
			}
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
