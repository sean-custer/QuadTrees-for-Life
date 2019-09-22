//file:  QuadTree.cpp
//author:  Sean Custer
//
//

#include <vector>
#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"

#include "QuadTree.h"

using namespace std;


QuadTree::iterator::iterator(){
  m_QTQuadPtr = NULL;
  m_point.m_x = 0;
  m_point.m_y = 0;
}

QuadTree::iterator::~iterator(){
  delete m_QTQuadPtr;
  m_QTQuadPtr = NULL;
}

const QuadTree::iterator& QuadTree::iterator::operator=(const QuadTree::iterator &rhs){
  m_QTQuadPtr = rhs.m_QTQuadPtr;
  m_point.m_x = rhs.m_point.m_x;
  m_point.m_y = rhs.m_point.m_y;
}

bool QuadTree::iterator::operator==(const QuadTree::iterator &other){
  if(m_QTQuadPtr == other.m_QTQuadPtr &&
     m_point.m_x == other.m_point.m_x &&
     m_point.m_y == other.m_point.m_y)
    return true;
  return false;
}


bool QuadTree::iterator::operator!=(const QuadTree::iterator &other){
  if(m_QTQuadPtr == other.m_QTQuadPtr &&
     m_point.m_x != other.m_point.m_x &&
     m_point.m_y != other.m_point.m_y)    
    return true;
  return false;
}

QuadTree::iterator& QuadTree::iterator::operator++(){
    
  //Case 1: if next value is in current m_cells
  int x;
  int y;
  //get the current m_cell value in terms of the dimensions of the 2D cell
  x = m_point.m_x % 4;
  y = m_point.m_y % 4;

  //iterate through m_cells indices
  for(int i = x + 1; i < QTQ_GRID_DIM; i++)
    {
      for(int j = y + 1; i < QTQ_GRID_DIM; j++)
	{
	  //if we encounter another nonzero m_cells value,
	  //add the change in values to x and y points, return
	  if(m_QTQuadPtr->m_cells[i][j] != 0)
	    {
	      m_point.m_x += (i - x);
	      m_point.m_y += (j - y);
	      return *this;	      
	    }
	}
    }
  //Case 2: if next value is not in current m_cells
  this->FindCells();
  return *this;
}

int QuadTree::iterator::FindChildIndex(){

  //find the current child index we are leaving
  int childNum;
  for(int i = 0; i < QTQ_GRID_DIM; i++)
    {
      if(m_QTQuadPtr->m_parent->m_quads[i] == this->m_QTQuadPtr)
        childNum = i;
    }
  return childNum;
}

QuadTree::iterator QuadTree::iterator::FindCells(){

  //make sure that we are checking a leaf node. If not, iterate down until leaf
  if(!this->m_QTQuadPtr->isLeaf())
    {
      int childNum = this->FindChildIndex();
      for(int i = childNum; i < QTQ_GRID_DIM; i++)
	{
	  this->m_QTQuadPtr = this->m_QTQuadPtr->m_quads[i];
	  this->FindCells();
	}
    }
  //pseudo-code: we want to leave this m_cells and check the next child's
  //m_cells. If that child does not have any m_cells, move on.
  int childNum;
  childNum = this->FindChildIndex();
  //iterate through the rest of the childs, check their m_cells
  for(int i = childNum + 1; i < QTQ_GRID_DIM; i++)
    {
      //set iterator to point to new node
      for(int j = 0; j < QTQ_GRID_DIM; j++)
        {
          for(int k = 0; k < QTQ_GRID_DIM; k++)
            {
	      //BASE CASE: if m_cells[j][k] is nonzero, we found it!
              if(this->m_QTQuadPtr->m_parent->m_quads[i]->m_cells[j][k] != 0){
		this->m_point.m_x += j;
		this->m_point.m_y += k;
		return *this;
              }
            }
        }
    }
  //RECURSIVE CASE:
  //if we iterate through all of the children and their m_cells and
  //have not found an existing m_cell value, recurse to parent and
  //move to next child
  this->m_QTQuadPtr = this->m_QTQuadPtr->m_parent;
  childNum = this->FindChildIndex();
  for(int i = childNum; i < QTQ_GRID_DIM; i++)
    {
      this->m_QTQuadPtr = this->m_QTQuadPtr->m_parent;	  
      this->FindCells();
    } 
}
QuadTree::iterator QuadTree::iterator::operator++(int dummy){
  //Case 1: if next value is in current m_cells
  int x;
  int y;
  //get the current m_cell value in terms of the dimensions of the 2D cell
  x = m_point.m_x % 4;
  y = m_point.m_y % 4;
  
  //iterate through m_cells indices
  for(int i = x + 1; i < QTQ_GRID_DIM; i++)
    {
      for(int j = y + 1; i < QTQ_GRID_DIM; j++)
        {
          //if we encounter another nonzero m_cells value,
          //add the change in values to x and y points, return
          if(m_QTQuadPtr->m_cells[i][j] != 0)
            {
              m_point.m_x += (i - x);
              m_point.m_y += (j - y);
              return *this;
            }
        }
    }
  //Case 2: if next value is not in current m_cells
  
  this->FindCells();
  return *this; 
}

Point& QuadTree::iterator::operator*(){
  return m_point;
}

//
// Now, the stuff for the main class:
//
QuadTree::QuadTree(){
  // quadrant representing the whole tree
  m_root = new QTQuad();
  m_root->m_parent = NULL;
  m_bounds = BBox(Point(0,0), 64);
  m_qBounds = m_bounds;
}  // Creates a 32x32 default board


QuadTree::QuadTree(const BBox &bounds) {
   // quadrant representing the whole tree
  m_root = new QTQuad();
  m_root->m_parent = NULL;
  m_bounds = bounds;
  int roundedDim = bounds.roundUpPow2(bounds.m_dim);
  m_qBounds = BBox(bounds.m_bL, roundedDim);
}

QuadTree::~QuadTree() {
  delete m_root;
}


// Retrieve cell value
int QuadTree::get(const Point &pt){
  return m_root->get(pt,m_qBounds);
}

// Set cell value
void QuadTree::set(const Point &pt, int data){
  m_root->set(pt,data,m_qBounds);
}

// Modify cell value by a signed delta amount; faster then get() + set()
int QuadTree::increment(const Point &pt, int delta) {
  m_root->increment(pt, delta, m_qBounds);
  return 0;
}

// Clear all cells in tree
void QuadTree::clearAll(){
  m_root->clearAll();
  m_root = NULL;
}
// return iterator to first non-zero cell; == end() if qtree empty
QuadTree::iterator QuadTree::begin(){
  
  //ASSUMPTION: Tree is compact!!!
  QuadTree::iterator it;
  it.m_QTQuadPtr = m_root;
  //starting at 32, as you traverse add this value to m_point to keep track
  int addCoords = m_qBounds.m_dim/2;
  //RECURSIVE CASE
  //check all child nodes m_cells, if they do not exist, move on to next child
  while(!it.m_QTQuadPtr->isLeaf())
    {
      
      if(it.m_QTQuadPtr->m_quads[0] != NULL)
	{
	  //if we enter Quadrant 1, leave m_point's x and y values the same
	  addCoords = addCoords / 2;
	  it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[0];
	}
      else if(it.m_QTQuadPtr->m_quads[1] != NULL)
        {
	  //when we enter Quadrant 2, add m_dim to x value of m_point, then
	  //divide addCoords by 2.
	  it.m_point.m_x += addCoords;
	  addCoords = (addCoords / 2);
	  
          it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[1];
        }
      else if(it.m_QTQuadPtr->m_quads[2] != NULL)
        {
	  //when we enter Quadrant 3, add m_dim to y value of m_point, then
          //divide addCoords by 2.
          it.m_point.m_y += addCoords;
          addCoords = (addCoords / 2);
          it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[2];
        }
      else if(it.m_QTQuadPtr->m_quads[3] != NULL)
        {
          //when we enter Quadrant 4, add m_dim to x  and yvalue of m_point,
	  //then divide addCoords by 2.
          it.m_point.m_x += addCoords;
	  it.m_point.m_y += addCoords;
          addCoords = (addCoords / 2);
          it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[3];
        }
    }
  //BASE CASE
  //if we are at a leaf, iterate through m_cells and return the first value
  //in the form of an iterator

for(int i = 0; i < QTQ_GRID_DIM; i++)
  {
    for(int j = 0; j < QTQ_GRID_DIM; j++)
      {
	if(it.m_QTQuadPtr->m_cells[i][j] != 0)
	  {
	    it.m_point.m_x += i;
	    it.m_point.m_y += j;
	    return it;
	  }
      }
  }
}


// iterator pointing beyond last cell;
QuadTree::iterator QuadTree::end(){
  
  //ASSUMPTION: Tree is compact!!!
  QuadTree::iterator it;
  it.m_QTQuadPtr = m_root;
  int addCoords = m_qBounds.m_dim/2;
  //RECURSIVE CASE
  //check all child nodes m_cells, if they do not exist, move on to next child
  //checks each child from IV to I quadrants
  while(!it.m_QTQuadPtr->isLeaf())
    {
      if(it.m_QTQuadPtr->m_quads[3] != NULL)
        {
	  it.m_point.m_x += addCoords;
	  it.m_point.m_y += addCoords;
	  addCoords = (addCoords / 2);
          it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[3];
        }
      else if(it.m_QTQuadPtr->m_quads[2] != NULL)
        {
	  it.m_point.m_y += addCoords;
	  addCoords = (addCoords / 2);
          it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[2];
        }
      else if(it.m_QTQuadPtr->m_quads[1] != NULL)
        {
	  it.m_point.m_x += addCoords;
	  addCoords = (addCoords / 2);
          it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[1];
        }
      else if(it.m_QTQuadPtr->m_quads[0] != NULL)
        {
	  addCoords = (addCoords / 2);
          it.m_QTQuadPtr = it.m_QTQuadPtr->m_quads[0];
        }

    }
  //BASE CASE
  //if we are at a leaf, iterate backwards through m_cells and return the first
  //value in the form of an iterator

  for(int i = QTQ_GRID_DIM - 1; i >= 0; i--)
  {
    for(int j = QTQ_GRID_DIM - 1; j >= 0; j--)
      {
        if(it.m_QTQuadPtr->m_cells[i][j] != 0)
          {
            it.m_point.m_x += i;
            it.m_point.m_y += j;
            return it;
          }
      }
  }
}

// Print out structure of tree

/*void QuadTree::dump() {
  cout << "============================================================\n"
       << "START Dump of quadtree: true bounds = " << m_bounds << "\n"
       << "------------------------------------------------------------"
       << endl;
  m_root->dump(m_qBounds);
  cout << "------------------------------------------------------------\n"
       << "END Dump of quadtree\n"
       << "============================================================"
       << endl;
}
*/

