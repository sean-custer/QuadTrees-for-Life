//file:  QTQuad.cpp
//author:  Sean Custer
//desc:  Implementation of QTQuad

#include "QTQuad.h"
#include "Point.h"
#include "BBox.h"

#include <cstddef>
#include <cstring>
#include <iostream>
using namespace std;

QTQuad::QTQuad(){
  m_parent = NULL;
  for(int i; i < QTQ_NUM_QUADS; i++){
    m_quads[i] = NULL;
  }
  m_cells = NULL;
}

QTQuad::~QTQuad(){
  for(int i; i < QTQ_NUM_QUADS; i++)
    {
      if(m_quads[i] != NULL)
	{
	  delete m_quads[i];
	}
    }
}

//recursive fxn that iterates through the partitions until the point is found
int QTQuad::get(const Point &pt, const BBox &bounds){
  
  //***BASE CASE***
  //if the node is a leaf, then we have reached the point.
  if (isLeaf())
    {
      //check this points' m_cells and return the value we want from our
      //updated point
      if(m_cells != NULL)
	{
	  int x = pt.m_x - bounds.m_bL.m_x;
	  int y = pt.m_y - bounds.m_bL.m_y;
	  return m_cells[x][y];
	}
    }

  //***RECURSIVE CASE***
  for(int i=0; i < QTQ_NUM_QUADS; i++)
    {
    if (m_quads[i] != NULL)

      {
	//if the partition of the QuadTree exists, call get() on
	//new boundaries (sort of like modulus conceptually)
	BBox newBounds(bounds.m_bL, bounds.m_dim/2);
	//check what quadrant point is in, update bounds accordingly
	
	//quadrant 1
	if(i == 0)
	  {
	    //if the point is within the quadrant boundaries
	    if(newBounds.inBounds(pt))
	      {
		return m_quads[i]->get(pt, newBounds);
	      }
	  }
	//quadrant 2
	else if(i == 1)
	  {
	    newBounds.m_bL.m_x += newBounds.m_dim;
	    if(newBounds.inBounds(pt))
	      {
		return m_quads[i]->get(pt, newBounds);
	      }
	  }
	//quadrant 3
	else if(i == 2)
	  {
	    newBounds.m_bL.m_y += newBounds.m_dim;	    
	    if(newBounds.inBounds(pt))
	      {
                return m_quads[i]->get(pt, newBounds);
	      }
	  }
	//quadrant 4
	else
	  {
	    newBounds.m_bL.m_x += newBounds.m_dim;
	    newBounds.m_bL.m_y += newBounds.m_dim;	    
	    if(newBounds.inBounds(pt))
	      {
		return m_quads[i]->get(pt, newBounds);
	      }
	  }
      }
    }
}

bool QTQuad::isLeaf(){
  //check all four children, iterate through and check if all children are null
  //returns false if any children exist, returns true if all children are null
  for(int i = 0; i < QTQ_NUM_QUADS; i++)
    {
      if (m_quads[i] != NULL)
	{
	  return false;
	}
    }
  return true;
}

void QTQuad::set(const Point &pt, int data, const BBox &bounds){
  //***BASE CASE***
  //if the node is a leaf and our dim is smallest it can be,
  //then we have reached the point.
  if (isLeaf() && bounds.m_dim == QTQ_GRID_DIM)
    {
      //if leaf is already initialized
      if(m_cells != NULL)
	{
	  //set m_cell's value to data
	  int x = pt.m_x - bounds.m_bL.m_x;
	  int y = pt.m_y - bounds.m_bL.m_y;
	  m_cells[x][y] = data;
	  
	}
      //if leaf is not already initialized, allocate and set it.
      else
	{
	  m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
	  int x = pt.m_x - bounds.m_bL.m_x;
          int y = pt.m_y - bounds.m_bL.m_y;
	  m_cells[x][y] = data;
	  
	}
      
      //check to see if newly added data makes m_cells all 0. If so, delete it
      if(checkCells())
	{
	  delete [] m_cells;
	  //prune the tree
	  this->prune();
	}
      //break out of recursion
      return;
    }
  //***RECURSIVE CASE***
  for(int i = 0; i < QTQ_NUM_QUADS; i++)
    {
      //if the partition of the QuadTree exists, call get() on
      //new boundaries (sort of like modulus conceptually)
      BBox newBounds = BBox(bounds.m_bL, bounds.m_dim/2);
      //check what quadrant point is in, update bounds accordingly
      //quadrant 1
      if(i == 0)
	{
	  //check to see if point is within the boundaries of this quadrant
	  if (newBounds.inBounds(pt))
	    {
	      //dynamically allocate a new QTQuad node, set its parent
	      if(m_quads[i] == NULL)
		{
		  m_quads[i] = new QTQuad();
		  m_quads[i]->m_parent = this;
		  m_quads[i]->set(pt, data, newBounds);
		}
	      //if node already exists, set its parent and continue traversing
	      else if(m_quads[i] != NULL)
		{
		  m_quads[i]->m_parent = this;
		  m_quads[i]->set(pt, data, newBounds);
		}
	    }
	}
      //quadrant 2
      else if(i == 1)
	{
	  newBounds.m_bL.m_x += newBounds.m_dim;
	  //if the point requested is in bounds	  
	  if (newBounds.inBounds(pt))
	    {
	      //dynamically allocate a new QTQuad node, set its parent
              if(m_quads[i] == NULL)
                {
                  m_quads[i] = new QTQuad();
                  m_quads[i]->m_parent = this;
                  m_quads[i]->set(pt, data, newBounds);
                }
              //if node already exists, set its parent and continue traversing
              else if(m_quads[i] != NULL)
                {
                  m_quads[i]->m_parent = this;
                  m_quads[i]->set(pt, data, newBounds);
                }	      
	    }
	}
      //quadrant 3
      else if(i == 2)
	{
	  newBounds.m_bL.m_y += newBounds.m_dim;
	  //if the point requested is in bounds	  
	  if (newBounds.inBounds(pt))
	    {
	      //dynamically allocate a new QTQuad node, set its parent
              if(m_quads[i] == NULL)
                {
                  m_quads[i] = new QTQuad();
                  m_quads[i]->m_parent = this;
                  m_quads[i]->set(pt, data, newBounds);
                }
              //if node already exists, set its parent and continue traversing
              else if(m_quads[i] != NULL)
                {
                  m_quads[i]->m_parent = this;
                  m_quads[i]->set(pt, data, newBounds);
                }
	    }
	}
      //quadrant 4
      else
	{
	  newBounds.m_bL.m_x += newBounds.m_dim;
	  newBounds.m_bL.m_y += newBounds.m_dim;
	  //if the point requested is in bounds
	  if (newBounds.inBounds(pt))
	    {
	      //dynamically allocate a new QTQuad node, set its parent
              if(m_quads[i] == NULL)
                {
                  m_quads[i] = new QTQuad();
                  m_quads[i]->m_parent = this;
                  m_quads[i]->set(pt, data, newBounds);
                }
              //if node already exists, set its parent and continue traversing
              else if(m_quads[i] != NULL)
                {
                  m_quads[i]->m_parent = this;
                  m_quads[i]->set(pt, data, newBounds);
                }
	    }
	}
    }
}

int QTQuad::increment(const Point &pt, int delta, const BBox &bounds){
  //***BASE CASE***
  //if the node is a leaf and our dim is smallest it can be,
  //then we have reached the point.
  if (isLeaf() && bounds.m_dim == QTQ_GRID_DIM)
    {
      int x = pt.m_x - bounds.m_bL.m_x;
      int y = pt.m_y - bounds.m_bL.m_y;

      //if leaf is already initialized
      if(m_cells != NULL)
        {
          //increment m_cell's value by delta
	  m_cells[x][y] += delta;
        }
      //if leaf is not already initialized, allocate and set it.
      else
        {
          m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
	  m_cells[x][y] += delta;		  
        }
      
      //check to see if newly added data makes m_cells all 0. If so, delete it
      if(checkCells())
        {

          delete [] m_cells;
	  //prune
	  this->prune();
        }
      
      //Return the value we just incremented
      return m_cells[x][y];
    }
  //***RECURSIVE CASE***
  for(int i = 0; i < QTQ_NUM_QUADS; i++)
    {
      //if the partition of the QuadTree exists, call get() on
      //new boundaries (sort of like modulus conceptually)
      BBox newBounds = BBox(bounds.m_bL, bounds.m_dim/2);
      //check what quadrant point is in, update bounds accordingly
      //quadrant 1
      if(i == 0)
        {
	  //check to see if point is within the boundaries of this quadrant
          if (newBounds.inBounds(pt))
            {          
              //dynamically allocate a new QTQuad node, set its parent
              if(m_quads[i] == NULL)
                {
                  m_quads[i] = new QTQuad();
                  m_quads[i]->m_parent = this;
		  return m_quads[i]->increment(pt, delta, newBounds);
                }
              //if node already exists, set its parent and continue traversing
              else if(m_quads[i] != NULL)
                {
                  m_quads[i]->m_parent = this;
                  return m_quads[i]->increment(pt, delta, newBounds);
                }
            }
        }
      //quadrant 2
      else if(i == 1)
        {
          newBounds.m_bL.m_x += newBounds.m_dim;
          //if the point requested is in bounds
          if (newBounds.inBounds(pt))
            {
              //dynamically allocate a new QTQuad node, set its parent
              if(m_quads[i] == NULL)
                {
                  m_quads[i] = new QTQuad();
                  m_quads[i]->m_parent = this;
                  return m_quads[i]->increment(pt, delta, newBounds);
                }
              //if node already exists, set its parent and continue traversing
              else if(m_quads[i] != NULL)
                {
                  m_quads[i]->m_parent = this;
                  return m_quads[i]->increment(pt, delta, newBounds);
                }
            }
        }
      //quadrant 3
      else if(i == 2)
        {
          newBounds.m_bL.m_y += newBounds.m_dim;
          //if the point requested is in bounds
          if (newBounds.inBounds(pt))
            {
              //dynamically allocate a new QTQuad node, set its parent
              if(m_quads[i] == NULL)
                {
                  m_quads[i] = new QTQuad();
                  m_quads[i]->m_parent = this;
                  return m_quads[i]->increment(pt, delta, newBounds);
                }
              //if node already exists, set its parent and continue traversing
              else if(m_quads[i] != NULL)
                {
                  m_quads[i]->m_parent = this;
                  return m_quads[i]->increment(pt, delta, newBounds);
                }
            }
        }
      //quadrant 4
      else
        {
          newBounds.m_bL.m_x += newBounds.m_dim;
          newBounds.m_bL.m_y += newBounds.m_dim;
          //if the point requested is in bounds
          if (newBounds.inBounds(pt))
            {
              //dynamically allocate a new QTQuad node, set its parent
              if(m_quads[i] == NULL)
                {
                  m_quads[i] = new QTQuad();
                  m_quads[i]->m_parent = this;
                  return m_quads[i]->increment(pt, delta, newBounds);
                }
              //if node already exists, set its parent and continue traversing
              else if(m_quads[i] != NULL)
                {
                  m_quads[i]->m_parent = this;
                  return m_quads[i]->increment(pt, delta, newBounds);
                }
            }
        }
    }
}

bool QTQuad::checkCells(){
  
  for(int i = 0; i < QTQ_GRID_DIM; i++)
    {
      for(int j = 0; j < QTQ_GRID_DIM; j++)
	{
	  //if any m_cells point is nonzero, m_cells is not empty
	  if(m_cells[i][j] != 0)
	    return false;
	}
    }
  //m_cells is empty
  return true;
}


void QTQuad::prune(){
  //if this is the root of the tree, exit prune
  if(this->m_parent == NULL)
    {
      return;
    }
  
  for(int i = 0; i < QTQ_NUM_QUADS; i++)
    {
      //BASE CASE**********
      //if any child does not point to null, then return
      if(m_quads[i] != NULL)
	{
	  return;
	}
      //if this is the child of m_parent we want to delete, and all children
      //point to NULL, then delete it
      else if(this == this->m_parent->m_quads[i])
	{
	  delete m_parent->m_quads[i];
	  m_parent->m_quads[i] = NULL;
	}
    }
  
  //recursive call
  this->m_parent->prune();
}

void QTQuad::clearAll(){
  //BASE CASE****
  //once we reach the leaf and m_cells is not empty, clear it
  if(m_cells != NULL)
    {
      delete [] m_cells;
      m_cells = NULL;
      this->prune();
      return;
    }
  
  //iterate down the subtree, set all m_cells to NULL.
  for(int i = 0; i < QTQ_NUM_QUADS; i++)
    {
      //RECURSIVE CASE***
      //if any child still exists that is not a leaf, call clearAll
      if(m_quads[i] != NULL)
	{
	  m_quads[i]->clearAll();
	}
    }
}
