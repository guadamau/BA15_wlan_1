void print_one_interval_prp( FILE* fp, stats_res_t* results )
{
  fprintf( fp,
           "*******************************************************************************\n\n"
           " Process specific statistics, interval no: %5d\n\n"
           " PID:  %d \n"
           " Time: elapsed %5.3lfs, user %3.2lfs, sys %3.2lfs, overhead %dus\n"
           " CPU:  cpu workload %3.2lf%%\n\n\n"
           " Machine wide network stats for PRP involved interfaces, not process specific:\n\n"
           " Interface       RX Bytes       TX Bytes      RX Bitrate [MBit/s]      TX Bitrate [MBit/s]\n"
           " =============================================================================\n"
           " prp (%.4s)  %12lld   %12lld                 %8.2lf                 %8.2lf\n"
           " if0 (%.4s)  %12lld   %12lld                 %8.2lf                 %8.2lf\n"
           " if1 (%.4s)  %12lld   %12lld                 %8.2lf                 %8.2lf\n"
           " =============================================================================\n\n"
           "*******************************************************************************\n",
           results->interval_no,
           results->pid,
           results->etime_sec, 
           results->utime_sec, 
           results->stime_sec,
           results->otime_usec,
           results->cpu_percent,
           results->if_name_prp, 
           results->rx_bytes_prp,
           results->tx_bytes_prp,
           bit_s_to_mbit_s( results->rx_bitrate_prp ),
           bit_s_to_mbit_s( results->tx_bitrate_prp ),
           results->if_name_if0,
           results->rx_bytes_if0,
           results->tx_bytes_if0,
           bit_s_to_mbit_s( results->rx_bitrate_if0 ),
           bit_s_to_mbit_s( results->tx_bitrate_if0 ),
           results->if_name_if1,
           results->rx_bytes_if1,
           results->tx_bytes_if1,
           bit_s_to_mbit_s( results->rx_bitrate_if1 ),
           bit_s_to_mbit_s( results->tx_bitrate_if1 ) );
}
