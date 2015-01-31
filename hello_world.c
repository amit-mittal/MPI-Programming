#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int total, my_rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &total);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	printf("From process %d out of %d, Hello World!\n", my_rank, total);
	MPI_Finalize();

	return 0;
}