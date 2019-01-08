#ifndef GENETICALGORITHMFORCARDPROBLEMMPI_H
#define GENETICALGORITHMFORCARDPROBLEMMPI_H



class GeneticAlgorithmForCardProblemMPI {

public: 

	GeneticAlgorithmForCardProblemMPI(int maxiteration = 1000, int popsize = 30, int dim = 10,
		double probCross = 0.5, double probaMut = 0.1,
		double sumTrag = 36, double prodTrag = 360
		);
	
	~GeneticAlgorithmForCardProblemMPI();

	double random(double min, double max) const;

	//Runs the Microbial GA to solve the problem domain
	//Where the problem domain is specified as follows

	//You have 10 cards numbered 1 to 10.
	//You have to divide them into 2 piles so that:

	//The sum of the first pile is as close as possible to 36
	//And the product of all in second pile is as close as poss to 360
	void run(int argc, char * argv[]);
private :

	//population size
	 int _popsize ;
	 
	 //Dimension
	 int _dimension ;

	 //mutation Probability
	 double _mutation_probability ;

	 //Crossover probability
	 double _crossover_probability ;

	 //the genes array, 30 members, 10 cards each
	 int **  _population;

	 //how many tournaments should be played
	 int _max_iterations ;

	 // the sum pile, end result for the SUM pile
	 //card1 + card2 + card3 + card4 + card5, MUST = 36 for a good GA
	 double _sum_targ ;

	 //the product pile, end result for the PRODUCT pile
	 //card1 * card2 * card3 * card4 * card5, MUST = 360 for a good GA
	 double _prod_targ;

	 //the genes array, 30 members, 10 cards each	 int **_population ;
	 int _number_of_iteration_before_sending ;


	 //evaluate the the nth member of the population
	 //@param n : the nth member of the population
	 //@return : the score for this member of the population.
	 //If score is 0.0, then we have a good GA which has solved
	 //the problem domain
	 double evaluate(int n);
	 

	 //Display the results. Only called for good GA which has solved
	 //the problem domain
	 //@param iteration : the current iteration loop number
	 //@param n : the nth member of the population.
	 void display(int iteration, int individual);

	 void initial_population();
	


};
#endif // !GENETICALGORITHMFORCARDPROBLEMMPI_H
