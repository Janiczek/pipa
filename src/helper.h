//
//  helper.h
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

#include "pipa.h"

#include <errno.h>
#include <libgen.h>
#include <sys/stat.h> 
#include <sys/types.h>

void    print              (char *message);
void    printError         (char *message);
void    printUsage         (void);

#define PRINT(...) fprintf(stderr,__VA_ARGS__)

#define ZERO(x) memset((x),0,sizeof(x))

int     mkpath             (const char *s, mode_t mode);

char   *replace_str        (char *str, char *orig, char *rep);

double  parse_string_size  (char *value);
double  parse_string_time  (char *value);

void    openFile           (void);
void    closeFile          (void);
int     checkRotation      (void);
