//
//  z_xz.h
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

#include <lzma.h>

#include "pipa.h"

#define Z_XZ_OUT_LEN 256*1024
#define Z_XZ_COMPRESSION_EXTREME 1
#define Z_XZ_INTEGRITY_CHECK LZMA_CHECK_NONE

unsigned char z_xz_out[Z_XZ_OUT_LEN];
int           z_xz_out_max;
int           z_xz_out_len;

unsigned int  z_xz_preset;
lzma_check    z_xz_check;
lzma_stream   z_xz_stream;
lzma_action   z_xz_action;
lzma_ret      z_xz_ret;
FILE         *z_xz_fd;

void z_xz_run   (void);
void z_xz_init  (void);
void z_xz_open  (void);
void z_xz_write (void *location, size_t nbytes);
void z_xz_close (void);
