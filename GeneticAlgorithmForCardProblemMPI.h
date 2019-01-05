#ifndef GENETICALGORITHMFORCARDPROBLEMMPI_H
#define GENETICALGORITHMFORCARDPROBLEMMPI_H



class GeneticAlgorithmForCardProblemMPI {

public: 

	GeneticAlgorithmForCardProblemMPI(int maxiteration = 1000, int popsize = 30, int dim = 10,
		double probCross = 0.5, double probaMut = 0.1,
		double sumTrag = 36, double prodTrag = 360, int number_of_individual_to_send = 2,
		int number_of_iteration_before_sending = 100
		);
	
	~GeneticAlgorithmForCardProblemMPI();
	double random(double min, double max) const;

	void run(int argc, char * argv[]);
private :

	 int _popsize ;
	 int _dimension ;

	 double _mutation_probability ;
	 double _crossover_probability ;

	 int _max_iterations ;

	 double _sum_targ ;
	 double _prod_targ;

	 int **_population ;
	
	 int _number_of_individual_to_send ;
	 int _number_of_iteration_before_sending ;

	 
	 double evaluate(int n);
	 
	 void display(int iteration, int individual);
	 void initial_population();
	


};
#endif // !GENETICALGORITHMFORCARDPROBLEMMPI_H
