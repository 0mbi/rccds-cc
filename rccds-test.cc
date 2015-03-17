#include "rccds.h"
#include <iostream>

typedef vector<chord_t> diagram_t;

void test_chord4() {
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
		BinTree<int>* int_tree = fast_build_int_tree(d,4);
		BinTree<int>* ins_tree = get_insertion_tree(int_tree);
		print_pre_BinTree<int>( ins_tree );
		delete_BinTree<int>(int_tree);
		delete_BinTree<int>(ins_tree);
	}	
}

int total = 0;


void generate_diagrams( set<int> avail, diagram_t& temp_diagram, void *(*callback_fun)(diagram_t*) )
{
  if( avail.empty() )      // new diagram found, add it
    {
      (*callback_fun)(&temp_diagram);
      return;
    }

  set<int>::iterator min = min_element( avail.begin(), avail.end() , isSmaller );
  set<int>::iterator max = max_element( avail.begin(), avail.end() , isSmaller );
  for( int i = *(min)+2 ; i <= *max ; i++ )
    {
      if( avail.find(i) == avail.end() )
        {
          continue;
        }
      set<int> avail_new = avail;
      avail_new.erase(*min);
      avail_new.erase(i);
      diagram_t temp_diagram_new = temp_diagram;
      chord_t c;
      c.first= *min;
      c.second = i;
      temp_diagram_new.push_back( c );
      //      temp_diagram_new.push_back( pair<int,int>(*min,i) );
      generate_diagrams( avail_new, temp_diagram_new, callback_fun );
    }  
}

void *proc_diags( diagram_t* per )
{

  chord_t chord_array[CHORDS];
  int k = 1;
  
  for( diagram_t::iterator j = per->begin() ; j != per->end() ; j++ )
    {
      chord_t chord = *j;
      chord.id = k;
      chord_array[k-1] = chord;
      k++;
    }

  if( fast_is_connected(chord_array,CHORDS) )
	  {
		int label = 0;
	  	int num_term = 0;
	  	vector<int> terminals(CHORDS-1);
	  	get_intersection_terminals(chord_array,CHORDS,chord_array,CHORDS,&label,&terminals,&num_term);		  
	  	total++;
      	BinTree<int>* int_tree = fast_build_int_tree( chord_array, CHORDS );
      	// printf("\n\t");
      	// print_pre_BinTree<int>(int_tree);
      	// printf("\n\t");
      	BinTree<int>* ins_tree = get_insertion_tree(int_tree);
      	// print_pre_BinTree<int>(ins_tree);
		delete_BinTree<int>(int_tree);
		delete_BinTree<int>(ins_tree);
      	// printf("\n");
	  }

  return NULL;
}



int main(int argc, char const *argv[])
{
  struct timespec tstart={0,0}, tend={0,0};
  clock_gettime(CLOCK_MONOTONIC, &tstart);
  
  set<int> avail;
  for( int i = 1 ; i <= 2*CHORDS ; i++ ) avail.insert(i);
  diagram_t d;
  generate_diagrams(avail,d,&proc_diags);
  printf("\nNum rcds: %d\n",total);

  clock_gettime(CLOCK_MONOTONIC, &tend);
  printf("some_long_computation took about %.5f seconds\n",
         ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
         ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

	return 0;
}