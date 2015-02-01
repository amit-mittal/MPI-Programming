#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int myid, numprocs;
	int count, source, i, j, tag;
	int *buffer, *temp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	count = 4;
	source = 0;
	tag = 1234;
	if (myid == source)
	{
		int total = count*(numprocs-1);
		buffer = (int*)malloc(total*sizeof(int));
		for (i = 0; i < total; ++i)
			buffer[i] = i;

		// serial transmission of data
		for (i = 0; i < (numprocs-1); ++i)
			MPI_Send(buffer + (count*i), count, MPI_INT, i+1, tag, MPI_COMM_WORLD);

		int final_ans = 0;
		for (i = 0; i < (numprocs-1); ++i)
		{
			int sum;
			MPI_Recv(&sum, 1, MPI_INT, i+1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			final_ans += sum;
		}

		printf("Final Sum is %d\n", final_ans);
	}
	else
	{
		temp = (int*)malloc(count*sizeof(int));
		MPI_Recv(temp, count, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		int sum = 0;
		for (i = 0; i < count; ++i)
			sum+=temp[i];

		MPI_Send(&sum, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
	
		printf("Sum sent by process %d is %d\n", myid, sum);
	}

	MPI_Finalize();

	return 0;
}