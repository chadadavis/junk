#include "t.h"

// define what exceptions can be thrown
void hand(int n) throw (const char*, int) {
    cout << "Damn, got a signal" << endl;
    throw("Doh!");
}

// pointer to void function
// typdefs are local to file, generally should be in header file
typedef void (*pfunc)();

// A function that takes a func. pointer
int want_func(pfunc* f) {
    (*f)(); // calls the func
}

class A {}; class B : public A {};

// inlining is also done when definition is inside of class body
inline int c::func(const int* p, const int& n) const {
    // do nothing
}

int main(int argc, char** argv) {

    cout << "Gimme a line: " << flush;
    char* liner = new char[3];
    cin.get(liner, 3); // '\n' is discarded
    cin.ignore(1000, '\n'); // need this to trash remainder of line (overflow)
    cout << "Got:\n" << liner << ":" << endl;

    cout << "enter something in hex: " << flush;
    int hexer;
    cin >> hex >> hexer;
    if (cin.fail()) { exit(1); }
    cout << "in oct: " << setw(20) << setfill('_') << oct << hexer << endl;

    clog << "Logging data to cerr (but buffered by default with clog)" << endl;

    ifstream my_input(argv[1]);

    vector<int> vic;
    vic.push_back(2);

    try {
        A a, *pa;
        pa = &a;
        //        B* pb = dynamic_cast<B*>(pa); // caught already by compiler!
    } catch (bad_cast) { // simply specifying the type is allowed (nameless var)
        cout << "bad_cast" << endl; 
    }

    //    signal(SIGINT, SIG_IGN);
    signal(SIGINT, hand);
    cout << "Sleeping ... ";
    cout.flush();
    try {
        sleep(1);
    } catch (...) { // catches any/all exceptions
        cout << "Caught 'ya" << endl;
    }
    cout << endl;

    int ar[5]; // const pointer
    int* p = ar; // non-const equivalent
    cout << "sizeof(ar) " << sizeof(ar) << " sizeof(p) " << sizeof(p) << endl;
    // get type names (strings)
    cout << "type(ar) " << typeid(ar).name() 
         << " type(p) " << typeid(p).name() << endl;

    // neigboring strings implicitly concatenated, even over line breaks
    cout << "smack me " 
    "down" << endl;
    
    // value is const and so is its pointer, 'const' operates to the left!
    int n = 5;
    int const * const pn = &n;
    //    pn = &o;  // can't change what pointer points to
    //    *pn = 2;  // can't change value, pointed to by pointer

    // auto vars
    c chuck, charley;
    chuck.func();

    cout << "chucks: " << chuck.get_counter() << endl;

    // heap vars, based on copy of chuck
    c* c2 = new c(chuck);
    delete c2;

    char* m;
    m = new char[40];
    // reaping array syntax
    delete [] m;

    cout << "int(max): " << numeric_limits<int>::max() <<endl;
    return(0);
} // int main(int argc, char** argv) 
