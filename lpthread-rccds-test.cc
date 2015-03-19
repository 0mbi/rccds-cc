#include "rccds.h"
#include <pthread.h>
#include <iostream>

typedef vector<chord_t> diagram_t;

unsigned int total = 0;

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
};

struct thread_data_t 
{
  set<int> A;
  diagram_t D;
  unsigned int total;
  TBLCounter tbl_counter;
} thread_data[(CHORDS*2-1)];




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
      data_ptr->tbl_counter.add( terminals, bvector );
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

  TBLCounter global;
  unsigned int total = 0;
  for( int i = 0 ; i < (CHORDS*2-1) ; i++ )
  {
    global.merge_from( thread_data[i].tbl_counter );
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