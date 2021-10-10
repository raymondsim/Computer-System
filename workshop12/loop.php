<?php

// these match the C++ implementations
function iterate($data,$indices,$length)
{
    for ( $i = 0 ; $i < $length ; $i++ )
    {
        $x = $data[$indices[$i]] ;
    }
}

function generate_data($length)
{
    $data = array() ;

    for ( $i = 0 ; $i < $length ; $i++ )
    {
        $data[$i] = 0 ;
    }

    return $data ;
}

function generate_indices_asc($length)
{
    $indices = array() ;

    for ( $i = 0 ; $i < $length ; $i++ )
    {
        $indices[$i] = $i ;
    }

    return $indices ;
}

function generate_indices_dsc($length)
{
    $indices = array() ;
    $last_index = $length ;

    for ( $i = 0 ; $i < $length ; $i++ )
    {
        $indices[$i] = --$last_index ;
    }

    return $indices ;
}

function generate_indices_random($length)
{
    $indices = array() ;

    for ( $i = 0 ; $i < $length ; $i++ )
    {
        $indices[$i] = rand(0,$length-1) ;
    }

    return $indices ;
}

// usage error
function usage($error)
{
    print "usage: loop <data length> <number of iterations> <iteration ordering>\n" ;
    print "{$error}\n" ;
    exit(-1) ;
}

// main program - generate data and iterate over it in a specified order
function main()
{
    // we must have 4 parameters, executable is first argument
    if ( $_SERVER['argc'] < 4 ) usage("there must be 3 parameters") ;

    // check length parameter is > 0
    $length = $_SERVER['argv'][1] ;
    if ( $length <= 0 ) usage("data length must be > 0" ) ;

    // check iterations parameter is > 0
    $iterations = $_SERVER['argv'][2] ;
    if ( $iterations <= 0 ) usage("number of iterations must be > 0" ) ;

    // check requested ordering
    $ordering = $_SERVER['argv'][3] ;

    // the data array and indices array - allow 8GB of memory
    ini_set('memory_limit','8192M');
    $data = generate_data($length) ;
    if (  $ordering == "asc" )
    {
        $indices = generate_indices_asc($length) ;
    }
    else if (  $ordering == "dsc" )
    {
        $indices = generate_indices_dsc($length) ;
    }
    else if (  $ordering == "random" )
    {
        $indices = generate_indices_random($length) ;
    }
    else
    {
        usage("iteration ordering must be one of asc, dsc or random") ;
    }

    //print "Starting iterations\n" ;

    // record start time - in seconds
    $start = gettimeofday(TRUE) ;

    // iterate over data in specified order iterations number of times
    for ( $i = 0 ; $i < $iterations ; $i++ )
    {
        iterate($data,$indices,$length) ;
    }

    // record stop time - in seconds
    $stop = gettimeofday(TRUE) ;

    //print "Finished iterations\n" ;

    // report time taken and read rate in millions per second
    $time = $stop - $start ;
    $rate = $length * $iterations / 1000000.0 / $time ;

    $time = number_format($time,2) ;
    $rate = number_format($rate,0) ;

    print "php loop.php,{$iterations},{$length},{$ordering},{$time},{$rate}," ;
}

main() ;

?>
