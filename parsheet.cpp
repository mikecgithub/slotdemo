#include "libxl.h"

#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::endl;
using std::string;

using namespace libxl;

enum class Symbols
{

};


struct Loader
{
	explicit Loader(string);

	protected:

	void loadSymbols(Sheet *sheet, int, int);

// map of short-name, desc, like <BAR, "Single bar">
	std::map<string,string> m_symbols;
};

int main() 
{

	Loader loader("./parsheet.xlsx");

    cout << "done" << endl;
    return 0;
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
                for(int j = 0; j < 100; ++ j)
                {
                    char const * text = sheet->readStr(i,j);
                    if(nullptr != text)
                    {
                        auto cellText {std::string(text) };
                        //cout << "Text: " << (text) << endl;
                        if("Symbols:" == cellText)
                            loadSymbols(sheet, i,j);
                    }
                }
            }
        }
        book->release();
    } 
} 


void Loader::loadSymbols(Sheet *sheet, int row, int col)
{
    cout << "Loading symbols: " << endl;

    for(int i = 2; i < 20; ++ i)
    {
        char const * text = sheet->readStr(row+i, col);
        if(nullptr != text)
        {
            string label = text;
            text = sheet->readStr(row+i, col+1);
            if(nullptr == text)
                throw std::invalid_argument("Symbol " + label + " must have short alias in next column.");
            string sym = text;

            cout << "Sym: " << label << " / " << sym << endl;
        }
		else
		{
			// cancel after this section is read
			break;
		}
    }

}
