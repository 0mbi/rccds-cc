#include <ginac/ginac.h>
#include "two-decorations.h"
#include "tbl_counter.h"
#include <iostream>

using namespace std;
using namespace GiNaC;

// returns (x+from)*...*(x+to) ; make sure from <= to
ex mul_from_to(ex x, int from, int to) {
	ex rval = 1;
	if( x == 0 ) return 0;
	for( int i = from ; i <= to ; i++ )
	{
		rval *= (x+i);
	}
	return rval;
}

int factorial(int n) {
	int rval = 1;
	if( n == 0 || n == 1) return rval;
	
	for( int i = 2 ; i <= n ; i++ )
	{
		rval *= i;
	}
	return rval;
}

// returns a weight polynomial in is given by
// the binomial coefficient binomial(dk*s+vk-2,vk)
// where dk is the k-th decoration of a chord diagram
// and vk is the k-th branch left  component of a chord diagram
ex weight( ex s, int vk, int dk )
{
	ex dks = dk*s;
	ex rval = mul_from_to(dks, -1, vk-2 );
	return rval/factorial(vk);
}

ex weight_from_chord(ex s, vector<int> vks, vector<int> dks )
{
	if( vks.size() != dks.size() )
	{
		return 0;
	}
	ex rval = 1;
	for( int i = 0 ; i < vks.size() ; i++ )
	{
		rval *= weight(s,vks[i],dks[i]);
	}
	return rval;
}

// maps size of chords to possible decorations;
typedef map<pair<int,int>, vector<vector<int> > > dec_map_t;
typedef pair<int,int> dkey_t;
typedef vector<vector<int> > dval_t;

class Decorations {
private:
	dec_map_t decoration_map;
public:
	// count non-zero entries
	pair<int,int> non_zeros_and_twos(const int *dec, int size) {
		pair<int,int> non_zero_and_two_pair;
		for( int i = 0 ; i < size ; i++ )
		 {
			if( dec[i] ) non_zero_and_two_pair.first++;
			if( dec[i] == 2) non_zero_and_two_pair.second++;
		}
		return non_zero_and_two_pair;
	}
	void insert( dkey_t non_zero_and_two_pair, 
		vector<int> decoration)
	{
		dec_map_t::iterator finder = decoration_map.find(non_zero_and_two_pair);
		if( finder == decoration_map.end() )
		{
			vector<vector<int> > v;
			v.push_back(decoration);
			decoration_map.insert(
				pair<dkey_t,dval_t>(non_zero_and_two_pair,v) );
		}
		else
		{
			finder->second.push_back(decoration);
		}
	}
	void iterate_insert( const int *d, int size, int num )
	{
		for( int i = 0 ; i < num ; i++ )
		{
			pair<int,int> nzat = non_zeros_and_twos(&d[i*size],size);
			vector<int> decoration;
			for( int j = 0 ; j < size ; j++ )
			{
				if( d[i*size+j] != 0 )
					decoration.push_back( d[i*size+j] );
			}
			insert(nzat,decoration);
		}
	}
	Decorations() {
		iterate_insert(d2[0],2,2);
		iterate_insert(d3[0],3,3);
		iterate_insert(d4[0],4,5);
		iterate_insert(d5[0],5,8);

		iterate_insert(d6[0],6,13);
		iterate_insert(d7[0],7,21);
		iterate_insert(d8[0],8,34);
		iterate_insert(d9[0],9,55);

		iterate_insert(d10[0],10,89);
		iterate_insert(d11[0],11,144);
		iterate_insert(d12[0],12,233);
	}

	vector< vector<int> > get_non_zeros_and_twos( pair<int,int> nzat )
	{
		dec_map_t::iterator finder = decoration_map.find(nzat);
		if( finder == decoration_map.end() )
		{
			vector< vector<int> > v;
			return v;
		}
		else
		{
			return (finder->second);
		}
	}
	
};



int main(int argc, char const *argv[])
{

	TBLCounter tbl_counter;
	tbl_counter.readfile( "CHORD-2" );
	tbl_counter.readfile( "CHORD-3" );
	tbl_counter.readfile( "CHORD-4" );
	tbl_counter.readfile( "CHORD-5" );
	tbl_counter.readfile( "CHORD-6" );
	tbl_counter.readfile( "CHORD-7" );
	tbl_counter.readfile( "CHORD-8" );
	tbl_counter.readfile( "CHORD-9" );	
	tbl_counter.readfile( "CHORD-10" );	
	symbol s("s");
	ex r = mul_from_to(s,1,3);
	cout << r << endl;
	Decorations d;
	vector< vector<int> > v = d.get_non_zeros_and_twos(pair<int,int>(5,2));
	for( vector< vector<int> >::iterator i = v.begin() ; 
		i != v.end() ; i++ )
	{
		for( vector<int>::iterator j = i->begin() ; j != i->end() ; j++ )
		{
			cout << *j << " ";
		}
		cout << endl;
	}

	vector< map<blv_t, int> >  blv_maps = tbl_counter.find_all_branchv_with_size(5);
	for( vector< map<blv_t, int> >::iterator i = blv_maps.begin() ; i != blv_maps.end() ; i++ )
	{
		for( map<blv_t,int>::iterator j = i->begin() ; j != i->end() ; j++ )
		{
			vector<int> bvector;
			for( blv_t::const_iterator k = j->first.begin() ; k != j->first.end() ; k++ )
			{
				bvector.push_back(k->second);
				cout << k->second << " ";
			}

			cout << " : " << j->second << endl;	
			// ex poly = weight_from_chord(s,bvector,);
		}
		
	}

	return 0;
}