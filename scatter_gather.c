#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int mpi_root = 0;
	int numnodes, myid;
	int *myray,*send_ray,*back_ray;
	int count;
	int size,mysize,i,k,j,total;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numnodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	count = 5;
	myray = (int*)malloc(count*sizeof(int));
	if(myid == mpi_root)
	{
	    size = count*numnodes;
		send_ray = (int*)malloc(size*sizeof(int));
		back_ray = (int*)malloc(numnodes*sizeof(int));
		for(i=0;i<size;i++)
			send_ray[i]=i;
	}

	MPI_Scatter(send_ray, count, MPI_INT, myray, count, MPI_INT, mpi_root, MPI_COMM_WORLD);

	total=0;
	for(i=0;i<count;i++)
	    total=total+myray[i];
	printf("myid = %d total = %d\n",myid,total);
    MPI_Gather(&total, 1, MPI_INT, back_ray, 1, MPI_INT, mpi_root, MPI_COMM_WORLD);

	if(myid == mpi_root){
		total=0;
		for(i=0;i<numnodes;i++)
	    	total=total+back_ray[i];
		printf("Results from all processors = %d \n",total);
	}

	MPI_Finalize();

	return 0;
}