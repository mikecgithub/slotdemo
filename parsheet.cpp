
#include "loader.h"
#include "evaluator.h"
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

static constexpr int ONE_MILLION = 1000000;

int main() 
{
    Loader loader("./parsheet.xlsx");

    loader.validate();

    loader.load();

    Evaluator ev{loader};

    Reels reels(loader.getReels());

    int coinIn = 5 * ONE_MILLION;
    int coinOut= 0;

    for(int i = 0; i < ONE_MILLION; ++i)
    {
        coinOut += ev.evaluateNoPrint(reels.spin());
    }

    for(int i = 0; i < 5; ++i)
        ev.evaluate(reels.spin());

    cout << "coinIn = " << coinIn << endl;
    cout << "coinOut = " << coinOut << endl;
    cout << "Percentage: " << std::setprecision(3) << 100.f * (double(coinIn - coinOut)/double(coinIn)) << "% " << endl;

    return 0;
}

