//
//  f_text.c
//
//  Copyright 2012 Martin Janiczek (martin.janiczek@linuxbox.cz)
//                 LinuxBox.cz, s.r.o.
//                 www.linuxbox.cz
//
//  This file is part of pipa.
//  
//  pipa is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  pipa is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with pipa.  If not, see <http://www.gnu.org/licenses/>.
//

#include "f_text.h"

void f_text_init (void)
{
  //NONBLOCK_UNSET();
  uses_header = 0;
}

void f_text_header (void)
{}

void f_text_read (void)
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
        may_send = 1;
        buffer[buffer_bytes] = '\0';
        break;

      // if \n, write and cut it and send it
      case '\n':
        may_send = 1;
        buffer[buffer_bytes++] = c;
        buffer[buffer_bytes] = '\0';
        break;

      // if other, write and if full buffer, cut it and send it
      default:
        buffer[buffer_bytes++] = c;
        if (buffer_bytes == BUFSIZE-1)
        {
          // if line is longer than the buffer, it's its problem!)
          may_send = 1;
          buffer[buffer_bytes] = '\0';
        }
    }
    // complete line? (full buffer counts too)
    if (may_send || eof) break;
  } while (1);
}
