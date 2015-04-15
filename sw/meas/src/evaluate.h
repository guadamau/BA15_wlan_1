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
   File:       evaluate.h
   Authors:    leibupro
               guadamau
   Date:       03.2015
   
   Purpose:    Evaluate statistics data from /proc-filesystem. 
               
   Desc:       -
   ***************************************************************************** */
#ifndef MEAS_EVALUATE_H_
#define MEAS_EVALUATE_H_

#include "types.h"
#include "mtimer.h"

#define BIT_PER_BYTE 8

/* function prototypes */
stats_res_t*         evaluate_interval_data( stats_t* start_data, stats_t* end_data, ttimer_t* timer );
stats_res_overall_t* evaluate_overall_data( stats_res_t** results_list, uint32_t list_len, uint16_t interval_dur );

#endif /* MEAS_EVALUATE_H_ */


