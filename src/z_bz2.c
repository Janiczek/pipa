//
//  z_bz2.c
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

#include "z_bz2.h"

void z_bz2_init ()
{
  sprintf(parameters,"wb%d",comp_level);   
}

void z_bz2_open  ()
{
  if ((z_bz2_fd = BZ2_bzopen(filename, parameters)) == NULL)
  {
    fprintf(stderr,"%s: ERROR - can't open!\n",filename);
    exit(1);
  }
}

void z_bz2_write (void *location, size_t nbytes)
{
  if (BZ2_bzwrite(z_bz2_fd,location,nbytes) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write!\n",filename);
    exit(1);
  }
}

void z_bz2_close ()
{
  // BZ2_bzflush(z_bz2_fd); // BZ2_bzflush doesn't do anything in v1.0.6
  BZ2_bzclose(z_bz2_fd);    // BZ2_bzclose doesn't have return value in v1.0.6
}
