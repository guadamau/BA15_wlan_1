/* *****************************************************************************
   File:       evaluate.c
   Authors:    leibupro
               guadamau
   Date:       03.2015
   
   Purpose:    Evaluate statistics data from /proc-filesystem. 
               
   Desc:       -
   ***************************************************************************** */

#include <sys/times.h>

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "evaluate.h"

/* "private" function prototypes */
void check_min( double* current, double* actual, uint16_t* interval, uint32_t i );
void check_max( double* current, double* actual, uint16_t* interval, uint32_t i );
void check_min_uint( uint32_t* current, uint32_t* actual, uint16_t* interval, uint32_t i );
void check_max_uint( uint32_t* current, uint32_t* actual, uint16_t* interval, uint32_t i );


stats_res_t* evaluate_interval_data( stats_t* start_data, stats_t* end_data, uint16_t interval_no )
{
  stats_res_t* results = NULL;

  /* clock ticks per second */
  long clktck;

  results = ( stats_res_t* )malloc( sizeof( stats_res_t ) );
  
  if( results == NULL )
  {
    /* Should never come here. */
    perror( "Memory allocation failure. Exiting now.\n" );
    exit( EXIT_FAILURE );
  }
  
  clktck = sysconf( _SC_CLK_TCK );

  if( start_data->pid != end_data->pid )
  {
    perror( "PID of start and end data statistics must match. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }


  /* set the interval number to a result set */
  results->interval_no = interval_no;
  /* ****************************************** */
  
  /* set the pid to a result set */
  results->pid = start_data->pid;
  /* ****************************************** */
  
  /* add elapsed time to the result set */
  results->etime_sec = start_data->etime_sec;
  /* ****************************************** */
  
  /* add overhead time to the result set */
  results->otime_usec = start_data->otime_usec;
  /* ****************************************** */
  
  /* utime and stime */
  if( clktck > 0 )
  {
    results->utime_sec = ( end_data->utime - start_data->utime ) / ( double )clktck;
    results->stime_sec = ( end_data->stime - start_data->stime ) / ( double )clktck;
  }
  else
  {
    perror( "Cant calculate time values. \n" 
            "Couldn't determine Clock ticks per second.\n"
            " Exiting now ..." );
    exit( EXIT_FAILURE );
  }
  /* ****************************************** */

  /* cpu percentage */
  if( results->etime_sec > 0.0 )
  {
    results->cpu_percent = ( results->utime_sec + results->stime_sec ) 
                           / results->etime_sec * 100;
  }
  else
  {
    results->cpu_percent = 0.0;
  }
  /* ****************************************** */
  
  /* interface names were allready set in collect.c
   * we can just memcpy here. */

  memcpy( results->if_name_prp,     /* destination */
          start_data->if_name_prp,  /* source */
          IF_NAME_LEN );            /* length */
  
  memcpy( results->if_name_if0,     /* destination */
          start_data->if_name_if0,  /* source */
          IF_NAME_LEN );            /* length */
  
  memcpy( results->if_name_if1,     /* destination */
          start_data->if_name_if1,  /* source */
          IF_NAME_LEN );            /* length */
  
  /* ****************************************** */

  /* machine wide network data, not process specific */

  if( results->etime_sec > 0.000000 )
  {

    /* 
     * The values about the prp interface in the 
     * /proc-filesystemhave have a width of 32 bit.
     * So if UINT32_MAX has reached, an overflow occurs
     * in the /proc byte statistics.
     *
     * In our resultset struct we use signed 64 bit integers,
     * which let us easily correct the byte value
     * in case of an overflow.
     *
     * */
    if( end_data->rx_bytes_prp < start_data->rx_bytes_prp )
    {
      results->rx_bytes_prp =   ( UINT32_MAX + end_data->rx_bytes_prp )
                              - start_data->rx_bytes_prp;
    }
    else
    {
      results->rx_bytes_prp = end_data->rx_bytes_prp - start_data->rx_bytes_prp; 
    }
    
    if( end_data->tx_bytes_prp < start_data->tx_bytes_prp )
    {
      results->tx_bytes_prp =   ( UINT32_MAX + end_data->tx_bytes_prp )
                              - start_data->tx_bytes_prp;
    }
    else
    {
      results->tx_bytes_prp = end_data->tx_bytes_prp - start_data->tx_bytes_prp; 
    }

    /* following interface stats are not UINT32 limited */
    results->rx_bytes_if0 = end_data->rx_bytes_if0 - start_data->rx_bytes_if0; 
    results->tx_bytes_if0 = end_data->tx_bytes_if0 - start_data->tx_bytes_if0; 
    
    results->rx_bytes_if1 = end_data->rx_bytes_if1 - start_data->rx_bytes_if1; 
    results->tx_bytes_if1 = end_data->tx_bytes_if1 - start_data->tx_bytes_if1;
    /* ************************************************************************* */
    
    results->rx_byterate_prp = results->rx_bytes_prp / results->etime_sec;
    results->tx_byterate_prp = results->tx_bytes_prp / results->etime_sec;
    
    results->rx_byterate_if0 = results->rx_bytes_if0 / results->etime_sec;
    results->tx_byterate_if0 = results->tx_bytes_if0 / results->etime_sec;
    
    results->rx_byterate_if1 = results->rx_bytes_if1 / results->etime_sec;
    results->tx_byterate_if1 = results->tx_bytes_if1 / results->etime_sec;

    
    results->rx_bitrate_prp = results->rx_byterate_prp * BIT_PER_BYTE;
    results->tx_bitrate_prp = results->tx_byterate_prp * BIT_PER_BYTE;
    
    results->rx_bitrate_if0 = results->rx_byterate_if0 * BIT_PER_BYTE;
    results->tx_bitrate_if0 = results->tx_byterate_if0 * BIT_PER_BYTE;
    
    results->rx_bitrate_if1 = results->rx_byterate_if1 * BIT_PER_BYTE;
    results->tx_bitrate_if1 = results->tx_byterate_if1 * BIT_PER_BYTE;
  
  }
  else
  {

    perror( "Invalid elapsed time. Aborting result evalutation.\n" );
    exit( EXIT_FAILURE );

  }

  /* ****************************************** */
  
  return results;
}


stats_res_overall_t* evaluate_overall_data( stats_res_t** results_list, uint32_t list_len, uint16_t interval_dur )
{
  uint32_t i;

  stats_res_overall_t* overall_results = NULL;

  double utime_sec_sum      = 0.0;
  double stime_sec_sum      = 0.0;
  double cpu_percent_sum    = 0.0;
  
  double rx_bitrate_prp_sum = 0.0;
  double tx_bitrate_prp_sum = 0.0;
  
  double rx_bitrate_if0_sum = 0.0;
  double tx_bitrate_if0_sum = 0.0;
  
  double rx_bitrate_if1_sum = 0.0;
  double tx_bitrate_if1_sum = 0.0;
  
  
  if( results_list == NULL )
  {
    perror( "Invalid list of results. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }

  overall_results = ( stats_res_overall_t* )malloc( sizeof( stats_res_overall_t ) * 1 );
  if( overall_results == NULL )
  {
    perror( "Memory allocation failure. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }
  
  /* Interface names were allready set in collect.c
   * or in evaluate_interval_data().
   * We can just memcpy the interface names
   * of the first interval data struct
   * because the interface names do not change
   * over the range of a measurment period. */

  memcpy( overall_results->if_name_prp,            /* destination */
          ( *( results_list + 0 ) )->if_name_prp,  /* source */
          IF_NAME_LEN );                           /* length */
  
  memcpy( overall_results->if_name_if0,            /* destination */
          ( *( results_list + 0 ) )->if_name_if0,  /* source */
          IF_NAME_LEN );                           /* length */
  
  memcpy( overall_results->if_name_if1,            /* destination */
          ( *( results_list + 0 ) )->if_name_if1,  /* source */
          IF_NAME_LEN );                           /* length */
  
  
  /* ****************************************** */
  
  
  /* init the storage path with null bytes. 
   * storage path is set later when 
   * printing the results in print.c */
  memset( overall_results->storage_path, '\0', 
          PATH_LEN * sizeof( char ) );
  /* ****************************************** */
  
  
  /* take pid of the first */
  overall_results->pid = ( *( results_list + 0 ) )->pid;
  /* ****************************************** */

  /* interval duration */
  overall_results->interval_duration = interval_dur;
  /* ****************************************** */

  /* no of intervals */
  overall_results->no_intervals = list_len;
  /* ****************************************** */
  
  /* first initialisation ... */
  overall_results->etime_sec_total    = 0.0;

  overall_results->otime_usec_total   = 0;

  overall_results->rx_bytes_prp_total = 0;
  overall_results->tx_bytes_prp_total = 0;
  overall_results->rx_bytes_if0_total = 0;
  overall_results->tx_bytes_if0_total = 0;
  overall_results->rx_bytes_if1_total = 0;
  overall_results->tx_bytes_if1_total = 0;

  overall_results->etime_sec_min      = DBL_MAX;
  overall_results->etime_sec_max      = DBL_MAX * (-1);

  overall_results->otime_usec_min     = UINT32_MAX;
  overall_results->otime_usec_max     = 0;
  
  overall_results->utime_sec_min      = DBL_MAX;
  overall_results->utime_sec_max      = DBL_MAX * (-1);
  
  overall_results->stime_sec_min      = DBL_MAX;
  overall_results->stime_sec_max      = DBL_MAX * (-1);
  
  overall_results->cpu_percent_min    = DBL_MAX;
  overall_results->cpu_percent_max    = DBL_MAX * (-1);
  
  overall_results->rx_bitrate_prp_min = DBL_MAX;
  overall_results->rx_bitrate_prp_max = DBL_MAX * (-1);
  
  overall_results->tx_bitrate_prp_min = DBL_MAX;
  overall_results->tx_bitrate_prp_max = DBL_MAX * (-1);
  
  overall_results->rx_bitrate_if0_min = DBL_MAX;
  overall_results->rx_bitrate_if0_max = DBL_MAX * (-1);
  
  overall_results->tx_bitrate_if0_min = DBL_MAX;
  overall_results->tx_bitrate_if0_max = DBL_MAX * (-1);
  
  overall_results->rx_bitrate_if1_min = DBL_MAX;
  overall_results->rx_bitrate_if1_max = DBL_MAX * (-1);
  
  overall_results->tx_bitrate_if1_min = DBL_MAX;
  overall_results->tx_bitrate_if1_max = DBL_MAX * (-1);
  
  for( i = 0; i < list_len; i++ )
  {
    if( *( results_list + i ) == NULL )
    {
      perror( "No interval result set present. Exiting ...\n" );
      exit( EXIT_FAILURE );
    }
    
    /* sum things up ... */
    overall_results->etime_sec_total += ( *( results_list + i ) )->etime_sec;
    overall_results->otime_usec_total += ( *( results_list + i ) )->otime_usec;
    
    utime_sec_sum   += ( *( results_list + i ) )->utime_sec;
    stime_sec_sum   += ( *( results_list + i ) )->stime_sec;
    cpu_percent_sum += ( *( results_list + i ) )->cpu_percent;
    
    rx_bitrate_prp_sum += ( *( results_list + i ) )->rx_bitrate_prp;
    tx_bitrate_prp_sum += ( *( results_list + i ) )->tx_bitrate_prp;

    rx_bitrate_if0_sum += ( *( results_list + i ) )->rx_bitrate_if0;
    tx_bitrate_if0_sum += ( *( results_list + i ) )->tx_bitrate_if0;
    
    rx_bitrate_if1_sum += ( *( results_list + i ) )->rx_bitrate_if1;
    tx_bitrate_if1_sum += ( *( results_list + i ) )->tx_bitrate_if1;

    overall_results->rx_bytes_prp_total += ( *( results_list + i ) )->rx_bytes_prp;
    overall_results->tx_bytes_prp_total += ( *( results_list + i ) )->tx_bytes_prp;
    overall_results->rx_bytes_if0_total += ( *( results_list + i ) )->rx_bytes_if0;
    overall_results->tx_bytes_if0_total += ( *( results_list + i ) )->tx_bytes_if0;
    overall_results->rx_bytes_if1_total += ( *( results_list + i ) )->rx_bytes_if1;
    overall_results->tx_bytes_if1_total += ( *( results_list + i ) )->tx_bytes_if1;
    
    /* etime min and max check */
    check_min( &( ( *( results_list + i ) )->etime_sec ),      /* current */
               &( overall_results->etime_sec_min ),            /* actual */
               &( overall_results->etime_sec_min_interval ),   /* interval no */
               i );                                            /* current loop no */
    check_max( &( ( *( results_list + i ) )->etime_sec ),      /* current */
               &( overall_results->etime_sec_max ),            /* actual */
               &( overall_results->etime_sec_max_interval ),   /* interval no */
               i );                                            /* current loop no */
    /* ****************************************************************************** */
    
    /* otime min and max check */
    check_min_uint( &( ( *( results_list + i ) )->otime_usec ),      /* current */
                    &( overall_results->otime_usec_min ),            /* actual */
                    &( overall_results->otime_usec_min_interval ),   /* interval no */
                    i );                                             /* current loop no */
    check_max_uint( &( ( *( results_list + i ) )->otime_usec ),      /* current */
                    &( overall_results->otime_usec_max ),            /* actual */
                    &( overall_results->otime_usec_max_interval ),   /* interval no */
                    i );                                             /* current loop no */
    /* ****************************************************************************** */
    
    /* utime min and max check */
    check_min( &( ( *( results_list + i ) )->utime_sec ),      /* current */
               &( overall_results->utime_sec_min ),            /* actual */
               &( overall_results->utime_sec_min_interval ),   /* interval no */
               i );                                            /* current loop no */
    check_max( &( ( *( results_list + i ) )->utime_sec ),      /* current */
               &( overall_results->utime_sec_max ),            /* actual */
               &( overall_results->utime_sec_max_interval ),   /* interval no */
               i );                                            /* current loop no */
    /* ****************************************************************************** */
    
    /* stime min and max check */
    check_min( &( ( *( results_list + i ) )->stime_sec ),      /* current */
               &( overall_results->stime_sec_min ),            /* actual */
               &( overall_results->stime_sec_min_interval ),   /* interval no */
               i );                                            /* current loop no */
    check_max( &( ( *( results_list + i ) )->stime_sec ),      /* current */
               &( overall_results->stime_sec_max ),            /* actual */
               &( overall_results->stime_sec_max_interval ),   /* interval no */
               i );                                            /* current loop no */
    /* ****************************************************************************** */
    
    /* cpu_percent min and max check */
    check_min( &( ( *( results_list + i ) )->cpu_percent ),      /* current */
               &( overall_results->cpu_percent_min ),            /* actual */
               &( overall_results->cpu_percent_min_interval ),   /* interval no */
               i );                                              /* current loop no */
    check_max( &( ( *( results_list + i ) )->cpu_percent ),      /* current */
               &( overall_results->cpu_percent_max ),            /* actual */
               &( overall_results->cpu_percent_max_interval ),   /* interval no */
               i );                                              /* current loop no */
    /* ****************************************************************************** */
    
    /* prp interface min and max check */
    check_min( &( ( *( results_list + i ) )->rx_bitrate_prp ),      /* current */
               &( overall_results->rx_bitrate_prp_min ),            /* actual */
               &( overall_results->rx_bitrate_prp_min_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_max( &( ( *( results_list + i ) )->rx_bitrate_prp ),      /* current */
               &( overall_results->rx_bitrate_prp_max ),            /* actual */
               &( overall_results->rx_bitrate_prp_max_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_min( &( ( *( results_list + i ) )->tx_bitrate_prp ),      /* current */
               &( overall_results->tx_bitrate_prp_min ),            /* actual */
               &( overall_results->tx_bitrate_prp_min_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_max( &( ( *( results_list + i ) )->tx_bitrate_prp ),      /* current */
               &( overall_results->tx_bitrate_prp_max ),            /* actual */
               &( overall_results->tx_bitrate_prp_max_interval ),   /* interval no */
               i );                                                 /* current loop no */
    /* ****************************************************************************** */
    
    /* if0 interface min and max check */
    check_min( &( ( *( results_list + i ) )->rx_bitrate_if0 ),      /* current */
               &( overall_results->rx_bitrate_if0_min ),            /* actual */
               &( overall_results->rx_bitrate_if0_min_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_max( &( ( *( results_list + i ) )->rx_bitrate_if0 ),      /* current */
               &( overall_results->rx_bitrate_if0_max ),            /* actual */
               &( overall_results->rx_bitrate_if0_max_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_min( &( ( *( results_list + i ) )->tx_bitrate_if0 ),      /* current */
               &( overall_results->tx_bitrate_if0_min ),            /* actual */
               &( overall_results->tx_bitrate_if0_min_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_max( &( ( *( results_list + i ) )->tx_bitrate_if0 ),      /* current */
               &( overall_results->tx_bitrate_if0_max ),            /* actual */
               &( overall_results->tx_bitrate_if0_max_interval ),   /* interval no */
               i );                                                 /* current loop no */
    /* ****************************************************************************** */
    
    /* if1 interface min and max check */
    check_min( &( ( *( results_list + i ) )->rx_bitrate_if1 ),      /* current */
               &( overall_results->rx_bitrate_if1_min ),            /* actual */
               &( overall_results->rx_bitrate_if1_min_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_max( &( ( *( results_list + i ) )->rx_bitrate_if1 ),      /* current */
               &( overall_results->rx_bitrate_if1_max ),            /* actual */
               &( overall_results->rx_bitrate_if1_max_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_min( &( ( *( results_list + i ) )->tx_bitrate_if1 ),      /* current */
               &( overall_results->tx_bitrate_if1_min ),            /* actual */
               &( overall_results->tx_bitrate_if1_min_interval ),   /* interval no */
               i );                                                 /* current loop no */
    check_max( &( ( *( results_list + i ) )->tx_bitrate_if1 ),      /* current */
               &( overall_results->tx_bitrate_if1_max ),            /* actual */
               &( overall_results->tx_bitrate_if1_max_interval ),   /* interval no */
               i );                                                 /* current loop no */
    /* ****************************************************************************** */

  }

  overall_results->etime_sec_avg  = overall_results->etime_sec_total / list_len;
  overall_results->otime_usec_avg = overall_results->otime_usec_total / list_len;

  overall_results->utime_sec_avg   = utime_sec_sum / list_len;
  overall_results->stime_sec_avg   = stime_sec_sum / list_len;
  overall_results->cpu_percent_avg = cpu_percent_sum / list_len;

  overall_results->rx_bitrate_prp_avg = rx_bitrate_prp_sum / list_len;
  overall_results->tx_bitrate_prp_avg = tx_bitrate_prp_sum / list_len;
  
  overall_results->rx_bitrate_if0_avg = rx_bitrate_if0_sum / list_len;
  overall_results->tx_bitrate_if0_avg = tx_bitrate_if0_sum / list_len;
  
  overall_results->rx_bitrate_if1_avg = rx_bitrate_if1_sum / list_len;
  overall_results->tx_bitrate_if1_avg = tx_bitrate_if1_sum / list_len;

  return overall_results;
}


void check_min( double* current, double* actual, uint16_t* interval, uint32_t i )
{
  if( *current < *actual )
  {
    *actual = *current;
    *interval = i;
  }
}


void check_max( double* current, double* actual, uint16_t* interval, uint32_t i )
{
  if( *current > *actual )
  {
    *actual = *current;
    *interval = i;
  }
}


void check_min_uint( uint32_t* current, uint32_t* actual, uint16_t* interval, uint32_t i )
{
  if( *current < *actual )
  {
    *actual = *current;
    *interval = i;
  }
}


void check_max_uint( uint32_t* current, uint32_t* actual, uint16_t* interval, uint32_t i )
{
  if( *current > *actual )
  {
    *actual = *current;
    *interval = i;
  }
}
