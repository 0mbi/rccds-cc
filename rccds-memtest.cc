#include "rccds.h"
#include <iostream>

typedef vector<chord_t> diagram_t;
int main(int argc, char const *argv[])
{
  struct timespec tstart={0,0}, tend={0,0};
  clock_gettime(CLOCK_MONOTONIC, &tstart);
  
  chord_t d[4];
  d[0] = mk_chord(1,5,0);
  d[1] = mk_chord(2,8,0);
  d[2] = mk_chord(3,6,0);
  d[3] = mk_chord(4,7,0);
  if( fast_is_connected(d,4) )
  {
    cout << "is connected" << endl;
    vector<int> terminals(4-1);
    int num_terminals = 0;
    int label = 0;
    get_intersection_terminals(d,4,d,4,&label,&terminals,&num_terminals);
    for( int i = 0 ; i < 4 ; i++ )
    {
      printf("%d %d %d\n", d[i].first, d[i].second, d[i].id);
    }
    int size1 = 0; int size2 = 0;
    int index = 0;
    BinTreeManager<int> btm;
    BinTree<int>* int_tree = fast_build_int_tree(btm,d,4);
    BinTree<int>* ins_tree = get_insertion_tree(btm,int_tree);
    print_pre_BinTree<int>( int_tree );
    print_pre_BinTree<int>( ins_tree );

  }  
  cout << endl;

  clock_gettime(CLOCK_MONOTONIC, &tend);
  printf("some_long_computation took about %.5f seconds\n",
         ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
         ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

	return 0;
}