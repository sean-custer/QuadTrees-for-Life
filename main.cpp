#include "QuadTree.cpp"
#include "QTQuad.cpp"
#include <iostream>

int main(int argc, char** argv){
  QTQuad *Test = new QTQuad();
  Test->set(Point(0,0),5,BBox(Point(0,0),64));
  cout << Test->get(Point(0,0), BBox(Point(0,0),64)) << endl;
}
