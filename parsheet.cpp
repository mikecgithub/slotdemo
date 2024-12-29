#include "libxl.h"

#include <iostream>
#include <codecvt>
#include <locale>


using std::cout;
using std::endl;

using namespace libxl;

int main() 
{
    auto book = xlCreateXMLBook();
    if(book->loadSheet("./parsheet.xlsx", 0))
    {
	    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	cout << "loading parshet" << endl;
        auto sheet = book->getSheet(0);
        if(sheet)
        {
	for(int i =0 ;i < 100; ++ i)
	{
	for(int j = 0; j < 100; ++ j)
	{
		char const * text = sheet->readStr(i,j);
		if(nullptr != text)
		    cout << "Text: " << (text) << endl;
	}
        }
	}
        book->release();
    } 
	cout << "done" << endl;
    return 0;
}
