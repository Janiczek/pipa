//
//  f_pcap.c
//
//  Date Created: 25.8.2011
//  Last Updated: 19.9.2011
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

#include "f_pcap.h"

void f_pcap_init ()
{
  NONBLOCK_SET();
  packet_len = 0;
  uses_header = 1;
}

void f_pcap_header ()
{
  if (!header[0])
  {
    //header_bytes = fread(header,1,PCAP_HDR_SIZE,stdin);
    header_bytes = fread_nb(header,PCAP_HDR_SIZE);
    if (header_bytes < PCAP_HDR_SIZE)
    {
      eof = 1;
      return;
    }
  }
  else header_bytes = PCAP_HDR_SIZE;
}

void f_pcap_read ()
{
  // read packet header into buffer
  //buffer_bytes = fread(buffer,1,PACKET_HDR_SIZE,stdin);
  buffer_bytes = fread_nb(buffer,PACKET_HDR_SIZE);

  if (buffer_bytes < PACKET_HDR_SIZE) // didn't read anything
  {
    eof = 1;
    return;
  }

  // cast the right part into an uint, read length
  packet_len = (unsigned int) *(buffer + PACKET_LEN_POS)
             + (unsigned int) *(buffer + PACKET_LEN_POS + 1)*256
             + (unsigned int) *(buffer + PACKET_LEN_POS + 2)*256*256
             + (unsigned int) *(buffer + PACKET_LEN_POS + 3)*256*256*256;

  // packet must fit into rest of the buffer!
  packet_len = (BUFSIZE < packet_len) ? BUFSIZE - PACKET_HDR_SIZE
                                      : packet_len;

  // read packet into buffer after the header
  //buffer_bytes += fread(buffer + buffer_bytes,1,packet_len,stdin);
  buffer_bytes += fread_nb(buffer + buffer_bytes,packet_len);
}
