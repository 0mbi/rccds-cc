#include <fstream>
#include <iostream>
#include <sstream>
#include "tbl_counter.h"

using namespace std;

TBLCounter tbl_counter;

void read_branchleft(terminal_t terminal,string line)
{
	size_t pos = line.find(":");
	string blstr = line.substr( 0, pos );
	string numstr = line.substr( pos+1, line.length() );
    blv_t branch_left_vector;
	stringstream ss;
	ss << blstr;
	int x;
    int i = 1;
	while( ss >> x )
	{
		cout << x << " ";
        branch_left_vector.insert( pair<int,int>(i++,x) );
	}
	stringstream ss2;
	ss2 << numstr;
	cout << "|";
	ss2 >> x;
    // pair: branchleft_vector -> amount of occurence
    tbl_counter.add( terminal, branch_left_vector, x);
	cout << x << " ";
	cout << endl;
}

void readfile(const char *file_name)
{
    string line;
    ifstream infile;
    infile.open (file_name);
    terminal_t terminal;
    while(!infile.eof() ) // To get you all the lines.
    {
        getline(infile,line); // Saves the line in STRING.

        if( line.size() > 1 )
        {
        	if( line[0] == ' ' )
        	{
                terminal.clear();
        		cout << "New terminal" << endl;
        		stringstream ss;
        		ss << line;

        		int x;
        		while(ss >> x)
        		{
        			cout << x << " " ;
                    terminal.push_back(x);
        		}
        		cout << endl;
        	}
        	if( line[0] == '0' )
        	{
        		read_branchleft(terminal, line);
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

	tbl_counter.readfile( argv[1] );
    tbl_counter.out();

	return 0;
}
