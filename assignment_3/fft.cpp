#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex>
#include <vector>
#include <thread>

using namespace std;

void generate_polynomials(vector< complex<double> > &a, int n)
{
	for (int i = 0; i < n; ++i)
	{
		double t = (double)(rand()%10);
		a.push_back(polar(t, 0.0));
	}

	for (int i = 0; i < n; ++i)
	{
		double t = 0.0;
		a.push_back(polar(t, 0.0));
	}
}

// printing polynomial
void print_polynomials(vector< complex<double> > a)
{
	int n = a.size();
	for (int j = 0; j < n; ++j)
	{
		cout << a[j] << " ";
	}
	cout << endl;
}

vector< complex<double> > fft(vector< complex<double> > a, complex<double> w)
{
	vector< complex<double> > f_a;
	int n = a.size();
	f_a.resize(n);

	if(n == 1)
	{
		f_a[0] = a[0];
		return f_a;
	}

	vector< complex<double> > a_even;
	vector< complex<double> > a_odd;
	for(int i = 0 ; i < n ; ++i)
	{
		if(i%2 == 0)
		{
			a_even.push_back(a[i]);
		}
		else
		{
			a_odd.push_back(a[i]);
		}
	}

	vector< complex<double> > f_even = fft(a_even, pow(w, 2));
	vector< complex<double> > f_odd = fft(a_odd, pow(w, 2));

	complex<double> x = polar(1.0, 0.0);
	for(int i = 0 ; i < (n/2) ; ++i)
	{
		f_a[i] = f_even[i] + (x*f_odd[i]);
		f_a[i + (n/2)] = f_even[i] - (x*f_odd[i]);
		x = x*w;
	}

	return f_a;
}

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	vector< complex<double> >  a, b, c;
	int n = 1024*1024;
	int n_f = (2*n) - 1;

	generate_polynomials(a, n);
	generate_polynomials(b, n);

	//print_polynomials(a);
	//print_polynomials(b);
	
	double angle = (double) (M_PI)/n;
	complex<double> w = polar(1.0, angle);

	vector< complex<double> > f_a = fft(a, w);
	vector< complex<double> > f_b = fft(b, w);
	vector< complex<double> > f_c;
	for(int i = 0 ; i < 2*n ; ++i)
	{
		f_c.push_back(f_a[i]*f_b[i]);
	}
	
	c = fft(f_c, conj(w));
	complex<double> n_complex = polar((double)2*n, 0.0);
	for(int i = 0 ; i < n_f ; ++i)
	{
		c[i] = c[i]/n_complex;
	}

	//print_polynomials(c);

	return 0;
}