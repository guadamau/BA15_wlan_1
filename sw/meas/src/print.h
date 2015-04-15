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
   File:       print.h
   Autor:      leibupro
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
