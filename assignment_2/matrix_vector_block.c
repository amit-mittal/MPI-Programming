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

void copy_data(int **mat, int row, int col, int size, int *buffer)
{
	int i, j;

	for (i = 0; i < size; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			buffer[(size*i)+j] = mat[row+i][col+j];
		}
	}
}

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	int **mat;
	int *buffer, *b, *c, *mybuffer, *myc, *myb;
	int n, i, j, k, tag;
	int source, each_row, each_col, dest_id, buffer_size;
	int numprocs, myid;
	int size[2], grid_coords[2], periodic[2], coords[2];
	MPI_Comm grid_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	n = 4;
	tag = 0;
	source = 0;
	size[0] = size[1] = 0;
	periodic[0] = periodic[1] = 0;

	// allocating memory to matrix
	buffer = (int *)malloc(n*n*sizeof(int));
	b = (int *)malloc(n*sizeof(int));
	c = (int *)malloc(n*sizeof(int));
	mat = (int **)malloc(n*sizeof(int *));
	for (i = 0; i < n; ++i)
		mat[i] = buffer + (n*i);

	// matric and vectors generated
	if(myid == source)
	{
		printf("INPUT\n");
		generate_input(mat, b, n);
		print_matrix(mat, n);
		print_vector(b, n);
	}

	// Creating virtual grid
	MPI_Dims_create(numprocs, 2, size);
	MPI_Cart_create(MPI_COMM_WORLD, 2, size, periodic, 0, &grid_comm);
	MPI_Cart_coords(grid_comm, myid, 2, grid_coords);

	each_col = n/size[1];
	each_row = n/size[0];
	buffer_size = each_col*each_row;

	int *temp_buffer;
	temp_buffer = (int *)malloc(buffer_size*sizeof(int));
	if(myid == source)
	{
		for (i = 0; i < size[0]; ++i)
		{
			coords[0] = i;
			for (j = 0; j < size[1]; ++j)
			{
				if(i == 0 && j == 0)
					continue;

				coords[1] = j;
				MPI_Cart_rank (grid_comm, coords, &dest_id);

				// copy data to a buffer and needs to send
				
				copy_data(mat, i*each_row, j*each_col, each_row, temp_buffer);

				// sending data
				MPI_Send (temp_buffer, buffer_size, MPI_INT, dest_id, source, grid_comm);

				// Send b also
			}
		}
	}
	else
	{
		// receiving the data
		MPI_Recv (temp_buffer, buffer_size, MPI_INT, source, tag, grid_comm, MPI_STATUS_IGNORE);
	}

	MPI_Finalize();

	return 0;
}