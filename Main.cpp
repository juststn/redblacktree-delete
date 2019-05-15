#include <iostream>

#include <string.h>
#include "TreeNode.h"
#include <stack>
#include <cstring>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <Vector>

using namespace std;


/*
Justin He 
Period 2
Red-Black Tree: This program is an implementation of a red-black tree. 
It will ask to read in a file on startup, if a file name is provided then it will create a tree (if data separated by commas/space). 
Single numbers can be added using the add command. 
The print command can be used to print a visual of the red-black tree.
The printlist command prints out the number, red/black value, and parents.
"exit" exits the program.
*/

//function prototypes
void add(int number, TreeNode* &root);
void rotateTree(TreeNode* &current);
void printTree(TreeNode* root);
void getPrintList(TreeNode* root, int k, int array[1000], int colorArray[1000]);
int  getHeight(TreeNode* root);
void printBinary(TreeNode* root, int numberOfSpace);
void printRow(int array[1000], int colorArray[1000], int cnt);
void rotateLeft(TreeNode* &node);
void rotateRight(TreeNode* &node);
void printList(TreeNode* root);


int main(){

  //initialize variables
  char array[999] = "";
  char token[99] = "";
  int arrayLength = 0;
  int layer = 0;
   char in[200] ="";
  ifstream infile;
  int  hasFile =0;

  TreeNode* root = NULL;
  char filename[200] ="";
   
  // get file name, and read the first line from the file
  cout<< "Read in a file (yes, no)?"<<endl;
  cin.getline(in, 200);
  if (!strcmp(in, (char*)"yes")) {
    cout << "Please enter the file name: ";
    cin.getline(filename, 200);
    infile.open(filename);
    if (infile.is_open()) {
      infile.getline(array, 5000);
      infile.close();

 
      hasFile =1;
    }
  else
    {
      cout << "Unable open the input file. " << endl;
    }
  }

  //if a file is present then read it (separated by spaces/commas)
  if (hasFile ==1) {
  // parse the line with list of numbers
  arrayLength = strlen(array);
  for (int i = 0; i <= arrayLength; i++){
    if (array[i] != ' ' && array[i] != ',' && array[i] != '\0'){
      strncat(token, &array[i], 1);
    }
    else{
      if(token[0] != ' ' && token[0] != ',' && token[0] != '\0'){
      int temp = atoi(token);
      token[0] = '\0';
      add(temp, root);
      }
    }
  }
  
  //  cout << "HELLO: " << array << endl;
  cout << "Print of the red-black binary search tree (R for red, B for black): " << endl;
  printTree(root);

  }
  
  char input[10]="";
  int  value;
  int looping=1;
  while (looping ==1) {
    //prompt input
    cout << endl << "Enter command (add, print, printlist, exit):" << endl << "(\"printlist\" displays parent/value/R or B, for each node, \"print\" outputs a visual tree.)" << endl;
    cin >> input;

    //compare strings to look for words
    if (!strcmp(input, (char*)"print")) {
      //printBinary(root, 0);
      printTree(root);
    }

    if (!strcmp(input, (char*)"printlist")){
      printList(root);
    }
    if (!strcmp(input, (char*)"add")) {
      cout << "Enter a number to add to tree (up to 3 digits):" << endl;
      cin >> value;
      add(value, root);
    }
    if (!strcmp(input, (char*)"delete")) {
      cout << "Enter number to delete"
       
    }
    if (!strcmp(input, (char*)"exit")) {
      looping = 0;
    }
    cin.clear();
  }
}

//Shifts and recolors the tree to fit the conditions for the red black tree
void rotateTree(TreeNode* &current){

  if (current->getParent() == NULL){
    return;
  }

  if (current->getParent()->getColor() == 1){
    return;
  }
  
  //Get color of uncle (0 is red 1 is black)
  TreeNode* temp = current->getParent()->getParent();
  
  //If no grandparent, set parent to black
  if(temp == NULL){
    current->getParent()->setColor(1);
    return;
  }

  //If uncle is the left child of grandparent
  if(temp->getRight() == current->getParent() && temp->getLeft() != NULL){
    int uncleColor = temp->getLeft()->getColor();

    //If uncle is red, recolor parent and uncle to black
    if(uncleColor == 0){
      current->getParent()->setColor(1);
      temp->getLeft()->setColor(1);
      if(temp->getParent() != NULL){
        temp->setColor(0);
        rotateTree(temp);
      }
    }
    else{
      //rotateLeft
      rotateLeft(current);
    }
    return;
  }
  // if has no uncle as left child of grandparent (current is on right side of grandparent)
  if (temp->getRight() == current->getParent() && temp->getLeft() == NULL) {
    rotateLeft(current);
    return;
  }
    
  
  //If the uncle is the right child of grandparent
  if(temp->getLeft() == current->getParent() && temp->getRight() != NULL){
    int uncleColor = temp->getRight()->getColor();

    //If color is red, recolor parent and uncle to black
    if(uncleColor == 0){
      current->getParent()->setColor(1);
      temp->getRight()->setColor(1);
      if(temp->getParent() != NULL){
	temp->setColor(0);
	rotateTree(temp);
      }
    }
    else{
      //rotate node Right
      rotateRight(current);
    }
    return;
  }
  
  // if has no uncle as right child of grandparent rotate right
  if (temp->getLeft() == current->getParent() && temp->getRight() == NULL) {
    rotateRight(current);
    return; 
  }
    
}

// rotate tree to right
void rotateRight(TreeNode* &node) {

  // if the current node is a right child, rotate it to be left child
  if (node == node->getParent()->getRight()) {

    //get parent sibling, grandpa
    TreeNode* p =node->getParent();
    TreeNode* t2 =node->getLeft();
    TreeNode* g = node->getParent()->getParent();

    g->setLeft(node);
    node->setLeft(p);
    p->setRight(t2);
    node->setParent(g);
    p->setParent(node);

    //if there is no sibling set one
    if(t2 != NULL){
      
    t2->setParent(p);
    
    }
    
    node = p;
  }
  
  // If the current node is left child
  if (node == node->getParent()->getLeft()) {
    
    //Nodes are parent grandparent uncle
    TreeNode* p = node->getParent();
    TreeNode* g = node->getParent()->getParent();
    TreeNode* u = g->getRight();
    TreeNode* t3 = p->getRight();
    
    int pVal = node->getParent()->getValue();
    int gVal = node->getParent()->getParent()->getValue();

    g->setLeft(node);
    g->setValue(pVal);
    node->setParent(g);

    p->setLeft(t3);
    p->setRight(u);
    p->setParent(g);
    p->setValue(gVal);
    g->setRight(p);
    if (t3 != NULL) {t3->setParent(p); }
    if (u != NULL) { u->setParent(p); }

  }
  
}

//rotate to the left, if the uncle is on left side of grandparent
void rotateLeft(TreeNode* &node) {
   if (node == node->getParent()->getLeft()) {
    TreeNode* p = node->getParent();
    TreeNode* t2 = node->getRight();
    TreeNode* g = node->getParent()->getParent();

    g->setRight(node);
    node->setRight(p);
    p->setLeft(t2);
    node->setParent(g);
    p->setParent(node);

    if(t2 != NULL){

    t2->setParent(p);

    }

    node = p;
  }

    // when current node is right child
  if (node == node->getParent()->getRight()) {
    TreeNode* p = node->getParent();
    TreeNode* g = node->getParent()->getParent();
    TreeNode* u = g->getLeft();
    TreeNode* t3 = p->getLeft();

    int pVal = node->getParent()->getValue();
    int gVal = node->getParent()->getParent()->getValue();

    g->setRight(node);
    g->setValue(pVal);
    node->setParent(g);

    p->setRight(t3);
    p->setLeft(u);
    p->setParent(g);
    p->setValue(gVal);
    g->setLeft(p);
    if (t3 != NULL) {t3->setParent(p); }
    if (u != NULL) { u->setParent(p); }

  }

   

}

//Adding new values to the tree
void add(int number, TreeNode* &root){

  //Add new node with value to the tree
  if (root == NULL){
    TreeNode* newNode = new TreeNode;
    newNode->setValue(number);
    newNode->setColor(1);
    root = newNode;
    return;
  }
  else{
    if(number > root->getValue()){
      //If root has no right child
      if(root->getRight() == NULL){
	
	TreeNode* newNode = new TreeNode;
	newNode->setValue(number);
	newNode->setParent(root);
	root->setRight(newNode);
	if(root->getColor() == 0){
	  //Recolor tree
          rotateTree(newNode);
	}
	return;
	}
      //Use recursive call to add to right subtree using temporary variable
      else{
	TreeNode* temp = root->getRight();
	add(number, temp);
      }
      return;
    }
    if(number <= root->getValue()){
      //If the root has no left child
      if(root->getLeft() == NULL){
	  TreeNode* newNode = new TreeNode;
	  newNode->setValue(number);
	  newNode->setParent(root);
	  root->setLeft(newNode);
	  if(root->getColor() == 0){
	    //Recolor
	    rotateTree(newNode);
	  }
	  return;
      }
      //Recursive call to add to left subtree using temporary variable
      else{
	TreeNode* temp = root->getLeft();
	add(number, temp);
      }

      return;
    } 
  }
}

// print a binary search tree, using printRow
void printTree(TreeNode* root)
 {
      int array[1000];
      int colorArray[1000];
      // initialize the array
      for (int i = 0; i < 1000; i++){
      array[i] = -999;  // represent a null node
      colorArray[i] = -999;
      }
      // put the tree node value into the array
      getPrintList(root, 0, array, colorArray);

      int height = getHeight(root);
      //      cout << "Tree height: " << height << endl;

      int cnt=1;
      //print a tree with doubling rows
      for (int i = 0 ; i < height; i ++) {

        printRow(array, colorArray, cnt);
        cnt = cnt * 2;
      }

  }



//Prints a list with parent color and value
void printList(TreeNode* root){
  if(root == NULL){
    cout << "NULL; NO TREE" << endl;
  }
  else{
    //Display VALUE  
    cout << "Value: " << root->getValue() << ", ";

    //COLOR
    if(root->getColor() == 0){
      cout << "Color: " << "R, ";
    }
    else{
      cout << "Color: " << "B, ";
    }

    //PARENT
    if(root->getParent() != NULL){
      cout << "Parent: " << root->getParent()->getValue() << ", ";
    }
    else{
      cout << "Parent: " << "NULL (Root), ";
    }    

    //LEFT OR RIGHT CHILD
    if(root->getParent() != NULL && root->getParent()->getLeft() == root){
      cout << "Left/Right Child: " << "Left,";
    }
    if(root->getParent() != NULL && root->getParent()->getRight() == root){
      cout << "Left/Right Child: " << "Right,";
    }

    //CHILDREN
    if(root->getLeft() != NULL){
      cout << " Left Child: " << root->getLeft()->getValue() << ",";
    }
    else{
      cout << " Left Child: " << "NULL, ";
    }

    if(root->getRight() != NULL){
      cout << " Right Child: " << root->getRight()->getValue() << endl << endl;
    }
    else{
      cout << " Right Child: " << "NULL," << endl << endl;
    }

    //Recursively call
    if(root->getLeft() != NULL){
      printList(root->getLeft());
    }
    if(root->getRight() != NULL){
      printList(root->getRight());
    }
  }
}

// get height of tree
int getHeight(TreeNode* root)
{
  if (root == NULL) {
    return 0;
  }

  int heightR, heightL;
  heightR = getHeight(root->getRight());
  heightL = getHeight(root->getLeft());

  if (heightR > heightL) {
    return heightR +1;
  }
  else
    {
      return heightL+1;
    }
}

// fit the tree into an array for printing
void getPrintList(TreeNode* root, int k, int array[1000], int colorArray[1000])
{
  if (root == NULL)
    return;

  array[k] = root->getValue();
  colorArray[k] = root->getColor();
  getPrintList(root->getLeft(), k*2+1, array, colorArray);
  getPrintList(root->getRight(), k*2+2, array, colorArray);
}

// print a row of the tree
void printRow(int array[1000], int colorArray[1000], int cnt)
{
  int startIndex = cnt-1;
  int endIndex = startIndex + cnt;

  for (int i = startIndex; i < endIndex; i++) {
    if (array[i] != -999){
      printf("%3d", array[i]);
      if (colorArray[i] == 0) {
	printf("R ");
      }
      else{
	printf("B ");
      }   
    }
   else{
      if (cnt <= 63)
         printf(" ___ ");
      else
         printf(" _ ");
    }
  }

  printf("\n");
}
