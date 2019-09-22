#include <iostream>
#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"
#include "QuadTree.h"

using namespace std;

int main(int argc, char **argv) {
    int data;
    QTQuad *rootNode;
    BBox userBounds, quadBounds;

    // Following is proper order for traversal
    Point pts[] = {
	Point(256, 256), // dummy, not to be used
	Point(0, 0),
	Point(200, 10),
	Point(47, 147),
	Point(253, 254),
	Point(254, 253),
	Point(254, 254),
    };
    int ndx;
    // Large 256x256 board
    QuadTree basicBd = QuadTree(BBox(Point(0, 0), 256));
    QuadTree::iterator it, itEnd;
    QTQuad *quads[QTQ_NUM_QUADS];
    int (*cells)[QTQ_GRID_DIM];

    cout << "Starting configuration--default board:" << endl;
    basicBd.dump();

    basicBd.set(pts[1], 1);     // Quadrant I
    basicBd.set(pts[6], 6);     // Quadrant IV->IV->...IV
    basicBd.set(pts[5], 5);     // Quadrant IV->IV->...IV
    basicBd.set(pts[4], 4);     // Quadrant IV->IV->...IV
    basicBd.set(pts[2], 2);     // Quadrant II
    basicBd.set(pts[3], 3);     // Quadrant III

    cout << "Modified configuration--default board:" << endl;
    basicBd.dump();

    itEnd = basicBd.end();
    cout << "Test of iteration over non-zero points in basicBd:" << endl;
    // Must retrieve in following order:
    //   (0, 0) = 1
    //   (200, 10) = 2
    //   (47, 147) = 3
    //   (253, 254) = 4
    //   (254, 253) = 5
    //   (254, 254) = 6
    ndx = 1;
    for (it = basicBd.begin(); it != itEnd; ++it) {
	Point pt = *it;
	cout << "(" << pt.m_x << ", " << pt.m_y << ") = " <<
	    (data = basicBd.get(pt));
	if (pt == pts[ndx] && data == ndx++) {
	    cout << " OK" << endl;
	} else {
	    cout << " WRONG PT/ORDER/DATA" << endl;
	}
    }

    // Just to make sure these interfaces work
    basicBd.inspect(rootNode, userBounds, quadBounds);
    rootNode->inspect(quads, cells);
    basicBd.clearAll();
    cout << "After clearAll():\n";
    basicBd.dump();
    cout << ">>>END OF PROGRAM<<<\n";
    return 0;
}

bool QuadTree::inspect(QTQuad * &root, BBox &bounds, BBox &qBounds) {
    root = m_root;
    bounds = m_bounds;
    qBounds = m_qBounds;
    return true;
}

bool QTQuad::inspect(QTQuad *(&quads)[QTQ_NUM_QUADS],
		     int (*&cells)[QTQ_GRID_DIM]) {
    for (int q = 0; q < QTQ_NUM_QUADS; q++) {
	quads[q] = m_quads[q];
    }
    cells = m_cells;
    return true;
}
