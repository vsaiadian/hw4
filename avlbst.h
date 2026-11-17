#ifndef AVLBST_H
#define AVLBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor
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
    void leftRotat(AVLNode<Key, Value>* node);
    void rightRotat(AVLNode<Key, Value>* node);


};

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotat(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = node->getRight();
    AVLNode<Key, Value>* middle = node->getRight()->getLeft();

    child->setParent(parent);
    if(parent==NULL) this->root_ = child;
    else if(parent->getLeft() == node) parent->setLeft(child);
    else parent->setRight(child);

    node->setRight(middle);
    if(middle != NULL) middle->setParent(node);

    child->setLeft(node); 
    node->setParent(child);

}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotat(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = node->getLeft();
    AVLNode<Key, Value>* middle = node->getLeft()->getRight();

    child->setParent(parent);

    if(parent==NULL) this->root_ = child;
    else if(parent->getLeft() == node) parent->setLeft(child);
    else parent->setRight(child);

    node->setLeft(middle);
    if(middle != NULL) middle->setParent(node);

    child->setRight(node); 
    node->setParent(child);

}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL)
    {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      return;//inserted very first node
    }
    else
    {
      AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
      AVLNode<Key, Value>* parent = NULL;

      while(curr != NULL)
      {
        if(new_item.first < curr->getKey())
        {
          parent = curr;
          curr = curr->getLeft();
        }
        else if(new_item.first > curr->getKey())
        {
          parent = curr;
          curr = curr->getRight();
        }
        else
        {
          parent = curr;
          curr->setValue(new_item.second);
          return;
        }   
        
      }

      AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
      if(new_item.first < parent->getKey()){
        parent->setLeft(newNode);
      }
      else{
        parent->setRight(newNode);
      }

      AVLNode<Key, Value>* child = newNode;
      AVLNode<Key, Value>* ancestor = newNode->getParent();
      while(ancestor != NULL)
      {
        if(ancestor->getLeft() == child)
        {
          ancestor->updateBalance(-1);
        }
        else
          ancestor->updateBalance(1);

        int balance = ancestor->getBalance();
      
        if(balance == 0) break;

        if(balance == -2)
        {
          AVLNode<Key, Value>* leftChild = ancestor->getLeft();
          
          if(leftChild->getBalance() == -1)//left left
          {
            rightRotat(ancestor);
            ancestor->setBalance(0);
            leftChild->setBalance(0);
          }
          else// left right
          {
            AVLNode<Key, Value>* middle = leftChild->getRight();
            int8_t middleBalance = middle->getBalance();
            
            leftRotat(leftChild);
            rightRotat(ancestor);
            if(middleBalance == -1)
            {
              leftChild->setBalance(0);
            ancestor->setBalance(1);
            }
            else if(middleBalance == 0)
            {
              leftChild->setBalance(0);
              ancestor->setBalance(0);
            }
            else
            {
              leftChild->setBalance(-1);
              ancestor->setBalance(0);
            }
            
            middle->setBalance(0);
          }

          break;
      }

      else if(balance == 2)
      {
        AVLNode<Key, Value>* rightChild = ancestor->getRight();
        if(rightChild->getBalance() == 1){
          //right right
          leftRotat(ancestor);
          ancestor->setBalance(0);
          rightChild->setBalance(0);
        }
        else
        {
          //right left
          AVLNode<Key, Value>* middle = rightChild->getLeft();
          int8_t middleBalance = middle->getBalance();
          rightRotat(rightChild);
          leftRotat(ancestor);

          if(middleBalance == -1)
            {
              ancestor->setBalance(0);
            rightChild->setBalance(1);
            }
            else if(middleBalance == 0)
            {
              rightChild->setBalance(0);
              ancestor->setBalance(0);
            }
            else
            {
              ancestor->setBalance(-1);
              rightChild->setBalance(0);
            }
            
            middle->setBalance(0);
          }

          break;
      }

      child = ancestor;
      ancestor = ancestor->getParent();
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
    // TODO
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    //search for existing key
    while(curr != NULL)
    {
      if(curr->getKey() > key) curr = curr->getLeft();
      else if(curr->getKey() < key) curr = curr->getRight();
      else break;//found the node
    }

    if(curr ==  NULL) return; //key not found so nothing was removed

    if(curr->getLeft() != NULL && curr->getRight() != NULL)
    {
      AVLNode<Key, Value>* predec = curr->getLeft();

      while(predec->getRight() != NULL)
      {
        predec = predec->getRight();
      }
      this->nodeSwap(curr, predec);
    }

      //the only child curr may have left after the swap
      AVLNode<Key, Value>* child = NULL;
      if(curr->getLeft() != NULL) child = curr->getLeft();
      else child = curr->getRight();

      AVLNode<Key, Value>* parent = curr->getParent();
      bool wasNodeLeftChild = (parent != NULL && parent->getLeft() == curr);
      
      if(parent == NULL) this->root_ = child;
      else if(wasNodeLeftChild) parent->setLeft(child);
      else parent->setRight(child);

      if(child != NULL) child->setParent(parent);

      delete curr;

      AVLNode<Key, Value>* ancestor = parent;
      while(ancestor != NULL)
      {
        if(wasNodeLeftChild) 
          ancestor->updateBalance(1);
        else 
          ancestor->updateBalance(-1);

        int balance = ancestor->getBalance();
        AVLNode<Key, Value>* subtreeRoot = ancestor;

        if(balance == 1 || balance == -1)
          break;

        if(balance == 2)
        {
          AVLNode<Key, Value>* rightChild = ancestor->getRight();
          int rightBalance = rightChild->getBalance();
          if(rightBalance == 0)
          {
            leftRotat(ancestor);
            ancestor->setBalance(1);
            rightChild->setBalance(-1);
            break;
          }
          else if(rightBalance == 1)
          {
            //right right
            leftRotat(ancestor);
            ancestor->setBalance(0);
            rightChild->setBalance(0);

            subtreeRoot = rightChild;
          }
          else
          {
            //right left
            AVLNode<Key, Value>* middle = rightChild->getLeft();
            int8_t middleBalance = middle->getBalance();

            rightRotat(rightChild);
            leftRotat(ancestor);

            subtreeRoot = middle;

            if(middleBalance == -1)
            {
              ancestor->setBalance(0);
              rightChild->setBalance(1);
            }
            else if(middleBalance == 0)
            {
              ancestor->setBalance(0);
              rightChild->setBalance(0);
            }
            else
            {
              ancestor->setBalance(-1);
              rightChild->setBalance(0);
            }
            middle->setBalance(0);
          }
      }
      else if(balance == -2)
      {
        AVLNode<Key, Value>* leftChild = ancestor->getLeft();
        int leftBalance = leftChild->getBalance();
        if(leftBalance == 0)
          {
            rightRotat(ancestor);
            ancestor->setBalance(-1);
            leftChild->setBalance(1);
            break;
          }
          else if(leftBalance == -1)
          {
            //left left
            rightRotat(ancestor);
            ancestor->setBalance(0);
            leftChild->setBalance(0);

            subtreeRoot = leftChild;
          }
          else
          {
            // left right
            AVLNode<Key, Value>* middle = leftChild->getRight();
            int8_t middleBalance = middle->getBalance();

            leftRotat(leftChild);
            rightRotat(ancestor);

            subtreeRoot = middle;

            if(middleBalance == -1)
            {
              ancestor->setBalance(1);
              leftChild->setBalance(0);
            }
            else if(middleBalance == 0)
            {
              ancestor->setBalance(0);
              leftChild->setBalance(0);
            }
            else
            {
              leftChild->setBalance(-1);
              ancestor->setBalance(0);
            }
            middle->setBalance(0);
          }
        }
      AVLNode<Key, Value>* nextAncestor = subtreeRoot->getParent();
      
      if(nextAncestor != NULL)
      {
        wasNodeLeftChild = (nextAncestor->getLeft() == subtreeRoot);
      }
      ancestor = nextAncestor;

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
