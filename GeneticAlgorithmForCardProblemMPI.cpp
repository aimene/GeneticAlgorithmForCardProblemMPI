#include < mpi.h>
#include "pch.h"
#include "GeneticAlgorithmForCardProblemMPI.h"

#include<cstdlib>
#include<iostream>
#include<ctime>



GeneticAlgorithmForCardProblemMPI::GeneticAlgorithmForCardProblemMPI(int maxiteration, int popsize, int dim, double probCross,
	double probaMut, double sumTrag, double prodTrag):
	_max_iterations{ maxiteration }, _popsize{ popsize }, _dimension{ dim }, _crossover_probability{ probCross },
	_mutation_probability{ probaMut }, _sum_targ{ sumTrag }, _prod_targ{ prodTrag }

{

		_population = new int*[_popsize];
		for (int i = 0; i < _popsize; i++)
			_population[i] = new int[_dimension];

}

GeneticAlgorithmForCardProblemMPI::~GeneticAlgorithmForCardProblemMPI()
{
	for (int i = 0; i < _popsize; i++)
		delete[] _population[i];
	delete[] _population;

	//free(_population);
	
	
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
			std::cout << sum;

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
		std::cout << "=" << prod << std::endl;
	}

}

//initialise population
void GeneticAlgorithmForCardProblemMPI::initial_population()
{
	//for entire population
	for (int i = 0; i < _popsize; ++i)
	{
		//for all genes
		for (int j = 0; j < _dimension; ++j)
		{
			double x = random(0, 1);
			//randomly create gene values
			if (random(0, 2) < 0.5)
				_population[i][j] = 0;
			else
				_population[i][j] = 1;
		}
	}

}

double GeneticAlgorithmForCardProblemMPI::random(double min, double max) const
{

	double f = (double)rand() / RAND_MAX;
	return min + f * (max - min);

}


double GeneticAlgorithmForCardProblemMPI::evaluate(int n)
{	           
	//initialise field values
	int sum = 0; int prod = 1;
	double scaled_sum_error, scaled_prod_error, combined_error;
	//loop though all genes for this population member
	for (int i = 0; i < _dimension; ++i)
	{
		//if the gene value is 0, then put it in the sum (pile 0), and calculate sum
		if (_population[n][i]==0)
			sum += (1 + i);
		//if the gene value is 1, then put it in the product (pile 1), and calculate sum
		else 
			prod *= (i + 1);
	

	}
	//work out how food this population member is, based on an overall error
	//for the problem domain
	//NOTE : The fitness function will change for every problem domain.
	scaled_sum_error = (sum - _sum_targ) / _sum_targ;
	scaled_prod_error = (prod - _prod_targ) / _prod_targ; 

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

	
	int a, b, winner, loser;
	//initialize the population (randomly)
	initial_population();

	// 2 vector for the 2 best population that will be send
	int * best_pop1;
	int * best_pop2;

	// initialize vector
	best_pop1 = new int[11];
	best_pop2 = new int[11];

	int tag = 123;

	// the previous process
	int num_proc_precedent = (num_procs + my_rank - 1) % num_procs;

	// the next process
	int num_proc_suivant = (my_rank + 1) % num_procs;

	// fitness values 
	double best1, best2, test;
	double worst1, worst2;

	//population indexes
	int indiceBest1, indiceBest2;
	int indiceWorst1, indiceWorst2;

	// pick up the index of the first population to start comparing
	indiceBest1 = indiceWorst1 = 0;

	//calculate fitness
	best1 = worst1 = evaluate(indiceBest1);

	// pick up the index of the second population to start comparing
	indiceBest2 = indiceWorst2 = 1;

	//calculate fitness
	best2 = worst2 = evaluate(indiceBest2);

	//start a tournament
	for (int iter = 0; iter < _max_iterations; iter++)
	{
		//pull 2 population members at random
		a = (int)random(1, 10);
		b = (int)random(1, 10);

		//have a fight, see who has best genes
		if (evaluate(a) < evaluate(b))
		{
			winner = a;
			loser = b;
		}
		else {
			winner = b;
			loser = a;
		}
		//Possibly do some gene jiggling, on all genes of loser
		//again depends on randomness (simulating the natural selection
		//process of evolutionary selection)
		for (int i = 0; i < _dimension; i++)
		{

			//Crossover operator
			if (random(0, 0.5) < _crossover_probability) {
				_population[loser][i] = _population[winner][i];
			}


			//Mutation operator
			if (random(0, 0.5) < _mutation_probability) {
				_population[loser][i] = 1 - _population[loser][i];
			}
			//then test to see if the new population member is a winner
			if (evaluate(loser) == 0.0)
			{
				display(iter, loser);
				iter = _max_iterations;
				break;
			}
		}		

		// send 2 best individuals to the next process and receive the 2 best process from the previous process
		if (iter % 100 == 0) 
		{
			//evaluate the 30 individuals and pick up the 2 best and the 2 worst
			for (int i = 2; i < _popsize; i++)
			{
				// get fitness of the current index (i) and test it with the 2 best an 2 worst
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


			// store the 2 best individuals
			for (int j = 0; j < 10; j++)
			{		
				best_pop1[j] = _population[indiceBest1][j];
				best_pop2[j] = _population[indiceBest2][j];
			}

			// store the indice of the tho best indivduals in the last case of each vector
			best_pop1[10] =indiceBest1;
			best_pop2[10] = indiceBest2;


			// each process send his 2 best population to the next process  
			// and get the 2 best population of the previous process
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

			//each process replace the 2 worst by the 2 best
			for (int j = 0; j < 10; j++)
			{
				_population[indiceWorst1][j] = best_pop1[j];
				_population[indiceWorst2][j] = best_pop2[j] ;

			}
			//each process pick up the 2 index of the best population for next iteration
			best1 = best_pop1[10];
			best2 = best_pop2[10];	
		}
	}

	// process number 0 will display the final  result
	if (my_rank == 0)
			display(_max_iterations, indiceBest1);
		
	
	
	delete[] best_pop1;
	delete[] best_pop2;
	MPI_Finalize();
	
}


