//
//  z_none.h
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 14.9.2011
//  Last Updated: 15.9.2011
//

#include "pipa.h"

FILE *z_none_fd;

void z_none_init  ();
void z_none_open  ();
void z_none_write (void *location, size_t nbytes);
void z_none_close ();
