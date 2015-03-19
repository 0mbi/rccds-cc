
#include "BinTree.h"

using namespace markus;

BinTree<int>* insert_at( BinTree<int>* left, BinTree<int>* right, int n )
{
  // copy right to rval
  // debugging
  cout << "Insert: " ;
  // cout << "Right node: " << right << endl;
  BinTree<int>* rval = getBinTree<int>::copy(right);
   // BinTree<int>* rval = right;
  print_pre_BinTree<int>( left );
  cout << " at " << n << " into " ;
  print_pre_BinTree<int>( rval );
  cout << endl;
 
  // run insert_at_nth_pre(rval,left,n)
  insert_at_nth_pre(&rval,left,n,0);
  cout << "Results in ";
    print_pre_BinTree<int>( rval );
  cout << endl;

  return rval;
}


int main(int argc, char const *argv[])
{
	BinTree<int>* r = mk_BinTree_ptr<int>(NULL,NULL,NULL,0);
	BinTree<int>* l = mk_BinTree_ptr<int>(NULL,NULL,r,8);
	BinTree<int>* ri = mk_BinTree_ptr<int>(NULL,NULL,r,15);
	cout << "r: " << r << endl;
	cout << "l: " << l << endl;
	cout << "ri: " << ri << endl;
	r->left = l;
	l->parent = r;
	r->right = ri;
	ri->parent = r;
	BinTree<int> *r2 = getBinTree<int>::copy(r);
	BinTree<int> *r3 = insert_at( r, r2, 2 );
	delete_BinTree<int>(r);
	delete_BinTree<int>(r2);
	delete_BinTree<int>(r3);
	return 0;
}