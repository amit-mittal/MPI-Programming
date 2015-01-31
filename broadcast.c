#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int myid, numprocs;
	int count, source, i;
	int buffer[10];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	source = 0;
	count = 10;
	if(myid == source)
	{
		for (i = 0; i < count; ++i)
			buffer[i] = i;
	}

	MPI_Bcast(buffer, count, MPI_INT, source, MPI_COMM_WORLD);
	for (i = 0; i < count; ++i)
		printf("%d:%d ", myid, buffer[i]);
	printf("\n");

	MPI_Finalize();

	return 0;
}