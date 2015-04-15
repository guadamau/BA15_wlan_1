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
               
   Desc:       We have a timer resolution of 10 ms here.
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

#include "mtimer.h"
#include "collect.h"
#include "evaluate.h"
#include "print.h"

#define PROC_PREFIX    "/proc/"

#define PATH_PID_MAX   "/proc/sys/kernel/pid_max"

#define DEF_STORE_PATH "/tmp"

#define MIN_TIME_VAL      1
#define MAX_TIME_VAL   7200

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

  stats_res_t** results_list = NULL;

  stats_res_overall_t* overall_stats = NULL;

  file_params_t** file_params_list = NULL;

  ttimer_t timer;

  const char* errstr;

  char proc_loc[ 16 ] = { '\0' };

  file_coords_t pid_max_coords;
  file_params_t pid_max_params;

  uint32_t i;

  args.pid             =  0;
  args.interval        = -1;
  args.no_intervals    =  0;
  args.verbosity_level =  0;
  /* init memory area and set the default store path */
  memset( args.store_path, '\0', PATH_LEN );
  strncpy( args.store_path, DEF_STORE_PATH, ( PATH_LEN - 1 ) );

  pid_max_coords.line = 0;
  pid_max_coords.column = 0;
  pid_max_params.path = PATH_PID_MAX;
  pid_max_params.coordinates = pid_max_coords;
  pid_max_params.column_delimiter = " ";
  pid_max_params.lines_count = 1;
  pid_max_params.columns_count[ 0 ] = 1;
  pid_max_params.file_content = 
    ( char* )read_file( pid_max_params.path );
  pid_max_params.value_type = VALUE_NUM;
  
  if( !file_exists( PATH_PID_MAX ) )
  {
    printf( "PID-Max file not found "
            "in the proc file-system. Using INT16_MAX: %d\n", INT16_MAX );
    pid_max = INT16_MAX;
  }
  else
  {
    prepare_file_matrix( &pid_max_params );
    get_proc_value( &pid_max_params );
    pid_max = ( pid_t )pid_max_params.value_num;
  }
  free( pid_max_params.file_content );

  /* 
   * Specifiying the expected options
   * The two options p and t expect numbers as argument.
   * */
  while( ( option = getopt( argc, argv, "p:i:n:v:P:" ) ) != -1 )
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
        args.interval = strtonum( optarg, MIN_TIME_VAL, MAX_TIME_VAL, &errstr );
        if( errstr )
        {
          errx( 1, "specified timespan is %s: %s\n", errstr, optarg );
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
      default:
      {
        print_usage();
        exit( EXIT_FAILURE );
        break;
      }
    }
  }
  
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

  results_list = ( stats_res_t** )malloc( sizeof( stats_res_t* ) * args.no_intervals );
  if( results_list == NULL )
  {
    perror( "Memory allocation failure. Exiting...\n" );
    exit( EXIT_FAILURE );
  }
  
  file_params_list = create_file_params( args );

  for( i = 0; i < args.no_intervals; i++ )
  {
    /* ============================================================== */
    
    startTTimer( timer );
    start_data = get_stat_vals( file_params_list );
    purge_file_contents( file_params_list );
    
    sleep( args.interval );
    
    end_data = get_stat_vals( file_params_list );
    purge_file_contents( file_params_list );
    stopTTimer( timer );
   
    /* ============================================================== */

    *( results_list + i ) = evaluate_interval_data( start_data, end_data, &timer );
    free( start_data );
    free( end_data );
    
    /* print data of each elapsed interval 
     * at this verbosity level */
    if( args.verbosity_level == 2 )
    {
      output_results_single( ( void* )stdout, *( results_list + i ), NULL );
    }

  }

  overall_stats = evaluate_overall_data( results_list, args.no_intervals, args.interval );
  
  if( args.verbosity_level == 1 || args.verbosity_level == 2 )
  {
    output_results_overall( ( void* )stdout, overall_stats, NULL );
  }
  
  output_results_all( args.store_path, results_list, args.no_intervals, FILE_PREFIX_1 );
  output_results_overall( args.store_path, overall_stats, FILE_PREFIX_2 );
  
  clear_file_params( file_params_list );

  /* clean up that ... */
  for( i = 0; i < args.no_intervals; i++ )
  {
    free( *( results_list + i ) );
  }
  free( results_list );

  free( overall_stats );
  
  return EXIT_SUCCESS;
}
