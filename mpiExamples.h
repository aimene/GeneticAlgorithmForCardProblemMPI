#pragma once
#ifndef MPIEXAMPLECLASS_H
#define MPIEXAMPLECLASS_H
#include < mpi.h>

class mpiexamples {

public : 
	

	// who am i 
	void program1(int argc, char * argv[]);


	/*Communications point à point*/
	/*============================*/
	// MPI_Send() MPI_Resv()
	void program2(int argc, char * argv[]);

	// sendrecv
	void program3(int argc, char * argv[]);

	// programme joker .cxx 
	// programme sendrecv1 .cxx : Exemple d’envoi des ´el´ements d’un tableau Tab1D
	void program4(int argc, char * argv[]);

	// programme joker2 .cxx
	// programme joker2 . cxx : Envoi des l m e n t s d’un tableau 2D
	void program5(int argc, char * argv[]);


	/*Communications point à point*/
	/*============================*/
	// Anneau de communication
	void program6(int argc, char * argv[]);

	// programme joker .cxx
	void program7(int argc, char * argv[]);

	// programme joker .cxx
	void program8(int argc, char * argv[]);

private : 
	int num_procs;
	int nb_procs; // nombre de processus
	int my_rank;
	int tag = 0; // ´etiquette
	int source, destination;
	MPI_Status status;
	int rang, valeurAenvoyee, valeurRecu, num_proc;

	//program4
	int  mytag = 123, ierr, nbelements,  j, *tab;
	
	// program6
	// Anneau de communication
	int nb_processus,  num_proc_precedent, num_proc_suivant, valeur, val, etiquette = 100;
};
#endif // !MPIEXAMPLECLASS_H
