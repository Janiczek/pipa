//
//  z_gz.h
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 14.9.2011
//  Last Updated: 15.9.2011
//

#include <zlib.h>

#include "pipa.h"

gzFile z_gz_fd;

void z_gz_init  ();
void z_gz_open  ();
void z_gz_write (void *location, size_t nbytes);
void z_gz_close ();
