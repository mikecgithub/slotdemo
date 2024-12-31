
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

    explicit Reels(ReelSet const &reels);

    ~Reels();

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

