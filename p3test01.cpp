#include <iostream>
#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"
#include "QuadTree.h"

using namespace std;

int main(int argc, char **argv) {

  QuadTree A;
  QuadTree::iterator it;


  A.set(Point(14,11), 6);
  A.set(Point(12,11), 8);
  
  it = A.begin();
  cout << A.get(*it) << endl;
  
  it = A.end();
  cout << A.get(*it) << endl;
  
  
  /*int data;

    // test default board construction: 16x16 w/origin (0,0)
    QuadTree basicBd = QuadTree();
    QuadTree::iterator it, itEnd;

    cout << "Starting configuration--default board:" << endl;
    basicBd.dump();

    //cout << "Starting configuration--default board:" << endl;
    //bigBd.dump();

    basicBd.set(Point(0, 0), 1);
    basicBd.set(Point(15, 15), 1);
    basicBd.increment(Point(15, 15), 1);
    cout << "Modified configuration--default board:" << endl;
    basicBd.dump();
    data = basicBd.get(Point(0, 0));
    cout << "get(0, 0): expected 1, returned " << data << endl;
    data = basicBd.get(Point(15, 15));
    cout << "get(15, 15): expected 2,  returned " << data << endl;
    // Do a "get" of a non-put() point (but should be in an existing grid)
    data = basicBd.get(Point(1, 1));
    cout << "get(1, 1): expected 0, returned " << data << endl;
    // Do a "get" of a virtual point (should be in an empty quadrant right
    // off the root
    data = basicBd.get(Point(8, 7));
    cout << "get(8, 7): expected 0, returned " << data << endl;
    // Do a "get" of a virtual point (should be in an empty quadrant several
    // levels down
    data = basicBd.get(Point(4, 0));
    cout << "get(4, 0): expected 0, returned " << data << endl;
    return 0; 
  

  */
}
