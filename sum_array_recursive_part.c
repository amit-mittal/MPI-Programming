#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]){
	int myid, numprocs;
	int count, source, i, j, tag;
	long long int left_sum, right_sum;
	int *buffer, *temp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	count = 500000;
	source = 1;
	tag = 1234;

	int total = count*(numprocs/2);
	buffer = (int*)malloc(total*sizeof(int));
	
	int left = 2*myid; // left child
	int right = left+1; // right child
	int parent = myid/2; // parent node
	bool if_source = (source == myid);
	bool if_leaf = (right >= numprocs);

	if(myid >= source)
	{
		if (myid == source)
		{
			// making the data in the source
			for (i = 0; i < total; ++i)
				buffer[i] = i;
		}
		else
		{
			// if not source receiving the no. of 
			MPI_Recv(&total, 1, MPI_INT, parent, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(buffer, total, MPI_INT, parent, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
		}

		if(!if_leaf)
		{
			// Sending buffer size and data to left child
			int val = total/2;
			MPI_Send(&val, 1, MPI_INT, left, tag, MPI_COMM_WORLD);
			MPI_Send(buffer, total/2, MPI_INT, left, tag, MPI_COMM_WORLD);

			// Sending buffer size and data to right child
			MPI_Send(&val, 1, MPI_INT, right, tag, MPI_COMM_WORLD);
			MPI_Send(buffer+(total/2), total/2, MPI_INT, right, tag, MPI_COMM_WORLD);

			// Receiving the respective left and right sum
			MPI_Recv(&left_sum, 1, MPI_LONG_LONG_INT, left, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&right_sum, 1, MPI_LONG_LONG_INT, right, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			long long int ans = left_sum + right_sum;
			if(myid == source)
			{
				printf("Final sum is %lld\n", ans);			
			}
			else
			{
				// Sending back the sum computed
				MPI_Send(&ans, 1, MPI_LONG_LONG_INT, parent, tag, MPI_COMM_WORLD);
			}
		}
		else
		{
			// computing the sum in leaf node
			long long int sum = 0LL;
			for (i = 0; i < total; ++i)
			{
				sum+=buffer[i];
			}

			// Sending sum back to parent
			MPI_Send(&sum, 1, MPI_LONG_LONG_INT, parent, tag, MPI_COMM_WORLD);
		}
	}	

	MPI_Finalize();

	return 0;
}