
#ifndef BINTREE_H

#include <iostream>
#include <deque>
#include <climits>
#include <vector>

using namespace std;

namespace markus {

template <class T>
  struct BinTree {
    BinTree<T> *left;
    BinTree<T> *right;
    BinTree<T> *parent;
    T data;
  };

template <class T>
  BinTree<T> *mk_BinTree_ptr( BinTree<T> *left, BinTree<T> *right, BinTree<T> *parent, T data_)
  {
    BinTree<T> *node = new BinTree<T>;
    node->left = left;
    node->right = right;
    node->parent = parent;
    node->data = data_;
    return node;
  }

template <class T>
  BinTree<T> *insert_left_BinTree( BinTree<T> *t, BinTree<T> *s )
  {
    t->left = s;
    s->parent = t;
    return s;
  }

template <class T>
  BinTree<T> *insert_right_BinTree( BinTree<T> *t, BinTree<T> *s )
  {
    t->right = s;
    s->parent = t;
    return s;
  }

template <class T>
  class getBinTree 
  {
    static int pre_label;
    static void pre_nth_helper(BinTree<T> *node, int nth, BinTree<T> **target)
    {
      if( node == NULL || pre_label > nth) return;
      if( pre_label == nth ) { 
       *target = node;
       pre_label++;
       return;
     }
     pre_label++;
     pre_nth_helper(node->left,nth,target);
     pre_nth_helper(node->right,nth,target);
   }

 public:
  static BinTree<T>* pre_nth(BinTree<T> *root_, int nth)
  {
    getBinTree::pre_label = 1;
    BinTree<T> *rval = new BinTree<T>;
    getBinTree::pre_nth_helper(root_,nth,&rval);
    return rval;
  }

  static bool isLeaf(BinTree<T> *root)
  {
    return ( root->left == NULL && root->right == NULL);
  }

  static BinTree<T> *copy( BinTree<T> *original_node )
  {
   BinTree<T> *rval;
   if( original_node != NULL ) {
     rval = mk_BinTree_ptr<T>(NULL,NULL,NULL,original_node->data);
     rval->left = copy(original_node->left);
     if( rval->left != NULL )
      rval->left->parent = rval;
    rval->right = copy(original_node->right);
    if( rval->right != NULL )
      rval->right->parent = rval;
  } else return NULL;
  return rval;
}
};

template<class T>
int getBinTree<T>::pre_label = 1;

template<class T>
void insert_at_nth_pre( BinTree<T> **target_root, BinTree<T> *source_root, int nth, T insert_data )
{
  BinTree<T> *nth_element = getBinTree<T>::pre_nth(*target_root,nth);
  BinTree<T> *new_node = mk_BinTree_ptr<T>(NULL,NULL,NULL,insert_data);
  new_node->left = source_root;
  new_node->right = nth_element;

  // reassign the parents node: make adoption
  
  
  if( nth_element->parent == NULL ) 
  {
      // the nth_element is a root, so only insertion 1 is allowed and the nth_element goes right
    if( nth != 1 ) return;
    nth_element->parent = new_node;
    *target_root = new_node;
  }
  else
  {
      // was the nth element the left or the right node?
    if( nth_element->parent->left == nth_element )
    {
     nth_element->parent->left = new_node;
     nth_element->parent = new_node;
   }
   else
   {
     nth_element->parent->right = new_node;
     nth_element->parent = new_node;
   }
 }
}

 template<class T>
void print_pre_BinTree( BinTree<T> *root )
{
 if( root == NULL ) return;
 if( root->left != NULL || root->right != NULL )
 {
  cout << " [";
}

cout << root->data ;
cout << "";

if( root->left != NULL || root->right != NULL )
{
  cout << " ";
  print_pre_BinTree( root->left );
  cout << ",";
  print_pre_BinTree( root->right );
  cout << "]";
}    
}


 template<class T>
void pikz_BinTree( BinTree<T> *root, bool start = false )
{
  if( root == NULL ) return;
  if( start ) {
    cout << "\\coordinate\n";
  } else
  if( root->left != NULL || root->right != NULL )
  {
    cout << "child{";
  }

  if( root->data != 0 )
  {
    cout << "child{node{";
    cout << root->data ;
    cout << "}}";
  }
  cout << "";

  if( root->left != NULL || root->right != NULL )
  {
    cout << " ";
    pikz_BinTree( root->left );
    cout << "";
    pikz_BinTree( root->right );
    if( start )
      cout << ";";
    else
      cout << "}";
  }    


}


template <class T>
void delete_BinTree( BinTree<T> * root ) {
  if( (root == NULL) || (root == NULL) )
    return;
  if( root->left != NULL || root->right != NULL ) {
    if( root->left != NULL ) 
      root->left->parent = NULL;
    if( root->right != NULL ) 
      root->right->parent = NULL;
    if( root->parent != NULL ) 
    {
      delete root->parent;
      root->parent = NULL;
    }     
    BinTree<T> * left = root->left;
    BinTree<T> * right = root->right;
    delete root;
    root = NULL;

    if( left != NULL )
      delete_BinTree(left);
    if( right != NULL )
      delete_BinTree(right);
  }

}


}

#define BINTREE_H
#endif
