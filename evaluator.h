

#pragma once

#include "loader.h"
#include "reels.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
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

    int evaluateNoPrint(ReelWindow const &win);

    // look up the details of the pay
    string lookup(int pay);

protected:

    // hard-coded max of 16 symbols
    // used for optimized lookup
    int m_primes[16];

    std::vector<std::vector<int>> m_paylines;

    // how many distinct wins per eval
    int m_num_pays = 0;

    std::vector<WinTuple> m_pays;

    std::vector<string> m_symbols;

    // optimized version of pay lookup
    std::map<int, int> m_pay_lookup;
};

