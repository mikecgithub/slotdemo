
#include "loader.h"
#include "reels.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

// vector of winning pay indicies of all
// wins based on paylines 
using ResultSet = std::vector<int>;

// evaluator only cares about paytables
// and reel windows
struct Evaluator
{
    explicit Evaluator(Loader const &);

    ResultSet evaluate(ReelWindow const &);

    // look up the details of the pay
    string lookup(int pay);

protected:

	// hard-coded max of 16 symbols
	// used for optimized lookup
	int m_primes[16];

	PaySchedule m_pays;

    std::map<string,string> m_symbols;

	// optimized version of pay lookup
	std::map<int, int> m_pay_lookup;
};


int main() 
{
    Loader loader("./parsheet.xlsx");

    loader.validate();

    loader.load();

    Evaluator ev{loader};

    Reels reels(loader.getReels());

	ev.evaluate(reels.spin());

    return 0;
}

Evaluator::Evaluator(Loader const &loader)
{
	m_symbols = loader.getSymbols();

	m_pays = loader.getPays();

	// load up prime numbers
	m_primes[0] = 2;  m_primes[1] = 3;
	m_primes[2] = 5;  m_primes[3] = 7;
	m_primes[4] = 13; m_primes[5] = 17;
	m_primes[6] = 19; m_primes[7] = 23;
	m_primes[8] = 29; m_primes[9] = 31;
	m_primes[10] = 37; m_primes[11] = 41;
	m_primes[12] = 43; m_primes[13] = 47;
	m_primes[14] = 53; m_primes[15] = 59;

	// create a hash for the win based on the product of 
	// 3 primes. This guarantees the key will always be unique
	for(auto pay : m_pays)
	{
		cout << "pay1 : " << pay.first << " ";
		int product = 1;
		for(auto p2 : pay.second)
			product *= m_primes[p2];
		cout << "pay2 " << product << endl;
		m_pay_lookup[product] = pay.first;
	}
}

ResultSet Evaluator::evaluate(ReelWindow const &win)
{
    ResultSet rs;

	// optimization: a lookup table that
	// converts from (line index) => window idx
	static const int xlat[][3] =
	{
		{ 0, 1, 2 },
		{ 3, 4, 5 },
		{ 6, 7, 8 }
	};

	for(auto const & line : m_pays)
	{
		// create a symbol combination from
		// the reel window + line
		for(int reel = 0; reel < NUM_REELS; ++ reel)
		{



		}


	}



    return rs;
}
