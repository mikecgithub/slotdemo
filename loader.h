
#pragma once

#include "libxl.h"

#include <map>
#include <string>
#include <vector>

using std::string;

using namespace libxl;

using ReelSet = std::vector<std::vector<int>>;

using PaySchedule = std::map<int, std::vector<int>>;

int constexpr NUM_REELS = 3;

class Loader
{
public:

    explicit Loader(string);

    // ensure parsheet is logically consistent
    bool validate() const;

    bool load();

	ReelSet getReels() {
		return m_reels;
	}

protected:

    void loadPays(Sheet *, int, int);

    void loadPaylines(Sheet *, int, int);

    void loadSymbols(Sheet *, int, int);

    void loadReels(Sheet *, int, int);

    // 2D string rep of reels, i.e. { SEV, BAR, CHR }
    std::vector<std::vector<string>> m_reels_str;

    // 2D numeric rep of reels, i.e. { 3, 8, 1 }
	ReelSet	m_reels;

    // map of short-name, desc, like <BAR, "Single bar">
    std::map<string,string> m_symbols;

    // map of payline number -> shape, like { 1, {-1,0,1}}
    PaySchedule m_paylines;

    // 2D list of payout schedule, i.e. { BAR, BAR, BAR, 30}
    std::vector<std::vector<string>> m_payout_str;
};
