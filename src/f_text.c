//
//  f_text.c
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 25.8.2011
//  Last Updated: 14.9.2011
//

#include "f_text.h"

void f_text_init ()
{
  //NONBLOCK_UNSET();
  uses_header = 0;
}

void f_text_header ()
{}

void f_text_read ()
{
  do
  {
    // get it char by char
    c = fgetc(stdin);
    switch (c)
    {
      // if EOF, cut it and send it
      // (later at -t and gzwrite check it something new in buffer)
      case EOF:
        eof = 1;
        send = 1;
        buffer[buffer_bytes] = '\0';
        break;

      // if \n, write and cut it and send it
      case '\n':
        send = 1;
        buffer[buffer_bytes++] = c;
        buffer[buffer_bytes] = '\0';
        break;

      // if other, write and if full buffer, cut it and send it
      default:
        buffer[buffer_bytes++] = c;
        if (buffer_bytes == BUFSIZE-1)
        {
          // if line is longer than the buffer, it's its problem!)
          send = 1;
          buffer[buffer_bytes] = '\0';
        }
    }
    // complete line? (full buffer counts too)
    if (send || eof) break;
  } while (1);
}
