#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void read_branchleft(string line)
{
	size_t pos = line.find(":");
	string blstr = line.substr( 0, pos );
	string numstr = line.substr( pos+1, line.length() );
	stringstream ss;
	ss << blstr;
	int x;
	while( ss >> x )
	{
		cout << x << " ";
	}
	stringstream ss2;
	ss2 << numstr;
	cout << "|";
	ss2 >> x;
	cout << x << " ";
	cout << endl;
}

void readfile(const char *file_name)
{
    string line;
    ifstream infile;
    infile.open (file_name);
    while(!infile.eof() ) // To get you all the lines.
    {
        getline(infile,line); // Saves the line in STRING.
        if( line.size() > 1 )
        {
        	if( line[0] == ' ' )
        	{
        		cout << "New terminal" << endl;
        		stringstream ss;
        		ss << line;
        		int x;
        		while(ss >> x)
        		{
        			cout << x << " " ;
        		}
        		cout << endl;
        	}
        	if( line[0] == '0' )
        	{
        		read_branchleft(line);
        	}
        }
        else 
        {
        	
        }
    }
    infile.close();	
}

int main(int argc, char const *argv[])
{
	if( argc != 2 )
	{
		cout << "Usage read-chord-data [file_name]" << endl;
		return 0;
	}

	readfile( argv[1] );

	return 0;
}
