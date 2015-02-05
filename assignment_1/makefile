MCC=mpicc
CC=gcc

parallel: floyd_warshall_row.c serial optimized
	$(MCC) floyd_warshall_row.c -o floyd_warshall_row -lm

serial: floyd_warshall_serial.c
	$(CC) floyd_warshall_serial.c

optimized: floyd_warshall_row_optimized.c
	$(MCC) floyd_warshall_row_optimized.c -o floyd_warshall_row_optimized -lm