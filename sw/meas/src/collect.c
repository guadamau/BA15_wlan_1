/* *****************************************************************************
   File:       collect.c
   Authors:    leibupro
               guadamau
   Date:       03.2015
   
   Purpose:    Collect specified values from the /proc-Filesystem 
               
   Desc:       -
   ***************************************************************************** */

#include "collect.h"

#include <bsd/stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "pstring.h"

#define MAX_CMD_LINES        16
#define TOKEN_MALLOC_SIZE     8
#define LINE_DELIM         "\n"


/* 
 * read the global vars of the command lines: 
 * 
 * init_paths
 * init_search
 * init_val_pos
 * init_val_delim
 * init_val_types
 * cmd_line_count
 * 
 * */
#include "attrs.h"

/* private function prototypes */
unsigned char is_printable_str( const char* str );
file_coords_t get_file_coordinates( uint8_t par_index );
void get_proc_value( file_params_t* file_params );
void prepare_file_matrix( file_params_t* file_params );
char* get_file_content_from_buffer( char* path, meas_buffer_t** meas_buffers,
                                    uint16_t meas_buffers_len, uint16_t interval_no );

unsigned char is_printable_str( const char* str )
{
  unsigned char ret_val = 1;
  
  while( *str )
  {
    if( !isprint( *str ) )
    {
      ret_val = 0;
      break;
    }
    else
    {
      str++;
    }
  }

  return ret_val;
}


file_coords_t get_file_coordinates( uint8_t par_index )
{
  file_coords_t coords;
  coords.line = 0;
  coords.column = 0;
  char* token_str = NULL;
  char* token = NULL;
  uint8_t coords_parsed = 0;
  const char* errstr;
  
  token_str = ( char* )malloc( sizeof( char ) * TOKEN_MALLOC_SIZE );
  if( token_str == NULL )
  {
    /* Should never come here. */
    perror( "Memory allocation failure. Exiting now.\n" );
    exit( EXIT_FAILURE );
  }

  strncpy( token_str, init_val_pos[ par_index ], ( TOKEN_MALLOC_SIZE - 1 ) );
  /* just if token is too long */
  *( token_str + ( TOKEN_MALLOC_SIZE -1 ) ) = '\0';
  
  token = strtok( token_str, ":" );

  while( token != NULL && coords_parsed < 2 )
  {
    if( coords_parsed == 0 && token == NULL )
    {
      perror( "No token found for parsing coordinates. Exiting ...\n" );
      exit( EXIT_FAILURE );
    }
    coords_parsed++;

    switch( coords_parsed )
    {
      case 1:
      {
        coords.line = strtonum( token, 1, FILE_BUFFER_SIZE / 2 - 1, &errstr );
        coords.line -= 1;
        break;
      }
      case 2:
      {
        coords.column = strtonum( token, 1, FILE_BUFFER_SIZE / 2 - 1, &errstr );
        coords.column -= 1;
        break;
      }
      default:
      {
        perror( "Invalid token. Exiting ...\n" );
        exit( EXIT_FAILURE );
        break;
      }
    }

    if( errstr )
    {
      errx( 1, "specified coordinate is %s: %s\n", errstr, token );
      exit( EXIT_FAILURE );
    }

    token = strtok( NULL, ":" );
    
  }

  free( token_str );
  token_str = NULL;

  /* just for debugging purposes */
  /* printf( "line: %d, column: %d\n", coords.line, coords.column ); */

  return coords;
}


file_params_t** create_file_params( cmd_args_t args )
{
  file_params_t** file_params_list = NULL;
  
  char* path    = NULL;
  char* search  = NULL;
  char* replace = NULL;
  char* delim   = NULL;

  file_params_t* file_params = NULL;
 
  const char* errstr = NULL; 

  /* Buffer on stack with to verify the strings */
  char path_buf[ STD_STRING_MALLOC ] = { '\0' };
  char search_buf[ STD_STRING_MALLOC ]   = { '\0' };

  unsigned char i;
  uint16_t j;
  uint16_t k;

  /* check cmd_line_count */
  if( cmd_line_count <= 0 )
  {
    perror( "At least one command line has to be configured.\n" );
    exit( EXIT_FAILURE );
  }
  else if( cmd_line_count > MAX_CMD_LINES )
  {
    perror( "C'mon, srsly? Set a realistic vlaue to cmd_line_count.\n" );
    exit( EXIT_FAILURE );
  }

  /* Allocate memory for the file parameters */
  file_params_list = ( file_params_t** )malloc( sizeof( file_params_t* ) * cmd_line_count );
  
  if( file_params_list == NULL )
  {
    /* Should never come here. */
    perror( "Memory allocation failure. Exiting now.\n" );
    exit( EXIT_FAILURE );
  }

  for( i = 0; i < cmd_line_count; i++ )
  {
    file_params = ( file_params_t* )malloc( sizeof( file_params_t ) * 1 );
    if( file_params == NULL )
    {
      /* Should never come here. */
      perror( "Memory allocation failure. Exiting now.\n" );
      exit( EXIT_FAILURE );
    }

    /* PID we can alredy assign here. */
    file_params->pid = args.pid;

    /* init the struct reference values */
    file_params->path = NULL;
    file_params->column_delimiter = NULL;
    
    *( file_params_list + i ) = file_params;
    file_params = NULL;
  }

  /* check if the cmd_line strings are valid */
  for( i = 0; i < cmd_line_count; i++ )
  {
    strncpy( path_buf, init_paths[ i ], STD_STRING_MALLOC - 1 );
    strncpy( search_buf, init_search[ i ], STD_STRING_MALLOC - 1 );

    if( !is_printable_str( path_buf ) || 
        !is_printable_str( search_buf ) )
    {
      perror( "Invalid strings in init_cmd_lines" 
              " or init_cmd_lines_replace. Exiting now.\n" );
      exit( EXIT_FAILURE );
    }
            
    /* Strings must be placed on the heap now. */
    path    = ( char* )malloc( sizeof( char ) * STD_STRING_MALLOC );
    search  = ( char* )malloc( sizeof( char ) * STD_STRING_MALLOC );
    replace = ( char* )malloc( sizeof( char ) * STD_STRING_MALLOC );
    /* delimiter must be just one character, a string of length 1 */
    delim   = ( char* )malloc( sizeof( char ) * 2 );

    if( path == NULL || search == NULL || 
        replace == NULL || delim == NULL )
    {
      /* Should never come here. */
      perror( "Memory allocation failure. Exiting now.\n" );
      exit( EXIT_FAILURE );
    }

    /* Assign strings to the allocated heap space.  */
    snprintf( path,    STD_STRING_MALLOC, path_buf );
    snprintf( search,  STD_STRING_MALLOC, search_buf );
    snprintf( replace, STD_STRING_MALLOC, "%d", args.pid );
    
    path = string_replace_ext( search, replace, path );

    ( *( file_params_list + i ) )->path = path; 
    
    ( *( file_params_list + i ) )->coordinates = get_file_coordinates( i );
    
    
    /* delimiter can only consist of one char,
     * so we take just the first character of init_val_delim */
    strncpy( delim, init_val_delim[ i ], 1 );
    *( delim + 1 ) = '\0';
    
    ( *( file_params_list + i ) )->column_delimiter = delim;
    
    /* value_type can be directly set ... */
    ( *( file_params_list + i ) )->value_type = 
    strtonum( init_val_types[ i ], 0, UINT8_MAX, &errstr );
    if( errstr )
    {
      errx( 1, "Invalid value type specified: %s, %s\n", 
            errstr, init_val_types[ i ] );
      exit( EXIT_FAILURE );
    }

    /* init the file_matrix with NULL-values */
    for( j = 0; j < FILE_BUFFER_SIZE / 2; j++ )
    {
      for( k = 0; k < FILE_BUFFER_SIZE / 2; k++ )
      {
        ( *( file_params_list + i ) )->file_matrix[ j ][ k ] = NULL;
      }
    }

    memset( path_buf, '\0', STD_STRING_MALLOC );
    memset( search_buf, '\0', STD_STRING_MALLOC );

    free( search );
    search = NULL;
    free( replace );
    replace = NULL;

    path     = NULL;
    search   = NULL;
    replace  = NULL;
    delim    = NULL;
  }
  
  return file_params_list;
}


void get_proc_value( file_params_t* file_params )
{
  char* str_value = NULL;

  const char* errstr = NULL;
  
  /* Check if file coordintaes do exist. */
  if( file_params->coordinates.line >= file_params->lines_count )
  {
    perror( "Specified line does not exist. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }
  if( file_params->coordinates.column >= file_params->columns_count[ file_params->coordinates.line ] )
  {  
    perror( "Specified column does not exist. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }
  
  str_value = file_params->file_matrix[ file_params->coordinates.line ][ file_params->coordinates.column ]; 
  
  switch( file_params->value_type )
  {
    case VALUE_NUM:
    {
      file_params->value_num = strtonum( str_value, INT64_MIN, INT64_MAX, &errstr );
      break;
    }
    case VALUE_STR:
    {
      file_params->value_str = str_value;
      break;
    }
    default:
    {
      break;
    }
  }

  if( errstr )
  {
    errx( 1, "specified coordinate is %s: %s\n", errstr, str_value ); 
    exit( EXIT_FAILURE );
  }
}


void prepare_file_matrix( file_params_t* file_params )
{
  char* file_content = NULL;

  char* line = NULL;
  uint16_t lines_count = 0;

  char* column = NULL;
  uint16_t columns_count = 0;

  /* the size of the array can be changed.
   * dependent to the file buffer size
   * FILE_BUFFER_SIZE / 2 is the case if each line
   * would just contian one character. */
  char* lines[ FILE_BUFFER_SIZE / 2 ] = { NULL };
  
  uint16_t i;

  /* assign the file content */
  file_content = file_params->file_content;

  /* just for debugging purposes ... */
  /* printf( "File content: %s\n\n", file_content ); */

  /* first lets split the input buffer in single lines. */
  line = strtok( file_content, LINE_DELIM );
  
  while( line != NULL )
  {
    lines[ lines_count ] = line;
    lines_count++;

    line = strtok( NULL, LINE_DELIM );
  }

  file_params->lines_count = lines_count;
  
  /* 
   * Now we have to split every line with specified delimiter
   * consecutive delimiters are ignored. 
   * This is a property of the strtok-function.
   * */
  for( i = 0; i < lines_count; i++ )
  {
    line = lines[ i ];
    column = strtok( line, file_params->column_delimiter );

    while( column != NULL )
    {
      file_params->file_matrix[ i ][ columns_count ] = column;
      columns_count++;

      column = strtok( NULL, file_params->column_delimiter );
    }
    file_params->columns_count[ i ] = columns_count;

    /* reset columns count here */
    columns_count = 0;
  }
}


stats_t* get_stat_vals( file_params_t** file_params_list, meas_buffer_t** meas_buffers, 
                        uint16_t meas_buffers_len, uint16_t interval_no )
{
  stats_t* data_snapshot = NULL;

  unsigned char i, j;
  uint32_t k, l;
  char* cur_path = NULL;

  char** file_paths = NULL;

  data_snapshot = ( stats_t* )malloc( sizeof( stats_t ) );

  if( data_snapshot == NULL )
  {
    /* Should never come here. */
    perror( "Memory allocation failure. Exiting now.\n" );
    exit( EXIT_FAILURE );
  }

  memset( ( void* )( data_snapshot->if_name_prp ), '\0', IF_NAME_LEN );
  memset( ( void* )( data_snapshot->if_name_if0 ), '\0', IF_NAME_LEN );
  memset( ( void* )( data_snapshot->if_name_if1 ), '\0', IF_NAME_LEN );
  
  /* 
   * If there is the same file twice (or more) in the 
   * file params list, with just other coordinates to read, 
   * we read each file ONLY ONCE here. Otherwise the 
   * consistency of the values cannot be guaranteed.
   *
   * Be also aware about the fact, that there are files 
   * in the /proc-filesystem whose consistency scope 
   * is only per row!
   * */
  file_paths = ( char** )malloc( sizeof( char* ) * cmd_line_count );
  
  if( file_paths == NULL )
  {
    /* Should never come here. */
    perror( "Memory allocation failure. Exiting now.\n" );
    exit( EXIT_FAILURE );
  }

  /* 
   * Read the content of the first file and 
   * add the path to the known file paths.
   * And of course parse the content previously read
   * to the file matrix structure, to access the
   * coordinates.
   * */
  ( *( file_params_list + 0 ) )->file_content = 
    get_file_content_from_buffer( ( *( file_params_list + 0 ) )->path, meas_buffers,
                                  meas_buffers_len, interval_no );

  *( file_paths + 0 ) =  ( *( file_params_list + 0 ) )->path;
  prepare_file_matrix( *( file_params_list + 0 ) );

  /* 
   * continue with the rest ... 
   * 
   * We are looking for duplicate file paths here.
   * If there is a duplicate path the file content
   * has to be read just once.
   * In case of a duplicate we just assign a pointer
   * to an already read content. 
   * */
  for( i = 1; i < cmd_line_count; i++ )
  {
    cur_path = ( *( file_params_list + i ) )->path;
    for( j = 0; j < i; j++ )
    {
      if( strcmp( cur_path, *( file_paths + j ) ) == 0 )
      {
        /* file content, just a pointer assignment */
        ( *( file_params_list + i ) )->file_content = 
          ( *( file_params_list + j ) )->file_content;
        
        /* 
         * We also need to copy the file matrix (deep copy), 
         * lines count and columns count per line here. 
         * */
        /* file matrix (2d) */
        for( k = 0; k < ( *( file_params_list + j ) )->lines_count; k++ )
        {
          for( l = 0; l < ( *( file_params_list + j ) )->columns_count[ k ]; l++ )
          {
            ( *( file_params_list + i ) )->file_matrix[ k ][ l ] =
              ( *( file_params_list + j ) )->file_matrix[ k ][ l ];
          } 
        }
        
        /* lines count */
        ( *( file_params_list + i ) )->lines_count = 
          ( *( file_params_list + j ) )->lines_count;
        
        /* columns count per line (1d) */
        for( k = 0; k < ( *( file_params_list + j ) )->lines_count; k++ )
        {
          ( *( file_params_list + i ) )->columns_count[ k ] =
            ( *( file_params_list + j ) )->columns_count[ k ];
        }

        break;
      }
      else if( j == ( i - 1 ) )
      {
        ( *( file_params_list + i ) )->file_content = 
          get_file_content_from_buffer( ( *( file_params_list + i ) )->path, meas_buffers,
                                        meas_buffers_len, interval_no );
          
          prepare_file_matrix( *( file_params_list + i ) );
      }  
    }
    *( file_paths + i ) = cur_path;
  }

  /* just for debugging ... 
  for( i = 0; i < cmd_line_count; i++ )
  {
    printf( "index: %d, pointer to file content: %p\n",
            i, ( *( file_params_list + i ) )->file_content );
  }
  */
  
  /* no more need of the file_paths list */
  free( file_paths );

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

  return data_snapshot;
}


meas_buffer_meta_t get_meas_buffers_meta_data( file_params_t** file_params_list )
{
  meas_buffer_meta_t buffers_meta;
  uint16_t unique_paths_count = 0;
  uint16_t i, j;
  
  char* cur_path = NULL;

  buffers_meta.unique_paths = ( char** )malloc( sizeof( char* ) * cmd_line_count );
  if( buffers_meta.unique_paths == NULL )
  {
    perror( "Memory allocation failure unique paths. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }

  /* init our path strings ... */
  for( i = 0; i < cmd_line_count; i++ )
  {
    *( buffers_meta.unique_paths + i ) = ( char* )malloc( sizeof( char ) * STD_STRING_MALLOC );
    if( *( buffers_meta.unique_paths + i ) == NULL )
    {
      perror( "Memory allocation failure unique paths. Exiting ...\n" );
      exit( EXIT_FAILURE );
    }
    memset( *( buffers_meta.unique_paths + i ), '\0', STD_STRING_MALLOC );
  }

  /* 
   * Detect all the unique paths in our file_params_list. 
   * This is for performance reasons, to minimize
   * file reads while measuring.
   * */
  strncpy( *( buffers_meta.unique_paths + 0 ),  /* destination */
           ( *( file_params_list + 0 ) )->path, /* source */
           STD_STRING_MALLOC );                 /* length */
  unique_paths_count++;

  for( i = 1; i < cmd_line_count; i++ )
  {
    cur_path = ( *( file_params_list + i ) )->path;
    for( j = 0; j < i; j++ )
    {
      if( strcmp( cur_path, *( buffers_meta.unique_paths + j ) ) == 0 )
      {
        break;
      }
      else if( j == ( i - 1 ) )
      {
        strncpy( *( buffers_meta.unique_paths + unique_paths_count ),  /* destination */
                 ( *( file_params_list + i ) )->path,                  /* source */
                 STD_STRING_MALLOC );                                  /* length */
        unique_paths_count++;
      }
    }
  }
  buffers_meta.unique_paths_count = unique_paths_count;

  return buffers_meta;
}


meas_buffer_t** init_meas_buffers( meas_buffer_meta_t* buffers_meta, uint16_t no_intervals )
{
  meas_buffer_t** meas_buffers = NULL;
  uint16_t i;
  meas_buffer_t* mb = NULL;
  
  meas_buffers = 
    ( meas_buffer_t** )malloc( sizeof( meas_buffer_t* ) * buffers_meta->unique_paths_count );
  if( meas_buffers == NULL )
  {
    perror( "Memory allocation failure. meas buffers. Exiting...\n" );
    exit( EXIT_FAILURE );
  }

  for( i = 0; i < buffers_meta->unique_paths_count; i++ )
  {
    ( *( meas_buffers + i ) ) = ( meas_buffer_t* )malloc( sizeof( meas_buffer_t ) );
    if( ( *( meas_buffers + i ) ) == NULL )
    {
      perror( "Memory allocation failure. meas buffers. Exiting...\n" );
      exit( EXIT_FAILURE );
    }
    
    mb = ( *( meas_buffers + i ) );
    
    memcpy( mb->file_path,                           /* dest */
            *( ( buffers_meta->unique_paths ) + i ), /* src */
            STD_STRING_MALLOC );                     /* n */
    
    /* open file relative to directory file descriptor */
    mb->file_descriptor = open( mb->file_path, O_RDONLY | O_SYNC );
    if( mb->file_descriptor == -1 )
    {
      perror( "Failed to open requested file. Exiting ...\n" );
      exit( EXIT_FAILURE );
    }

    /* no_intervals + 1 is because we need one more interval
     * than the user specifies to calculate the last intervals data. */
    mb->content_buffer =
      ( char* )malloc( sizeof( char ) * FILE_BUFFER_SIZE * ( no_intervals + 1 ) );
    if( mb->content_buffer == NULL )
    {
      perror( "Memory allocation failure. file content buffer. Exiting...\n" );
      exit( EXIT_FAILURE );
    }

    /* initialize the content buffer, fill with zeros */
    memset( mb->content_buffer,
            '\0',
            ( sizeof( char ) * FILE_BUFFER_SIZE * ( no_intervals + 1 ) ) );
    
    mb->buffer_size = FILE_BUFFER_SIZE * ( no_intervals + 1 );
    mb->slice_size = FILE_BUFFER_SIZE;
    mb->slice_count = ( no_intervals + 1 );
  }

  return meas_buffers;
}


void read_to_meas_buffers( meas_buffer_t** meas_buffers, uint16_t meas_buffers_len,
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
     * Do not panic about the next instruction, here is the explanation ... 
     * read to the given file_descriptor. The start position of the
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


char* get_file_content_from_buffer( char* path, meas_buffer_t** meas_buffers,
                                    uint16_t meas_buffers_len, uint16_t interval_no )
{
  char* file_content = NULL;
  uint16_t i;
  meas_buffer_t* mb = NULL;
  
  for( i = 0; i < meas_buffers_len; i++ )
  {
    mb = ( *( meas_buffers + i ) );
    if( strncmp( path, mb->file_path, STD_STRING_MALLOC ) == 0 )
    {
      file_content = ( ( mb->content_buffer ) + ( mb->slice_size * interval_no ) );
      break;
    }
    else if( i == ( meas_buffers_len - 1 ) )
    {
      perror( "Requested path not found in meas buffers. Exiting...\n" );
      exit( EXIT_FAILURE );
    }
  }

  return file_content;
}


void release_meas_buffers( meas_buffer_meta_t* buffers_meta, meas_buffer_t** meas_buffers )
{
  /* 
   * This function frees all the 
   * reference data which were allocated 
   * in get_meas_buffers_meta_data 
   * and init_meas_buffers.
   * It also closes the file descriptors. 
   * */
  uint16_t i;
  uint16_t count;
  meas_buffer_t* mb = NULL;

  count = buffers_meta->unique_paths_count;

  /* Space was allocated for all possible cmd lines
   * with distinct paths. They all have to be freed again. */
  for( i = 0; i < cmd_line_count; i++ )
  {
    free( *( buffers_meta->unique_paths + i ) );
  }
  free( buffers_meta->unique_paths );

  for( i = 0; i < count; i++ )
  {
    mb = ( *( meas_buffers + i ) );

    /* close the filedescriptor */
    if( close( mb->file_descriptor ) == -1 )
    {
      perror( "Failed to close file descriptor. Continuing ...\n" );
    }

    free( mb->content_buffer );
    free( mb );
  }
  free( meas_buffers );
}


void clear_file_params( file_params_t** file_params_list )
{
  uint8_t i;

  /* clean the heap */
  for( i = 0; i < cmd_line_count; i++ )
  {
    free( ( *( file_params_list + i ) )->path );
    ( *( file_params_list + i ) )->path = NULL;

    free( ( *( file_params_list + i ) )->column_delimiter );
    ( *( file_params_list + i ) )->column_delimiter = NULL;
    
    free( *( file_params_list + i ) );
    *( file_params_list + i ) = NULL;
    
  }
  free( file_params_list );
  file_params_list = NULL;
}

