#include <iostream>
#include <cmath>
#include "sorted_list.h"
#include "tour.h"

int choose(int p)
{
	double f;
	f = rand() % (p * p);
	f = sqrt(f);
	return (int) f;
}

void usage(char *progname)
{
   std::cout << "Usage: " << progname << "population generations method [quiet]"
   << std::endl << "\twhere:" << std::endl
   << "\t\t\"population\" is the population size" << std::endl
   << "\t\t\"generations\" is the number of generations you want" << std::endl
   << "\t\t\"method\" is the method of storage:" << std::endl
   << "\t\t\t(1 == dynamic array, 2 == linked list" << std::endl
   << "\t\t\t 3 == binary tree, 4 == avl tree)" << std::endl
   << "\t\t\"quiet\" is optional, and will disable dumping info" << std::endl
   << "\t\t\t about the tours.  use this when timing." << std::endl;
   exit(1);
}

int main(int argc, char **argv)
{
	int pop_size, gens, i, choice;
	SortedList<Tour> *s;

	if (argc < 4) usage(argv[0]);
	if (argc > 5) usage(argv[0]);

	choice = atoi(argv[3]);
	if (choice == 1)
		s = new ArrayList<Tour>;
	else if (choice == 2)
		s = new LinkedList<Tour>;
	else if (choice == 3)
		s = new BSTList<Tour>;
	else if (choice == 4)
		s = new AVLList<Tour>;
	else
		usage(argv[0]);
	pop_size = atoi(argv[1]);
	gens = atoi(argv[2]);

	initialize_distances();

	for (i = 0; i < pop_size; i++)
		s->insert(build_a_random_bit_string());

	if (argc == 4) { // no "quiet" parameter.
		std::cout << "The initial population, sorted (descending), is " << std::endl;
		for (i = pop_size; i > 0; i--)
			(*s)[pop_size - i].print();
	}
    
	for (i = 0; i < gens; i++) {

		s->remove();
		s->insert(combine((*s)[choose(pop_size - 1)], (*s)[choose(pop_size - 1)]));
	}

	if (argc == 4) { // no "quiet" parameter.
		std::cout << "The final population is " << std::endl;
		for (i = pop_size; i > 0; i--)
			(*s)[pop_size - i].print();
	}

	return 0;
}

