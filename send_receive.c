#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int myid, numprocs;
	int tag, source, destination, count;
	int buffer;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	tag = 11;
	source = 0;
	destination = 1;
	count = 1;

	if(myid == source)
	{
		buffer = 1234;
		MPI_Send(&buffer, count, MPI_INT, destination, tag, MPI_COMM_WORLD);
		printf("Processor %d sent %d\n", myid, buffer);
	}

	if(myid == destination)
	{
		MPI_Recv(&buffer, count, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Processor %d got %d\n", myid, buffer);
	}

	MPI_Finalize();

	return 0;
}