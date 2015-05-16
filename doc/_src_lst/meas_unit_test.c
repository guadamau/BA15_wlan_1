#define UTIME_EXPECTED             8319
#define STIME_EXPECTED            61330
#define IF_NAME_EXPECTED        "eth0:"
#define RX_BYTES_EXPECTED   208868176ul
#define TX_BYTES_EXPECTED  9437161222ul


int main( int argc, char* argv[] )
{
  ...
  
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

  ...
}
