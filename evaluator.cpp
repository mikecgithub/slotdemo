
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
                break;
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

    // optimization: a lookup table that
    // converts from (reel, line index) => window idx
    static const int xlat[][3] =
    {
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 }
    };

    cout << "------------------------------------" << endl;

    using std::setw;
    for(int i = 0; i < 3; ++i)
    {
        int consumed = 0;
        for(int j = 0; j < 3; ++j)
        {
            auto whichsym { win[xlat[i][j]]};
            auto const & sym{ m_symbols[whichsym] };
            cout << sym << "   ";
        }
        cout << endl;
    }

    // shorthand alias
    auto pr = m_primes;

    bool newline = true;
    int npay = 0;
    // create a symbol combination from
    // the reel window + line
    for(auto & line : m_paylines)
    {
        // compute a product from the 3 symbols
        int product = pr[win[xlat[0][line[0]]]];
        product *= pr[win[xlat[1][line[1]]]];
        product *= pr[win[xlat[2][line[2]]]];

        if(m_pay_lookup.end() != m_pay_lookup.find(product))
        {
            if(newline)
                cout << endl;
            newline = false;
            cout << "line: " << npay << ", win: " << m_pay_lookup[product] << ", sym: ";
           
           cout << m_symbols[win[xlat[0][line[0]]]] << " " << m_symbols[win[xlat[1][line[1]]]] << " " << m_symbols[win[xlat[2][line[2]]]] << endl;
        }
        ++npay;
    }

    cout << "------------------------------------" << endl;

    cout << endl;

    return rs;
}


int Evaluator::evaluateNoPrint(ReelWindow const &win)
{
    // optimization: a lookup table that
    // converts from (reel, line index) => window idx
    static const int xlat[][3] =
    {
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 }
    };

    int paid = 0;
    auto const pr = m_primes;
    for(auto & line : m_paylines)
    {
        // compute a product from the 3 symbols
        int product = pr[win[xlat[0][line[0]]]];
        product *= pr[win[xlat[1][line[1]]]];
        product *= pr[win[xlat[2][line[2]]]];

        if(m_pay_lookup.end() != m_pay_lookup.find(product))
        {
            paid += m_pay_lookup[product];
            ++m_num_pays;
        }
    }

    return paid;
}
