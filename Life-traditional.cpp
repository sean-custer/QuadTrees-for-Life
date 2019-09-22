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
void dump(int **bd, int dim);

int
main(int argc, char **argv) {
    BBox bdBBox;
    Point pt;
    int x, y;
     int dim;
    int numGens, gen;
    // Set up for ping pong-ing
    int **srcBoard, **dstBoard, **tmpBoard;
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

    // Set up for ping pong-ing
    srcBoard = new int*[dim];
    for (x = 0; x < dim; x++) {
	srcBoard[x] = new int[dim];
	for (y = 0; y < dim; y++) {
	    srcBoard[x][y] = 0;
	}
    }
    dstBoard = new int*[dim];
    for (x = 0; x < dim; x++) {
	dstBoard[x] = new int[dim];
	for (y = 0; y < dim; y++) {
	    dstBoard[x][y] = 0;
	}
    }

    while (readPoint(pt)) { // Get coords from user or file
	srcBoard[pt.m_x][pt.m_y] = 1;
    }

    for (gen = 1; gen < numGens; gen++) {
	cnt = 0;
	for (x = 0; x < dim; x++) {
	    for (y = 0; y < dim; y++) {
		int sum = 0;

		for (int xOff = -1; xOff <= 1; xOff++) {
		    for (int yOff = -1; yOff <= 1; yOff++) {
			if (xOff == 0 && yOff == 0) continue;
			// Implement wraparound for toroidal board
			sum += srcBoard[(x + xOff + dim) % dim]
			    [(y + yOff + dim) % dim];
		    }
		}
		if (srcBoard[x][y] == 1 && (sum == 2 || sum == 3)) {
		    dstBoard[x][y] = 1;
		    cnt++;
		} else if (srcBoard[x][y] == 0 && sum == 3) {
		    dstBoard[x][y] = 1;
		    cnt++;
		} else {
		    dstBoard[x][y] = 0;
		}
	    }
	}
	if (arg5) cout << endl << endl << "Generation " << gen <<
		      ": cnt = " << cnt << endl;
	if (arg5) dump(dstBoard, dim);
	// ping-pong to other board
	tmpBoard = srcBoard;
	srcBoard = dstBoard;
	dstBoard = tmpBoard;
    }
    cout << "\nCompleted " << numGens << " generations; final cnt = " <<
	cnt << endl;
}

void
dump(int **bd, int dim) {
    for (int y = dim - 1; y >= 0; --y) {
	cout << '[';
	for (int x = 0; x < dim; x++) {
	    cout << (bd[x][y] > 0 ? 'O' : '.');
	}
	cout << "]\n";
    }
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

    
