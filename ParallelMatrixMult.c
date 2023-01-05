#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define R1 15 //rows1
#define C1 13 //cols1
#define R2 13 //rows2
#define C2 14 //cols2

void initializeMatrix(int rows, int cols, int matrix[rows][cols])
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			matrix[i][j] = rand() % 20;
}

void zero(int rows, int cols, int matrix[rows][cols])
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			matrix[i][j] = 0;
}

void initializeVector(int rows, int vector[rows])
{
	for (int i = 0; i < rows; ++i)
		vector[i] = rand() % 20;
}


void printMatrix(int rows, int cols, int matrix[rows][cols])
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			printf("%2d ", matrix[i][j]);
		printf("\n");
	}
}

void printVector(int rows, int vector[rows])
{
	for (int j = 0; j < rows; ++j)
		printf("%2d\n", vector[j]);
}

void mult(int rows, int cols, int matrix[rows][cols], int vector[cols], int result[rows])
{
	for (int i = 0; i < rows; ++i)
	{
		result[i] = 0;
		for (int j = 0; j < cols; ++j)
			result[i] = result[i] + matrix[i][j]*vector[j];
	}
}

void main (int argc, char *argv[])
{
	MPI_Status status;
	int myrank, P;

	// To keep algorithm simple, we fix to 2, 4 or 8 the number of processes
	// and 8, 16 or 32 the size of the square matrix and the vector
	// where 8 is a multiple of 4

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	int matrix1[R1][C1];
    int matrix2[R2][C2];
	int piece; //number of rows per process
	int tailR; //number of extra rows for process
	tailR=R1%P; // number of " extra " rows 
	piece=(R1-tailR)/P; 
	
	int localResult[piece][C2];
	int recvMatrix[piece][C1];
	int result[R1][C2];
	int rowIndexStart;

	if (myrank == 0)
	{
		// STEP 1: process 0 inizializes matrix and vector, then prints
		initializeMatrix(R1, C1, matrix1);
		initializeMatrix(R2, C2, matrix2);

		// print matrix
		printf("Matrix1:\n");
		printMatrix(R1, C1, matrix1);
		printf("\n");
		printf("Matrix2:\n");
		printMatrix(R2, C2, matrix2);
		printf("\n");
		// STEP 2: Process 0 give a pre calculated number of rows of matrix1 to all processes
		// of course not the whole matrix is sent to all workers, only a submatrix
		//but whole matrix2 is sent to all workers

		for(int i=0 ; i<P-1 ;i++ ){
				MPI_Send(&matrix1[i*piece][0], (C1 *piece),MPI_INT,i+1,555,MPI_COMM_WORLD);
				MPI_Send(&i,1,MPI_INT,i+1,555,MPI_COMM_WORLD);
				MPI_Bcast(&matrix2[0][0],(R2*C2), MPI_INT, 0, MPI_COMM_WORLD);

		} 



		
	}

	else{
		
			//slave section
			MPI_Recv(&recvMatrix, (C1 *piece),MPI_INT,0,555,MPI_COMM_WORLD,&status);
			MPI_Recv(&rowIndexStart,1,MPI_INT,0,555,MPI_COMM_WORLD,&status);
			printf("pezzo:\n");
		    printMatrix(piece, C2, matrix2);
			//dont recive the correct matrix, there somethin like a recursive pattern

	}
	




	
	MPI_Finalize();
}
