//
//  f_none.c
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 25.8.2011
//  Last Updated: 14.9.2011
//

#include "f_none.h"

void f_none_init ()
{
  //NONBLOCK_UNSET();
  uses_header = 0;
}

void f_none_header ()
{}

void f_none_read ()
{
  // read full buffer and send it
  // if EOF, cut it before
  
  buffer_bytes = fread(buffer,1,BUFSIZE,stdin);
  eof = buffer_bytes < BUFSIZE;

  //buffer_bytes = fread_nb(buffer,BUFSIZE);
  // maybe TODO someday?
}
