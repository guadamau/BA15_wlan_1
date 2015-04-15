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
   File:       evaluate.h
   Autor:      leibupro
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


