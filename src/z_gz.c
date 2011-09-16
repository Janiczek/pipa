//
//  z_gz.c
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 14.9.2011
//  Last Updated: 15.9.2011
//

#include "z_gz.h"

void z_gz_init ()
{
  sprintf(parameters,"wb%d",comp_level);   
}

void z_gz_open  ()
{
  if ((z_gz_fd = gzopen(filename, parameters)) == NULL)
  {
    fprintf(stderr,"%s: ERROR - can't open!\n",filename);
    exit(1);
  }
}

void z_gz_write (void *location, size_t nbytes)
{
  if (gzwrite(z_gz_fd,location,nbytes) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write!\n",filename);
    exit(1);
  }
}

void z_gz_close ()
{
  gzflush(z_gz_fd,Z_FINISH);
  if (gzclose(z_gz_fd) != Z_OK)
  {
    fprintf(stderr,"%s: ERROR - can't close!\n",filename);
    exit(1);
  }
}
