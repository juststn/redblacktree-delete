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

PART2:

DELETE and SEARCH function added:

delete command: removes a number from the tree, and updates it
search: searchs for the inputted number and returns whether it is in the tree
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
void restructure(TreeNode* &node);
void deleteTree(TreeNode* &current, TreeNode* &treeRoot, int number);
void searchTree(TreeNode* &current, TreeNode* root, int value, int &find);
void deleteNode(TreeNode* &current, TreeNode* &treeRoot);

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
    cout << endl << "Enter command (add, delete, print, printlist, search, exit):" << endl << "(\"printlist\" displays parent/value/R or B, for each node, \"print\" outputs a visual tree.)" << endl;
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
      cout << "Enter a number to add to tree (up to 3 digits, no duplicates):" << endl;
      cin >> value;
      
      add(value, root);
    }
    //search for a number
    if (!strcmp(input, (char*)"search")){
      cout<<"Enter a number to search in tree"<<endl;
      cin >> value;

      int find=0;
      searchTree(root, root, value, find);

      if (find==1){
	cout<<value<<" is inside the tree"<<endl;

      }
      else{
	cout<<value<< " is not inside the tree"<<endl;
      }	
    }
    if (!strcmp(input, (char*)"delete")) {
      cout << "Enter a number to delete "<<endl;
      cin>>value;

      int find = 0;

      searchTree(root, root, value, find);


      if(find ==1){
	cout<<"deleting "<<value<< " from the tree.";
	  TreeNode* temp = root;
	deleteTree(temp, root, value);
	if(temp->getColor() == 1){
	  restructure(temp);
	}
	deleteNode(temp, root);

      }
       
    
    else{

      cout<<value<<" was not found in the tree."<<endl;
      
    }
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

//scans through tree to search for number to delete
void searchTree(TreeNode* &current, TreeNode* root, int value, int &find){
  if(root == NULL){
    cout << "Tree is empty" << endl;
  }

  if(current == NULL){
    return;
  }
  
  if(current->getValue() == value){
    //   cout << value << " is in the tree. Deleting " << value << "." << endl;
    find = 1; 
    return;
  }
  //scan left
  if(current->getLeft() != NULL){
    TreeNode* temp = current->getLeft();
    searchTree(temp, root, value, find);
  }

  //scan right
  if(current->getRight() != NULL){
    TreeNode* temp1 = current->getRight();
    searchTree(temp1, root, value, find);
  }
 

}

//delete number from tree
void deleteTree(TreeNode* &root, TreeNode* &treeRoot, int deletedNumber){

  //Nothing
  if(root == NULL){
    return;
  }

  //deleted Number is bigger than the root, recursive call for root right child
  if(deletedNumber > root->getValue()){
    root = root->getRight();
    deleteTree(root, treeRoot, deletedNumber);
    return;
  }

  //deleted Number is less than the root, recursive call for root left child
  if(deletedNumber < root->getValue()){
    root = root->getLeft();
    deleteTree(root, treeRoot, deletedNumber);
    //root->setLeft(temp);
    return;
  }

  //deleted Number is equal to root
  if(deletedNumber == root->getValue()){
    // if is leaf node to delete
    if(root->getLeft() == NULL && root->getRight() == NULL) {
       return;  // if is leaf node
     }  

    // if left child is not null, find the max from the left, and promot to root
    if (root->getLeft() != NULL){
      TreeNode* temp = root->getLeft();
      while(temp->getRight() != NULL){
	temp = temp->getRight();
      }
      //cout << temp->getValue() << endl;
      root->setValue(temp->getValue());
      temp->setValue(deletedNumber);
      root = temp; 
      deleteTree(root, treeRoot, deletedNumber);
      return;
    }
    
    if (root->getRight() != NULL){
      TreeNode* temp = root->getRight();
      while(temp->getLeft() != NULL){
	temp = temp->getLeft();
      }
      root->setValue(temp->getValue());
      temp->setValue(deletedNumber);
      root = temp;
      
      deleteTree(root, treeRoot, deletedNumber);
      return;
    }

  }
  
}

//restructure tree after deleting to make it balanced
void restructure(TreeNode* &current){

  
  if(current->getParent() == NULL) {   // if it is root 
    return;
  }

  //Black sibling with red child, current is left child
  if(current->getParent()->getLeft() == current){
    TreeNode* rightSibling = current->getParent()->getRight();
    if(rightSibling->getColor() == 1){

      // black right sibling with red child (on right)
      if(rightSibling->getRight() != NULL && rightSibling->getRight()->getColor() == 0){
	
	TreeNode* p = current->getParent();
	TreeNode* z = rightSibling->getRight();
	TreeNode* x = rightSibling->getLeft();
	TreeNode* s = rightSibling;
	
	int pValue = p->getValue();
	int sValue = rightSibling->getValue();

	p->setValue(sValue);
	p->setRight(z);
	z->setColor(1);
	z->setParent(p);
	p->setLeft(s);
	s->setValue(pValue);
	s->setParent(p);
	s->setRight(x);
	s->setLeft(current);
	current->setParent(s);
	return;
      }


      // black right sibling with red child (on left)
      if(rightSibling->getLeft() != NULL && rightSibling->getLeft()->getColor() == 0){
	TreeNode* p = current->getParent();
	TreeNode* s = rightSibling;
	TreeNode* z = s->getLeft();
	TreeNode* a = z->getLeft();
	TreeNode* b = z->getRight();

	int pValue = p->getValue();
	int zValue = z->getValue();

	p->setValue(zValue);
	s->setLeft(b);
	if(b != NULL){
	  b->setParent(s);
	}
	p->setValue(zValue);
	p->setLeft(z);
	z->setValue(pValue);
	z->setParent(p);
	z->setColor(1);
	z->setRight(a);
	z->setLeft(current);
	current->setParent(z);
	return;

      }

      // black right sibling with black child (both) and red parent
      if(current->getParent()->getColor() == 0 && (rightSibling->getLeft() == NULL || rightSibling->getLeft()->getColor() == 1) && (rightSibling->getRight() == NULL || rightSibling->getRight()->getColor() == 1)){
	current->getParent()->setColor(1);
	rightSibling->setColor(0);
	return; 
	}

      // black right sibling with black child (both), and black parent
      if(current->getParent()->getColor() == 1 && (rightSibling->getLeft() == NULL || rightSibling->getLeft()->getColor() == 1) && (rightSibling->getRight() == NULL || rightSibling->getRight()->getColor() == 1)){
	rightSibling->setColor(0);
	TreeNode* parent = current->getParent();
	restructure(parent);
	return; 
      }    

    }

    // if right sibling is RED
    if(rightSibling->getColor() == 0){
      TreeNode* p = current->getParent();
      TreeNode* s = rightSibling;
      TreeNode* x = rightSibling->getLeft();
      TreeNode* y = rightSibling->getRight();

      int pValue = p->getValue();
      int sValue = s->getValue();

      p->setValue(sValue);
      p->setRight(y);
      if(y != NULL){
	y->setParent(p);
      }
      p->setLeft(s);
      s->setParent(p);
      s->setValue(pValue);
      s->setRight(x);
      s->setLeft(current);
      current->setParent(s);
      restructure(current);
      return;
    }
  }

  
  
  // Black sibling with red child, current is RIGHT child
  if (current->getParent()->getRight() == current){
    TreeNode* leftSibling = current->getParent()->getLeft();
    if(leftSibling->getColor() == 1){

      // cout << "left sibling is black" << endl;
      
      // black right sibling with red child (on left)
      if(leftSibling->getLeft() != NULL && leftSibling->getLeft()->getColor() == 0){
	
        TreeNode* p = current->getParent();
        TreeNode* z = leftSibling->getLeft();
        TreeNode* x = leftSibling->getRight();
        TreeNode* s = leftSibling;

        int pValue = p->getValue();
        int sValue = leftSibling->getValue();

        p->setRight(s);
        p->setValue(sValue);
        p->setLeft(z);
        z->setColor(1);
        z->setParent(p);
        
        s->setValue(pValue);
        s->setParent(p);
        s->setLeft(x);
        s->setRight(current);
        current->setParent(s);

        return;
      }
   // black left sibling with red child (on right)
      if(leftSibling->getRight() != NULL && leftSibling->getRight()->getColor() == 0){
        TreeNode* p = current->getParent();
        TreeNode* s = leftSibling;
        TreeNode* z = s->getRight();
        TreeNode* a = z->getRight();
        TreeNode* b = z->getLeft();

       int pValue = p->getValue();
        int zValue = z->getValue();

        p->setValue(zValue);
        s->setRight(b);
        if(b != NULL){
          b->setParent(s);
        }
        p->setValue(zValue);
        p->setRight(z);
        z->setValue(pValue);
        z->setParent(p);
        z->setColor(1);
        z->setLeft(a);
        z->setRight(current);
        current->setParent(z);
        return;
      }
       // black left sibling with black child (both) and red parent
      if(current->getParent()->getColor() == 0 && (leftSibling->getRight() == NULL || leftSibling->getRight()->getColor() == 1) && (leftSibling->getLeft() == NULL || leftSibling->getLeft()->getColor() == 1\
)){
        current->getParent()->setColor(1);
        leftSibling->setColor(0);
	return;
        }

      // black left sibling with black child (both), and black parent
      if(current->getParent()->getColor() == 1 && (leftSibling->getRight() == NULL || leftSibling->getRight()->getColor() == 1) && (leftSibling->getLeft() == NULL || leftSibling->getLeft()->getColor() == 1\
)){
        leftSibling->setColor(0);
        TreeNode* parent = current->getParent();
        restructure(parent);
	return;
      }

    }

    
    //RED SIBLING
     if(leftSibling->getColor() == 0){
      TreeNode* p = current->getParent();
      TreeNode* s = leftSibling;
      TreeNode* x = leftSibling->getRight();
      TreeNode* y = leftSibling->getLeft();

      int pValue = p->getValue();
      int sValue = s->getValue();

      p->setValue(sValue);
      p->setLeft(y);
      if(y != NULL){
        y->setParent(p);
      }
      p->setRight(s);
      s->setParent(p);
      s->setValue(pValue);
      s->setLeft(x);
      s->setRight(current);
      current->setParent(s);
      restructure(current);
      return;
    }
  
  }

  
}

// delete a node from the tree
void deleteNode(TreeNode* &current, TreeNode* &treeRoot){

  if (current == NULL) {
    return;
  }

  if (current == treeRoot) {
    delete current;
    treeRoot = NULL;
    return;
  }

  if (current->getParent()->getLeft() ==current) {
    current->getParent()->setLeft(NULL);
    delete current;
    current= NULL;
    return;
  }

  if (current->getParent()->getRight()== current) {
      current->getParent()->setRight(NULL);
      delete current;
      current =NULL; 
  }
}
