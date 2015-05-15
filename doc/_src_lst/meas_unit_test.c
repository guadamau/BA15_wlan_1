#define UTIME_EXPECTED             8319
#define STIME_EXPECTED            61330
#define IF_NAME_EXPECTED        "eth0:"
#define RX_BYTES_EXPECTED   208868176ul
#define TX_BYTES_EXPECTED  9437161222ul


int main( int argc, char* argv[] )
{
  cmd_args_t args;
 
  stats_t* data_snapshot = NULL;
  
  file_params_t** file_params_list = NULL;
  
  meas_buffer_meta_t buffers_metadata;
  meas_buffer_t** buffers = NULL;

  uint32_t i;

  args.pid             =  0;
  args.prp_mode        =  0;
  args.interval        =  10*1000;
  args.no_intervals    =  1;
  args.verbosity_level =  0;
  
  memset( args.store_path, '\0', PATH_LEN );
  
  set_prp_mode( args.prp_mode );
  init_proc_properties();
  
  data_snapshot = ( stats_t* )malloc( sizeof( stats_t ) * 1 );
  if( data_snapshot == NULL )
  {
    perror( "Memory allocation failure. Exiting...\n" );
    exit( EXIT_FAILURE );
  }
  
  file_params_list = create_file_params( args );
  
  buffers_metadata = get_meas_buffers_meta_data( file_params_list );
  buffers = init_meas_buffers( &buffers_metadata, ( uint16_t )args.no_intervals );
  
  read_to_meas_buffers( buffers, buffers_metadata.unique_paths_count, 0 );

  data_snapshot = get_stat_vals( file_params_list, buffers,
                                 buffers_metadata.unique_paths_count, 0 );
  
  release_meas_buffers( &buffers_metadata, buffers );

  assert( data_snapshot->utime == UTIME_EXPECTED ); 
  assert( data_snapshot->stime == STIME_EXPECTED ); 
  assert( strcmp( data_snapshot->if_name_if0, IF_NAME_EXPECTED ) == 0 ); 
  assert( data_snapshot->rx_bytes_if0 == RX_BYTES_EXPECTED ); 
  assert( data_snapshot->tx_bytes_if0 == TX_BYTES_EXPECTED );

  printf( "All unit tests successful.\n" ); 

  free( data_snapshot );
  data_snapshot = NULL;
  
  clear_file_params( file_params_list );
 
  return EXIT_SUCCESS;
}
