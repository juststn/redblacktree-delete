#include <iostream>

#ifndef TREENODE_H
#define TREENODE_H


//JUSTIN HE
//Definitions for TreeNode

class TreeNode{
 public:
  TreeNode();
  ~TreeNode();
  TreeNode* getRight();
  TreeNode* getLeft();

  void setLeft(TreeNode* newLeft);
  void setRight(TreeNode* newRight);
  void setValue(int newvalue);
  void setParent(TreeNode* newParent);
  TreeNode* getParent();
  int getValue();
  int getColor();
  void setColor(int newColor);
 private:
  
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent;
  int value;
  int color;
};

#endif
