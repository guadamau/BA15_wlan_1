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

/* ==============================================================================

              KEEP IN MIND THAT YOU HAVE 
              TO RECOMPILE THE APPLICATION 
              IN ORDER THAT CHANGES 
              IN THIS FILE TAKE AFFECT

              DO NOT CHANGE THE NAMES 
              OF THE VARIABLES !!!!!!

              USE: make all
              TO BUILD THE APPLICATION

   ============================================================================== */

/* 
 * Global initial arrays that hold the cmd_lines to concatenate the values
 * from the /proc-File-Systems. With a pattern to replace for example 
 * dynamically passed PIDs.
 * Your command lines should not exceed 1000 characters.
 * A perfect overview for the files in /proc consult the man page of proc
 * man 5 proc.
 * */
const char const* init_paths[] =     { 
                                       "/proc/$PID$/stat", /* utime %lu */
                                       "/proc/$PID$/stat", /* stime %lu */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev",    /* traffic stats of nics */
                                       "/proc/net/dev"     /* traffic stats of nics */
                                     };


/* 
 * Has to contain the same amount of replacement strings
 * as the array init_cmd_lines. If nothing has to be replaced
 * use an empty string. 
 * */
const char const* init_search[] =    { 
                                       "$PID$", 
                                       "$PID$",
                                       "", 
                                       "", 
                                       "", 
                                       "", 
                                       "", 
                                       "", 
                                       "", 
                                       "", 
                                       "" 
                                     };


/* 
 * Specify the lines and columns, where the values are you want to use.
 * For example: If you want: "cat /proc/$PID$/stat | cut -d' ' -f14", the user time,
 * which is located on line 1, column 14 of the specified file,
 * mak an entry here like this: "1:14"
 * The starting index is 1 not 0.
 */
const char const* init_val_pos[] =   {
                                       "1:14",
                                       "1:15",
                                       "7:1",
                                       "7:2",
                                       "7:10",
                                       "6:1",
                                       "6:2",
                                       "6:10",
                                       "5:1",
                                       "5:2",
                                       "5:10"
                                     };


/*
 * Define a delimiter which separates the columns in the file, you want to parse.
 * It has to be a single character.
 */
const char const* init_val_delim[] = {
                                       " ",
                                       " ",
                                       " ",
                                       " ",
                                       " ",
                                       " ",
                                       " ",
                                       " ",
                                       " ",
                                       " ",
                                       " "
                                     };


/*
 * Define wheter your /proc-Value is a number or a textstring.
 */
const char const* init_val_types[] = {
                                       STR_VALUE_NUM,
                                       STR_VALUE_NUM,
                                       STR_VALUE_STR,
                                       STR_VALUE_NUM,
                                       STR_VALUE_NUM,
                                       STR_VALUE_STR,
                                       STR_VALUE_NUM,
                                       STR_VALUE_NUM,
                                       STR_VALUE_STR,
                                       STR_VALUE_NUM,
                                       STR_VALUE_NUM
                                     };


/* Amount of elements in init_cmd_lines and init_cmd_lines */
const unsigned char cmd_line_count = 11;
