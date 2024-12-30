
#include "loader.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

using namespace libxl;

// flatten the 3x3 window to 1D
using ReelWindow = int *;

// vector of winning pay indicies of all
// wins based on paylines 
using ResultSet = std::vector<int>;

// evaluator only cares about paytables
// and reel windows
struct Evaluator
{
	explicit Evaluator(Loader const &);

	ResultSet evaluate(ReelWindow const &);

protected:

};


// reels only care about reel defs and RNG
struct Reels
{
	Reels() :
		m_rd (),
		m_gen( m_rd() )
	{}


	std::random_device m_rd;
	std::mt19937 m_gen;
	std::uniform_int_distribution<> m_dist;

	std::vector<std::vector<int>> m_reels;
};

int main() 
{
    Loader loader("./parsheet.xlsx");

	loader.validate();

	loader.load();

	Evaluator ev{loader};

    cout << "done" << endl;
    return 0;
}

Evaluator::Evaluator(Loader const &loader)
{



}



ResultSet Evaluator::evaluate(ReelWindow const &win)
{
	ResultSet rs;



	return rs;
}
