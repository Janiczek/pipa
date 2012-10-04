//
//  f_pcap.h
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

#include "pipa.h"

#define PCAP_FILE_HDR_SIZE   24
#define PCAP_PACKET_HDR_SIZE 16 // 4 * uint32
#define PCAP_PACKET_LEN_POS  8  // third uint32 in the struct is our length
                                // (0 - first, 4 - second, 8 - third)

unsigned int packet_len;

void f_pcap_init   (void);
void f_pcap_header (void);
void f_pcap_read   (void);
