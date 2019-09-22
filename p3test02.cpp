#include <iostream>
#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"
#include "QuadTree.h"

using namespace std;

int main(int argc, char **argv) {
    int data;

    // Board that covers (x=-64..-1, y=-64..-1)
    QuadTree basicBd = QuadTree(BBox(Point(-64, -64), 64));
    QuadTree::iterator it, itEnd;

    cout << "Starting configuration--default board:" << endl;
    basicBd.dump();

    basicBd.set(Point(-64, -64), 1);
    basicBd.set(Point(-1, -1), 1);
    basicBd.increment(Point(-1, -1), 1);
    cout << "Modified configuration--default board:" << endl;
    basicBd.dump();
    data = basicBd.get(Point(-64, -64));
    cout << "get(-64, -64): expected 1, returned " << data << endl;
    data = basicBd.get(Point(-1, -1));
    cout << "get(-1, -1): expected 2,  returned " << data << endl;
    // Do a "get" of a non-put() point (but should be in an existing grid)
    data = basicBd.get(Point(-63, -63));
    cout << "get(-63, -63): expected 0, returned " << data << endl;
    // Do a "get" of a virtual point (should be in an empty quadrant right
    // off the root
    data = basicBd.get(Point(-32, -31));
    cout << "get(-32, -31): expected 0, returned " << data << endl;
    // Do a "get" of a virtual point (should be in an empty quadrant several
    // levels down
    data = basicBd.get(Point(-64, -60));
    cout << "get(-64, -60): expected 0, returned " << data << endl;
    return 0;
}
