#include < mpi.h>
#include "pch.h"
#include "GeneticAlgorithmForCardProblemMPI.h"

#include<cstdlib>
#include<iostream>
#include<ctime>



GeneticAlgorithmForCardProblemMPI::GeneticAlgorithmForCardProblemMPI(int maxiteration, int popsize, int dim, double probCross,
	double probaMut, double sumTrag, double prodTrag, int number_of_individual_to_send, int number_of_iteration_before_sending):
	_max_iterations{ maxiteration }, _popsize{ popsize }, _dimension{ dim }, _crossover_probability{ probCross },
	_mutation_probability{ probaMut }, _sum_targ{ sumTrag }, _prod_targ{ prodTrag },
	_number_of_individual_to_send{ number_of_individual_to_send }, _number_of_iteration_before_sending{ number_of_iteration_before_sending }

{

		_population = new int*[_popsize];
		for (int i = 0; i < _popsize; i++)
			_population[i] = new int[_dimension];

	/*_population = (int**)malloc(_popsize * sizeof(int));
	for (int i = 0; i < _popsize; i++) {
		_population[i] = (int*)malloc(_dimension * sizeof(int));
		std::cout << i << " , ";
	}*/
   //	_population[0][0]= 12;
	//std::cout << _population[0][0];
}

GeneticAlgorithmForCardProblemMPI::~GeneticAlgorithmForCardProblemMPI()
{
	for (int i = 0; i < _popsize; i++)
		delete[] _population[i];
	delete[] _population;

	//free(_population);
	
	
}

double GeneticAlgorithmForCardProblemMPI::evaluate(int n)
{
	int sum = 0; int prod = 0;
	double scaled_sum_error, scaled_prod_error, combined_error;

	for (int i = 0; i < _dimension; ++i)
	{
		if (_population[n][i]==0)
		{
			sum += (1 + i);
		}
		else {
			prod *= (i + 1);
		}

	}
	scaled_sum_error = (sum - _sum_targ) / _sum_targ;
	scaled_prod_error = (prod - -_prod_targ) / _prod_targ; 

	combined_error = abs(scaled_sum_error) + abs(scaled_prod_error);
	return combined_error;
}

void GeneticAlgorithmForCardProblemMPI::run(int argc, char * argv[])
{
	int num_procs, my_rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Status my_status;

	std::cout << " run begin " << std::endl;
	int a, b, winner, loser;
	initial_population();


	int * best_pop1;
	int * best_pop2;

	int tag = 123;


	double best1, best2, test;
	double worst1, worst2;

	int indiceBest1, indiceBest2;
	int indiceWorst1, indiceWorst2;

	
	

	best_pop1 = new int[11];
	best_pop2 = new int[11];




	for (int iter = 0; iter < _max_iterations; iter++)
	{
		a = (int)random(1, 10);
		b = (int)random(1, 10);
	
		if (evaluate(a) < evaluate(b))
		{
			winner = a;
			loser = b;
		}
		else {
			winner = b;
			loser = a;
		}

		for (int i = 0; i < _dimension; i++)
		{

			//Crossover operator
			if (random(0, 2) < _crossover_probability) {
				_population[loser][i] = _population[winner][i];
			}


			//Mutation operator
			if (random(0, 2) < _mutation_probability) {
				_population[loser][i] = 1 - _population[loser][i];
			}

			if (evaluate(loser) == 0.0)
			{
				display(iter, loser);
				iter = _max_iterations;
				break;
			}
		}		
		//std::cout << " rank +iter % 20 " << my_rank << "  "<<iter % 20 << std::endl;

		// send 2 best individuals to the next process and receive the 2 best process from the previous process
		if (iter % 10 == 0) 
		{
			int num_proc_precedent = (num_procs + my_rank - 1) % num_procs;
			int num_proc_suivant = (my_rank + 1) % num_procs;

			indiceBest1 = indiceWorst1 = 0;
			best1 = worst1 = evaluate(indiceBest1);

			indiceBest2 = indiceWorst2 = 1;
			best2 = worst2 = evaluate(indiceBest2);

			//evaluate the 30 individuals and pick up the 2 best and the 2 worst
			for (int i = 2; i < _popsize; i++)
			{
				test = evaluate(i);

				if (test<best2)
				{
					if (test<best1)
					{
						best2 = best1;
						best1 = test;

						indiceBest2 = indiceBest1;
						indiceBest1 = i;
					}
					else
					{
						best2 = test;
						indiceBest2 = i;
					}

				}
				else
				{
					if (test>worst2)
					{
						if (test>worst1)
						{
							worst2 = worst1;
							worst1 = test;

							indiceWorst2 = indiceWorst1;
							indiceWorst1 = i;
						}
						else
						{
							worst2 = test;
							indiceWorst2 = i;
						}
					}
				}

			}
			std::cout << " iteration " << iter << std::endl;
			std::cout << " process numero my_rank " << my_rank << std::endl;
			std::cout << " evaluate(indiceBest1)  " << evaluate(indiceBest1) << std::endl;
			std::cout << " evaluate(indiceBest2)  " << evaluate(indiceBest2) << std::endl;

			// store the 2 best individuals
			for (int j = 0; j < 10; j++)
			{		
				best_pop1[j] = _population[indiceBest1][j];
				best_pop2[j] = _population[indiceBest2][j];
			}

			// store the indice of the tho best indivduals in the last case of each 
			best_pop1[10] =indiceBest1;
			best_pop2[10] = indiceBest2;

			int r;
			
			if (my_rank == 0)
			{
				MPI_Send(best_pop1, 11, MPI_INT, num_proc_suivant, tag, MPI_COMM_WORLD);
				MPI_Send(best_pop2, 11, MPI_INT, num_proc_suivant, tag, MPI_COMM_WORLD);
				MPI_Recv(best_pop1, 11, MPI_INT, num_proc_precedent, tag, MPI_COMM_WORLD, &my_status);
				MPI_Recv(best_pop2, 11, MPI_INT, num_proc_precedent, tag,  MPI_COMM_WORLD, &my_status);
			}
			else {
				MPI_Recv(best_pop1, 11, MPI_INT, num_proc_precedent, tag, MPI_COMM_WORLD, &my_status);
				MPI_Recv(best_pop2, 11, MPI_INT, num_proc_precedent, tag, MPI_COMM_WORLD, &my_status);
				MPI_Send(best_pop1, 11, MPI_INT, num_proc_suivant, tag, MPI_COMM_WORLD);
				MPI_Send(best_pop2, 11, MPI_INT, num_proc_suivant, tag, MPI_COMM_WORLD);

			}

			//replace the 2 worst by the 2 best
			for (int j = 0; j < 10; j++)
			{
				_population[indiceWorst1][j] = best_pop1[j];
				_population[indiceWorst2][j] = best_pop2[j] ;

			}

			best1 = best_pop1[10];
			best2 = best_pop2[10];

			
		}

		

	}
	if (my_rank == 0)
			display(_max_iterations, indiceBest1);
		
	
	
	delete[] best_pop1;
	delete[] best_pop2;
	MPI_Finalize();
	
}


void GeneticAlgorithmForCardProblemMPI::display(int iteration, int individual)
{
	std::cout << "==================================" << std::endl;
	std::cout << " after " << iteration << " Iterations, Solution sum pile ( should be 36 ) card are :" << std::endl;
	int sum = 0;
	int prod = 1;
	for (int i = 0; i < _dimension; ++i)
	{
		
		if (_population[individual][i] == 0)
		{
			std::cout << (i + 1) << "+";
			std::cout << sum ;

			sum += (i + 1);
		}
		std::cout << "=" << sum << std::endl;
	}
	std::cout << " and Product pile ( should be 360 ) cards are :" << std::endl;

	for (int i = 0; i < _dimension; ++i)
	{
		
		if (_population[individual][i] == 1)
		{
			std::cout << (i + 1) << "*";
			std::cout << prod;
			prod *= (i + 1);
		
		}
		std::cout << "=" << prod<< std::endl;
	}

}

void GeneticAlgorithmForCardProblemMPI::initial_population()
{

	

	for (int i = 0; i < _popsize; ++i)
	{
		for (int j = 0; j < _dimension; ++j)
		{
			double x = random(0, 1);
			

			if (random(0, 2) < 0.5) {
				_population[i][j] = 0;
				//std::cout << "_population[" << i << "][" << j << "] " << _population[i][j] << " || ";
			}
			else {
				_population[i][j] = 1;
				//std::cout << "_population[" << i << "][" << j << "] " << _population[i][j] << " || ";
			}
			//std::cout << "random(0, 2) " << x << std::endl;
			
		}
	}
	//std::cout << "_population[29][9]" << _population[29][9] << std::endl;
}

double GeneticAlgorithmForCardProblemMPI::random(double min, double max) const
{
	
	double f = (double)rand() / RAND_MAX;
	return min + f * ( max - min ) ;
	
}
