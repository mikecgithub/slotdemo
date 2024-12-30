
#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

// flatten the 3x3 window to 1D
using ReelWindow = int *;

using ReelSet = std::vector<std::vector<int>>;

static constexpr int NUM_REELS = 3;

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
		{
            m_rng[i] = std::uniform_int_distribution<>(0, reels[i].size()); 

			// optimization: add the first 2 symbols to the end, this means
			// that when the RNG selects the "last" element, you can still
			// go 2 more symbols after the "end" and not have to program the
			// wrap-around logic. I claim this change doesn't affect the
			// probability.
			m_reels[i].push_back(m_reels[i][0]);
			m_reels[i].push_back(m_reels[i][1]);
		}
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

