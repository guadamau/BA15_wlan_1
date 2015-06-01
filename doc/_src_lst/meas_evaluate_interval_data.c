stats_res_t* evaluate_interval_data( stats_t* start_data, stats_t* end_data, uint16_t interval_no )
{
  stats_res_t* results = NULL;

  /* clock ticks per second */
  long clktck;

  results = ( stats_res_t* )malloc( sizeof( stats_res_t ) );
  
  if( results == NULL )
  {
    /* Should never come here. */
    perror( "Memory allocation failure. Exiting now.\n" );
    exit( EXIT_FAILURE );
  }
  
  clktck = sysconf( _SC_CLK_TCK );

  if( start_data->pid != end_data->pid )
  {
    perror( "PID of start and end data statistics must match. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }


  /* set the interval number to a result set */
  results->interval_no = interval_no;
  /* ****************************************** */
  
  /* set the pid to a result set */
  results->pid = start_data->pid;
  /* ****************************************** */
  
  /* add elapsed time to the result set */
  results->etime_sec = start_data->etime_sec;
  /* ****************************************** */
  
  /* add overhead time to the result set */
  results->otime_usec = start_data->otime_usec;
  /* ****************************************** */
  
  /* utime and stime */
  if( clktck > 0 )
  {
    results->utime_sec = ( end_data->utime - start_data->utime ) / ( double )clktck;
    results->stime_sec = ( end_data->stime - start_data->stime ) / ( double )clktck;
  }
  else
  {
    perror( "Cant calculate time values. \n" 
            "Couldn't determine Clock ticks per second.\n"
            " Exiting now ..." );
    exit( EXIT_FAILURE );
  }
  /* ****************************************** */

  /* cpu percentage */
  if( results->etime_sec > 0.0 )
  {
    results->cpu_percent = ( results->utime_sec + results->stime_sec ) 
                           / results->etime_sec * 100;
  }
  else
  {
    results->cpu_percent = 0.0;
  }
  /* ****************************************** */
  
  /* interface names were allready set in collect.c
   * we can just memcpy here. */

  memcpy( results->if_name_prp,     /* destination */
          start_data->if_name_prp,  /* source */
          IF_NAME_LEN );            /* length */
  
  memcpy( results->if_name_if0,     /* destination */
          start_data->if_name_if0,  /* source */
          IF_NAME_LEN );            /* length */
  
  memcpy( results->if_name_if1,     /* destination */
          start_data->if_name_if1,  /* source */
          IF_NAME_LEN );            /* length */
  
  /* ****************************************** */

  /* machine wide network data, not process specific */

  if( results->etime_sec > 0.000000 )
  {

    /* 
     * The values about the prp interface in the 
     * /proc-filesystemhave have a width of 32 bit.
     * So if UINT32_MAX has reached, an overflow occurs
     * in the /proc byte statistics.
     *
     * In our resultset struct we use signed 64 bit integers,
     * which let us easily correct the byte value
     * in case of an overflow.
     *
     * */
    if( end_data->rx_bytes_prp < start_data->rx_bytes_prp )
    {
      results->rx_bytes_prp =   ( UINT32_MAX + end_data->rx_bytes_prp )
                              - start_data->rx_bytes_prp;
    }
    else
    {
      results->rx_bytes_prp = end_data->rx_bytes_prp - start_data->rx_bytes_prp; 
    }
    
    if( end_data->tx_bytes_prp < start_data->tx_bytes_prp )
    {
      results->tx_bytes_prp =   ( UINT32_MAX + end_data->tx_bytes_prp )
                              - start_data->tx_bytes_prp;
    }
    else
    {
      results->tx_bytes_prp = end_data->tx_bytes_prp - start_data->tx_bytes_prp; 
    }

    /* following interface stats are not UINT32 limited */
    results->rx_bytes_if0 = end_data->rx_bytes_if0 - start_data->rx_bytes_if0; 
    results->tx_bytes_if0 = end_data->tx_bytes_if0 - start_data->tx_bytes_if0; 
    
    results->rx_bytes_if1 = end_data->rx_bytes_if1 - start_data->rx_bytes_if1; 
    results->tx_bytes_if1 = end_data->tx_bytes_if1 - start_data->tx_bytes_if1;
    /* ************************************************************************* */
    
    results->rx_byterate_prp = results->rx_bytes_prp / results->etime_sec;
    results->tx_byterate_prp = results->tx_bytes_prp / results->etime_sec;
    
    results->rx_byterate_if0 = results->rx_bytes_if0 / results->etime_sec;
    results->tx_byterate_if0 = results->tx_bytes_if0 / results->etime_sec;
    
    results->rx_byterate_if1 = results->rx_bytes_if1 / results->etime_sec;
    results->tx_byterate_if1 = results->tx_bytes_if1 / results->etime_sec;

    
    results->rx_bitrate_prp = results->rx_byterate_prp * BIT_PER_BYTE;
    results->tx_bitrate_prp = results->tx_byterate_prp * BIT_PER_BYTE;
    
    results->rx_bitrate_if0 = results->rx_byterate_if0 * BIT_PER_BYTE;
    results->tx_bitrate_if0 = results->tx_byterate_if0 * BIT_PER_BYTE;
    
    results->rx_bitrate_if1 = results->rx_byterate_if1 * BIT_PER_BYTE;
    results->tx_bitrate_if1 = results->tx_byterate_if1 * BIT_PER_BYTE;
  
  }
  else
  {

    perror( "Invalid elapsed time. Aborting result evalutation.\n" );
    exit( EXIT_FAILURE );

  }

  /* ****************************************** */
  
  return results;
}

