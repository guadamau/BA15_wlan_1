/* *****************************************************************************
   File:       print.c
   Authors:    leibupro
               guadamau
   Date:       03.2015
   
   Purpose:    Output collected statistics data.
               
   Desc:       -
   ***************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "print.h"

/* private function prototypes */
FILE* setup_output( void* output_loc, char* file_prefix );
void print_one_interval( FILE* fp, stats_res_t* results );
void print_overall_stats( FILE* fp, stats_res_overall_t* overall_stats );
void close_output( FILE* fp );


void print_one_interval( FILE* fp, stats_res_t* results )
{
  fprintf( fp,
           "*******************************************************************************************\n\n"
           " Process specific statistics, interval no: %5d\n\n"
           " PID:  %d \n"
           " Time: elapsed %5.3lfs, user %3.2lfs, sys %3.2lfs, overhead %dus\n"
           " CPU:  cpu workload %3.2lf%%\n\n\n"
           " Machine wide network stats for PRP involved interfaces, not process specific:\n\n"
           " Interface       RX Bytes       TX Bytes      RX Bitrate [Mbit/s]      TX Bitrate [Mbit/s]\n"
           " =========================================================================================\n"
           " prp (%.4s)  %12lld   %12lld                 %8.2lf                 %8.2lf\n"
           " if0 (%.4s)  %12lld   %12lld                 %8.2lf                 %8.2lf\n"
           " if1 (%.4s)  %12lld   %12lld                 %8.2lf                 %8.2lf\n"
           " =========================================================================================\n\n"
           "*******************************************************************************************\n",
           results->interval_no,
           results->pid,
           results->etime_sec, 
           results->utime_sec, 
           results->stime_sec,
           results->otime_usec,
           results->cpu_percent,
           results->if_name_prp, 
           results->rx_bytes_prp,
           results->tx_bytes_prp,
           bit_s_to_mbit_s( results->rx_bitrate_prp ),
           bit_s_to_mbit_s( results->tx_bitrate_prp ),
           results->if_name_if0,
           results->rx_bytes_if0,
           results->tx_bytes_if0,
           bit_s_to_mbit_s( results->rx_bitrate_if0 ),
           bit_s_to_mbit_s( results->tx_bitrate_if0 ),
           results->if_name_if1,
           results->rx_bytes_if1,
           results->tx_bytes_if1,
           bit_s_to_mbit_s( results->rx_bitrate_if1 ),
           bit_s_to_mbit_s( results->tx_bitrate_if1 ) );
}


void print_overall_stats( FILE* fp, stats_res_overall_t* overall_stats )
{
  uint32_t int_dur = overall_stats->interval_duration;

  /* too lazy to write "overall_stats" every time... */
  stats_res_overall_t* os = overall_stats;
  
  fprintf( fp, 
           "*************************************************************************************************\n\n"
           " Overall statistics of Process with PID: %6d   |   Total elapsed time [s]:   %10.2lf\n"
           "                                                  |   Total overhead time [us]: %10d\n"
           " Total no of intervals:         %9d\n"
           " Duration of each interval [s]: %9.3f\n\n"
           " -----------------------------------------------------------------------------------------------\n"
           " Elapsed time [s]:   Average: %7.2lf\n"
           " (per interval)      Min:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           "                     Max:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           " -----------------------------------------------------------------------------------------------\n"
           " Overhead time [us]: Average: %7d\n"
           " (per interval)      Min:     %7d at interval: [%9.3fs, %9.3fs]\n"
           "                     Max:     %7d at interval: [%9.3fs, %9.3fs]\n"
           " -----------------------------------------------------------------------------------------------\n"
           " Usertime [s]:       Average: %7.2lf\n"
           "                     Min:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           "                     Max:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           " -----------------------------------------------------------------------------------------------\n"
           " Systemtime [s]:     Average: %7.2lf\n"
           "                     Min:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           "                     Max:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           " -----------------------------------------------------------------------------------------------\n"
           " CPU workload [%%]:   Average: %7.2lf\n"
           "                     Min:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           "                     Max:     %7.2lf at interval: [%9.3fs, %9.3fs]\n"
           " -----------------------------------------------------------------------------------------------\n"
           "\n\n\n"
           " Machine wide statistics of the network interfaces: \n"
           " Following statistics are bound to a specific NIC not to the PID mentioned above.\n"
           " ===============================================================================================\n\n"
           " Statistics for virtual PRP interface with label: %.4s\n"
           " -----------------------------------------------------------------------------------------------\n"
           "             RX [Mbit/s] | at interval              ||| TX [Mbit/s] | at interval\n"
           "            ------------------------------------------------------------------------------------\n"
           " Average:       %8.2lf |                      --- |||    %8.2lf |                      ---\n"
           " Min:           %8.2lf | [%9.3fs, %9.3fs] |||    %8.2lf | [%9.3fs, %9.3fs]\n"
           " Max:           %8.2lf | [%9.3fs, %9.3fs] |||    %8.2lf | [%9.3fs, %9.3fs]\n"
           "            ------------------------------------------------------------------------------------\n\n"
           " Total received bytes:    %13lld\n"
           " Total transmitted bytes: %13lld\n\n"
           " -----------------------------------------------------------------------------------------------\n\n"
           " Statistics for first PRP involved physical interface with label: %.4s\n"
           " -----------------------------------------------------------------------------------------------\n"
           "             RX [Mbit/s] | at interval              ||| TX [Mbit/s] | at interval\n"
           "            ------------------------------------------------------------------------------------\n"
           " Average:       %8.2lf |                      --- |||    %8.2lf |                      ---\n"
           " Min:           %8.2lf | [%9.3fs, %9.3fs] |||    %8.2lf | [%9.3fs, %9.3fs]\n"
           " Max:           %8.2lf | [%9.3fs, %9.3fs] |||    %8.2lf | [%9.3fs, %9.3fs]\n"
           "            ------------------------------------------------------------------------------------\n\n"
           " Total received bytes:    %13lld\n"
           " Total transmitted bytes: %13lld\n\n"
           " -----------------------------------------------------------------------------------------------\n\n"
           " Statistics for second PRP involved physical interface with label: %.4s\n"
           " -----------------------------------------------------------------------------------------------\n"
           "             RX [Mbit/s] | at interval              ||| TX [Mbit/s] | at interval\n"
           "            ------------------------------------------------------------------------------------\n"
           " Average:       %8.2lf |                      --- |||    %8.2lf |                      ---\n"
           " Min:           %8.2lf | [%9.3fs, %9.3fs] |||    %8.2lf | [%9.3fs, %9.3fs]\n"
           " Max:           %8.2lf | [%9.3fs, %9.3fs] |||    %8.2lf | [%9.3fs, %9.3fs]\n"
           "            ------------------------------------------------------------------------------------\n\n"
           " Total received bytes:    %13lld\n"
           " Total transmitted bytes: %13lld\n\n"
           " -----------------------------------------------------------------------------------------------\n\n"
           " ===============================================================================================\n\n"
           "*************************************************************************************************\n"

           ,
           
           /* global information */
           os->pid,
           os->etime_sec_total,
           os->otime_usec_total,
           os->no_intervals,
           ms_to_s( os->interval_duration ),
           
           /* elapsed time per interval */
           os->etime_sec_avg,
           
           os->etime_sec_min,
           ms_to_s( ( os->etime_sec_min_interval * int_dur ) ),
           ms_to_s( ( os->etime_sec_min_interval * int_dur + int_dur ) ),

           os->etime_sec_max,
           ms_to_s( ( os->etime_sec_max_interval * int_dur ) ),
           ms_to_s( ( os->etime_sec_max_interval * int_dur + int_dur ) ),
           
           /* overhead time per interval */
           ( uint32_t )os->otime_usec_avg,
           
           os->otime_usec_min,
           ms_to_s( ( os->otime_usec_min_interval * int_dur ) ),
           ms_to_s( ( os->otime_usec_min_interval * int_dur + int_dur ) ),

           os->otime_usec_max,
           ms_to_s( ( os->otime_usec_max_interval * int_dur ) ),
           ms_to_s( ( os->otime_usec_max_interval * int_dur + int_dur ) ),
           
           /* usertime */
           os->utime_sec_avg,
           
           os->utime_sec_min,
           ms_to_s( ( os->utime_sec_min_interval * int_dur ) ),
           ms_to_s( ( os->utime_sec_min_interval * int_dur + int_dur ) ),

           os->utime_sec_max,
           ms_to_s( ( os->utime_sec_max_interval * int_dur ) ),
           ms_to_s( ( os->utime_sec_max_interval * int_dur + int_dur ) ),
           
           /* systemtime */
           os->stime_sec_avg,
           
           os->stime_sec_min,
           ms_to_s( ( os->stime_sec_min_interval * int_dur ) ),
           ms_to_s( ( os->stime_sec_min_interval * int_dur + int_dur ) ),

           os->stime_sec_max,
           ms_to_s( ( os->stime_sec_max_interval * int_dur ) ),
           ms_to_s( ( os->stime_sec_max_interval * int_dur + int_dur ) ),

           /* cpu percentage */
           os->cpu_percent_avg,
           
           os->cpu_percent_min,
           ms_to_s( ( os->cpu_percent_min_interval * int_dur ) ),
           ms_to_s( ( os->cpu_percent_min_interval * int_dur + int_dur ) ),

           os->cpu_percent_max,
           ms_to_s( ( os->cpu_percent_max_interval * int_dur ) ),
           ms_to_s( ( os->cpu_percent_max_interval * int_dur + int_dur ) ),

           /* prp interface */
           os->if_name_prp,

           bit_s_to_mbit_s( os->rx_bitrate_prp_avg ),
           bit_s_to_mbit_s( os->tx_bitrate_prp_avg ),

           bit_s_to_mbit_s( os->rx_bitrate_prp_min ),
           ms_to_s( ( os->rx_bitrate_prp_min_interval * int_dur ) ),
           ms_to_s( ( os->rx_bitrate_prp_min_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->tx_bitrate_prp_min ),
           ms_to_s( ( os->tx_bitrate_prp_min_interval * int_dur ) ),
           ms_to_s( ( os->tx_bitrate_prp_min_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->rx_bitrate_prp_max ),
           ms_to_s( ( os->rx_bitrate_prp_max_interval * int_dur ) ),
           ms_to_s( ( os->rx_bitrate_prp_max_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->tx_bitrate_prp_max ),
           ms_to_s( ( os->tx_bitrate_prp_max_interval * int_dur ) ),
           ms_to_s( ( os->tx_bitrate_prp_max_interval * int_dur + int_dur ) ),

           os->rx_bytes_prp_total,
           os->tx_bytes_prp_total,

           /* first prp involved interface */
           os->if_name_if0,

           bit_s_to_mbit_s( os->rx_bitrate_if0_avg ),
           bit_s_to_mbit_s( os->tx_bitrate_if0_avg ),

           bit_s_to_mbit_s( os->rx_bitrate_if0_min ),
           ms_to_s( ( os->rx_bitrate_if0_min_interval * int_dur ) ),
           ms_to_s( ( os->rx_bitrate_if0_min_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->tx_bitrate_if0_min ),
           ms_to_s( ( os->tx_bitrate_if0_min_interval * int_dur ) ),
           ms_to_s( ( os->tx_bitrate_if0_min_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->rx_bitrate_if0_max ),
           ms_to_s( ( os->rx_bitrate_if0_max_interval * int_dur ) ),
           ms_to_s( ( os->rx_bitrate_if0_max_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->tx_bitrate_if0_max ),
           ms_to_s( ( os->tx_bitrate_if0_max_interval * int_dur ) ),
           ms_to_s( ( os->tx_bitrate_if0_max_interval * int_dur + int_dur ) ),

           os->rx_bytes_if0_total,
           os->tx_bytes_if0_total,
           
           /* second prp involved interface */
           os->if_name_if1,

           bit_s_to_mbit_s( os->rx_bitrate_if1_avg ),
           bit_s_to_mbit_s( os->tx_bitrate_if1_avg ),

           bit_s_to_mbit_s( os->rx_bitrate_if1_min ),
           ms_to_s( ( os->rx_bitrate_if1_min_interval * int_dur ) ),
           ms_to_s( ( os->rx_bitrate_if1_min_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->tx_bitrate_if1_min ),
           ms_to_s( ( os->tx_bitrate_if1_min_interval * int_dur ) ),
           ms_to_s( ( os->tx_bitrate_if1_min_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->rx_bitrate_if1_max ),
           ms_to_s( ( os->rx_bitrate_if1_max_interval * int_dur ) ),
           ms_to_s( ( os->rx_bitrate_if1_max_interval * int_dur + int_dur ) ),
           
           bit_s_to_mbit_s( os->tx_bitrate_if1_max ),
           ms_to_s( ( os->tx_bitrate_if1_max_interval * int_dur ) ),
           ms_to_s( ( os->tx_bitrate_if1_max_interval * int_dur + int_dur ) ),

           os->rx_bytes_if1_total,
           os->tx_bytes_if1_total
         );
}


void output_results_single( void* output_loc, stats_res_t* results, char* file_prefix )
{
  FILE* fp = NULL;

  fp = setup_output( output_loc, file_prefix );
  
  print_one_interval( fp, results );
  
  fflush( fp );

  close_output( fp );
}


void output_results_all( void* output_loc, stats_res_t** results_list, uint32_t res_list_len, 
                         char* file_prefix )
{
  FILE* fp = NULL;
  uint32_t i;
  
  fp = setup_output( output_loc, file_prefix );

  for( i = 0; i < res_list_len; i++ )
  {
    print_one_interval( fp, *( results_list + i ) );
  }

  fflush( fp );
  close_output( fp );
}


void output_results_overall( void* output_loc, stats_res_overall_t* overall_stats, 
                             char* file_prefix )
{
  FILE* fp = NULL;

  fp = setup_output( output_loc, file_prefix );

  print_overall_stats( fp, overall_stats );

  fflush( fp );

  close_output( fp );
}


FILE* setup_output( void* output_loc, char* file_prefix )
{
  const uint16_t file_loc_len = PATH_LEN + FILE_PREF_LEN + TIME_STMP_LEN;
  char file_loc[ file_loc_len ];
  
  time_t time_stmp;
  char time_stmp_str[ TIME_STMP_LEN ];

  FILE* fp = NULL;

  fp = ( FILE* )output_loc;

  if( fp == NULL )
  {
    perror( "file pointer still NULL. Cast failed ..." );
    exit( EXIT_FAILURE );
  }

  if( fp == stdout )
  {
    /* the output location is a terminal
     * nothing has to be done here. */
    return fp;
  }
  else
  {
    /* prepare output file */
    memset( file_loc, '\0', file_loc_len );
    memset( time_stmp_str, '\0', TIME_STMP_LEN );

    /* get a more or less unique timestamp. 
     * seconds since the epoch (1970.01.01) */
    time_stmp = time( NULL );
    snprintf( time_stmp_str, TIME_STMP_LEN, "%lld", ( int64_t )time_stmp );
    
    if( output_loc == NULL )
    {
      perror( "invalid or not set output location. Exiting ...\n" );
      exit( EXIT_FAILURE );
    }
    else if( file_prefix == NULL )
    {
      perror( "A file prefix must be passed," 
              "if the output should be redirected to a file. Exiting ...\n" );
      exit( EXIT_FAILURE );
    }
    else
    {
      /* concatenate string for output file location */
      strncat( file_loc, ( ( char* )output_loc ), PATH_LEN );
      strncat( file_loc, FILE_SEPARATOR, 1 );
      strncat( file_loc, file_prefix, FILE_PREF_LEN );
      strncat( file_loc, time_stmp_str, TIME_STMP_LEN );

      /* just to debug */
      printf( "Writing to: %s\n", file_loc );

      fp = fopen( file_loc, "a" );
      
      if( fp == NULL )
      {
        perror( "Failed to open output file. Exiting ..." );
        exit( EXIT_FAILURE );
      }
    }
  }
  return fp;
}

void close_output( FILE* fp )
{
  if( fp == stdout )
  {
    /* its stdout ...
     * no underlying filedescriptor has to be closed */
    return;
  }
  
  /* just to debug */
  /* printf( "\n\nclosing file ...\n\n" ); */
  
  fclose( fp );
}

