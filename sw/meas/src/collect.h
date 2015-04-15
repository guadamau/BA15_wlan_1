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
   File:       collect.h
   Autor:      leibupro
   Date:       03.2015
   
   Purpose:    Collect specified values from the /proc-Filesystem 
               
   Desc:       -
   ***************************************************************************** */
#ifndef MEAS_COLLECT_H_
#define MEAS_COLLECT_H_

#include "types.h"

#define STD_STRING_MALLOC 1024

#define INDEX_UTIME         0
#define INDEX_STIME         1
#define INDEX_IF_NAME_PRP   2
#define INDEX_RX_BYTES_PRP  3
#define INDEX_TX_BYTES_PRP  4
#define INDEX_IF_NAME_IF0   5
#define INDEX_RX_BYTES_IF0  6
#define INDEX_TX_BYTES_IF0  7
#define INDEX_IF_NAME_IF1   8
#define INDEX_RX_BYTES_IF1  9
#define INDEX_TX_BYTES_IF1 10


/* function prototypes */
stats_t* get_stat_vals( file_params_t** file_params_list );
void     get_proc_value( file_params_t* file_params );
void     prepare_file_matrix( file_params_t* file_params );
void*    read_file( char* file_path );

file_params_t** create_file_params( cmd_args_t args );
void            purge_file_contents( file_params_t** file_params_list );
void            clear_file_params( file_params_t** file_params_list );

#endif /* MEAS_COLLECT_H_ */
