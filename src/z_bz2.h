//
//  z_bz2.h
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 16.9.2011
//  Last Updated: 16.9.2011
//

#include <bzlib.h>

#include "pipa.h"

BZFILE *z_bz2_fd;

void z_bz2_init  ();
void z_bz2_open  ();
void z_bz2_write (void *location, size_t nbytes);
void z_bz2_close ();
