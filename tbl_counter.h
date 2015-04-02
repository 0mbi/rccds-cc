#ifndef TBL_COUNTER_H

#include <vector>
#include <map>
#include <sstream>
#include <fstream>

using namespace std;

typedef vector<int> terminal_t;
typedef map<int,int> blv_t;
typedef pair<blv_t,int> blv_int_pair;
typedef map< terminal_t, map<blv_t, int> > tbl_map_t;
typedef pair< terminal_t,map<blv_t, int> > tbl_pair;

class TBLCounter {
  private:
    tbl_map_t tbl_map;
  public:
    void add( terminal_t terminals, blv_t bvector )
    {
      tbl_map_t::iterator tbl_finder = tbl_map.find( terminals );
      if( tbl_finder == tbl_map.end() )
      {
        map<blv_t,int> new_map;
        new_map.insert(blv_int_pair(bvector,1));
        tbl_map.insert( tbl_pair(terminals,new_map) );
      }
      else
      {
        map<blv_t,int>& blv_map = tbl_finder->second;
        map<blv_t,int>::iterator blv_finder = blv_map.find( bvector );
        if( blv_finder == blv_map.end() ) // should never happen ;)
        {
          blv_map.insert( blv_int_pair(bvector,1) );
        }
        else
        {
          blv_finder->second++;
        }
      }
    }

    void add( terminal_t terminals, blv_t bvector, int amount ) {
      tbl_map_t::iterator tbl_finder = tbl_map.find( terminals );
      if( tbl_finder == tbl_map.end() )
      {
        map<blv_t,int> new_map;
        new_map.insert(blv_int_pair(bvector,amount));
        tbl_map.insert( tbl_pair(terminals,new_map) );
      }
      else
      {
        map<blv_t,int>& blv_map = tbl_finder->second;
        map<blv_t,int>::iterator blv_finder = blv_map.find( bvector );
        if( blv_finder == blv_map.end() ) // should never happen ;)
        {
          blv_map.insert( blv_int_pair(bvector,amount) );
        }
        else
        {
          blv_finder->second += amount;
        }
      }

    }

    void out()
    {
      for( tbl_map_t::iterator i = tbl_map.begin() ; i != tbl_map.end() ; i++ )
      {
        for( terminal_t::const_iterator j = i->first.begin() ; j != i->first.end() ; j++ )
        {
          if( *j != 0 )
            cout << " " << *j;
        }
        cout << ":" << endl;
        
        for( map<blv_t,int>::const_iterator j = i->second.begin() ; j != i->second.end() ; j++ )
        {
          for( blv_t::const_iterator k = j->first.begin() ; k != j->first.end() ; k++ )
            cout << k->second << " ";
          cout << ": " << j->second << endl;
        }
      }
    }
    void merge_from( TBLCounter& tblc )
    {
      for( tbl_map_t::iterator i = tblc.tbl_map.begin() ; i != tblc.tbl_map.end() ; i++ )
      {
        tbl_map_t::iterator tbl_finder = this->tbl_map.find( i->first );
        if( tbl_finder == this->tbl_map.end() )
        {
          this->tbl_map.insert( tbl_pair(i->first,i->second) );
        }
        else
        {
          for( map<blv_t,int>::const_iterator j = i->second.begin() 
            ; j != i->second.end() ; j ++ )
          {
            map<blv_t,int>::iterator blv_finder = tbl_finder->second.find(j->first);
            if( blv_finder == tbl_finder->second.end() )
            {
              tbl_finder->second.insert( blv_int_pair(j->first,j->second) );
            }
            else
            {
              blv_finder->second += j->second;
            }
          }
        }

      }
    }


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
	        branch_left_vector.insert( pair<int,int>(i++,x) );
		}
		stringstream ss2;
		ss2 << numstr;
		ss2 >> x;
	    this->add( terminal, branch_left_vector, x);
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
	        	// if the first char is a space we can read a terminal string
	        	if( line[0] == ' ' ) 
	        	{
	                terminal.clear();
	        		stringstream ss;
	        		ss << line;

	        		int x;
	        		while(ss >> x)
	        		{
	                    terminal.push_back(x);
	        		}
	        	}
	        	// if the first char is a zero we have a branch left vector
	        	if( line[0] == '0' ) 
	        	{
	        		// read the branch left vector and add it to 
	        		// tblcounter with corresponding terminal set
	        		read_branchleft(terminal, line);
	        	}
	        }
	    }
	    infile.close();	
	}

  vector< map<blv_t, int> > find_all_branchv_with_size( int size )
  {
    vector< map<blv_t, int> > rval;
    for( tbl_map_t::iterator i = tbl_map.begin() ; i != tbl_map.end() ; i++ )
    {
      if( i->first.back() == size ) 
      // the last terminal is always 
      // the size of a chord diagram
      {
        rval.push_back( i->second );
      }
    }
    return rval;
  }

};

#define TBL_COUNTER_H
#endif