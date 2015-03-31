Polynomial Multiplication
--------------------------
Polynomial multiplication using both traditional, O(n^2) and fast fourier transform, O(n*log(n)) algorithms. Fast fourier will also be parallelized using threads library in C++ 
<br/><br/>
To compile the parallel FFT code: <br/>
g++ fft_parallel.cpp -std=c++11 -pthread