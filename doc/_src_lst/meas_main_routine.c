  ...
  file_params_list = create_file_params( args );
  
  buffers_metadata = get_meas_buffers_meta_data( file_params_list );
  buffers = init_meas_buffers( &buffers_metadata, ( uint16_t )args.no_intervals );
  
  /* capture measurement data into the measurement buffers */
  for( i = 0; i < ( args.no_intervals + 1 ); i++ )
  {
    /* ============================================================== */
    
    startGTimer( timer );
    
    startGTimer( overhead_timer );
    read_to_meas_buffers( buffers, buffers_metadata.unique_paths_count, i );
    stopGTimer( overhead_timer );

    usleep( ( args.interval * 1000 ) 
            - getWallGUTime( overhead_timer ) );
    
    stopGTimer( timer );

    ( time_values + i )->otime_usec = getWallGUTime( overhead_timer );
    ( time_values + i )->etime_sec = getWallGTime( timer );

    /* ============================================================== */
  }

  /* perpare the measurement snapshots for evaluation */
  for( i = 0; i < ( args.no_intervals + 1 ); i++ )
  {
    *( data_snapshots + i ) = get_stat_vals( file_params_list, buffers, 
                                             buffers_metadata.unique_paths_count, i );
  
    /* pass timevalues to data snapshot */
    ( *( data_snapshots + i ) )->otime_usec = ( time_values + i )->otime_usec;
    ( *( data_snapshots + i ) )->etime_sec = ( time_values + i )->etime_sec;
  }

  /* evaluate all the snapshot data */
  for( i = 1; i <= args.no_intervals; i++ )
  {  
    start_data = *( data_snapshots + ( i - 1 ) );
    end_data = *( data_snapshots + i );
    *( results_list + ( i - 1 ) ) = evaluate_interval_data( start_data, end_data, 
                                                            ( uint16_t )( i - 1 ) );
    
    /* print data of each elapsed interval 
     * at this verbosity level */
    if( args.verbosity_level == 2 )
    {
      output_results_single( ( void* )stdout, *( results_list + ( i - 1 ) ), NULL, prp_enabled );
    }
    start_data = NULL;
    end_data = NULL;
  }
  
  release_meas_buffers( &buffers_metadata, buffers );
  
  /* clean up the data snapshots */
  for( i = 0; i < ( args.no_intervals + 1 ); i++ )
  {
    free( *( data_snapshots + i ) );
    *( data_snapshots + i ) = NULL;
  }
  free( data_snapshots );
  data_snapshots = NULL;

  overall_stats = evaluate_overall_data( results_list, args.no_intervals, args.interval );
  
  if( args.verbosity_level == 1 || args.verbosity_level == 2 )
  {
    output_results_overall( ( void* )stdout, overall_stats, NULL, prp_enabled );
  }
  
  output_results_all( args.store_path, results_list, args.no_intervals, FILE_PREFIX_1, prp_enabled );
  output_results_overall( args.store_path, overall_stats, FILE_PREFIX_2, prp_enabled );
  
  clear_file_params( file_params_list );

  /* clean up ... */
  free( time_values );

  for( i = 0; i < args.no_intervals; i++ )
  {
    free( *( results_list + i ) );
  }
  free( results_list );

  free( overall_stats );
  ...
