//
//  z_none.c
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 14.9.2011
//  Last Updated: 15.9.2011
//

#include "z_none.h"

void z_none_init ()
{
  sprintf(parameters,"wb");   
}

void z_none_open  ()
{
  if ((z_none_fd = fopen(filename, parameters)) == NULL)
  {
    fprintf(stderr,"%s: ERROR - can't open!\n",filename);
    exit(1);
  }
}

void z_none_write (void *location, size_t nbytes)
{
  if (fwrite(location,1,nbytes,z_none_fd) == 0)
  {
    fprintf(stderr,"%s: ERROR - can't write!\n",filename);
    exit(1);
  }
}

void z_none_close ()
{
  if (fclose(z_none_fd) != 0)
  {
    fprintf(stderr,"%s: ERROR - can't close!\n",filename);
    exit(1);
  }
}
