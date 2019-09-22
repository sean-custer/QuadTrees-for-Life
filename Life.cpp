/******************************************************************************
 * Program to implement Game of Life optimized for Proj3's QuadTree
 * This version implements a toroidal game board with wraparound at the
 * north/south and east/west edges (thus all the modulo division)
 *****************************************************************************/

#include <iostream>
#include <cstdlib>
#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"
#include "QuadTree.h"

using namespace std;

static bool readPoint(Point &pt);

int
main(int argc, char **argv) {
    BBox bdBBox;
    Point pt;
    QuadTree *tmp;
    QuadTree::iterator it, itEnd;
    int x, y;
    unsigned int dim;
    int numGens, gen;
    // Set up for ping pong-ing
    QuadTree *srcBoard, *dstBoard;
    Point cellPt;
    int cnt;
    int arg5 = 1;

    if (argc < 5) {
	cerr << "Syntax: " << argv[0] << " originX originY dim gens [silent]\n";
	return 1;
    }
    bdBBox.m_bL.m_x = atoi(argv[1]);
    bdBBox.m_bL.m_y = atoi(argv[2]);
    bdBBox.m_dim = dim = atoi(argv[3]);
    numGens = atoi(argv[4]);
    if (argc > 5) {
	arg5 = atoi(argv[5]);
    }
    // Usually like to declare at top, but for efficiency's sake, want to
    // defer construction until here
    QuadTree board1(bdBBox);
    QuadTree board2(bdBBox);

    // Set up for ping pong-ing
    srcBoard = &board1, dstBoard = &board2;

    if (arg5) board1.dump();
    cnt = 0;
    while (readPoint(pt)) { // Get coords from user or file
	srcBoard->set(pt, 1);
	cnt++;
    }
    if (arg5) cout << endl << endl << "Starting configuration:" << endl;
    if (arg5) srcBoard->dump();

    for (gen = 1; gen < numGens; gen++) {
	cnt = 0;
	dstBoard->clearAll();

	// Now, do actual heart of update algorithm: iterate over all currently
	// live cells and compute their contributions to next-gen board
	itEnd = srcBoard->end(); // Cache this for efficiency
	for (it = srcBoard->begin(); it != itEnd; ++it) {
	    cellPt = *it;
	    // cout << cellPt; // DEBUG
	    dstBoard->increment(cellPt, 10);  // indicates "live"
	    for (int xOff = -1; xOff <= 1; xOff++) {
		for (int yOff = -1; yOff <= 1; yOff++) {
		    if (xOff == 0 && yOff == 0) continue;
		    // Implement wraparound for toroidal board
		    x = (cellPt.m_x + xOff + dim) % dim;
		    y = (cellPt.m_y + yOff + dim) % dim;
		    dstBoard->increment(Point(x, y), 1);
		}
	    }
	}
	if (arg5) cout << endl << endl << "\nGeneration " << gen << " RAW:\n";
	if (arg5) dstBoard->dump();                          // DEBUG
	// POST-PROCESS dstBoard;
	// Note that our definition of "live cell" is any cell > 0,
	// so we don't have to modify values > 1 down to 1.
	//
	itEnd = dstBoard->end(); // Cache this for efficiency
	for (it = dstBoard->begin(); it != itEnd; ++it) {
	    cellPt = *it;
	    int newVal = dstBoard->get(cellPt);

	    if (newVal != 3 && newVal != 12 && newVal != 13) {
		dstBoard->set(cellPt, 0);
	    } else {
		cnt++;
	    }
	    // No "else" necessary: remaining non-0 cells are considered live;
	    // we don't have to actually reset them to exactly 1
	}
	if (arg5) cout << endl << endl << "Generation " << gen <<
		      ": cnt = " << cnt << endl;
	if (arg5) dstBoard->dump();

	// ping-pong to other board
	tmp = srcBoard;
	srcBoard = dstBoard;
	dstBoard = tmp;
    }
    cout << "\nCompleted " << numGens << " generations; final cnt = " <<
	cnt << endl;
}

static bool
readPoint(Point &pt) {
    if (cin.eof())
	return false;
    else {
	cin >> pt.m_x >> pt.m_y;
	return true;
    }
}

    
