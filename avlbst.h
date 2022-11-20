#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void rotateRight( AVLNode<Key,Value>* root);
		void rotateLeft( AVLNode<Key,Value>* root);
		void insertFix( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
		void removeFix( AVLNode<Key,Value>* n, int diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		if(BinarySearchTree<Key, Value>::root_==NULL){
			BinarySearchTree<Key, Value>::root_=new AVLNode<Key,Value>(new_item.first,new_item.second,NULL);
		}
		else{
			AVLNode<Key,Value>* root=static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
			while(root){
        if(new_item.first < root->getKey()){
            if(root->getLeft()){
            root=root->getLeft();
            }
            else{
                root->setLeft(new AVLNode<Key,Value>(new_item.first,new_item.second,root));
								root->updateBalance(-1);
								if(root->getBalance()==-1){
									insertFix(root, root->getLeft());
								}
                return;
            }
        }
        else if (new_item.first > root->getKey()){
            if(root->getRight()){
            root=root->getRight();
            }
            else{
                root->setRight(new AVLNode<Key,Value>(new_item.first,new_item.second,root));
								root->updateBalance(1);
								if(root->getBalance()==1){
									insertFix(root, root->getRight());
								}
                return;
            }
        }
        else{
            root->setValue(new_item.second);
            return;
        }
			}
		}


}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
		AVLNode<Key,Value>* root=static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
		while(root){
			if(key < root->getKey()){
					root=root->getLeft();
			}
			else if (key > root->getKey()){
					root=root->getRight();
			}
			else{
					int diff=0;
					if(root->getLeft() && root->getRight()){
							nodeSwap(root,static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(root)));
					}
					AVLNode<Key, Value>* child=NULL;
					if(root->getLeft()){
							child=root->getLeft();
					}
					else if(root->getRight()){
							child=root->getRight();
					}
					if(root->getParent()){
							if(root->getParent()->getLeft()==root){
									root->getParent()->setLeft(child);
									diff=1;
							}
							else{
									root->getParent()->setRight(child);
									diff=-1;
							}
							if(child!=NULL){
							child->setParent(root->getParent());
							}
					}
					else{
							BinarySearchTree<Key, Value>::root_=child;
							if(child!=NULL){
								child->setParent(NULL);
							}
					}
					AVLNode<Key, Value>* parent=root->getParent();
					delete root;
					//std::cout<<"remove"<<std::endl;
					removeFix(parent,diff);
					return;
			}
		}


}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight( AVLNode<Key,Value>* root)
{
AVLNode<Key,Value>* temp=root->getLeft();
if(root==BinarySearchTree<Key, Value>::root_){
	BinarySearchTree<Key, Value>::root_=temp;
}
else if (root->getParent()->getLeft()==root){
	root->getParent()->setLeft(temp);
}
else{
	root->getParent()->setRight(temp);
}
root->setLeft(temp->getRight());

if(root->getLeft()){
root->getLeft()->setParent(root);
}
temp->setRight(root);
temp->setParent(root->getParent());
root->setParent(temp);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft( AVLNode<Key,Value>* root)
{
AVLNode<Key,Value>* temp=root->getRight();
if(root==BinarySearchTree<Key, Value>::root_){
	BinarySearchTree<Key, Value>::root_=temp;
}
else if (root->getParent()->getLeft()==root){
	root->getParent()->setLeft(temp);
}
else{
	root->getParent()->setRight(temp);
}
root->setRight(temp->getLeft());
if(root->getRight()){
root->getRight()->setParent(root);
}
temp->setLeft(root);
temp->setParent(root->getParent());
root->setParent(temp);

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
	if(p==NULL || p->getParent()==NULL){
		return;
	}
	AVLNode<Key,Value>* g=p->getParent();
	if(p==g->getLeft()){
		g->updateBalance(-1);
		if(g->getBalance()==0){
			return;
		}
		else if(g->getBalance()==-1)
		{
			insertFix(g,p);
		}
		else{
			if(n->getKey()<p->getKey()){
				rotateRight(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else{
				rotateLeft(p);
				rotateRight(g);
				if(n->getBalance()==-1){
					n->setBalance(0);
					p->setBalance(0);
					g->setBalance(1);
				}
				else if(n->getBalance()==0){
					p->setBalance(0);
					g->setBalance(0);
				}
				else{
					n->setBalance(0);
					p->setBalance(-1);
					g->setBalance(0);
				}
			}
		}
	}
	else{
		g->updateBalance(1);
		if(g->getBalance()==0){
			return;
		}
		else if(g->getBalance()==1)
		{
			insertFix(g,p);
		}
		else{
			if(n->getKey()>p->getKey()){
				rotateLeft(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else{
				rotateRight(p);
				rotateLeft(g);
				if(n->getBalance()==1){
					n->setBalance(0);
					p->setBalance(0);
					g->setBalance(-1);
				}
				else if(n->getBalance()==0){
					p->setBalance(0);
					g->setBalance(0);
				}
				else{
					n->setBalance(0);
					p->setBalance(1);
					g->setBalance(0);
				}
			}
		}
	}


}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix( AVLNode<Key,Value>* n, int diff){
if(n==NULL){
	return;
}
AVLNode<Key,Value>* p=n->getParent();
int ndiff=0;
if(p && p->getLeft()==n){
	ndiff=1;
}
else if(p){
	ndiff=-1;
}
if(diff==-1){
	if(n->getBalance()+diff==-2){
		AVLNode<Key,Value>* c=n->getLeft();
		if(c->getBalance()==-1){
			rotateRight(n);
			n->setBalance(0);
			c->setBalance(0);
			removeFix(p,ndiff);
		}
		else if(c->getBalance()==0){
			rotateRight(n);
			n->setBalance(-1);
			c->setBalance(1);
		}
		else{
			AVLNode<Key,Value>* g=c->getRight();
			rotateLeft(c);
			rotateRight(n);
			if(g->getBalance()==1){
				n->setBalance(0);
				c->setBalance(-1);
				g->setBalance(0);
			}
			else if(g->getBalance()==0){
				n->setBalance(0);
				c->setBalance(0);
			}
			else{
				n->setBalance(1);
				c->setBalance(0);
				g->setBalance(0);
			}
			removeFix(p,ndiff);
		}
	}
	else if(n->getBalance()+diff==-1){
		n->setBalance(-1);
	}
	else if(n->getBalance()+diff==0){
		n->setBalance(0);
		removeFix(p,ndiff);
	}
}
else if(diff==1){
	if(n->getBalance()+diff==2){
		AVLNode<Key,Value>* c=n->getRight();
		if(c->getBalance()==1){
			rotateLeft(n);
			n->setBalance(0);
			c->setBalance(0);
			removeFix(p,ndiff);
		}
		else if(c->getBalance()==0){
			rotateLeft(n);
			n->setBalance(1);
			c->setBalance(-1);
		}
		else{
			AVLNode<Key,Value>* g=c->getLeft();
			rotateRight(c);
			rotateLeft(n);
			if(g->getBalance()==-1){
				n->setBalance(0);
				c->setBalance(1);
				g->setBalance(0);
			}
			else if(g->getBalance()==0){
				n->setBalance(0);
				c->setBalance(0);
			}
			else{
				n->setBalance(-1);
				c->setBalance(0);
				g->setBalance(0);
			}
			removeFix(p,ndiff);
		}
	}
	else if(n->getBalance()+diff==1){
		n->setBalance(1);
	}
	else if(n->getBalance()+diff==0){
		n->setBalance(0);
		removeFix(p,ndiff);
	}
}



}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
