#include <stdio.h>
#include <stdlib.h>

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
			int temp = rand()%100;
			mat[i][j] = temp;
		}
	}

	for (j = 0; j < n; ++j)
	{
		int temp = rand()%100;
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

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	int **mat;
	int *buffer, *b, *c;
	int n, i, j, k;

	n = 3;

	// allocating memory to matrix
	buffer = (int *)malloc(n*n*sizeof(int));
	b = (int *)malloc(n*sizeof(int));
	c = (int *)malloc(n*sizeof(int));
	mat = (int **)malloc(n*sizeof(int *));
	for (i = 0; i < n; ++i)
		mat[i] = buffer + (n*i);

	printf("INPUT\n");
	//generate_input(mat, b, n);
	take_input(mat, b, n);
	print_matrix(mat, n);
	print_vector(b, n);

	for (i = 0; i < n; ++i)
	{
		int val = 0;
		for (j = 0; j < n; ++j)
			val+=(mat[i][j]*b[j]);
		c[i] = val;
	}

	printf("OUTPUT\n");
	print_vector(c, n);

	return 0;
}