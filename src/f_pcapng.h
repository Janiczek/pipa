//
//  f_pcap.h
//
//  Date Created: 1.10.2012 
//  Last Updated: 3.10.2012 
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

#include "pipa.h"

// http://www.winpcap.org/ntar/draft/PCAP-DumpFileFormat.html

// PCAP-NG file is divided into blocks.

// Each block starts with:
//   - block type   - uint32
//   - block length - uint32
// And ends with:
//   - block length - uint32 (for backwards reading+skipping)

// The format doesn't technically have a file header...
// But each file must have a SHB and if we are splitting
//   in middle of packets, we have to supply one.

// Some thoughts on this:
//
//  - editcap sets section length = -1 ("find for yourself")
//    when processing a file - probably so that it doesn't have to
//    parse the file beforehand and process it as a stream instead
//
//  - TODO: are we gonna run into problems if we discard
//          all options in the first SHB in a stream?
//          what would we gain by this:
//            file header would be constant string

// 4 bytes - 0x0a0d0d0a         - block type       = SHB
// 4 bytes - 0x0000001c         - block length     = 28
// 4 bytes - 0x1a2b3c4d         - byte order magic = const
// 2 bytes - 0x0001             - major version    = 1
// 2 bytes - 0x0000             - minor version    = 0
// 8 bytes - 0xffffffffffffffff - section length   = -1
// 4 bytes - 0x0000001c         - block length     = 28

//  - TODO: what about little/big endianness? checking of BOM? ???
//    - Likely way:
//        0. have two headers ready - one for little endian, one for big
//        1. endianness = find out in f_..._header() from the first SHB
//        2. on each block:
//            - if SHB:
//                - check the endianness
//                - copy our const header of that endianness
//            - else:
//                - copy right away
//        3. on rotation (on new file):
//            - use last endianness

#define PCAPNG_HDR_LEN     28
#define PCAPNG_BLK_LEN_POS 8 // block type + length

#define PCAPNG_SHB_BLKTYPE 0x0A0D0D0A
#define PCAPNG_SHB_SECLEN  0xFFFFFFFF

// little endian
#define PCAPNG_HDR_LIL_BLKLEN 0x1C000000
#define PCAPNG_HDR_LIL_BOM    0x4D3C2B1A
#define PCAPNG_HDR_LIL_VER_MA 0x0100
#define PCAPNG_HDR_LIL_VER_MI 0x0000

// big endian
#define PCAPNG_HDR_BIG_BLKLEN 0x0000001C
#define PCAPNG_HDR_BIG_BOM    0x1A2B3C4D
#define PCAPNG_HDR_BIG_VER_MA 0x0001
#define PCAPNG_HDR_BIG_VER_MI 0x0000

int png_tmp; // initializer
char png_hdrs[2][PCAPNG_HDR_LEN]; // static headers
int last_endian; // 0 = little, 1 = big
size_t png_bytes;
unsigned char png_hdrbuf[1024];

void f_pcapng_init   (void);
void f_pcapng_header (void);
void f_pcapng_read   (void);

void f_pcapng_readblock (unsigned char *buf, size_t *bufbytes);
