#include "loader.h"

#include "libxl.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

using namespace libxl;

// using a separate validate method allows more flexibility
// in the spreadsheet format. It allows any combination of 
// tables to be read in, whereas to make the code more 
// efficient (validate itself as it loads) would require
// posting the tables in the spreadsheet in specific order
bool Loader::validate() const
{
    bool ret = true;

    // 1) make sure every reel symbol appears in the list of symbols
    for(auto const & reel : m_reels_str)
    {
        for(auto const & sym : reel)
        {
            if(m_symbols.end() == m_symbols.find(sym))
            {
                cout << "Error: reel sym " << sym << " not found in symbols list" << endl;
                ret = false;
            }
        }
    }

    // 2) every payout symbol must be included in the symbols list
    for(auto const & payout : m_payout_str)
    {
		int count = 0; // only care about first 3 syms
        for(auto const &sym : payout)
        {
            if(m_symbols.end() == m_symbols.find(sym) && count < 3)
            {
                cout << "Error: payout sym " << sym << " not found in symbols list" << endl;
                ret = false;
            }
			++count;
        }
    }

    // 3) (skipped) to be _very_ technical, you would verify that
    // each pay was physically capable of being paid, i.e. each 
    // winning symbol must appear at least once on the reel

    return ret;
}


// convert strings to enumerated symbols, ready to be evaluated
bool Loader::load()
{
	// a vector of just the symbols (without descriptions)
	std::vector<string> symbols;

	for(auto const & sym : m_symbols)
		symbols.push_back(sym.first);

	// converts string BAR_2X to int, e.g. 2
	auto encode = [&symbols](auto const &sym) -> int {
		return (int)std::distance(symbols.begin(), 
			std::find(symbols.begin(), symbols.end(), sym));
	};

	for(auto const & reel : m_reels_str)
	{
		std::vector<int> reel_int;
		for(auto const & sym : reel)
		{
			auto len = std::distance(symbols.begin(), 
				std::find(symbols.begin(), symbols.end(), sym));

			// cout << "len = " << len << endl;
			reel_int.push_back(len);
		}
		m_reels.emplace_back(reel_int);
	}

	for(auto const & pay : m_payout_str)
	{
		auto win{ std::stoi(pay[3]) };

		std::vector<int> syms{ encode(pay[0]), encode(pay[1]), encode(pay[2]) };

		m_pays[win] = syms;
	}

    return true;
}


Loader::Loader(string fname)
{
    auto book = xlCreateXMLBook();
    if(book->loadSheet(fname.c_str(), 0))
    {
        auto sheet = book->getSheet(0);
        if(sheet)
        {
            for(int i =0 ;i < 100; ++ i)
            {
                for(int j = 0; j < 200; ++ j)
                {
                    char const * text = sheet->readStr(i,j);
                    if(nullptr != text)
                    {
                        auto cellText {std::string(text) };
                        if("Symbols:" == cellText)
                            loadSymbols(sheet, i,j);
                        else if("Reels:" == cellText)
                            loadReels(sheet,i,j);
                        else if("Pay Schedule:" == cellText)
                            loadPays(sheet,i+2,j);
                        else if("Paylines:" == cellText)
                            loadPaylines(sheet,i+2,j);
                    }
                }
            }
        }
        book->release();
    } 
} 

void Loader::loadPaylines(Sheet *sheet, int row, int col)
{
    cout << "Loading Paylines: " << endl;

    // assumes a 3-reel machine, although generalization is possible
    string reel_window[3][3];

    for(int reel = 0; reel < 3; ++ reel)
    {
        for(int j = 0; j < 3; ++ j)
        {
            auto text = sheet->readStr(row+j, col+reel);
            if(nullptr != text)
                reel_window[reel][j] = text;
            else
                throw std::invalid_argument("Paylines must be 3x3 integers");
        }
    }

    // helper function
    auto contains = [](string const &heystack, int needle) -> bool {
        bool ret = (needle < 10);
        ret &= (string::npos != heystack.find(char(needle + '0')));
        return ret;
    };

    // paylines are written graphically in excel, concatenated 
    // digit-wise with other payline geometries. they must start
    // at 1 and increment sequentially. e.g.
    // 1  X  X
    // X  1  X
    // X  X  1
    //
    // 1  X   X
    // 2  21  2
    // X  X   1
    //
    // decode up to 9 paylines. this could be extended to 35 if you used
    // letters + numbers, 61 if CAPS letters
    for(int pay_idx = 1; pay_idx < 10; ++pay_idx)
    {
        std::vector<int> pl(3,0);

        int found = 0;

        //  find this payline's position in the window
        //  relative to the centerline
        for(int reel=0; reel <3; ++reel)
        {
            for(int pos=0; pos <3; ++pos)
            {
                if(contains(reel_window[reel][pos],pay_idx))
                {
                    pl[reel] = pos;
                    ++ found;
                    break;
                }
            }
        }

        if(3 == found)
        {
            m_paylines[pay_idx] = pl;

            cout << "pl " <<  pay_idx << "> " ;
            for(int j = 0; j < 3; ++j )
                cout << pl[j] << " " ;
            cout << endl;
        }
        else
        {
            if(0 == found)
                break;
            else
                throw std::invalid_argument("Invalid Payline definition: " + std::to_string(pay_idx));
        }
    }
}

void Loader::loadPays(Sheet *sheet, int row, int col)
{
    cout << "Loading Pays: " << endl;

    std::vector<string> payline(4,"");

    for(int i = 0; i < 1000; ++ i)
    {
        auto text = sheet->readStr(row+i, col);
        if(nullptr != text)
        {
            // record the 3 syms + payout (as string)
            for(int j = 0; j < 3; ++ j)
            {
                payline[j] = text;
                text = sheet->readStr(row+i, col+1+j);
                if(nullptr == text)
                    throw std::invalid_argument("Payout Schedule must have 4 entries");
            }
            // the payout amount (as string)
            payline[3] = text;

            m_payout_str.push_back(payline);
        }
        else if (nullptr == sheet->readStr(row+i+1,col))
        {
            // quit after two successive blank rows
            break;
        }
    }
}

void Loader::loadReels(Sheet *sheet, int row, int col)
{
    cout << "Loading reels: " << endl;

    // right now, hard-coded to 3 reels, can easily be generalized
    for(int reel = 0; reel < 3; ++ reel)
    {
        std::vector<string> reel_text;
        for(int i = 2; i < 90; ++ i)
        {
            auto text = sheet->readStr(row+i, col + reel);
            if(nullptr != text)
            {
                string label = text;
                reel_text.push_back(label);

                // always add a blank
                reel_text.push_back("BL");
            }
            else
            {
                // cancel after this section is read
                break;
            }
        }
        m_reels_str.emplace_back(std::move(reel_text));
    }
}

void Loader::loadSymbols(Sheet *sheet, int row, int col)
{
    cout << "Loading symbols: " << endl;

    for(int i = 2; i < 20; ++ i)
    {
        auto text = sheet->readStr(row+i, col);
        if(nullptr != text)
        {
            string label = text;
            text = sheet->readStr(row+i, col+1);
            if(nullptr == text)
                throw std::invalid_argument("Symbol " + label + " must have short alias in next column.");
            string sym = text;

			m_symbols[sym] = label;
            //cout << "Sym: " << label << " / " << sym << endl;
        }
        else
        {
            // cancel after this section is read
            break;
        }
    }
}
