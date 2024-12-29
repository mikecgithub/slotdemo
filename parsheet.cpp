
#include "loader.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

using namespace libxl;

struct Evaluator
{




};

int main() 
{
    Loader loader("./parsheet.xlsx");

	loader.validate();

	loader.load();

    cout << "done" << endl;
    return 0;
}

