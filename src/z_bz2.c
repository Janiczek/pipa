//
//  z_bz2.c
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 16.9.2011
//  Last Updated: 16.9.2011
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
