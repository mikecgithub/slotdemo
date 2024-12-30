
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

    // look up the details of the pay
    string lookup(int pay);

protected:

    std::map<string,string> m_symbols;
};


// reels only care about reel defs and RNG
class Reels
{

public:

    Reels(ReelSet const &reels) :
        m_rd {},
        m_gen( m_rd() ),
        m_reels{reels}
    {
        m_window = new int[NUM_REELS * NUM_REELS];
        std::fill(m_window, m_window + (NUM_REELS * NUM_REELS), 0);

        for(int i = 0; i < NUM_REELS; ++i)
            m_rng[i] = std::uniform_int_distribution<>(0, reels[i].size()); 
    }

    ~Reels() {
        delete [] m_window;
    }

    ReelWindow spin();

    // return "BAR" from the integer
    string decode(int);

protected:

    std::map<string,string> m_symbols;

    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_rng[NUM_REELS];

    int * m_window = nullptr;
    ReelSet m_reels;
};

int main() 
{
    Loader loader("./parsheet.xlsx");

    loader.validate();

    loader.load();

    Evaluator ev{loader};

    Reels reels(loader.getReels());

	reels.spin();
	reels.spin();
	reels.spin();

    cout << "done" << endl;
    return 0;
}

ReelWindow Reels::spin()
{
    cout << m_rng[0](m_gen) << endl;
    cout << m_rng[1](m_gen) << endl;
    cout << m_rng[2](m_gen) << endl;


    return m_window;
}

Evaluator::Evaluator(Loader const &loader)
{


}



ResultSet Evaluator::evaluate(ReelWindow const &win)
{
    ResultSet rs;



    return rs;
}
