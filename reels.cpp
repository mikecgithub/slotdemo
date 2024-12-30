
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

ReelWindow Reels::spin()
{
    auto r0 = m_rng[0](m_gen);
    auto r1 = m_rng[1](m_gen);
    auto r2 = m_rng[2](m_gen);

	// m_window indices :
	// 0 1 2
	// 3 4 5
	// 6 7 8

	// load up the window with symbols
	// from the random stops on the reels
	m_window[0] = m_reels[0][r0] + 0;
	m_window[3] = m_reels[0][r0] + 1;
	m_window[6] = m_reels[0][r0] + 2;

	m_window[1] = m_reels[1][r1] + 0;
	m_window[4] = m_reels[1][r1] + 1;
	m_window[7] = m_reels[1][r1] + 2;

	m_window[2] = m_reels[2][r2] + 0;
	m_window[5] = m_reels[2][r2] + 1;
	m_window[8] = m_reels[2][r2] + 2;

    return m_window;
}
