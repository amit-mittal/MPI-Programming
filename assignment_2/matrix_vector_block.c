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

void copy_b(int *b, int start, int size, int *temp_b)
{
	int i;

	for (i = 0; i < size; ++i)
		temp_b[i] = b[start + i];
}

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	int **mat;
	int *buffer, *b, *c, *mybuffer, *myc, *myb;
	int n, i, j, k, tag_1, tag_2;
	int source, each_row, each_col, dest_id, buffer_size;
	int numprocs, myid;
	int size[2], grid_coords[2], periodic[2], coords[2];
	MPI_Comm grid_comm, column_comm, row_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	n = 1000;
	tag_1 = 0;
	tag_2 = 1;
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

	// matrix and vectors generated
	if(myid == source)
	{
		printf("INPUT\n");
		generate_input(mat, b, n);
		//print_matrix(mat, n);
		//print_vector(b, n);
	}

	// Broadcasting the vector
	MPI_Bcast (b, n, MPI_INT, source, MPI_COMM_WORLD);
	// TODO Send corresponding column entries only

	// Creating virtual grid
	MPI_Dims_create(numprocs, 2, size);
	MPI_Cart_create(MPI_COMM_WORLD, 2, size, periodic, 0, &grid_comm);
	MPI_Cart_coords(grid_comm, myid, 2, grid_coords);
	MPI_Comm_split (grid_comm, grid_coords[1], grid_coords[0], &column_comm); 

	each_col = n/size[1];
	each_row = n/size[0];
	buffer_size = each_col*each_row;

	int *temp_buffer, *temp_b, *temp_c;
	temp_buffer = (int *)malloc(buffer_size*sizeof(int));
	temp_b = (int *)malloc(each_row*sizeof(int));
	temp_c = (int *)malloc(each_row*sizeof(int));
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
				MPI_Send (temp_buffer, buffer_size, MPI_INT, dest_id, tag_1, grid_comm);

				/*// Send b also
				if(i == j)
				{
					// send to all processes of same column
					copy_data(b, i*each_row, each_row, temp_b);
					MPI_Send (temp_b, each_row, MPI_INT, dest_id, tag_2, grid_comm);
				}*/
			}
		}

		// make temp_buffer and temp_b for source process
		copy_data(mat, 0, 0, each_row, temp_buffer);
		// copy_data(b, 0, each_row, temp_b);
	}
	else
	{
		// receiving the matrix data
		MPI_Recv (temp_buffer, buffer_size, MPI_INT, source, tag_1, grid_comm, MPI_STATUS_IGNORE);

		/*if(dest_id)
		{
			MPI_Recv (temp_b, each_row, MPI_INT, source, tag_2, grid_comm, MPI_STATUS_IGNORE);
		}*/	
	}

	// Now multiply whatever matrix block you have and the correponding part of the vector
	int index = grid_coords[1]*each_row;
	for (i = 0; i < each_row; ++i)
	{
		int val = 0;
		for (j = 0; j < each_col; ++j)
			val+=(temp_buffer[(each_col*i)+j]*b[index+j]);// change to temp_b
		temp_c[i] = val;
	}

	// Splitting grid communicator into row communicator
	MPI_Comm_split(grid_comm, grid_coords[0], grid_coords[1], &row_comm);

	if(grid_coords[1] != 0) // except first entry of each row
	{
		// Now transmit the data across the row
		MPI_Send(temp_c, each_row, MPI_INT, 0, tag_1, row_comm);
	}
	else
	{
		// Receiving the vector from other processors in same row
		int *temp_acc;
		temp_acc = (int*)malloc(n*size[1]*sizeof(int));
		for (i = 1; i < size[1]; ++i)
			MPI_Recv(temp_acc+(i*each_col), each_col, MPI_INT, i, tag_1, row_comm, MPI_STATUS_IGNORE);

		// Adding corresponding vector values
		for (i = 1; i < size[1]; ++i)
		{
			for (j = 0; j < each_col; ++j)
			{
				temp_c[j]+=temp_acc[(i*each_col)+j];
			}
		}
	}

	// Now combine the first column values in root process
	if(grid_coords[1] == 0)
		MPI_Gather(temp_c, each_row, MPI_INT, c, each_row, MPI_INT, source, column_comm);

	// Finally printing the output in root process
	if(myid == source)
	{
		printf("OUTPUT\n");
//		print_vector(c, n);
	}

	MPI_Finalize();

	return 0;
}