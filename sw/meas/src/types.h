/*

===================================================================================================

########     ###       ##   ########         ##      ## ##          ###    ##    ##            ##   
##     ##   ## ##    ####   ##               ##  ##  ## ##         ## ##   ###   ##          ####   
##     ##  ##   ##     ##   ##               ##  ##  ## ##        ##   ##  ####  ##            ##   
########  ##     ##    ##   #######          ##  ##  ## ##       ##     ## ## ## ##            ##   
##     ## #########    ##         ##         ##  ##  ## ##       ######### ##  ####            ##   
##     ## ##     ##    ##   ##    ##         ##  ##  ## ##       ##     ## ##   ###            ##   
########  ##     ##  ######  ######  #######  ###  ###  ######## ##     ## ##    ## #######  ###### 

===================================================================================================

sssssssssssssssoooosssssssssssssssssyyyyyyyyyhhhhh
ssssssoooooooooooooooooooooooooossssssssyyyyyyhhhh
sssooooooo++++++++++++++++oooooooooossssssyyyyyyyh
oooooo+++++++++/+///++++++++++++ooooooossssssyyyyy
ooo++++++++///////////////////+++++oooooossssssyyy
++++++//////////////://+/////////++++++ooooossssyy
++++//////:://::::/+syhdhhyyhsso/////++++oooooosss
//////:::::::::::sdmmmmmmNmmmmddho//////+++oooooss
///:::::::::---/hmNmddddmmmmmmddmdo/::////++++ooos
::::::::------omNdyyssssoosyhdhyydmy/:::///++++ooo
::--------..-yNmdyysso+//////+++++smms::::///+++oo
:-----......oNNdhyyso+/::-----::/++ymm/:::////++++
----.....``:mNmhhyso+/::-.-...-://+ohm+-::::///+++
-.......```ommmhhyo++/::----..-::/+ohd/--::::///++
.....``````-hmdhhhhysso+/:::::////+odh:---:::://++
...`````````ohhysyyyhs+sy//+syhyyo+oh+.-----::///+
..``````````ohyys+++++oyy+++///////++...----::///+
.`````````` .hyhyo+//+sys+++/:::::/+-...-----:///+
``````````   :yhhso++shhs//++/:://+:`...----::///+
`````````   `/yhyoymdddddddysoo/++/.``..---::://++
````````   -dMmdyyossyyso++sssy+++-`....---::://++
``````  `:sNMMmddhyyyo++++//+o+++-``...---::://+++
`````-+shNmmmmddddddhyo+/:/+osss.``....--:::://+oo
`.:ohmmmmNNmhyhyyhdddddhyyyyyymmdho/-.--:::///+ooo
sddmmmmmmmNmh+ysoooyhyyyyssoshNNmmmmdhs+/////++oss
mmmmmmmmmmNmmhoso++++sssooosymNNmdmmmmmmdho++oooss
mmmmmmmmNmNNmmho++++/++oooosyNNmNNmmmmmmmmmhoosssy
mmmmmmmmNmmNmmmdo//+++oooooodmmNmmmmmmmmmmmmdssyyy
mmmmmmmmmNmNNmmmNho////////ymmmNmmmmmmmmmmmmmhyyhh
mmmmmmmmmNmmNmmNmNNNh+////ommmmNmmmmmmmmmmmmmdhhhd
mmdmmmmmmmmmmmmmmNmNNNh//+mmmmmmmmmmmmmmNmmmmmhddd
mmmdmmmmmmmmmmmmmmNmmmmh/hmmmmmmmmmmmmmmNmmmNmdddd
mmmdmmNmmmmmmmmmmmmNmmhmdmmmdmmddmmmmmmmmNmmNmmmmm

wlan approves


... yes, he does.

===================================================================================================
*/

/* *****************************************************************************
   File:       types.h
   Autor:      leibupro
   Date:       03.2015
   
   Purpose:    structs that are used 
               all over the application 
               are declared here. 
               
   Desc:       -
   ***************************************************************************** */

#ifndef MEAS_TYPES_H_
#define MEAS_TYPES_H_

#include <sys/types.h>
#include <stdint.h>
#include <float.h>

#define FILE_BUFFER_SIZE    1024
#define PATH_LEN             256
#define IF_NAME_LEN           16
#define TIME_STMP_LEN         16

#define FILE_SEPARATOR      "/"
#define FILE_PREFIX_1       "meas_interval_results_"
#define FILE_PREFIX_2       "meas_overall_stats_"
#define FILE_PREF_LEN         32

#define VALUE_NUM           0
#define STR_VALUE_NUM       "0"
#define VALUE_STR           1
#define STR_VALUE_STR       "1"

typedef struct 
{

  pid_t    pid;
  int32_t  interval;
  uint32_t no_intervals;
  uint8_t  verbosity_level;
  char     store_path[ FILE_BUFFER_SIZE ];

} cmd_args_t;

typedef struct
{
  pid_t    pid;

  uint64_t utime;
  uint64_t stime;

  char     if_name_prp[ IF_NAME_LEN ];
  uint64_t rx_bytes_prp;
  uint64_t tx_bytes_prp;
  
  char     if_name_if0[ IF_NAME_LEN ];
  uint64_t rx_bytes_if0;
  uint64_t tx_bytes_if0;

  char     if_name_if1[ IF_NAME_LEN ];
  uint64_t rx_bytes_if1;
  uint64_t tx_bytes_if1;

} stats_t;

typedef struct
{

  pid_t  pid;

  double utime_sec;
  double stime_sec;

  double etime_sec;
  
  /* cpu percentage values 
   * are only reliable for 
   * single threaded processes 
   * yet. */
  double cpu_percent;  


  char     if_name_prp[ IF_NAME_LEN ];
  uint64_t rx_bytes_prp;
  uint64_t tx_bytes_prp;
  double   rx_bitrate_prp;
  double   tx_bitrate_prp;
  double   rx_byterate_prp;
  double   tx_byterate_prp;

  char     if_name_if0[ IF_NAME_LEN ];
  uint64_t rx_bytes_if0;
  uint64_t tx_bytes_if0;
  double   rx_bitrate_if0;
  double   tx_bitrate_if0;
  double   rx_byterate_if0;
  double   tx_byterate_if0;

  char     if_name_if1[ IF_NAME_LEN ];
  uint64_t rx_bytes_if1;
  uint64_t tx_bytes_if1;
  double   rx_bitrate_if1;
  double   tx_bitrate_if1;
  double   rx_byterate_if1;
  double   tx_byterate_if1;

} stats_res_t;

typedef struct
{

  pid_t    pid;

  uint16_t interval_duration;

  uint32_t no_intervals;

  double   etime_sec_total;

  double   utime_sec_avg;
  double   utime_sec_min;
  double   utime_sec_max;
  uint16_t utime_sec_min_interval;
  uint16_t utime_sec_max_interval;
  
  double   stime_sec_avg;
  double   stime_sec_min;
  double   stime_sec_max;
  uint16_t stime_sec_min_interval;
  uint16_t stime_sec_max_interval;
  
  double   cpu_percent_avg;
  double   cpu_percent_min;
  double   cpu_percent_max;
  uint16_t cpu_percent_min_interval;
  uint16_t cpu_percent_max_interval;

  

  /* prp interface ... */
  char     if_name_prp[ IF_NAME_LEN ];
  
  double   rx_bitrate_prp_avg;
  double   rx_bitrate_prp_min;
  double   rx_bitrate_prp_max;
  uint16_t rx_bitrate_prp_min_interval;
  uint16_t rx_bitrate_prp_max_interval;
  
  double   tx_bitrate_prp_avg;
  double   tx_bitrate_prp_min;
  double   tx_bitrate_prp_max;
  uint16_t tx_bitrate_prp_min_interval;
  uint16_t tx_bitrate_prp_max_interval;

  

  /* first prp involved interface */
  char     if_name_if0[ IF_NAME_LEN ];
  
  double   rx_bitrate_if0_avg;
  double   rx_bitrate_if0_min;
  double   rx_bitrate_if0_max;
  uint16_t rx_bitrate_if0_min_interval;
  uint16_t rx_bitrate_if0_max_interval;

  double   tx_bitrate_if0_avg;
  double   tx_bitrate_if0_min;
  double   tx_bitrate_if0_max;
  uint16_t tx_bitrate_if0_min_interval;
  uint16_t tx_bitrate_if0_max_interval;

  

  /* second prp involved interface */
  char     if_name_if1[ IF_NAME_LEN ];
  
  double   rx_bitrate_if1_avg;
  double   rx_bitrate_if1_min;
  double   rx_bitrate_if1_max;
  uint16_t rx_bitrate_if1_min_interval;
  uint16_t rx_bitrate_if1_max_interval;

  double   tx_bitrate_if1_avg;
  double   tx_bitrate_if1_min;
  double   tx_bitrate_if1_max;
  uint16_t tx_bitrate_if1_min_interval;
  uint16_t tx_bitrate_if1_max_interval;

  
  
  /* total bytes received and sent */
  uint64_t rx_bytes_prp_total;
  uint64_t tx_bytes_prp_total;

  uint64_t rx_bytes_if0_total;
  uint64_t tx_bytes_if0_total;

  uint64_t rx_bytes_if1_total;
  uint64_t tx_bytes_if1_total;

} stats_res_overall_t;

typedef struct
{
  uint32_t line;
  uint32_t column;
} file_coords_t;

typedef struct
{

  pid_t         pid; 
  char*         path;
  file_coords_t coordinates;
  char*         file_content;
  char*         file_matrix[ FILE_BUFFER_SIZE / 2 ][ FILE_BUFFER_SIZE / 2 ];
  uint16_t      lines_count;
  /* columns per line */
  uint16_t      columns_count[ FILE_BUFFER_SIZE / 2 ];
  char*         column_delimiter;
  
  uint8_t       value_type;
  int64_t       value_num;
  char*         value_str;

} file_params_t;

#endif /* MEAS_TYPES_H_ */
