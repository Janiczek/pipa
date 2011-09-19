//
//  z_bz2.h
//
//  Date Created: 16.9.2011
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

#include <bzlib.h>

#include "pipa.h"

BZFILE *z_bz2_fd;

void z_bz2_init  ();
void z_bz2_open  ();
void z_bz2_write (void *location, size_t nbytes);
void z_bz2_close ();
