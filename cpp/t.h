#ifndef T_H
#define T_H

#include <iostream>
#include <string>
#include <limits> // numeric_limits<int>::max()
#include <typeinfo> // for typeid()
using namespace std;

#include <assert.h>
#include <time.h> // time()
#include <stdlib.h> // rand()

class c {
 public:
    // constructors and destructors:
    c() : bob(2) { counter++; } // init. list sets bob=2 (since bob is const)
    explicit c(int n) : bob(2) {} // does not allow implicit conversion from int
    c(const c& src) : bob(src.bob) {} // copy constructor, same as default
    operator int() { return bob; } // for casting a c instance to an int
    ~c() {}

    // an inlined function
    int get_counter() const { return counter; }
    // a func. decl.
    int func(const int* p = NULL, const int& n = 0) const;

 protected:

    static int counter;
    const int bob;

};

// file scope declaration still necessary
// counter is accessible, though private, here since this is the definition
int c::counter = 0;

#endif
