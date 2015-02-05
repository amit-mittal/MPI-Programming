#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]){
	int myid, numprocs;
	int count, source, i, j, tag, left_sum, right_sum;
	int *buffer, *temp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	count = 10000;
	source = 1;
	tag = 1234;

	int total = count*(numprocs/2);
	buffer = (int*)malloc(total*sizeof(int));
	
	int left = 2*myid;
	int right = left+1;
	int parent = myid/2;
	bool if_source = (source == myid);
	bool if_leaf = (right >= numprocs);

	if(myid >= source)
	{
		if (myid == source)
		{
			for (i = 0; i < total; ++i)
				buffer[i] = i;
		}
		else
		{
			MPI_Recv(&total, 1, MPI_INT, parent, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(buffer, total, MPI_INT, parent, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
		}

		if(!if_leaf)
		{
			int val = total/2;
			MPI_Send(&val, 1, MPI_INT, left, tag, MPI_COMM_WORLD);
			MPI_Send(buffer, total/2, MPI_INT, left, tag, MPI_COMM_WORLD);

			MPI_Send(&val, 1, MPI_INT, right, tag, MPI_COMM_WORLD);
			MPI_Send(buffer+(total/2), total/2, MPI_INT, right, tag, MPI_COMM_WORLD);

			MPI_Recv(&left_sum, 1, MPI_INT, left, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&right_sum, 1, MPI_INT, right, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			int ans = left_sum + right_sum;
			if(myid == source)
			{
				printf("Final sum is %d\n", ans);			
			}
			else
			{
				MPI_Send(&ans, 1, MPI_INT, parent, tag, MPI_COMM_WORLD);
			}
		}
		else
		{
			int sum = 0;
			for (i = 0; i < total; ++i)
			{
				sum+=buffer[i];
			}

			MPI_Send(&sum, 1, MPI_INT, parent, tag, MPI_COMM_WORLD);
		}
	}
	

	MPI_Finalize();

	return 0;
}