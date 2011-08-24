// ModemSim clas interface:run asimulation
//
// CONSTRUCTION: with (a) three parameters: the number of
//     modems, the average connect time, and the
//     interarrival time or (b) another ModemSim object
//
// ******************PUBLIC OPERATIONS*********************
// void Run( )          --> Run a simulation

#include <limits.h>
#include <stdlib.h>
#include <iostream.h>
#include "Heap.h"
#include "Random.h"

#ifdef __GNUC__
    #include "Heap.cpp"
    #include "Random.cpp"
#endif

class Event
{
    enum { DialIn = 1, Hangup = 2 };  // Moved from text location
                                      // to avoid g++ bug
  public:
    Event( int Name = 0, long Tm = 0, int Type = DialIn ) :
        Time( Tm ), Who( Name ), What( Type ) { }

    int operator<( const Event & Rhs ) const
        { return Time < Rhs.Time; }

    friend class ModemSim;
  private:
    int Who;        // the number of the user
    long Time;      // when the event will occur
    int What;       // Dial in or hangup
};

class ModemSim
{
  public:
    ModemSim( int Modems, double AvgLen, long CallIntrvl );

        // Add a call to EventSet at the current time,
        // and schedule one for Delta in the future.
    void NextCall( long Delta );

        // Run the simulation
    void Run( long StoppingTime = LONG_MAX );
  private:
    Random R;                       // A random source
    BinaryHeap<Event> EventSet;     // Pending events

        // Basic parameters of the simulation
    int FreeModems;                 // Number of modems unused
    const double AvgCallLen;        // Length of a call
    const long FreqOfCalls;         // Interval between calls
};

// Constructor for ModemSim

ModemSim::ModemSim( int Modems, double AvgLen, long CallIntrvl ) :
    EventSet( Event( 0 ) ),
    FreeModems( Modems ),
    AvgCallLen( AvgLen ),
    FreqOfCalls( CallIntrvl )
{
    NextCall( FreqOfCalls );  // Schedule first call
}

// Place a new DialIn event intothe event queue
// Then advance the time when next DialIn event will occur
// In practice, we would use a random number to set the time

void
ModemSim::NextCall( long Delta )
{
    static long NextCallTime = 0;
    static int UserNum = 0;

    EventSet.Insert( Event( UserNum++, NextCallTime ) );
    NextCallTime += Delta;
}

// Run the simulation until Stopping time occurs
// Print output as in Figure 13.4

void
ModemSim::Run( long StoppingTime )
{
    static Event E;
    long HowLong;

    while( !EventSet.IsEmpty( ) )
    {
        EventSet.DeleteMin( E );
        if( E.Time > StoppingTime )
            break;

        if( E.What == Event::Hangup )    // HanUup
        {
            FreeModems++;
            cout << "User " << E.Who << " hangs up at time "
                << E.Time << '\n';
        }
        else                             // DialIn
        {
            cout << "User " << E.Who << " dials in at time "
                << E.Time << " ";
            if( FreeModems > 0 )
            {
                FreeModems--;
                HowLong = R.Poisson( AvgCallLen );
                cout << "and connects for "
                    << HowLong << " minutes\n";
                E.Time += HowLong;
                E.What = Event::Hangup;
                EventSet.Insert( E );
            }
            else
                cout << "but gets busy signal\n";

            NextCall( FreqOfCalls );
        }
    }
}

// Quickie main for testing purposes

main( )
{
#ifndef NoExceptions

  try
  {
#endif

    ModemSim S( 3, 5.0, 1 );
    S.Run( 20 );
    return 0;

#ifndef NoExceptions
  }

  catch( ... )
  {
    cerr << "Out of memory!!" << endl;
    exit( 1 );
  }
#endif

}
