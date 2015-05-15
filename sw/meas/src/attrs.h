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
   File:       attrs.h
   Authors:    leibupro
               guadamau
   Date:       05.2015
   
   Purpose:    Specify and handle global vars used to contain information
               about values that have to be extracted 
               from the /proc-Filesystem.
               
   Desc:       -
   ***************************************************************************** */
#ifndef MEAS_ATTRS_H_
#define MEAS_ATTRS_H_

#include "types.h"

#define PRP_ENABLED      0x01
#define PRP_DISABLED     0x00


/* ***************************************************************************** */

#define PRP_NO_VALUES      11
#define NO_VALUES           5

/* Indexes mapped to the meaning of a file */
#define INDEX_UTIME         0
#define INDEX_STIME         1
#define INDEX_IF_NAME_IF0   2
#define INDEX_RX_BYTES_IF0  3
#define INDEX_TX_BYTES_IF0  4
#define INDEX_IF_NAME_IF1   5
#define INDEX_RX_BYTES_IF1  6
#define INDEX_TX_BYTES_IF1  7
#define INDEX_IF_NAME_PRP   8
#define INDEX_RX_BYTES_PRP  9
#define INDEX_TX_BYTES_PRP 10


/* ********************************************************************************* */


/* 
 * Global vars to use in attrs.c and collect.c 
 * but ONLY WRITE to them in attrs.c!!!!!!!!!! 
 *
 * KEEP IN MIND. GLOBAL VARIABLES ARE DEVIOUS!
 * */


/* 
 * PRP mode on or off. If PRP mode is enabled, meas watches
 * the virtual PRP interface and its two physical interfaces.
 * 
 * If it is disabled, watch only a single interface.
 * We use eth0 in BA15_wlan_1
 *
 * TODO: Allow the user to specify via commandline 
 *       which interfaces by their names/labels
 *       should be used, when PRP mode is on or off.
 *       !!! NO TIME LEFT IN THE SCOPE OF BA15_wlan_1 !!!
 * 
 * prp_enabled = 0x01 --> PRP mode is enabled
 * prp_enabled = 0x00 --> PRP mode is disabled
 * */
unsigned char prp_enabled;


/* Amount of elements in init_cmd_lines and init_cmd_lines */
unsigned char cmd_line_count;


/* 
 * Global initial arrays that hold the cmd_lines to concatenate the values
 * from the /proc-File-Systems. With a pattern to replace for example 
 * dynamically passed PIDs.
 * Your command lines should not exceed 1000 characters.
 * A perfect overview for the files in /proc consult the man page of proc
 * man 5 proc.
 * */
char init_paths    [ MAX_PROC_VALS ][ PATH_LEN ];


/* 
 * Has to contain the same amount of replacement strings
 * as the array init_cmd_lines. If nothing has to be replaced
 * use an empty string. 
 * */
char init_search   [ MAX_PROC_VALS ][ PATH_LEN ];


/* 
 * Specify the lines and columns, where the values are you want to use.
 * For example: If you want: "cat /proc/$PID$/stat | cut -d' ' -f14", the user time,
 * which is located on line 1, column 14 of the specified file,
 * mak an entry here like this: "1:14"
 * The starting index is 1 not 0.
 */
char init_val_pos  [ MAX_PROC_VALS ][ PATH_LEN ];


/*
 * Define a delimiter which separates the columns in the file, you want to parse.
 * It has to be a single character.
 */
char init_val_delim[ MAX_PROC_VALS ][ PATH_LEN ];


/*
 * Define wheter your /proc-Value is a number or a textstring.
 */
char init_val_types[ MAX_PROC_VALS ][ PATH_LEN ];

/* ********************************************************************************* */


/* ********************************************************************************* */
/* function prototypes */

void set_prp_mode( unsigned char prp_mode );
void init_proc_properties( void );

/* ********************************************************************************* */

#endif /* MEAS_ATTRS_H_ */
