//
//  f_none.c
//
//  Date Created: 25.8.2011
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

#include "f_none.h"

void f_none_init ()
{
  //NONBLOCK_UNSET();
  uses_header = 0;
}

void f_none_header ()
{}

void f_none_read ()
{
  // read full buffer and send it
  // if EOF, cut it before
  
  buffer_bytes = fread(buffer,1,BUFSIZE,stdin);
  eof = buffer_bytes < BUFSIZE;

  //buffer_bytes = fread_nb(buffer,BUFSIZE);
  // maybe TODO someday?
}
