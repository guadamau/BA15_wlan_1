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
   File:       print.h
   Authors:    leibupro
               guadamau
   Date:       03.2015
   
   Purpose:    Output of the collected statistics data. 
               
   Desc:       -
   ***************************************************************************** */
#ifndef MEAS_PRINT_H_
#define MEAS_PRINT_H_

#include "types.h"

#define bit_s_to_mbit_s( X )   ( X / ( double )( 1000 * 1000 ) )
#define ms_to_s( X )           ( ( float )X / ( float )1000 )

/* function prototypes */
void output_results_single( void* output_loc, stats_res_t* results, char* file_prefix, 
                            unsigned char prp_enabled );

void output_results_all( void* output_loc, stats_res_t** results_list, uint32_t res_list_len, 
                         char* file_prefix, unsigned char prp_enabled );

void output_results_overall( void* output_loc, stats_res_overall_t* overall_stats, 
                             char* file_prefix, unsigned char prp_enabled );

#endif /* MEAS_PRINT_H_ */
