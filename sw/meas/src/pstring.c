/* *****************************************************************************
   File:       pstring.c
   Authors:    leibupro
               guadamau
   Date:       03.2015
   
   Purpose:    Extra string functions like string replace. 
               
   Desc:       -
   ***************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "pstring.h"

/* function prototypes "private" */
char* string_replace( char* search, char* replace, char* string );



char* string_replace( char* search, char* replace, char* string )
{
  char* temp_string = NULL; 
  char* search_start = NULL;
  void* realloc_check = NULL;
  int offset = 0;
  int first_token_length = 0;
  int third_token_length = 0;


  /* check if search string is available */
  search_start = strstr( string, search );
  
  if( search_start == NULL ) 
  {
    /* Search pattern not found, just return the src string. */
    return string;
  }

  /* 
   * if it is an empty string to look for.
   * nothing has to be replaced. */
  if( *( search + 0 ) == 0 )
  {
    return string;
  }

  /* allocate memory */
  temp_string = ( char* )malloc( ( strlen( string ) + 1 ) * sizeof( char ) );
  
  if( temp_string == NULL ) 
  {
    perror( "Memory error. Exiting ... \n" );
    exit( EXIT_FAILURE );
  }

  /* create temporary copy */
  if( string != NULL )
  {
    strcpy( temp_string, string );
  }
  
  /* set first section in string */
  offset = ( search_start - string ) * sizeof( char );
  string[ offset ] = '\0';
  
  realloc_check = realloc( string, ( strlen( string ) + 1 ) * sizeof( char ) );
  if( realloc_check == NULL )
  {
    perror( "reallocation error. Exiting ... \n" );
    exit( EXIT_FAILURE );
  }
  string = ( char* )realloc_check;
  realloc_check = NULL;

  first_token_length = strlen( string );
  
  /* concatenate second section */ 
  realloc_check = realloc( string, 
                  ( strlen( string ) + strlen( replace ) + 1 ) * sizeof( char ) );
  if( realloc_check == NULL )
  {
    perror( "reallocation error. Exiting ... \n" );
    exit( EXIT_FAILURE );
  }
  string = ( char* )realloc_check;
  realloc_check = NULL;
  
  strcat( string, replace );

  /* concatenate third section */
  third_token_length = strlen( temp_string ) - first_token_length - strlen( search );
  
  realloc_check = realloc( string, strlen( string ) + third_token_length + 1 );
  if( realloc_check == NULL )
  {
    perror( "reallocation error. Exiting ... \n" );
    exit( EXIT_FAILURE );
  }
  string = ( char* )realloc_check;
  realloc_check = NULL;
  
  strcat( string, 
          ( char* )temp_string + 
          ( ( first_token_length + strlen( search ) ) * sizeof( char ) ) ) ;
  
  free( temp_string );

  return string;
}


char* string_replace_ext( char* search, char* replace, char* string )
{
  void* realloc_check = NULL;
  char* tmp_last_str = NULL;
  unsigned char keep_loopin = 1;
  uint16_t i = 0;
  size_t len = 0;

  tmp_last_str = ( char* )malloc( sizeof( char ) * ( strlen( string ) + 1 ) );

  if( tmp_last_str == NULL )
  {
    perror( "Memory error. Exiting ...\n" );
    exit( EXIT_FAILURE );
  }

  strcpy( tmp_last_str, string );
  len = strlen( tmp_last_str );

  while( keep_loopin )
  {
    string = string_replace( search, replace, string );
    if( strcmp( string, tmp_last_str ) == 0 )
    {
      keep_loopin = 0;
    }
    else if( i >= len )
    {
      keep_loopin = 0;
    }
    else
    {
      realloc_check = realloc( tmp_last_str, strlen( string ) + 1 );
      if( realloc_check == NULL )
      {
        perror( "reallocation error. Exiting ... \n" );
        exit( EXIT_FAILURE );
      }
      tmp_last_str = ( char* )realloc_check;
      realloc_check = NULL;

      strcpy( tmp_last_str, string );
      len = strlen( tmp_last_str );
    }

    i++;
  }
  
  free( tmp_last_str );
  
  return string;
}
