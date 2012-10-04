//
//  z_lzo.h
//
//  Date Created: 15.9.2011
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

#include <lzo/lzo1x.h>

#include "pipa.h"


#define Z_LZO_MAGIC {0x89,0x4c,0x5a,0x4f,0x00,0x0d,0x0a,0x1a,0x0a}
#define Z_LZO_MAGIC_LEN 9

#define Z_LZO_VERSION 0x1030 // as in LZOP 1.03
#define Z_LZO_LIB_VERSION (lzo_version() & 0xffff)
#define Z_LZO_VERSION_NEEDED_TO_EXTRACT 0x0940 // not using filters, otherwise 0x0950

#define Z_LZO_METHOD 1 // LZO1X
#define Z_LZO_COMPRESSION_LEVEL 1 // with lzo, we have compression level = 1.
                                  // maybe subject to change?
                                 
#define Z_LZO_FLAGS 0 // no checksums on data!!

#define Z_LZO_MODE 0x81a4 // 100644 oct

FILE         *z_lzo_fd;
lzo_bytep     z_lzo_out;
lzo_voidp     z_lzo_wrkmem;
lzo_uint      z_lzo_out_len;
lzo_uint32    z_lzo_hdr_adler;

void z_lzo_write8  (int c);
void z_lzo_write16 (unsigned long v);
void z_lzo_write32 (unsigned long v);
void z_lzo_init    (void);
void z_lzo_open    (void);
void z_lzo_write   (void *location, size_t nbytes);
void z_lzo_close   (void);
