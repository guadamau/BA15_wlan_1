/* *****************************************************************************
   File:       attrs.c
   Authors:    leibupro
               guadamau
   Date:       05.2015
   
   Purpose:    Specifiy the values that have to be extracted 
               from the /proc-Filesystem. 
               
   Desc:       -
   ***************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "attrs.h"


/* private function declarations */
void initerr( void );

void set_prp_mode( unsigned char prp_mode )
{
  prp_enabled = prp_mode;
}


void init_proc_properties( void )
{
  unsigned char i;
  
  /* Initialize memory. Fill with null bytes ... */
  for( i = 0; i < MAX_PROC_VALS; i++ )
  {
    memset( init_paths    [ i ], '\0', PATH_LEN * sizeof( char ) );
    memset( init_search   [ i ], '\0', PATH_LEN * sizeof( char ) );
    memset( init_val_pos  [ i ], '\0', PATH_LEN * sizeof( char ) );
    memset( init_val_delim[ i ], '\0', PATH_LEN * sizeof( char ) );
    memset( init_val_types[ i ], '\0', PATH_LEN * sizeof( char ) );
  }
  
  /* 
   * Initialize the all data to get requested
   * values from the /proc filesystem
   * This has to be done in a very static and
   * hardcoded way yet.
   * /proc-Parameters could be moved to config files,
   * and the global vars then can be treated
   * with dynamic allocation.
   *
   * We think it is an overkill, due to the complexity
   * the parameters are used.
   * They sould NOT be outsourced to user editable 
   * configuration files.
   * */
  strncpy( init_paths[ INDEX_UTIME        ], "/proc/$PID$/stat", PATH_LEN );
  strncpy( init_paths[ INDEX_STIME        ], "/proc/$PID$/stat", PATH_LEN );
  strncpy( init_paths[ INDEX_IF_NAME_IF0  ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_RX_BYTES_IF0 ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_TX_BYTES_IF0 ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_IF_NAME_IF1  ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_RX_BYTES_IF1 ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_TX_BYTES_IF1 ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_IF_NAME_PRP  ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_RX_BYTES_PRP ], "/proc/net/dev"   , PATH_LEN );
  strncpy( init_paths[ INDEX_TX_BYTES_PRP ], "/proc/net/dev"   , PATH_LEN );


  strncpy( init_search[ INDEX_UTIME        ], "$PID$", PATH_LEN );
  strncpy( init_search[ INDEX_STIME        ], "$PID$", PATH_LEN );
  strncpy( init_search[ INDEX_IF_NAME_IF0  ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_RX_BYTES_IF0 ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_TX_BYTES_IF0 ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_IF_NAME_IF1  ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_RX_BYTES_IF1 ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_TX_BYTES_IF1 ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_IF_NAME_PRP  ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_RX_BYTES_PRP ], ""     , PATH_LEN );
  strncpy( init_search[ INDEX_TX_BYTES_PRP ], ""     , PATH_LEN );

  
  strncpy( init_val_pos[ INDEX_UTIME        ], "1:14", PATH_LEN );
  strncpy( init_val_pos[ INDEX_STIME        ], "1:15", PATH_LEN );
  strncpy( init_val_pos[ INDEX_IF_NAME_IF0  ], "6:1" , PATH_LEN );
  strncpy( init_val_pos[ INDEX_RX_BYTES_IF0 ], "6:2" , PATH_LEN );
  strncpy( init_val_pos[ INDEX_TX_BYTES_IF0 ], "6:10", PATH_LEN );
  strncpy( init_val_pos[ INDEX_IF_NAME_IF1  ], "5:1" , PATH_LEN );
  strncpy( init_val_pos[ INDEX_RX_BYTES_IF1 ], "5:2" , PATH_LEN );
  strncpy( init_val_pos[ INDEX_TX_BYTES_IF1 ], "5:10", PATH_LEN );
  strncpy( init_val_pos[ INDEX_IF_NAME_PRP  ], "7:1" , PATH_LEN );
  strncpy( init_val_pos[ INDEX_RX_BYTES_PRP ], "7:2" , PATH_LEN );
  strncpy( init_val_pos[ INDEX_TX_BYTES_PRP ], "7:10", PATH_LEN );

  
  strncpy( init_val_delim[ INDEX_UTIME        ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_STIME        ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_IF_NAME_IF0  ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_RX_BYTES_IF0 ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_TX_BYTES_IF0 ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_IF_NAME_IF1  ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_RX_BYTES_IF1 ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_TX_BYTES_IF1 ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_IF_NAME_PRP  ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_RX_BYTES_PRP ], " ", PATH_LEN );
  strncpy( init_val_delim[ INDEX_TX_BYTES_PRP ], " ", PATH_LEN );
  
  
  strncpy( init_val_types[ INDEX_UTIME        ], STR_VALUE_NUM, PATH_LEN );
  strncpy( init_val_types[ INDEX_STIME        ], STR_VALUE_NUM, PATH_LEN );
  strncpy( init_val_types[ INDEX_IF_NAME_IF0  ], STR_VALUE_STR, PATH_LEN );
  strncpy( init_val_types[ INDEX_RX_BYTES_IF0 ], STR_VALUE_NUM, PATH_LEN );
  strncpy( init_val_types[ INDEX_TX_BYTES_IF0 ], STR_VALUE_NUM, PATH_LEN );
  strncpy( init_val_types[ INDEX_IF_NAME_IF1  ], STR_VALUE_STR, PATH_LEN );
  strncpy( init_val_types[ INDEX_RX_BYTES_IF1 ], STR_VALUE_NUM, PATH_LEN );
  strncpy( init_val_types[ INDEX_TX_BYTES_IF1 ], STR_VALUE_NUM, PATH_LEN );
  strncpy( init_val_types[ INDEX_IF_NAME_PRP  ], STR_VALUE_STR, PATH_LEN );
  strncpy( init_val_types[ INDEX_RX_BYTES_PRP ], STR_VALUE_NUM, PATH_LEN );
  strncpy( init_val_types[ INDEX_TX_BYTES_PRP ], STR_VALUE_NUM, PATH_LEN );
  
  
  if( prp_enabled == PRP_ENABLED )
  {
    cmd_line_count = PRP_NO_VALUES;
  }
  else if( prp_enabled == PRP_DISABLED )
  {
    cmd_line_count = NO_VALUES;
  }
  else
  {
    perror( "Invalid PRP-Mode-Value. Exiting...\n" );
    fflush( stderr );
    exit( EXIT_FAILURE );
  }


  /* check for too long strings ... */
  for( i = 0; i < cmd_line_count; i++ )
  {
    if( init_paths[ i ][ PATH_LEN - 1 ] != '\0' )
    {
      initerr();
    }
  }
}


void initerr( void )
{
    perror( "Specified init /proc-parameter too long. Exiting...\n" );
    fflush( stderr );
    exit( EXIT_FAILURE );
}

