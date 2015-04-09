#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

void generate_polynomials(vector<int> &a, int n)
{
	for (int i = 0; i < n; ++i)
	{
		int t = rand()%10;
		a.push_back(t);
	}
}

// printing polynomial
void print_polynomials(vector< int > a)
{
	int n = a.size();
	for (int j = 0; j < n; ++j)
		cout << a[j] << " ";
	cout << endl;
}

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	vector<int>  a, b, c;
	int n = 2048;
	int n_f = (2*n) - 1;

	generate_polynomials(a, n);
	generate_polynomials(b, n);

	// print_polynomials(a);
	// print_polynomials(b);
	
	c.resize(n_f);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			c[i+j] += (a[i]*b[j]);
		}
	}

	// print_polynomials(c);

	return 0;
}