#ifndef RCCDS_H

#include <stdio.h>
#include <map>
#include <list>
#include <set>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>
#include "BinTree.h"

#include <string.h> 

using namespace std;
//using namespace jeschli;
using namespace markus;

/*
 * chord_t chord type
 */
  struct chord_t
  {
	  int first;
	  int second;
	  int id; 

	  friend bool operator==(chord_t &c1, chord_t &c2);
	  friend bool operator==(const chord_t &c1, const chord_t &c2);
  };

  chord_t mk_chord(int f,int s,int i);

  extern const chord_t EMPTY_CHORD; // = mk_chord(-1,-1,-1);

  typedef vector<chord_t> diagram_t;

  struct sort_vertex_pair 
  {
	  int *ptr;
	  int n;
  };

  bool svpSmaller( sort_vertex_pair , sort_vertex_pair );

  bool crosses( chord_t, chord_t );
  bool fast_is_connected( chord_t*, int );
  void fast_remove_1( chord_t *, chord_t *, int );
  void fast_get_connected_components( chord_t *, int *, chord_t *, int );
  void fast_vertex_normalize( chord_t *, int );
  void fast_rsd_decomp( chord_t* &, int* , chord_t* &, int* , int* , chord_t* , int );
  /*
  Following method needs to be called like this:
  int label = 0;
  int num_term = 0;
  vector<int> terminals(CHORDS-1);

  get_intersection_terminals( chord_array, CHORDS, chord_array, CHORDS, &label, &terminals, &num_term );
  */
  void get_intersection_terminals( chord_t *dest, int dest_size, chord_t* c, int size, int* label, vector<int> *terminals, int *num_term );

  typedef unsigned long long myint;
  void get_intersection_terminals_bitmask( chord_t   *dest, int dest_size, chord_t* c, int size, int *label, myint *bitmask );

  
  BinTree<int>* fast_build_int_tree( BinTreeManager<int>& , chord_t *, int );
  BinTree<int>* insert_at(BinTreeManager<int>&,  BinTree<int>* , BinTree<int>* , int );
  BinTree<int>* get_insertion_tree( BinTreeManager<int>&, BinTree<int>* );
  void get_ld_map( map<int,int> *, BinTree<int>*, int );

  void generate_diagrams( set<int> , diagram_t& , void *(*)(diagram_t*) );

  bool isSmaller( int i, int j);


  #ifndef CHORDS
    #define CHORDS 4
  #endif


#define RCCDS_H
#endif
