
#include "loader.h"
#include "reels.h"
#include "evaluator.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

Evaluator::Evaluator(Loader const &loader)
{
    m_symbols = loader.getSymbols();

    m_pays = loader.getPays();

    for(auto const & payline : loader.getPaylines())
        m_paylines.push_back(payline.second);

    // load up prime numbers (one isn't prime,
    // but we take advantage of that later)
    m_primes[0] = 1;  m_primes[1] = 2;
    m_primes[2] = 3;  m_primes[3] = 5;
    m_primes[4] = 7; m_primes[5] = 13;
    m_primes[6] = 17; m_primes[7] = 19;
    m_primes[8] = 23; m_primes[9] = 29;
    m_primes[10] = 31; m_primes[11] = 37;
    m_primes[12] = 41; m_primes[13] = 43;
    m_primes[14] = 47; m_primes[15] = 53;

    // create a hash for the win based on the product of 
    // 3 primes. This guarantees the key will always be unique
    for(auto pay : m_pays)
    {
        int product = 1;
        for(auto p2 : pay.symbols)
        {
            //handle the ANY symbol
            if(1 == p2)
            {
                // for an ANY, add a win for every other permutation
                // of the other symbols
                for(int i = 2; i < m_symbols.size(); ++ i)
                {
                    for(auto p3 : pay.symbols)
                    {
                        //cout << "adding pay: " << i << " " << p3 << endl;
                        //cout << "composite: " << m_primes[i] * m_primes[p3] << endl;
                        //cout << "win: " << pay.win << endl;
                        m_pay_lookup[m_primes[i] * m_primes[p3]] = pay.win;
                    }
                }
                // signal to prevent double-add
                product = 1;
            }
            else
            {
                product *= m_primes[p2];
            }
        }
        if(1 != product)
            m_pay_lookup[product] = pay.win;
    }
}

ResultSet Evaluator::evaluate(ReelWindow const &win)
{
    ResultSet rs;

    cout << "------------------------------------" << endl;
    cout << "Reel Stops: " << endl;;

    using std::setw;
    for(int i = 0; i < 3; ++i)
    {
        int consumed = 0;
        for(int j = 0; j < 3; ++j)
        {
            auto whichsym { win[3*i +j] };
            auto const & sym{ m_symbols[whichsym] };
            cout << sym << "   ";
        }
        cout << endl;
    }

    // allows the reel window to be accessed as a 2D array
    static const int xlat[3][3] =
    {
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 }
    };

    // shorthand alias
    auto pr = m_primes;

    bool newline = true;
    int npay = 0;
    // create a symbol combination from
    // the reel window + line
    for(auto & line : m_paylines)
    {
        auto sym1 = win[xlat[line[0]][0]];
        auto sym2 = win[xlat[line[1]][1]];
        auto sym3 = win[xlat[line[2]][2]];

        // compute a product from the 3 symbols
        int product = pr[sym1] * pr[sym2] * pr[sym3];

        //cout << "payline: " << 
        //    m_symbols[sym1] << " " << m_symbols[sym2]  << m_symbols[sym3]  << endl;
        if(m_pay_lookup.end() != m_pay_lookup.find(product))
        {
            if(newline)
                cout << endl;
            newline = false;
            cout << "line: " << npay << ", win: " << m_pay_lookup[product] << ", sym: ";
            cout <<    m_symbols[sym1] << " " << m_symbols[sym2]  << " " << m_symbols[sym3]  << endl;
        }
        ++npay;
    }

    cout << "------------------------------------" << endl;

    cout << endl;

    return rs;
}


int Evaluator::evaluateNoPrint(ReelWindow const &win)
{
    // allows the reel window to be accessed as a 2D array
    static const int xlat[3][3] =
    {
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 }
    };

    int paid = 0;
    auto const pr = m_primes;
    for(auto & line : m_paylines)
    {
        auto sym1 = win[xlat[line[0]][0]];
        auto sym2 = win[xlat[line[1]][1]];
        auto sym3 = win[xlat[line[2]][2]];

        // compute a unique product from the 3 symbols (mapped to primes)
        auto product = pr[sym1] * pr[sym2] * pr[sym3];

        if(m_pay_lookup.end() != m_pay_lookup.find(product))
        {
            paid += m_pay_lookup[product];
            ++m_num_pays;
        }
    }

    return paid;
}
