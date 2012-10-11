//
//  read.c
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

#include "read.h"

void socket_init (void)
{
  if (!f_n) return;

  // fill server struct
  addr_server.sin_family      = AF_INET;
  addr_server.sin_addr.s_addr = INADDR_ANY;
  addr_server.sin_port        = htons(port); // -> big endian

  // open fd for socket
  sock_serv_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_serv_fd < 0) printError("Error - can't open socket!\n");

  // bind fd with struct
  if (bind(
           sock_serv_fd,
           (struct sockaddr *) &addr_server,
           sizeof(addr_server)
          ) < 0) printError("Error - can't bind socket!\n");

  // find out the client struct length (bah, C programming :) )
  client_len = sizeof(addr_client);

  // mark the fd as willing to accept connections
  // only one connection at a time
  listen(sock_serv_fd,1); 

  // accept the connection
  sock_conn_fd = accept(sock_serv_fd,
                        (struct sockaddr *) &addr_client,
                        &client_len);
  if (sock_conn_fd < 0) printError("Error - can't accept socket conn!\n");

  input_fd = sock_conn_fd;
}

size_t pipa_read (void *location, size_t nbytes)
{
  // TODO: NB READ
  // there's blocking read right now, works with both socket and stdin
  // (old nb code is on github)
  //
  // maybe there is a nonblocking way to do both the same?
  //
  // or will we have to differentiate between socket and stdin to do
  // different nb initializations?

  size_t read_remains = nbytes;
  size_t read_total = 0;
  size_t read_now = 0;

  do
  {
    read_now = read(input_fd, location + read_total, read_remains);
    if (read_now > 0)
    {
      read_total   += read_now;
      read_remains -= read_now;
    }
  }
  while (read_total != nbytes && read_now > 0);

  return read_total;
}
