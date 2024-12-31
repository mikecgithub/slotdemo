
#include "reels.h"


static int r0_size, r1_size, r2_size;

Reels::Reels(ReelSet const &reels) :
	m_rd {},
	m_gen( m_rd() ),
	m_reels{reels}
{
	m_window = new int[NUM_REELS * NUM_REELS];
	std::fill(m_window, m_window + (NUM_REELS * NUM_REELS), 0);

	r0_size = m_reels[0].size();
	r1_size = m_reels[1].size();
	r2_size = m_reels[2].size();

	for(int i = 0; i < NUM_REELS; ++i)
	{
		m_rng[i] = std::uniform_int_distribution<>(0, reels[i].size() - 1); 

		// optimization: add the first 2 symbols to the end, this means
		// that when the RNG selects the "last" element, you can still
		// go 2 more symbols after the "end" and not have to program the
		// wrap-around logic. I claim this change doesn't affect the
		// probability.
		m_reels[i].push_back(m_reels[i][0]);
		m_reels[i].push_back(m_reels[i][1]);
	}

}

Reels::~Reels() {
	delete [] m_window;
}

ReelWindow Reels::spin()
{
    auto r0 = m_rng[0](m_gen);
    auto r1 = m_rng[1](m_gen);
    auto r2 = m_rng[2](m_gen);

	if(r0 > r0_size)
	{
		cout << "r0 : " << r0 << endl;
		throw 0;
	}
	if(r1 > r1_size)
	{
		cout << "r1 : " << r1 << endl;
		throw 0;
	}
	if(r2 > r2_size)
	{
		cout << "r2 : " << r2 << endl;
		throw 0;
	}

	// m_window indices :
	// 0 1 2
	// 3 4 5
	// 6 7 8

	// load up the window with symbols
	// from the random stops on the reels
	// because we extended the reels by 2, 
	// we don't have to worry about wrap-around
	m_window[0] = m_reels[0][r0 + 0];
	m_window[3] = m_reels[0][r0 + 1];
	m_window[6] = m_reels[0][r0 + 2];

	m_window[1] = m_reels[1][r1 + 0];
	m_window[4] = m_reels[1][r1 + 1];
	m_window[7] = m_reels[1][r1 + 2];

	m_window[2] = m_reels[2][r2 + 0];
	m_window[5] = m_reels[2][r2 + 1];
	m_window[8] = m_reels[2][r2 + 2];

	for(int i=0;i < 9; ++i)
	{
		if(m_window[i] > 9)
		{
			cout << "Error: reel > 9: " << r2 << " - " << i << " - " << m_window[i] << endl;

			cout << "r0 : " << r0 << " " << r0_size << endl;
			cout << "r1 : " << r1 << " " << r1_size << endl;
			cout << "r2 : " << r2 << " " << r2_size << endl;
		}
	}

    return m_window;
}
