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

/* function prototypes */
void output_results_single( void* output_loc, stats_res_t* results, char* file_prefix );
void output_results_all( void* output_loc, stats_res_t** results_list, uint32_t res_list_len, 
                         char* file_prefix );
void output_results_overall( void* output_loc, stats_res_overall_t* overall_stats, 
                             char* file_prefix );

#endif /* MEAS_PRINT_H_ */
