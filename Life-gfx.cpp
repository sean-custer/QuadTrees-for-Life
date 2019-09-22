/******************************************************************************
 * Program to implement Game of Life optimized for Proj3's QuadTree
 * This version implements a toroidal game board with wraparound at the
 * north/south and east/west edges (thus all the modulo division)
 *****************************************************************************/

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"
#include "QuadTree.h"

using namespace std;

static bool readPoint(Point &pt);
void display(QuadTree *bd, BBox &bounds, int gen, int cnt, int SleepTime);

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
	cerr << "Syntax: " << argv[0] << " originX originY dim gens [sleepTime]\n";
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

    cnt = 0;
    while (readPoint(pt)) { // Get coords from user or file
	srcBoard->set(pt, 1);
	cnt++;
    }
    display(srcBoard, bdBBox, 0, 0, arg5);
    //srcBoard->dump();

    for (gen = 1; gen < numGens; gen++) {
	cnt = 0;
	dstBoard->clearAll();

	// Now, do actual heart of update algorithm: iterate over all currently
	// live cells and compute their contributions to next-gen board
	itEnd = srcBoard->end(); // Cache this for efficiency
	for (it = srcBoard->begin(); it != itEnd; ++it) {
	    cellPt = *it;
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
	display(dstBoard, bdBBox, gen, cnt, arg5);

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

#define SCR_WIDTH 79
#define SCR_HEIGHT 38

void display(QuadTree *bd, BBox &bounds, int gen, int cnt, int sleepTime) {
    int minX = bounds.m_bL.m_x + bounds.m_dim, maxX = bounds.m_bL.m_x - 1,
	minY = bounds.m_bL.m_y + bounds.m_dim, maxY = bounds.m_bL.m_y - 1;
    static int startX = 0, startY = 0;
    int oldStartX, oldStartY, xCnt, yCnt;
    QuadTree::iterator it, itEnd;
    Point cellPt;
    int val;
    char c;

    xCnt = min(SCR_WIDTH, (int) bounds.m_dim);
    yCnt = min(SCR_HEIGHT, (int) bounds.m_dim);
    oldStartX = startX, oldStartY = startY;
    if (bounds.m_dim > SCR_WIDTH || bounds.m_dim > SCR_HEIGHT) {
	// Do some screen centering
	itEnd = bd->end();
	for (it = bd->begin(); it != itEnd; ++it) {
	    cellPt = *it;
	    if (cellPt.m_x < minX)  minX = cellPt.m_x;
	    if (cellPt.m_x > maxX)  maxX = cellPt.m_x;
	    if (cellPt.m_y < minY)  minY = cellPt.m_y;
	    if (cellPt.m_y > maxY)  maxY = cellPt.m_y;
	}
	if (minX < startX || maxX > startX + SCR_WIDTH) {
	    startX = min(minX, bounds.m_bL.m_x + (int) (bounds.m_dim - xCnt));
	}
	if (minY < startY || maxY > startY + SCR_HEIGHT) {
	    startY = min(minY, bounds.m_bL.m_y + (int) (bounds.m_dim - yCnt));
	}
    }
    if (gen == 0) {
	cout << "\033[2J\033[H" << "Initial board:\n";
    } else {
	cout << "\033[H\033[2KGeneration " << gen << ": cnt = " << cnt << "\n";
    }
    for (int y = 0; y < yCnt; y++) {
	for (int x = 0; x < xCnt; x++) {
	    val = bd->get(Point(startX + x, startY + yCnt - 1 - y));
	    c = (val > 0 ? 'O' : (val < 0 ? '-' : '.'));
	    cout << c;
	}
	cout << '\n';
    }
    if (startX != oldStartX || startY != oldStartY) {
	cout << "\033[7m";
    }
    cout << "\033[KOrigin: (" << startX << ", " << startY <<
	")\033[m" << std::flush;
    if (sleepTime > 0) {
	sleep(sleepTime);
    }
}

    
