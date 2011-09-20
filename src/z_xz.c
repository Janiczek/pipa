//
//  z_xz.c
//
//  Date Created: 16.9.2011
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

#include "z_xz.h"

void z_xz_run()
{
  do
  {
    z_xz_stream.next_out = z_xz_out;
    z_xz_stream.avail_out = z_xz_out_max;

    z_xz_ret = lzma_code(&z_xz_stream,z_xz_action);
    if (z_xz_ret != LZMA_OK &&
        z_xz_ret != LZMA_STREAM_END)
      printError("Error while compressing with XZ!\n");
    else
    {
      z_xz_out_len = z_xz_out_max - z_xz_stream.avail_out;
      if (z_xz_out_len && fwrite(z_xz_out,1,z_xz_out_len,z_xz_fd) == 0)
      {
        fprintf(stderr,"%s: ERROR - can't write!\n",filename);
        exit(1);
      }
    }
  } while (z_xz_stream.avail_out == 0);
}

void z_xz_init ()
{
  sprintf(parameters,"wb");   

  z_xz_preset = comp_level | Z_XZ_COMPRESSION_EXTREME; 
  z_xz_check  = Z_XZ_INTEGRITY_CHECK;

  // weird, but liblzma/api/lzma/base.h says this is the way to go
  lzma_stream z_xz_tmp = LZMA_STREAM_INIT;
  z_xz_stream = z_xz_tmp;
  
  memset(z_xz_out,0,Z_XZ_OUT_LEN);
  z_xz_out_max = Z_XZ_OUT_LEN;
  z_xz_out_len = 0;
}

void z_xz_open  ()
{
  if ((z_xz_ret = lzma_easy_encoder(&z_xz_stream, z_xz_preset, z_xz_check))
          != LZMA_OK)
    printError("Error while initializing XZ!\n");

  if ((z_xz_fd = fopen(filename, parameters)) == NULL)
  {
    fprintf(stderr,"%s: ERROR - can't open!\n",filename);
    exit(1);
  }
}

void z_xz_write (void *location, size_t nbytes)
{
  z_xz_stream.next_in = location;
  z_xz_stream.avail_in = nbytes;
  z_xz_action = LZMA_RUN;

  z_xz_run();
}

void z_xz_close ()
{
  z_xz_stream.avail_in = 0;
  z_xz_action = LZMA_FINISH;

  z_xz_run();

  if (fclose(z_xz_fd) != 0)
  {
    fprintf(stderr,"%s: ERROR - can't close!\n",filename);
    exit(1);
  }

  lzma_end(&z_xz_stream);
}
