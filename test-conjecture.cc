#include "rccds.h"
#include <pthread.h>
#include <iostream>

typedef vector<chord_t> diagram_t;

unsigned int total = 0;

typedef vector<int> terminal_t;
typedef map<int,int> blv_t;
typedef pair<blv_t,int> blv_int_pair;
typedef map< terminal_t, int > tbl_map_t;
typedef pair< terminal_t, int > tbl_pair;

class LastCounter {
  private:
    tbl_map_t last_deg_map;
    tbl_map_t last_bleft_map;
  public:
    void add( terminal_t terminals, int last_bleft, int last_deg )
    {
      tbl_map_t::iterator last_deg_finder = last_deg_map.find( terminals );
      if( last_deg_finder == last_deg_map.end() )
      {
        last_deg_map.insert( tbl_pair(terminals,last_deg) );
      }
      else
      {
        last_deg_finder->second += last_deg;
      }
      tbl_map_t::iterator last_bleft_finder = last_bleft_map.find( terminals );
      if( last_bleft_finder == last_bleft_map.end() )
      {
        last_bleft_map.insert( tbl_pair(terminals,last_bleft) );
      }
      else
      {
        last_bleft_finder->second += last_bleft;
      }

    }
    void out()
    {
      bool conjecture = true;
      for( tbl_map_t::const_iterator i = this->last_deg_map.begin() ; i != this->last_deg_map.end() ; i++ )
      {
        if( this->last_deg_map[i->first] != this->last_bleft_map[i->first] )
        {
          cout << "Conjecture fails for ";
          for( vector<int>::const_iterator j = i->first.begin() ; j != i->first.end() ; j++ )
          {
            cout << *j << " ";
          }
          cout << endl;
          conjecture = false;
        }
      }
      if( conjecture ) {
        cout << "Conjecture holds for all chord diagrams of size " << CHORDS << endl;
      }
    }
    void merge_from( LastCounter& lc )
    {
      for( tbl_map_t::const_iterator i = lc.last_deg_map.begin() ; i != lc.last_deg_map.end() ; i++ )
      {
        tbl_map_t::iterator last_deg_finder = this->last_deg_map.find( i->first );
        if( last_deg_finder == last_deg_map.end() )
        {
          this->last_deg_map.insert( tbl_pair(i->first,i->second) );
        }
        else
        {
          last_deg_finder->second += i->second;
        }
      }
      for( tbl_map_t::const_iterator i = lc.last_bleft_map.begin() ; i != lc.last_bleft_map.end() ; i++ )
      {
        tbl_map_t::iterator last_bleft_finder = this->last_bleft_map.find( i->first );
        if( last_bleft_finder == last_bleft_map.end() )
        {
          this->last_bleft_map.insert( tbl_pair(i->first,i->second) );
        }
        else
        {
          last_bleft_finder->second += i->second;
        }
      }      
    }
};

struct thread_data_t 
{
  set<int> A;
  diagram_t D;
  unsigned int total;
  LastCounter lc_counter;
} thread_data[(CHORDS*2-1)];

int last_degree( chord_t* chord_array, int chord_size ) {
  chord_t* highest_terminal_chord = &chord_array[0];
  int rval = 0;
  for( int i = 0 ; i < chord_size ; i++ ) {
    if( chord_array[i].id > highest_terminal_chord->id ) 
      highest_terminal_chord= &chord_array[i];
  }
  for( int i = 0 ; i < chord_size ; i++ ) {
    if( crosses(*highest_terminal_chord,chord_array[i]) ) {
      rval++;
    }
  }
  return rval;
}


void *proc_diags( diagram_t* per,void* ptr )
{
  thread_data_t* data_ptr = (thread_data_t*) ptr;
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
	  	data_ptr->total++;
      BinTreeManager<int> btm;
    	BinTree<int>* int_tree = fast_build_int_tree( btm, chord_array, CHORDS );
     	BinTree<int>* ins_tree = get_insertion_tree( btm , int_tree);
      map<int,int> bvector;
      get_ld_map( &bvector, ins_tree, 0 );
      int last_deg = last_degree(chord_array,CHORDS);
      int last_bleft = bvector[CHORDS];
      data_ptr->lc_counter.add(terminals,last_bleft,last_deg);
	  }

  return NULL;
}


void generate_diagrams_mthread( set<int> avail, diagram_t& temp_diagram, void *(*callback_fun)(diagram_t*,void*), void* ptr )
{
  if( avail.empty() )      // new diagram found, add it
    {
    (*callback_fun)(&temp_diagram,ptr);
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
      generate_diagrams_mthread( avail_new, temp_diagram_new, callback_fun, ptr );
    }  

  
}



void *process_fun( void * ptr )
{
  thread_data_t* data = (thread_data_t*) ptr;
  generate_diagrams_mthread(data->A,data->D,&proc_diags,ptr);
  return NULL;
}


int main(int argc, char const *argv[])
{
  struct timespec tstart={0,0}, tend={0,0};
  clock_gettime(CLOCK_MONOTONIC, &tstart);
 
  set<int> avail;
  for( int i = 2 ; i <= 2*CHORDS ; i++ ) avail.insert(i);
  int k = 0;
  for( int i = 3 ; i <= (2*CHORDS - 1) ; i++ )
    {
      set<int> x = avail;
      x.erase(i);
      diagram_t d;
      chord_t first_chord = mk_chord(1,i,1);
      d.push_back(first_chord); 
      thread_data[k].A = x;
      thread_data[k].D = d;
      thread_data[k].total= 0;
      k++;
    }

  pthread_t threads[(CHORDS*2-1)];
  for( int i = 0 ; i < (CHORDS*2-1) ; i++ )
    pthread_create( &threads[i],NULL,process_fun,(void*)&thread_data[i] );

  for( int i = 0 ; i < (CHORDS*2-1) ; i++ )
    pthread_join( threads[i], NULL );

  LastCounter global;
  unsigned int total = 0;
  for( int i = 0 ; i < (CHORDS*2-1) ; i++ )
  {
    global.merge_from( thread_data[i].lc_counter );
    total += thread_data[i].total;
  }
  global.out();
  cout << "Total connected chord diagrams of size " << CHORDS <<  ": " << total << endl;
 

  clock_gettime(CLOCK_MONOTONIC, &tend);
  printf("computation took about %.2f seconds\n",
         ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
         ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

	return 0;
}