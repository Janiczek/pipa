//
//  sighandler.c
//
//  Date Created: 3.10.2011
//  Last Updated: 16.10.2011
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

#include "sighandler.h"

void sig_init(void)
{
  signal(SIGINT,end_gracefully); // we can't do much in this function,
                                 // so JMP into other one and do work there.
}

void end_gracefully (int sig)
{
  switch (state)
  {
    case 1:
      closeFile(); // makes sure we flush the compression buffers
                   // before exiting -> not losing any processed data.
    case 0:
      exit(sig);
      break;
  }
}
