
#include "BinTree.h"

using namespace markus;

int main(int argc, char const *argv[])
{
	BinTreeManager<int> btm;
	BinTree<int>* r = btm.mk_BinTree_ptr(NULL,NULL,NULL,0);
	BinTree<int>* l = btm.mk_BinTree_ptr(NULL,NULL,r,8);
	BinTree<int>* ri = btm.mk_BinTree_ptr(NULL,NULL,r,15);
	cout << "r: " << r << endl;
	cout << "l: " << l << endl;
	cout << "ri: " << ri << endl;
	r->left = l;
	l->parent = r;
	r->right = ri;
	ri->parent = r;
	BinTree<int> *r2 = btm.copy(r);
	return 0;
}