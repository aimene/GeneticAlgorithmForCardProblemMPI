#pragma once
#ifndef MPIEXAMPLECLASS_H
#define MPIEXAMPLECLASS_H
#include < mpi.h>

class mpiexamples {

public : 

	// who am i 
	void program1(int argc, char * argv[]);

	// point to point
	void program2(int argc, char * argv[]);

	// sendrecv
	void program3(int argc, char * argv[]);

private : 

	int nb_procs; // nombre de processus
	int my_rank;
	int tag = 0; // ´etiquette
	int source, destination, valeur;
	MPI_Status status;
	int rang, valeurAenvoyee, valeurRecu, num_proc;


};
#endif // !MPIEXAMPLECLASS_H
