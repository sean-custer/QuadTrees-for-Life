//my own test

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
  A.get(Point(14,11));
  it = A.begin();
  cout << (*it) << endl;
  
}
