#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000

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

void generate_matrix(int **mat, int n)
{
	int i, j;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			mat[i][j] = rand()%10000;
			if(mat[i][j] >= 4700 && mat[i][j] <= 5300)
				mat[i][j] = INF;
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

int main()
{
	int **mat;
	int i, j, k;
	int n;

	scanf("%d", &n);
	//n = 1000;
	
	mat = (int**)malloc(n*sizeof(int*));
	for (i = 0; i < n; ++i)
		mat[i] = (int*)malloc(n*sizeof(int));

	printf("INPUT\n");
	//generate_matrix(mat, n);
	take_input(mat, n);
	//print_matrix(mat, n);


	printf("OUTPUT\n");
	for(k = 0 ; k < n ; ++k)
	{
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

	print_matrix(mat, n);

	return 0;
}