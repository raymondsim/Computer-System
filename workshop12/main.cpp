#include <time.h>
#include <iostream>

using namespace std ;

// iteration code is in a separate source file
extern int iterate(int *data,int *indices,int length) ;

// generate an array - and initialise it
int *generate_data(int length)
{
    int *data = new int[length] ;

    for ( int i = 0 ; i < length ; i++ )
    {
        data[i] = 0 ;
    }

    return data ;
}

// generate an array of indices - 0 to length - 1
int *generate_indices_asc(int length)
{
    int *indices = new int[length] ;

    for ( int i = 0 ; i < length ; i++ )
    {
        indices[i] = i ;
    }

    return indices ;
}

// generate an array of indices - length - 1 down to 0
int *generate_indices_dsc(int length)
{
    int *indices = new int[length] ;
    int last_index = length ;

    for ( int i = 0 ; i < length ; i++ )
    {
        indices[i] = --last_index ;
    }

    return indices ;
}

// generate an array of indices - random numbers from 0 to length - 1
int *generate_indices_random(int length)
{
    int *indices = new int[length] ;

    for ( int i = 0 ; i < length ; i++ )
    {
        indices[i] = rand() % length ;
    }

    return indices ;
}

double real_time()
{
    struct timespec realtime ;

    clock_gettime(CLOCK_REALTIME,&realtime) ;

    return realtime.tv_sec + realtime.tv_nsec / 1000000000.0 ;
}

// usage error
void usage(string error)
{
    cout << "usage: loop <data length> <number of iterations> asc|dsc|random" << endl ;
    cout << error << endl ;
    exit(-1) ;
}

// main program - generate data and iterate over it in a specified order
int main(int argc,char **argv)
{
    // we must have 4 parameters, executable is first argument
    if ( argc < 4 ) usage("there must be 3 parameters") ;

    // check length parameter is > 0
    int length = stoi(argv[1]) ;
    if ( length <= 0 ) usage("data length must be > 0" ) ;

    // check iterations parameter is > 0
    int iterations = stoi(argv[2]) ;
    if ( iterations <= 0 ) usage("number of iterations must be > 0" ) ;

    // check requested ordering
    string ordering = argv[3] ;

    // pointers to the data array and indices array
    int *data = generate_data(length) ;
    int *indices ;
    if (  ordering == "asc" )
    {
        indices = generate_indices_asc(length) ;
    }
    else if (  ordering == "dsc" )
    {
        indices = generate_indices_dsc(length) ;
    }
    else if (  ordering == "random" )
    {
        indices = generate_indices_random(length) ;
    }
    else
    {
        usage("iteration ordering must be one of asc, dsc or random") ;
    }

    //cout << "Starting iterations\n" ;

    // record start time - in seconds
    double start = real_time() ;

    // iterate over data in specified order iterations number of times
    for ( int i = 0 ; i < iterations ; i++ )
    {
        iterate(data,indices,length) ;
    }

    // record stop time - in seconds
    double stop = real_time() ;

    //cout << "Finished iterations\n" ;

    // report time taken and read rate in millions per second
    double time = stop - start ;
    int rate = (int) (length * (double) iterations / time / 1000000.0) ;

    cout << argv[0] << "," << iterations << "," << length << "," << ordering << "," << time << "," << rate << "," ;
}

