#include "pch.h"
# include <cstdlib >
# include <iostream >
#include < mpi.h>


using namespace std;
int main(int argc, char * argv[]) {
	int my_rank; // rang du processus
	int nb_procs; // nombre de processus
	MPI_Init(&argc, &argv); // D´emarrer MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);
	cout << "Je suis le processus " << my_rank << " parmi " << nb_procs << endl;
	MPI_Finalize(); // Quitter MPI
}