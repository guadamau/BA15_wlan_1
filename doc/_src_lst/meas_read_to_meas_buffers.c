void read_to_meas_buffers( meas_buffer_t** meas_buffers, 
                           uint16_t meas_buffers_len,
                           uint16_t interval_no )
{
  uint16_t i;
  ssize_t bytes_read;
  off_t offset_location;
  uint32_t c_buf_offset;
  meas_buffer_t* mb = NULL;
  
  for( i = 0; i < meas_buffers_len; i++ )
  {
    mb = ( *( meas_buffers + i ) );
    c_buf_offset = mb->slice_size * interval_no;
    bytes_read = -1;
    offset_location = -1;
    
    /* 
     * Read to the given file_descriptor. The start position of the
     * file buffer is the start address of our content_buffer 
     * with an offset of our buffersize of one file read multiplied by
     * the interval number.
     * The last argument passed to read is the size of one file read.
     * The the status of the file at each intervals time is stored
     * consecutively in the content_buffer.
     * */
    bytes_read = read( mb->file_descriptor,
                       ( mb->content_buffer + c_buf_offset ),
                       mb->slice_size );
    
    /* reposition the file contents pointer to the beginning of the
     * file after reading from it */
    offset_location = lseek( mb->file_descriptor, 0, SEEK_SET );

    if( bytes_read == -1 || offset_location == -1 )
    {
      perror( "Failed to read from file. Exiting ...\n" );
      exit( EXIT_FAILURE );
    }
    else if( bytes_read >= mb->slice_size )
    {
      perror( "File read buffer is too small, "
              "for the requested file. Exiting" );
      exit( EXIT_FAILURE );
    }
    else
    {
      /* NULL-Terminate the text. */
      *( mb->content_buffer + c_buf_offset + bytes_read ) = '\0';
    }
  }
}
