#include < mpi.h>
#include "pch.h"

# include <cstdlib >
# include <iostream >



#include"mpiExamples.h"

using namespace std;




void mpiexamples::program1(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	cout << "Je suis le processus " << my_rank << " parmi " << num_procs << endl;
	MPI_Finalize(); // Quitter MPI
}

// point to point MPI_Send() MPI_Resv() 
void mpiexamples::program2(int argc, char * argv[]) {

	MPI_Init(&argc, &argv);

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

// sendrecv
void mpiexamples::program3(int argc, char * argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// On suppose avoir exactement 2 processus
	num_proc = (rang + 1) % 2;
	valeurAenvoyee = rang + 1000;
	MPI_Sendrecv(&valeurAenvoyee, 1, MPI_INT, num_proc, tag, &valeurRecu, 1, MPI_INT,
		num_proc, tag, MPI_COMM_WORLD, &status);
	cout << "Moi , processus " << rang << " j’ai re¸cu " << valeurRecu << " du processus " <<
		num_proc << endl;
	MPI_Finalize();

	}

// programme joker .cxx 
// programme sendrecv1 .cxx : Exemple d’envoi des ´el´ements d’un tableau Tab1D
void mpiexamples::program4(int argc, char * argv[]) {
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if( my_rank == 0)
	{// envoie au processus 1 les deux premi`eres valeurs du tableau tab
	int tab [6] ={1 , 2, 3, 5, 6, 7};
	nbelements =2;
	MPI_Send (tab , nbelements , MPI_INT ,1, mytag , MPI_COMM_WORLD );
	// envoie au processus 2 les trois valeurs restantes du tableau tab
	nbelements =3;
	MPI_Send (tab +3, nbelements , MPI_INT ,2, mytag , MPI_COMM_WORLD );
	}
	else
	{
	MPI_Probe (0, mytag , MPI_COMM_WORLD ,& status );
	MPI_Get_count (& status , MPI_INT ,& nbelements );
	tab =( int *) malloc ( nbelements * sizeof ( int ));
	cout << "Moi , processus " << my_rank << ", je re¸cois " << nbelements
	<< " ´el´ements du processus 0 " ;
	MPI_Recv (tab , nbelements , MPI_INT ,0, mytag , MPI_COMM_WORLD ,& status );
	cout << "Mon tableau C vaut ";
	for(j =0;j< nbelements ;j ++) cout << tab [j] << " ";
	cout << endl ; fflush ( stdout );
	free (tab);
	}
	MPI_Finalize ();
}

// programme joker2 . cxx : Envoi des l m e n t s d’un tableau 2D
void mpiexamples::program5(int argc, char * argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if (my_rank == 0)
	{// Envoi des deux p r e m i r e s valeurs de la p r e m i r e ligne de tab2 au processus 3
		int tab2[2][3] = { 1 , 2, 3, 5, 6, 7 };
		nbelements = 2;
		MPI_Send(tab2, nbelements, MPI_INT, 3, mytag, MPI_COMM_WORLD);
		// Envoi de deux d e r n i r e s valeurs du la d e u x i m e ligne de tab2 au processus 4
		nbelements = 2;
		MPI_Send(tab2[1] + 1, nbelements, MPI_INT, 4, mytag, MPI_COMM_WORLD);
	}
	else // if( my_rank == 4)
	{
		MPI_Probe(0, mytag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &nbelements);
		tab = (int *)malloc(nbelements * sizeof(int));
		cout << "Moi , processus " << my_rank << " nbelements r e u est " << nbelements << endl;
		MPI_Recv(tab, nbelements, MPI_INT, 0, mytag, MPI_COMM_WORLD, &status);
		cout << " tableau = ";
		for (j = 0; j < nbelements; j++)
			cout << tab[j] << " ";
		cout << endl;

		fflush(stdout);
		free(tab);
	}
	MPI_Finalize();
	

}

void mpiexamples::program6(int argc, char * argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	num_proc_precedent = (nb_processus + rang - 1) % nb_processus;
	num_proc_suivant = (rang + 1) % nb_processus;
	val = rang + 1000;
	if (rang == 0)
	{
		MPI_Send(&val, 1, MPI_INT, num_proc_suivant, etiquette, MPI_COMM_WORLD);
		MPI_Recv(&valeur, 1, MPI_INT, num_proc_precedent, etiquette, MPI_COMM_WORLD, &status);
		cout << "Moi , proc . 0, j’ai re¸cu " << valeur << " du proc . " << num_proc_precedent <<
			endl;
	}
	else
	{
		MPI_Recv(&valeur, 1, MPI_INT, num_proc_precedent, etiquette, MPI_COMM_WORLD, &status);
		cout << "Moi , proc . " << rang << " j’ai re¸cu " << valeur << " du proc . " <<
			num_proc_precedent << endl;
		MPI_Send(&val, 1, MPI_INT, num_proc_suivant, etiquette, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}

void mpiexamples::program7(int argc, char * argv[]) {
}

void mpiexamples::program8(int argc, char * argv[]) {
}