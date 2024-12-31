
#pragma once

#include "libxl.h"

#include <map>
#include <string>
#include <vector>

using std::string;

using namespace libxl;

using ReelSet = std::vector<std::vector<int>>;

using PayLines = std::map<int, std::vector<int>>;

struct WinTuple
{
	std::vector<int> symbols;

	int win = 0;
};

// Loader reads in the symbols, reels, and pays
// from an Excel spreadsheet using libxl 
class Loader
{
public:

    explicit Loader(string);

    // ensure parsheet is logically consistent
    bool validate() const;

    bool load();

	ReelSet getReels() const {
		return m_reels;
	}

	std::vector<string> getSymbols() const {
		return m_symbols;
	}

	PayLines getPaylines() const {
		return m_paylines;
	}

	std::vector<WinTuple> getPays() const {
		return m_pays;
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

    // vector of symbol abbreviation i.e. BAR
    std::vector<string> m_symbols;

    // map of payline number -> shape, like { 1, {-1,0,1}}
    PayLines m_paylines;

    // 2D list of payout schedule, i.e. { BAR, BAR, BAR, 30}
    std::vector<std::vector<string>> m_payout_str;

    // Same as above, but map<int award, symbols> i.e. <30, { BAR, BAR, BAR, 30}>
	std::vector<WinTuple> m_pays;
};

