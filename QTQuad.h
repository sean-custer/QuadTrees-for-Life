#ifndef QTQUAD_H
#define QTQUAD_H

#include <vector>
#include "Point.h"
#include "BBox.h"

#define QTQ_NUM_QUADS 4
#define QTQ_GRID_DIM 4

class QuadTree;

class QTQuad {

public:

    QTQuad();
    ~QTQuad();

    // Retrieve cell value
    int get(const Point &pt, const BBox &bounds);

    // Set cell value
    void set(const Point &pt, int data, const BBox &bounds);

    // Modify cell value by a signed delta amount; faster then get() + set()
    int increment(const Point &pt, int delta, const BBox &bounds);

    // Clear all cells in subtree
    void clearAll();

    // Print out structure of a node and recursively dump subtrees
    void dump(const BBox &bounds);
  
    // Grading function.  Do not implement!
    bool inspect(QTQuad *(&quads)[QTQ_NUM_QUADS],
		 int (*&cells)[QTQ_GRID_DIM]);
  
    bool isLeaf();

    bool checkCells();

    void prune();
private:
  
    // false if this is a leaf node, i.e. holds an actual data grid
    //
    // If we are acting as is an internal node, this field holds pointers
    // to its 4 subquadrants; if this is a leaf node, these must all be NULL!
    QTQuad *m_quads[QTQ_NUM_QUADS];

    // Alternatively, if we are a leaf node, this field points to a
    // dynamically allocated N x N grid (the strange declaration allows
    // it to act as a 2-D array.  If internal node, m_cells must be NULL!
    int (*m_cells)[QTQ_GRID_DIM];

  QTQuad *m_parent;
    //
    // YOU CAN ADD ANY OTHER CONSTRUCTORS, MEMBER FUNCTIONS, MEMBER DATA, ETC.
    // AS NEEDED HERE.
    //


    friend QuadTree;
};
#endif
