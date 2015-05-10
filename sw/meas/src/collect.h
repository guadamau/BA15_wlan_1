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
   File:       collect.h
   Authors:    leibupro
               guadamau
   Date:       03.2015
   
   Purpose:    Collect specified values from the /proc-Filesystem 
               
   Desc:       -
   ***************************************************************************** */
#ifndef MEAS_COLLECT_H_
#define MEAS_COLLECT_H_

#include "types.h"


/* function prototypes */
stats_t* get_stat_vals( file_params_t** file_params_list, meas_buffer_t** meas_buffers,
                        uint16_t meas_buffers_len, uint16_t interval_no );

file_params_t** create_file_params( cmd_args_t args );
void            clear_file_params( file_params_t** file_params_list );

meas_buffer_meta_t get_meas_buffers_meta_data( file_params_t** file_params_list );

meas_buffer_t** init_meas_buffers( meas_buffer_meta_t* buffers_meta, uint16_t no_intervals );

void read_to_meas_buffers( meas_buffer_t** meas_buffers, uint16_t meas_buffers_len,
                           uint16_t interval_no );

void release_meas_buffers( meas_buffer_meta_t* buffers_meta, meas_buffer_t** meas_buffers );

#endif /* MEAS_COLLECT_H_ */
