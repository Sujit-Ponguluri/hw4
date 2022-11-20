#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int countDistance(Node * root);


bool equalPaths(Node * root)
{
    // Add your code below
    if(root!=NULL && countDistance(root)==-1){
        return false;
    }
    return true;


}

int countDistance(Node * root)
{
int left=0;
int right=0;
if(root->left){
    left=countDistance(root->left);
}
if(root->right){
    right=countDistance(root->right);
}
if(left==-1 || right==-1){
    return -1;
}
if(left!=right){
    if(left!=0 && right!=0){
        return -1;
    }
    else if(left==0){
        return right+1;
    }
    else{
        return left+1;
    }
}
    return left+1;
}