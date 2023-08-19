#include "Josephus.h"
#include "CircularList.h"
#include "Soldier.h"

#include <iostream>
#include <vector>

#define PRINT_NAME_VALUE(x) #x << " = " << x << std::endl

using namespace std;

//--------------------------------------------

int main() {

#if 1 // The Josephus Problem

	JosephusProblemSolver<int> intSolver{};
	JosephusProblemSolver<Soldier> soldierSolver{};

	try {
		intSolver("numbers.txt", 15, 9);
		soldierSolver("soldiers.txt", 1, 2);
	}
	catch (const FileException& e) {
		e.what();
	}

#endif

	/**********************************/
	/* Circular List exemplary usage: */
	/**********************************/

#if 0 // Constructors & Assignment operators & Generating values

	CircularList<int> c1{};							// default constructor (empty list)
	CircularList<int> c2(5u, 3);						// c2 = { 3, 3, 3, 3, 3 }
	CircularList<int> c3(c2);						// copy constructor c3 = c2
	CircularList<int> c4(move(c3));					// move constructor
	CircularList<int> c5 = { 1, 2, 3 };				// using initializer_list

	cout << PRINT_NAME_VALUE(c1)
		<< PRINT_NAME_VALUE(c2)
		<< PRINT_NAME_VALUE(c4)
		<< PRINT_NAME_VALUE(c5);

	int tab[] = { 1, 2, 3, 4, 5 };
	vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
	CircularList<int> c6(begin(tab), end(tab));
	CircularList<int> c7(v.begin() + 1, v.end());	// c6 = { 2, 3, 4, 5, 6, 7 }


	c6 = c5;									// move assignment
	c6.print(cout, " --> ");					// pretty printing with delimeter " --> "
	cout << endl;								// 1 --> 2 --> 3

	CircularList<int> c8{};
	c8.generateN(10, [n = 1]() mutable { return n *= 2; });
	cout << PRINT_NAME_VALUE(c8);

#endif // Constructors & Assignment operators & Generating values


#if 0 // Inserting & Removing & Sorting & Reversing

	CircularList<int> i1 = { 1, 2, 3 };
	i1.insertAfter(1u, 3u, 0);					// i1 = { 1, 2, 0, 0, 0, 3 }

	cout << PRINT_NAME_VALUE(i1);

	vector<int> v = { 5, 6 };
	i1.insertAfter(2u, v.begin(), v.end());		// i1 = { 1, 2, 0, 5, 6, 0, 0, 3 }

	CircularList<int> r1(i1), r2(i1);
	r1.remove(0);								// r1 = { 1, 2, 5, 6, 3 }
	r2.remove_if([](							// r2 = { 1, 5, 3 }
		const int& el) {
			return el % 2 == 0;
		});

	cout << PRINT_NAME_VALUE(i1)
		<< PRINT_NAME_VALUE(r1)
		<< PRINT_NAME_VALUE(r2);

	r1.bubbleSort();							// r1 = { 1, 2, 3, 5, 6 }
	cout << PRINT_NAME_VALUE(r1);

	r1.reverse();								// r1 = { 6, 5, 3, 2, 1 }		
	cout << PRINT_NAME_VALUE(r1);

#endif // Inserting & Removing & Sorting & Reversing

	system("pause");
}

//--------------------------------------------
