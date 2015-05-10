/*

=================================================================================

      ___           ___           ___           ___       ___           ___
     /\  \         /\  \         /\__\         /\__\     /\  \         /\__\
    /::\  \       /::\  \       /:/ _/_       /:/  /    /::\  \       /::|  |
   /:/\:\  \     /:/\:\  \     /:/ /\__\     /:/  /    /:/\:\  \     /:|:|  |
  /::\~\:\__\   /::\~\:\  \   /:/ /:/ _/_   /:/  /    /::\~\:\  \   /:/|:|  |__
 /:/\:\ \:|__| /:/\:\ \:\__\ /:/_/:/ /\__\ /:/__/    /:/\:\ \:\__\ /:/ |:| /\__\
 \:\~\:\/:/  / \/__\:\/:/  / \:\/:/ /:/  / \:\  \    \/__\:\/:/  / \/__|:|/:/  /
  \:\ \::/  /       \::/  /   \::/_/:/  /   \:\  \        \::/  /      |:/:/  /
   \:\/:/  /        /:/  /     \:\/:/  /     \:\  \       /:/  /       |::/  /
    \::/__/        /:/  /       \::/  /       \:\__\     /:/  /        /:/  /
     ~~            \/__/         \/__/         \/__/     \/__/         \/__/

=================================================================================

*/

/* *****************************************************************************
   File:       meas.c
   Authors:    leibupro@students.zhaw.ch | guadamau@students.zhaw.ch
   Date:       03.2015
   
   Purpose:    Monitor system- and user-time and further parameters 
               of a given process by its PID over a specified timespan.
               
   Desc:       We have use a timer resolution of 1 us here.
               To determine the resolution of the times-function,
               use "sysconf( _SC_CLK_TCK );".
               It will return the amount of clock ticks per second.
               To provide the requested system-values, we access
               the files on the /proc-filesystem.

   Linking:    needs bsd library (-lbsd) [to make use of the strtonum-function]
   ***************************************************************************** */

#include <bsd/stdlib.h>

#include <errno.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
#include <unistd.h>

#include "attrs.h"
#include "mtimer.h"
#include "collect.h"
#include "evaluate.h"
#include "print.h"

#define PROC_PREFIX    "/proc/"

#define PATH_PID_MAX   "/proc/sys/kernel/pid_max"

#define DEF_STORE_PATH "/tmp"

#define MIN_TIME_VAL          10   /* in milliseconds */
#define MAX_TIME_VAL   7200*1000   /* in milliseconds */

#define README_FILE    "README"


/* function prototypes */
void print_usage( void );
int file_exists( const char* file );


void print_usage( void )
{
  if( system( "less " README_FILE ) == -1 )
  {
    printf( "no README file present or less is not installed on this system." );
  }
}


int file_exists( const char* file ) 
{
  struct stat buf;
  return ( stat( file, &buf ) == 0 );
}


void check_opt_str_len( char* optarg )
{
  if( ( strlen( optarg ) + 1 ) > STD_STRING_MALLOC )
  {
    perror( "argument string too long, max. %d "
            "characters allowed\n" );
    print_usage();
    exit( EXIT_FAILURE );
  }
}


/* program entry point of meas */
int main( int argc, char* argv[] )
{
  int option =  0;
  pid_t pid_max = 0;
  cmd_args_t args;
 
  stats_t* start_data = NULL;
  stats_t* end_data = NULL;
  stats_t** data_snapshots = NULL;
  
  time_vals_t* time_values = NULL;

  stats_res_t** results_list = NULL;

  stats_res_overall_t* overall_stats = NULL;

  file_params_t** file_params_list = NULL;
  
  meas_buffer_meta_t buffers_metadata;
  meas_buffer_t** buffers = NULL;

  gtimer_t timer;
  gtimer_t overhead_timer;

  const char* errstr;

  char proc_loc[ 16 ] = { '\0' };

  uint32_t i;

  float interval_flt;

  args.pid             =  0;
  args.prp_mode        =  1;
  args.interval        = -1;
  args.no_intervals    =  0;
  args.verbosity_level =  0;
  /* init memory area and set the default store path */
  memset( args.store_path, '\0', PATH_LEN );
  strncpy( args.store_path, DEF_STORE_PATH, ( PATH_LEN - 1 ) );
  
  pid_max = INT16_MAX;
  
  /* 
   * Specifiying the expected options
   * The two options p and t expect numbers as argument.
   * */
  while( ( option = getopt( argc, argv, "p:i:n:v:P:m:" ) ) != -1 )
  {
    switch( option )
    {
      case 'p' :
      {
        check_opt_str_len( optarg );
        args.pid = strtonum( optarg, 1, pid_max, &errstr );
        if( errstr )
        {
          errx( 1, "specified PID is %s: %s\n", errstr, optarg );
          print_usage();
          exit( EXIT_FAILURE );
        }

        /* check if process with specified id exists. */
        strcat( proc_loc, PROC_PREFIX );
        strcat( proc_loc, optarg );

        if( !file_exists( proc_loc ) )
        {
          perror( "The process you specified does not exist.\n" );
          print_usage();
          exit( EXIT_FAILURE );
        }
        break;
      }
      case 'i' :
      {
        check_opt_str_len( optarg );
        interval_flt = strtof( optarg, NULL );
        
        if( errno )
        {
          perror( "Failed to parse the interval duration. Exiting.\n" );
          print_usage();
          exit( EXIT_FAILURE );
        }

        args.interval = ( ( int32_t )( interval_flt * 1000 ) );

        if( args.interval < MIN_TIME_VAL || args.interval > MAX_TIME_VAL )
        {
          perror( "Please choose a valid interval duration. Exiting. \n" );
          print_usage();
          exit( EXIT_FAILURE );
        }
        break;
      }
      case 'n' :
      {
        check_opt_str_len( optarg );
        args.no_intervals = strtonum( optarg, 1, UINT16_MAX, &errstr );
        if( errstr )
        {
          errx( 1, "specified amount of intervals is %s: %s\n", errstr, optarg );
          print_usage();
          exit( EXIT_FAILURE );
        }
        printf( "no of intervals: %d\n", args.no_intervals );
        break;
      }
      case 'v' :
      {
        check_opt_str_len( optarg );
        args.verbosity_level = strtonum( optarg, 0, 2, &errstr );
        if( errstr )
        {
          errx( 1, "specified verbosity level not supported %s: %s\n", errstr, optarg );
          print_usage();
          exit( EXIT_FAILURE );
        }
        break;
      }
      case 'P' :
      {
        check_opt_str_len( optarg );
        if( strlen( optarg ) >= PATH_LEN )
        {
          perror( "Specified path is too long. Use another location.\n" );
          print_usage();
          exit( EXIT_FAILURE );
        }
        else
        {
          strncpy( args.store_path, optarg, ( PATH_LEN - 1 ) );
        }
        break;
      }
      case 'm' :
      {
        check_opt_str_len( optarg );
        args.prp_mode = strtonum( optarg, 0, 1, &errstr );
        if( errstr )
        {
          errx( 1, "specify a valid mode. %s: %s\n", errstr, optarg );
          print_usage();
          exit( EXIT_FAILURE );
        }
        break;
      }
      default:
      {
        print_usage();
        exit( EXIT_FAILURE );
        break;
      }
    }
  }
  /* Initially set up the values we want to get from /proc. */
  set_prp_mode( args.prp_mode );
  init_proc_properties();
  
  if( args.pid == 0 || args.interval == -1 )
  {
    print_usage();
    exit( EXIT_FAILURE );
  }

  /*  if there was no no of intervals set, 
   *  at least one run has to be performed. */
  if( args.no_intervals == 0 )
  {
    args.no_intervals = 1;
  }

  /* check if output location exists */
  if( file_exists( args.store_path ) )
  {
    printf( "Using \"%s\" as file output location.\n\n", args.store_path );
  }
  else
  {
    perror( "The path you specified does not exist. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }

  time_values = ( time_vals_t* )malloc( sizeof( time_vals_t ) * ( args.no_intervals + 1 ) );
  if( time_values == NULL )
  {
    perror( "Memory allocation failure time values. Exiting...\n" );
    exit( EXIT_FAILURE );
  }
  
  data_snapshots = ( stats_t** )malloc( sizeof( stats_t* ) * ( args.no_intervals + 1 ) );
  if( data_snapshots == NULL )
  {
    perror( "Memory allocation failure. Exiting...\n" );
    exit( EXIT_FAILURE );
  }

  results_list = ( stats_res_t** )malloc( sizeof( stats_res_t* ) * args.no_intervals );
  if( results_list == NULL )
  {
    perror( "Memory allocation failure. Exiting...\n" );
    exit( EXIT_FAILURE );
  }
  
  file_params_list = create_file_params( args );
  
  buffers_metadata = get_meas_buffers_meta_data( file_params_list );
  buffers = init_meas_buffers( &buffers_metadata, ( uint16_t )args.no_intervals );
  
  /* capture measurement data into the measurement buffers */
  for( i = 0; i < ( args.no_intervals + 1 ); i++ )
  {
    /* ============================================================== */
    
    startGTimer( timer );
    
    startGTimer( overhead_timer );
    read_to_meas_buffers( buffers, buffers_metadata.unique_paths_count, i );
    stopGTimer( overhead_timer );

    usleep( ( args.interval * 1000 ) 
            - getWallGUTime( overhead_timer ) );
    
    stopGTimer( timer );

    ( time_values + i )->otime_usec = getWallGUTime( overhead_timer );
    ( time_values + i )->etime_sec = getWallGTime( timer );

    /* ============================================================== */
  }

  /* perpare the measurement snapshots for evaluation */
  for( i = 0; i < ( args.no_intervals + 1 ); i++ )
  {
    *( data_snapshots + i ) = get_stat_vals( file_params_list, buffers, 
                                             buffers_metadata.unique_paths_count, i );
  
    /* pass timevalues to data snapshot */
    ( *( data_snapshots + i ) )->otime_usec = ( time_values + i )->otime_usec;
    ( *( data_snapshots + i ) )->etime_sec = ( time_values + i )->etime_sec;
  }

  /* evaluate all the snapshot data */
  for( i = 1; i <= args.no_intervals; i++ )
  {  
    start_data = *( data_snapshots + ( i - 1 ) );
    end_data = *( data_snapshots + i );
    *( results_list + ( i - 1 ) ) = evaluate_interval_data( start_data, end_data, 
                                                            ( uint16_t )( i - 1 ) );
    
    /* print data of each elapsed interval 
     * at this verbosity level */
    if( args.verbosity_level == 2 )
    {
      output_results_single( ( void* )stdout, *( results_list + ( i - 1 ) ), NULL, prp_enabled );
    }
    start_data = NULL;
    end_data = NULL;
  }
  
  release_meas_buffers( &buffers_metadata, buffers );
  
  /* clean up the data snapshots */
  for( i = 0; i < ( args.no_intervals + 1 ); i++ )
  {
    free( *( data_snapshots + i ) );
    *( data_snapshots + i ) = NULL;
  }
  free( data_snapshots );
  data_snapshots = NULL;

  overall_stats = evaluate_overall_data( results_list, args.no_intervals, args.interval );
  
  if( args.verbosity_level == 1 || args.verbosity_level == 2 )
  {
    output_results_overall( ( void* )stdout, overall_stats, NULL, prp_enabled );
  }
  
  output_results_all( args.store_path, results_list, args.no_intervals, FILE_PREFIX_1, prp_enabled );
  output_results_overall( args.store_path, overall_stats, FILE_PREFIX_2, prp_enabled );
  
  clear_file_params( file_params_list );

  /* clean up that ... */
  free( time_values );

  for( i = 0; i < args.no_intervals; i++ )
  {
    free( *( results_list + i ) );
  }
  free( results_list );

  free( overall_stats );
  
  return EXIT_SUCCESS;
}
