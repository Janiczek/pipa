//
//  nonblock.c
//
//  Date Created: 16.10.2011
//  Last Updated: 16.10.2011
//
//  Copyright 2011 Martin Janiczek (martin.janiczek@linuxbox.cz)
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

#include "nonblock.h"

size_t fread_nb (void *location, size_t nbytes)
{
  fd_set rfds;
  FD_ZERO(&rfds);

  struct timeval tv;

  size_t has_to_read = nbytes;
  size_t read_so_far = 0;
  size_t read_bytes = 0;

  int select_result = 0;

  do
  {
    FD_SET(STDIN_FILENO,&rfds);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if ((select_result = select(STDIN_FILENO+1,&rfds,NULL,NULL,&tv)))
    {
      // we have data!
      read_bytes = fread(location + read_so_far, 1, has_to_read, stdin);
      if (feof(stdin))
      {
        eof = 1;
        if (!read_bytes) // read it all and ended on EOF
          has_to_read = 0;
        break;
      }
      if (read_bytes > 0)
      {
        has_to_read -= read_bytes;
        read_so_far += read_bytes;
      }
      if (!has_to_read) break;
    }
    else if (select_result != -1)
    {
      // no data
      if (checkRotation())
      {
        closeFile();
        openFile();
      }
    }

  } while (has_to_read);

  return read_so_far;
}
