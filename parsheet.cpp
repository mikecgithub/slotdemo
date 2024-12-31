
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

int main() 
{
    Loader loader("./parsheet.xlsx");

    loader.validate();

    loader.load();

    Evaluator ev{loader};

    Reels reels(loader.getReels());

    for(int i = 0; i < 5; ++i)
        ev.evaluate(reels.spin());

    return 0;
}

