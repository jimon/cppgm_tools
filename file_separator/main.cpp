// jimon, 2013

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		cout << "separate file to multiple ones by lines count" << endl;
		cout << "using :" << endl;
		cout << "file_separator.exe file_name lines_count" << endl;
		return 0;
	}

	string name(argv[1]);

	size_t count = 0;
	istringstream temp(argv[2]);
	temp >> count;

	if(count == 0)
	{
		cerr << "lines_count == 0" << endl;
		return 1;
	}

	ifstream file(name);

	size_t n = 0;
	for(string line; !file.eof(); ++n)
	{
		ostringstream temp;
		temp << string("s") << n << string("_") << name;
		ofstream output(temp.str());

		ostringstream out;
		for(size_t i = 0; (i < count) && getline(file, line); ++i)
			output << line << endl;
	}

	return 0;
}
