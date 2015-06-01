  ...
  /* get the requested values from /proc */
  for( i = 0; i < cmd_line_count; i++ )
  {
    data_snapshot->pid = ( *( file_params_list + i ) )->pid;
    get_proc_value( *( file_params_list + i ) );
    
    /* fill the value to the corresponding struct field
     * according to the prepared cmd_lines in cmds-file */
    switch( i )
    {
      case INDEX_UTIME:
      {
        data_snapshot->utime = ( *( file_params_list + i ) )->value_num;
        break;
      }
      case INDEX_STIME:
      {
        data_snapshot->stime = ( *( file_params_list + i ) )->value_num;
        break;
      }
      case INDEX_IF_NAME_PRP:
      {
        /* it is a string we don't have to get 
         * the proc value as number (int64_t) */
        strncpy( data_snapshot->if_name_prp,                 /* destination */
                 ( *( file_params_list + i ) )->value_str,   /* source */ 
                 ( IF_NAME_LEN - 1 ) );                      /* length */
        break;
      }
      case INDEX_RX_BYTES_PRP:
      {
        data_snapshot->rx_bytes_prp = ( *( file_params_list + i ) )->value_num;
        break;
      }
      case INDEX_TX_BYTES_PRP:
      {
        data_snapshot->tx_bytes_prp = ( *( file_params_list + i ) )->value_num;
        break;
      }
      case INDEX_IF_NAME_IF0:
      {
        /* it is a string we don't have to get 
         * the proc value as number (int64_t) */
        strncpy( data_snapshot->if_name_if0,                 /* destination */
                 ( *( file_params_list + i ) )->value_str,   /* source */ 
                 ( IF_NAME_LEN - 1 ) );                      /* length */
        break;
      }
      case INDEX_RX_BYTES_IF0:
      {
        data_snapshot->rx_bytes_if0 = ( *( file_params_list + i ) )->value_num;
        break;
      }
      case INDEX_TX_BYTES_IF0:
      {
        data_snapshot->tx_bytes_if0 = ( *( file_params_list + i ) )->value_num;
        break;
      }
      case INDEX_IF_NAME_IF1:
      {
        strncpy( data_snapshot->if_name_if1,                 /* destination */
                 ( *( file_params_list + i ) )->value_str,   /* source */ 
                 ( IF_NAME_LEN - 1 ) );                      /* length */
        /* it is a string we don't have to get 
         * the proc value as number (int64_t) */
        break;
      }
      case INDEX_RX_BYTES_IF1:
      {
        data_snapshot->rx_bytes_if1 = ( *( file_params_list + i ) )->value_num;
        break;
      }
      case INDEX_TX_BYTES_IF1:
      {
        data_snapshot->tx_bytes_if1 = ( *( file_params_list + i ) )->value_num;
        break;
      }
      default:
      {
        perror( "Error in assigning collected data to stats_t. Exiting ...\n" );
        exit( EXIT_FAILURE );
        break;
      }
    }
  }
  ...
