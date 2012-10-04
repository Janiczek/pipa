//
//  f_pcap.c
//
//  Date Created: 1.10.2012
//  Last Updated: 4.10.2012
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

#include "f_pcapng.h"

void f_pcapng_init (void)
{
  NONBLOCK_SET();
  uses_header = 1;
  memset(png_tmp_buffer,0,sizeof(png_tmp_buffer));

  // is C so stupid you can't do it any better than this?
  png_tmp = PCAPNG_SHB_BLKTYPE;     memcpy(png_hdrs[0],    &png_tmp, 4); 
  png_tmp = PCAPNG_LIL_SHB_BLKLEN;  memcpy(png_hdrs[0]+4,  &png_tmp, 4);
  png_tmp = PCAPNG_LIL_SHB_BOM;     memcpy(png_hdrs[0]+8,  &png_tmp, 4);
  png_tmp = PCAPNG_LIL_SHB_VER_MA;  memcpy(png_hdrs[0]+12, &png_tmp, 2);
  png_tmp = PCAPNG_LIL_SHB_VER_MI;  memcpy(png_hdrs[0]+14, &png_tmp, 2);
  png_tmp = PCAPNG_SHB_SECLEN;      memcpy(png_hdrs[0]+16, &png_tmp, 4);
  png_tmp = PCAPNG_SHB_SECLEN;      memcpy(png_hdrs[0]+20, &png_tmp, 4);
  png_tmp = PCAPNG_LIL_SHB_BLKLEN;  memcpy(png_hdrs[0]+24, &png_tmp, 4);

  png_tmp = PCAPNG_SHB_BLKTYPE;     memcpy(png_hdrs[1],    &png_tmp, 4); 
  png_tmp = PCAPNG_BIG_SHB_BLKLEN;  memcpy(png_hdrs[1]+4,  &png_tmp, 4);
  png_tmp = PCAPNG_BIG_SHB_BOM;     memcpy(png_hdrs[1]+8,  &png_tmp, 4);
  png_tmp = PCAPNG_BIG_SHB_VER_MA;  memcpy(png_hdrs[1]+12, &png_tmp, 2);
  png_tmp = PCAPNG_BIG_SHB_VER_MI;  memcpy(png_hdrs[1]+14, &png_tmp, 2);
  png_tmp = PCAPNG_SHB_SECLEN;      memcpy(png_hdrs[1]+16, &png_tmp, 4);
  png_tmp = PCAPNG_SHB_SECLEN;      memcpy(png_hdrs[1]+20, &png_tmp, 4);
  png_tmp = PCAPNG_BIG_SHB_BLKLEN;  memcpy(png_hdrs[1]+24, &png_tmp, 4);
}

void f_pcapng_header (void)
{
  // if this is the first file,
  if (!inc_needed || inc == 1)
  {
    // read SHB block to find out the endianness
    f_pcapng_readblock(png_tmp_buffer,&png_tmp_bytes);

    // and blurt out the header with that endianness
    header_bytes = PCAPNG_HDR_LEN;
    memcpy(header,png_hdrs[is_big_endian],header_bytes);
  }
  // otherwise the header should be OK already.
}

void f_pcapng_read (void)
{
  f_pcapng_readblock(buffer,&buffer_bytes);
}

void f_pcapng_readblock (unsigned char *buf, size_t *bufbytes)
{
  // side effect: sets the endianness flag
  //              reads data into the buf var
  //              len of that data is in bufbytes

  int block_type   = 0;
  int block_length = 0;

  // read upto block length
  *bufbytes = fread_nb(buf, PCAPNG_BLK_LEN_POS);

  if (*bufbytes != PCAPNG_BLK_LEN_POS)
    printError("bad block!");

  // convert into ints
  memcpy(&block_type,   buf,   sizeof(block_type));
  memcpy(&block_length, buf+4, sizeof(block_length));

  // read upto the rest of the block
  *bufbytes += fread_nb(buf + (*bufbytes),
                        block_length - PCAPNG_BLK_LEN_POS);

  // if it's SHB block, find out the endianness
  if (block_type == PCAPNG_SHB_BLKTYPE)
  {
    int bom = 0;

    memcpy(&bom, buf+8, sizeof(bom));
    switch (bom)
    {
      case PCAPNG_LIL_SHB_BOM: is_big_endian = 0; break;
      case PCAPNG_BIG_SHB_BOM: is_big_endian = 1; break;
      default: printError("unknown BOM!\n");
    }
  } // if it's IDB block, append it to the header
  else if (block_type == (is_big_endian ? PCAPNG_BIG_IDB_BLKTYPE
                                        : PCAPNG_LIL_IDB_BLKTYPE))
  {
    memcpy(header + header_bytes, buf, block_length);
    header_bytes += block_length;
  }
}
