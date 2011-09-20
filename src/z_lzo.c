//
//  z_lzo.c
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

#include "z_lzo.h"

void z_lzo_write8 (int c)
{
  unsigned char b = (unsigned char) c;

  if (fwrite((const lzo_voidp) &b,1,1,z_lzo_fd) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write!\n",filename);
    exit(1);
  }

  z_lzo_hdr_adler = lzo_adler32(z_lzo_hdr_adler,&b,1);
}

void z_lzo_write16 (unsigned long v)
{
  unsigned char b[2];
  b[1] = (unsigned char) ((v >> 0) & 0xff);
  b[0] = (unsigned char) ((v >> 8) & 0xff);

  if (fwrite(b,1,2,z_lzo_fd) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write!\n",filename);
    exit(1);
  }

  z_lzo_hdr_adler = lzo_adler32(z_lzo_hdr_adler,b,2);
}

void z_lzo_write32 (unsigned long v)
{
  unsigned char b[4];
  b[3] = (unsigned char) ((v >>  0) & 0xff);
  b[2] = (unsigned char) ((v >>  8) & 0xff);
  b[1] = (unsigned char) ((v >> 16) & 0xff);
  b[0] = (unsigned char) ((v >> 24) & 0xff);

  if (fwrite(b,1,4,z_lzo_fd) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write!\n",filename);
    exit(1);
  }

  z_lzo_hdr_adler = lzo_adler32(z_lzo_hdr_adler,b,4);
}

void z_lzo_init ()
{
  z_lzo_out     = NULL;
  z_lzo_out_len = 0;

  z_lzo_wrkmem  = NULL;

  sprintf(parameters,"wb");   

  if (lzo_init() != LZO_E_OK)
    printError("Error while initializing LZO!\n");
}

void z_lzo_open ()
{
  z_lzo_wrkmem = (lzo_voidp) malloc (LZO1X_1_MEM_COMPRESS);
  if (z_lzo_wrkmem == NULL)
    printError("Run out of memory for LZO working buffer!\n");

  if ((z_lzo_fd = fopen(filename, parameters)) == NULL)
  {
    fprintf(stderr,"%s: ERROR - can't open!\n",filename);
    exit(1);
  }

  // init the hdr checksum
  z_lzo_hdr_adler = lzo_adler32(1,NULL,0);
 
  // magic number
  unsigned char z_lzo_magic[Z_LZO_MAGIC_LEN] = Z_LZO_MAGIC;
  if (fwrite(z_lzo_magic,1,Z_LZO_MAGIC_LEN,z_lzo_fd) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write magic number!\n",filename);
    exit(1);
  }

  // other fields of the header
  z_lzo_write16(Z_LZO_VERSION); // version
  z_lzo_write16(Z_LZO_LIB_VERSION); // lib version
  z_lzo_write16(Z_LZO_VERSION_NEEDED_TO_EXTRACT); // version needed to extract
  z_lzo_write8 (Z_LZO_METHOD); // method
  z_lzo_write8 (Z_LZO_COMPRESSION_LEVEL); // level
  z_lzo_write32(Z_LZO_FLAGS); // flags
  // (if flags & filter then filter) - we don't use filter
  z_lzo_write32(Z_LZO_MODE); // mode
  z_lzo_write32(time(NULL)); // modification time = now
  z_lzo_write32(0); // 0 as int32
  z_lzo_write8 (0); // (strlen(filename)) - put 0, we don't use filenames
  // if strlen(filename) > 0 then filename       - we don't use filenames

  z_lzo_write32(z_lzo_hdr_adler); // checksum: (flags & crc) ? crc : adler
                                  // - we use adler
}

void z_lzo_write (void *location, size_t nbytes)
{
  z_lzo_out = (lzo_bytep) malloc (nbytes + nbytes/16 + 64 + 3);
  if (z_lzo_out == NULL)
    printError("Run out of memory for LZO buffer!\n");

  if (lzo1x_1_compress(location,   nbytes,
                       z_lzo_out, &z_lzo_out_len,
                       z_lzo_wrkmem) != LZO_E_OK)
  {
    fprintf(stderr,"%s: ERROR - can't compress!\n",filename);
    exit(1);
  }

  z_lzo_write32(nbytes);

  if (z_lzo_out_len < nbytes)
  {
    z_lzo_write32(z_lzo_out_len);
    if (fwrite(z_lzo_out,1,z_lzo_out_len,z_lzo_fd) == 0)
    {
      fprintf(stderr,"%s: ERROR - can't write!\n",filename);
      exit(1);
    }
  }
  else
  {
    // not compressible, write original block
    z_lzo_write32(nbytes);
    if (fwrite(location,1,nbytes,z_lzo_fd) == 0)
    {
      fprintf(stderr,"%s: ERROR - can't write!\n",filename);
      exit(1);
    }
  }

  free(z_lzo_out);
}

void z_lzo_close ()
{
  // write EOF marker
  z_lzo_write32(0);

  if (fclose(z_lzo_fd) != 0)
  {
    fprintf(stderr,"%s: ERROR - can't close!\n",filename);
    exit(1);
  }

  if (z_lzo_wrkmem != NULL) free(z_lzo_wrkmem);
}
