//
//  z_xz.h
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 16.9.2011
//  Last Updated: 16.9.2011
//

#include <lzma.h>

#include "pipa.h"

#define Z_XZ_OUT_LEN 256*1024
#define Z_XZ_COMPRESSION_EXTREME 1
#define Z_XZ_INTEGRITY_CHECK LZMA_CHECK_NONE

unsigned char z_xz_out[Z_XZ_OUT_LEN];
int           z_xz_out_max;
int           z_xz_out_len;

unsigned int  z_xz_preset;
lzma_check    z_xz_check;
lzma_stream   z_xz_stream;
lzma_action   z_xz_action;
lzma_ret      z_xz_ret;
FILE         *z_xz_fd;

void z_xz_run   ();
void z_xz_init  ();
void z_xz_open  ();
void z_xz_write (void *location, size_t nbytes);
void z_xz_close ();
