
#include "rccds.h"

bool operator==(chord_t &c1, chord_t &c2)
{
  return (c1.first==c2.first) && (c1.second==c2.second) && (c1.id==c2.id);
}

bool operator==(const chord_t &c1, const chord_t &c2)
{
  return (c1.first==c2.first) && (c1.second==c2.second) && (c1.id==c2.id);
}

chord_t mk_chord(int f,int s,int i)
{
  chord_t rval;
  rval.first = f;
  rval.second = s;
  rval.id = i;
  return rval;
}

const chord_t EMPTY_CHORD = mk_chord(-1,-1,-1);

bool isSmaller( int i, int j) { return i<j; }

int min( set<int> s ) 
{
  return *min_element( s.begin(), s.end() , isSmaller );
}

bool svpSmaller( sort_vertex_pair s1, sort_vertex_pair s2 )
{
  return (s1.n<s2.n);
}


bool crosses( chord_t x1, chord_t x2 )
{
  return ( (x1.first < x2.first) && (x2.first < x1.second) && (x1.second < x2.second) ) ||
    ( (x2.first < x1.first) && (x1.first < x2.second) && (x2.second < x1.second) );
}

/*
 * For fast processing the chord diagram should be a string
 * of size CHORD, it is assumed that it is this size! It also assumes
 * that bitsize(int) > CHORD > 1!
 */
bool fast_is_connected( chord_t* d, int size ) 
{
  int * disj_sets = new int [size]();
  disj_sets[0] = 1;

  for( int v = 1 ; v < size ; v++ )
    { 
      int marked_sets = 0;
      
      for( int s = 0 ; disj_sets[s] != 0 && s < size ; s++ )
        {
          for( int w = 0 ; w <= v ; w++ )
            {
              int W = disj_sets[s] & ( 1 << w ); // test if the bit w
                                                 // is set
              if( !W ) continue;

              if( /*markus::*/crosses( d[v], d[w] ) )
                {
                  marked_sets |= ( 1 << s );
                  break;
                }
            }
        }
      if( marked_sets == 0 )
        {
          int s;
          for( s = 0 ; disj_sets[s] != 0 ; s++ ) {};
          disj_sets[s] = (1 << v);
        }
      else 
        {
          int pos;
          int min;
          bool first = true;

          for( pos = 0 ; pos <= v ; pos++ )
            {
              if( marked_sets & (1<<pos) )
                {
                  if( first == true ) 
                    {
                      min = pos;
                      first = false;
                      continue;
                    }
                  disj_sets[min] |= disj_sets[pos];
                  disj_sets[pos] = 0;
                }
            }
          disj_sets[min] |= (1 << v);
        }
    }
  int s = 0;
  for( int k = 0 ; k < size ; k++ )
    {
      if( disj_sets[k] != 0 ) s++;
     }

  delete [] disj_sets;
  if( s == 1 ) return true;
  return false;
}


/*
  Removes chord 1 from diagram and normalizes the labels but not the ids!
  Requires a chord_t string of length N, dest must already be
  allocated with N-1. Writes the resulting chord string to dest
*/ 
void fast_remove_1( chord_t *dest, chord_t *src, int N )
{
  memcpy( dest, src+1, (N-1)*sizeof(chord_t) );
}  
  
/* fast_get_connected_components(chord_t*,int*,chord_t*,int=)
 * In: src, size
 * src - array of chord_t of size
 * Out: num, dest
 * num - number of connected component ; this is not accurate if you set a bigger size than your chord diagram is actually is
 * dest - array of chord_t of size*size ; must be allocated before!
 */
void fast_get_connected_components( chord_t *dest, int *num, chord_t *src, int size )
{
  //  int disj_sets[size] = { 0 };
  int *disj_sets = new int[size]();
  disj_sets[0] = 1;
  for( int v = 1 ; v < size ; v++ )
    { 
      int marked_sets = 0;
      for( int s = 0 ; disj_sets[s] != 0 && s < size ; s++ )
        {
          for( int w = 0 ; w <= v ; w++ )
            {
              int W = disj_sets[s] & ( 1 << w ); // test if the bit w
                                                 // is set
              if( !W ) continue;

              if( /*markus::*/crosses( src[v], src[w] ) )
                {
                  marked_sets |= ( 1 << s );
                  break;
                }
            }
        }
      if( marked_sets == 0 )
        {
          int s;
          for( s = 0 ; disj_sets[s] != 0 ; s++ ) {};
          disj_sets[s] = (1 << v);
        }
      else 
        {
          int pos;
          int min;
          bool first = true;
          for( pos = 0 ; pos <= v ; pos++ )
            {
              if( marked_sets & (1<<pos) )
                {
                  if( first == true ) 
                    {
                      min = pos;
                      first = false;
                      continue;
                    }
                  disj_sets[min] |= disj_sets[pos];
                  disj_sets[pos] = 0;
                }
            }
          disj_sets[min] |= (1 << v);
        }
    }
  int s = 0;
  for( int k = 0 ; k < size ; k++ )
    {
      if( disj_sets[k] != 0 ) 
        {
          int t = 0;
          for( int m = 0 ; m < size ; m++ )
            {
              if( (disj_sets[k] & (1 << m)) )
                {
                  dest[ s*size + t] = src[m];
                  t++;
                }              
            }
          if( t < size ) dest[ s*size + t ] = mk_chord(-1,-1,-1);
          s++;
        }      
    }
  if( s < size ) dest[ s*size ] = mk_chord(-1,-1,-1);
  *num = s;

  delete [] disj_sets;
}

void fast_vertex_normalize( chord_t *src, int size )
{
  //  sort_vertex_pair * svp = new sort_vertex_pair[size*2];
  vector<sort_vertex_pair> svp(2*size);
  for( int i = 0 ; i < 2*size ; i++ )
    {
      if( i%2 )
        {
          svp[i].ptr = &src[i/2].second;
          svp[i].n = src[i/2].second;
        }
      else
        {
          svp[i].ptr = &src[i/2].first;
          svp[i].n = src[i/2].first;
        }
    }
  sort( svp.begin() , svp.end() , svpSmaller );
  for( int i = 0 ; i < 2*size ; i++ )
    {
      *svp[i].ptr = i+1;
    }
}

/*
 * fast_rsd_decomp
 */
void fast_rsd_decomp( chord_t* &dest1, int* size1, chord_t* &dest2, int* size2, int* index, chord_t* src, int src_size )
{
  if( src_size == 1 )
    {
      dest1 = src;
      *size1 = 1;
      dest2 = NULL;
      *size2 = 0;
      return;
    }
  chord_t* src_rem1 = new chord_t [src_size-1];
  chord_t* comp = new chord_t[(src_size-1)*(src_size-1)];
  memset(comp,0,(src_size-1)*(src_size-1)*sizeof(chord_t));
  int comp_num = 0;
  fast_remove_1( src_rem1, src, src_size );
  fast_get_connected_components(comp,&comp_num,src_rem1,src_size-1);
  if( comp_num == 1 )
    {
      dest1 = new chord_t;
      memcpy( dest1, src, sizeof(chord_t));
      *size1 = 1;
      dest2 = new chord_t [src_size-1];
      memcpy( dest2, src_rem1, sizeof(chord_t)*(src_size-1)) ;
      *size2 = src_size-1;
      *index = dest1[0].second - 2;
    }
  else
    {
      int comp_num = 1;
      chord_t * temp_dest1 = new chord_t[src_size-1]();
      chord_t * temp_dest2 = new chord_t[src_size-1]();
      int size_1 = 0;
      int size_2 = 0;
      temp_dest1[size_1++] = src[0];
      for( int i = 0 ; i < (src_size-1)*(src_size-1) ; i++ )
	     {
	       if( (i%(src_size-1))==0 && comp[i] == EMPTY_CHORD )
	         break;
	       if( (i%(src_size-1))==0 );
	       if( comp[i] == EMPTY_CHORD ) {
	       continue;
	       }
	       if( comp[i].id == 0 ) continue;
	       if( (i/(src_size-1)) == 0 )
	       {
  	      temp_dest2[size_2++] = comp[i];
	       }
	       else
	       {
	         temp_dest1[size_1++] = comp[i];
	       }
	     }
      dest1 = new chord_t[size_1];
      dest2 = new chord_t[size_2];

      memcpy( dest1, temp_dest1, sizeof(chord_t)*size_1 );
      memcpy( dest2, temp_dest2, sizeof(chord_t)*size_2 );

      *size1 = size_1;
      *size2 = size_2;
      
      int y1 = dest1[0].second;
      int x2 = dest1[1].first;
      int min = (y1<x2)?y1:x2;
      
      *index = min - 2;

      
      delete [] temp_dest1;
      delete [] temp_dest2;

      
    }
  delete [] src_rem1;  
  delete [] comp;
}

// this is quite crappy and depends that fast_rem1 and fast_get_connected_components do not call by reference but by value
void fast_modify_to_intersection_order( chord_t *dest, int dest_size, chord_t* c, int size, int *label, int *terminals, int *num_term )
{
  if( c == NULL ) return;
  // find the right chord to modify the label
  for( int i = 0 ; i < dest_size ; i++ )
    {
      if( dest[i].first == c->first && dest[i].second == c->second )
	{
	  dest[i].id = *label;
	  (*label)++;
	  break;
	}
    }
 // the last chord in intersection order of a connected component is 
 // always a terminal chord
  if( size == 1 ) {
    terminals[(*num_term)++] = (*label)-1; // label already increased before
    return;
  }
  chord_t d[CHORDS] = {0};
  chord_t conn_matrix[(CHORDS-1)*(CHORDS-1)] = {0};
  chord_t* comps[CHORDS] = {0};
  int comp_sizes[CHORDS] = {0};

  fast_remove_1(d,c,size);
  int num_comp;
  fast_get_connected_components( conn_matrix, &num_comp, d, size-1 );
  // parsing through connected components

  if( num_comp == 1 )
    {
      fast_modify_to_intersection_order(dest,dest_size,d, size-1, label,terminals,num_term);
    }
  
  else
    {

      for( int i = 0 ; i < num_comp ; i++ )
	{
	  comps[i] = new chord_t [size];
	}
      int actual_comp = 0;
      int actual_size = 0;

      for( int i = 0 ; i < (size-1)*(size-1) ; i++ )
	{
	  if( (i%(size))==0 && conn_matrix[i].id == -1 )
	    {      
	      comp_sizes[actual_comp] = actual_size;
	      break;
	    }
	  if( conn_matrix[i].id == -1 )
	    {
	      comp_sizes[actual_comp] = actual_size;
	      actual_comp++;
	      actual_size = 0;
	      continue;
	    }
	  if( conn_matrix[i].id == 0 ) continue;
	  comps[actual_comp][actual_size] = conn_matrix[i];
	  actual_size++;
	}


      for( int i = 0 ; i < num_comp ; i++ )
	{
	  if( comp_sizes[i] == 0 ) 
		  {
			  printf("should not happen - but happens from time to time %d ?!\n",size);
			  //  terminals[(*num_term)++] = CHORDS; // label already increased before
			  continue; // should not happen - but happens from time to time ?!
		  }
	  fast_modify_to_intersection_order( dest, dest_size, comps[i], comp_sizes[i], label,terminals,num_term );
	}
    }  
}


BinTree<int>* fast_build_int_tree( BinTreeManager<int>& btm, chord_t *src, int size )
{
  chord_t *first = NULL; chord_t *second = NULL;
  int size1 = 0; int size2 = 0;
  int index = 0;
  fast_vertex_normalize(src,size); // vertex normalize
  fast_rsd_decomp(first,&size1,second,&size2,&index,src,size);
  BinTree<int>* iroot = btm.mk_BinTree_ptr(NULL,NULL,NULL,index);
  BinTree<int>* lnode; 
  BinTree<int>* rnode; 
  if( size1 == 1 ) {
    lnode = btm.mk_BinTree_ptr(NULL,NULL,NULL,first[0].id);
  }
  else {
    lnode = fast_build_int_tree(btm,first,size1);
  }
  if( size2 == 1 ) {
    rnode = btm.mk_BinTree_ptr(NULL,NULL,NULL,second[0].id);
  }
  else { 
    rnode = fast_build_int_tree(btm,second,size2);
  }

  lnode->parent = iroot; rnode->parent = iroot;
  iroot->left = lnode; iroot->right = rnode;

// cleanup the memory allocated by fast_rsd_decomp
  if( first != NULL )
  {
    if( size1 == 1 ) 
      delete first;
    else {
      delete [] first;
      first = NULL;
    }
  }
  if( second != NULL )
  {

    // if( size2 == 1) // leads to invalid free/delete?!
    // {
    //   delete second;
    // }
    // else 
      delete [] second;
    second = NULL;
  }
  return iroot;
}


BinTree<int>* insert_at( BinTreeManager<int>& btm, BinTree<int>* left, BinTree<int>* right, int n )
{
  // copy right to rval
  // debugging
  cout << "Insert: " ;
  // cout << "Right node: " << right << endl;
  BinTree<int>* rval = btm.copy(right);
   // BinTree<int>* rval = right;
 
  // run insert_at_nth_pre(rval,left,n)
  insert_at_nth_pre(btm,&rval,left,n,0);

  return rval;
}


BinTree<int>* get_insertion_tree( BinTreeManager<int>& btm, BinTree<int>* node )
{
  if( node == NULL )
    return NULL;
  if( isLeaf(node) ) {
   return btm.copy(node);
  }
  return insert_at(btm,
       get_insertion_tree(btm,node->left),
       get_insertion_tree(btm,node->right),
       node->data);
}


void get_intersection_terminals( chord_t *dest, int dest_size, chord_t* c, int size, int* label, vector<int> *terminals, int *num_term )
{
    if( size == 1 )
    {

        *label += 1;
        // fast_connected_component returns copies no references, we have to find the right reference manually :(
        for( int i = 0 ; i < dest_size ; i++ )
        {
            if( dest[i].first==c->first && dest[i].second==c->second)
            {
                dest[i].id = *label;
                (*terminals)[*num_term] = *label;
                *num_term += 1;
                break;
            }
        }

        return;
    }
    else
    {
        chord_t d[CHORDS] = {0};
        chord_t conn_matrix[(CHORDS-1)*(CHORDS-1)] = {0};
        chord_t* comps[CHORDS] = {0};
        int comp_sizes[CHORDS] = {0};
        int num_comp;

        *label += 1;

        for( int i = 0 ; i < dest_size ; i++ )
        {
            if( dest[i].first==c[0].first && dest[i].second==c[0].second )
            {
                dest[i].id = *label;
                break;
            }
        }
        fast_remove_1(d,c,size);
        fast_get_connected_components( conn_matrix, &num_comp, d, size-1 );
        for( int i = 0 ; i < num_comp ; i++ )
        {
            int mul = i*(size-1);
            if( conn_matrix[mul].id == -1 )
                break;
            int actual_size = 0;
            for( int j = 0 ; j < size-1 ; j++ )
            {
                if( conn_matrix[mul+j].id == -1 )
                    break;
                actual_size++;
            }
            get_intersection_terminals(dest,dest_size,&conn_matrix[mul], actual_size,label,terminals,num_term);
        }

    }
}