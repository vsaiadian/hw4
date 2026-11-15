#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int levels(Node* root){
  int leftLevels, rightLevels;

  if(root->left == NULL && root->right == NULL) return 1;
  
  else if(root->right == NULL){
    leftLevels = levels(root->left);
    
    if(leftLevels == -1) return -1;
    else return 1+leftLevels;
  }

  else if(root->left == NULL)
  {
    rightLevels = levels(root->right);

    if(rightLevels == -1) return -1;
    else return 1+rightLevels;
  }

  else{
    rightLevels = levels(root->right);
    leftLevels = levels(root->left);

    if(rightLevels == -1 || leftLevels == -1 || (rightLevels != leftLevels))
    {
      return -1;
    }
    else return 1+rightLevels;
  }
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL) return true;
    else{
      int pathway = levels(root);
      if(pathway == -1) return false;
      else return true;
    }

}

