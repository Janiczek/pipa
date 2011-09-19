//
//  z_gz.c
//
//  Date Created: 14.9.2011
//  Last Updated: 19.9.2011
//
//  Copyright 2011 Martin Janiczek (martin@janiczek.cz)
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

#include "z_gz.h"

void z_gz_init ()
{
  sprintf(parameters,"wb%d",comp_level);   
}

void z_gz_open  ()
{
  if ((z_gz_fd = gzopen(filename, parameters)) == NULL)
  {
    fprintf(stderr,"%s: ERROR - can't open!\n",filename);
    exit(1);
  }
}

void z_gz_write (void *location, size_t nbytes)
{
  if (gzwrite(z_gz_fd,location,nbytes) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write!\n",filename);
    exit(1);
  }
}

void z_gz_close ()
{
  gzflush(z_gz_fd,Z_FINISH);
  if (gzclose(z_gz_fd) != Z_OK)
  {
    fprintf(stderr,"%s: ERROR - can't close!\n",filename);
    exit(1);
  }
}
