#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex>
#include <vector>
#include <thread>

using namespace std;

void generate_polynomial(vector< complex<double> > &a, int n)
{
	for (int i = 0; i < n; ++i)
	{
		double t = (double)(rand()%10);
		a.push_back(polar(t, 0.0));
	}

	for (int i = 0; i < n; ++i)
		a.push_back(polar(0.0, 0.0));
}

// printing polynomial
void print_polynomial(vector< complex<double> > a)
{
	int n = a.size();
	for (int j = 0; j < n; ++j)
		cout << a[j] << " ";
	cout << endl;
}

void fft(vector< complex<double> > a, complex<double> w, vector< complex<double> > &f_a)
{
	int n = a.size();
	f_a.resize(n);

	if(n == 1){
		f_a[0] = a[0];
		return;
	}

	vector< complex<double> > a_even, a_odd, f_even, f_odd;
	for(int i = 0 ; i < n ; ++i)
	{
		if(i%2 == 0)
			a_even.push_back(a[i]);
		else
			a_odd.push_back(a[i]);
	}

	fft(a_even, pow(w, 2), f_even);
	fft(a_odd, pow(w, 2), f_odd);

	complex<double> x = polar(1.0, 0.0);
	for(int i = 0 ; i < (n/2) ; ++i)
	{
		f_a[i] = f_even[i] + (x*f_odd[i]);
		f_a[i + (n/2)] = f_even[i] - (x*f_odd[i]);
		x = x*w;
	}
}

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	int n = 4;
	int n_f = (2*n) - 1;

	vector< complex<double> >  a, b, c, f_a, f_b, f_c;
	double angle = (double) (M_PI)/n;
	complex<double> w = polar(1.0, angle);
	complex<double> n_complex = polar((double)2*n, 0.0);

	generate_polynomial(a, n);
	generate_polynomial(b, n);

	print_polynomial(a);
	print_polynomial(b);

	// Step 1
	fft(a, w, f_a);
	fft(b, w, f_b);
	
	// Step 2
	for(int i = 0 ; i < 2*n ; ++i)
		f_c.push_back(f_a[i]*f_b[i]);
	
	// Step 3
	fft(f_c, conj(w), c);
	
	// Step 4
	for(int i = 0 ; i < n_f ; ++i)
		c[i] = c[i]/n_complex;

	print_polynomial(c);

	return 0;
}