#include "pch.h"
# include <cstdlib >
# include <iostream >
#include < mpi.h>

#include"mpiExamples.h"

using namespace std;



void mpiexamples::program1(int argc, char * argv[]) {
	
	MPI_Init(&argc, &argv); // D´emarrer MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);
	cout << "Je suis le processus " << my_rank << " parmi " << nb_procs << endl;
	MPI_Finalize(); // Quitter MPI
}

void mpiexamples::program2(int argc, char * argv[]) {

	

	MPI_Init(&argc, &argv); // D´emarrer MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if (my_rank == 2)
	{
		destination = 5;
		valeur = 1000;
		MPI_Send(&valeur, 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
	}
	else
	{
		if (my_rank == 5)
		{
			source = 2;
			MPI_Recv(&valeur, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			cout << "Moi , processus " << my_rank << " j’ai re¸cu " << valeur << " du processus "
				<< source << endl;
		}
	}
	MPI_Finalize(); // Quitter MPI
}


void mpiexamples::program3(int argc, char * argv[]) {

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rang);
	// On suppose avoir exactement 2 processus
	num_proc = (rang + 1) % 2;
	valeurAenvoyee = rang + 1000;
	MPI_Sendrecv(&valeurAenvoyee, 1, MPI_INT, num_proc, tag, &valeurRecu, 1, MPI_INT,
		num_proc, tag, MPI_COMM_WORLD, &status);
	cout << "Moi , processus " << rang << " j’ai re¸cu " << valeurRecu << " du processus " <<
		num_proc << endl;
	MPI_Finalize();

	}
