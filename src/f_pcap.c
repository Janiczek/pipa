//
//  f_pcap.c
//
//  Date Created: 25.8.2011
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

#include "f_pcap.h"

void f_pcap_init (void)
{
  NONBLOCK_SET();
  packet_len = 0;
  uses_header = 1;
}

void f_pcap_header (void)
{
  if (!header[0])
  {
    header_bytes = fread_nb(header,PCAP_FILE_HDR_SIZE);
    if (header_bytes < PCAP_FILE_HDR_SIZE)
    {
      eof = 1;
      return;
    }
  }
  else header_bytes = PCAP_FILE_HDR_SIZE;
}

void f_pcap_read (void)
{
  // read packet header into buffer
  buffer_bytes = fread_nb(buffer,PCAP_PACKET_HDR_SIZE);

  if (buffer_bytes < PCAP_PACKET_HDR_SIZE) // didn't read anything
  {
    eof = 1;
    return;
  }

  // cast the right part into an uint, read length
  memcpy(&packet_len, buffer + PCAP_PACKET_LEN_POS, sizeof(packet_len));

  // packet must fit into rest of the buffer!
  packet_len = (BUFSIZE < packet_len) ? BUFSIZE - PCAP_PACKET_HDR_SIZE
                                      : packet_len;

  // read packet into buffer after the header
  buffer_bytes += fread_nb(buffer + buffer_bytes,packet_len);
}
