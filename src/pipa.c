//
//  pipa.c
//
//  Date Created: 25.8.2011
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

#include "pipa.h"

int main(int argc, char **argv)
{
  ////////////////////////////////////////////// initializations ////

  arg = 0;
  f_x = 0;
  f_t = 0;
  f_z = 0;
  f_c = 0;
  f_i = 0;
  f_s = 0;
  f_f = 0;
  f_v = 0;

  command = NULL;
  
  cl_set     = 0;
  comp_level = DEFAULT_COMP_LEVEL;

  memset(parameters,0,sizeof(parameters));
  
  inc        = 0;
  inc_needed = 0;

  memset(inc_str,0,sizeof(inc_str));
  
  size = 0;
  
  interval   = 0;
  t_file     = 0;
  t_file_st  = NULL;
  t_first    = 1;
  t_divided  = 0;
  
  eof          = 0;
  uses_header  = 0;
  header_bytes = 0;
  buffer_bytes = 0;

  tee_wrote_pcap_header = 0;

  memset(header,0,sizeof(header));
  memset(buffer,0,sizeof(buffer));  

  memset(filename,     0,sizeof(filename));
  memset(filename_mask,0,sizeof(filename_mask));

  filename_dir  = NULL;
  filename_tmp  = NULL;
  filename_tmp2 = NULL;

  memset(currentpath,0,sizeof(currentpath));
  memset(fullpath,   0,sizeof(fullpath));

  state = 0;
  
  f_init   = &f_none_init;
  f_header = &f_none_header;
  f_read   = &f_none_read;

  z_init  = &z_none_init;
  z_open  = &z_none_open;
  z_write = &z_none_write;
  z_close = &z_none_close;

  //////////////////////////////////////// parsing the arguments ////

  if (argc == 1) printUsage();

  opterr = 0;
  while ((arg = getopt (argc, argv, "x:tc:z:i:s:fv0123456789")) != -1)
    switch (arg)
    {
      case 'x':
        f_x = 1;
        if (!strcmp(optarg,"text"))
        {
          f_init   = &f_text_init;
          f_header = &f_text_header;
          f_read   = &f_text_read;
        }
        else if (!strcmp(optarg,"pcap"))
        {
          f_init   = &f_pcap_init;
          f_header = &f_pcap_header;
          f_read   = &f_pcap_read;
        }
        break;

      case 't':
        f_t = 1;
        break;

      case 'c':
        f_c = 1;
        command = strdup(optarg);
        break;

      case 'z':
        f_z = 1;
        if (!strcmp(optarg,"gz"))
        {
          z_init  = &z_gz_init;
          z_open  = &z_gz_open;
          z_write = &z_gz_write;
          z_close = &z_gz_close;
        }
        else if (!strcmp(optarg,"lzo"))
        {
          z_init  = &z_lzo_init;
          z_open  = &z_lzo_open;
          z_write = &z_lzo_write;
          z_close = &z_lzo_close;
        }
        else if (!strcmp(optarg,"xz"))
        {
          z_init  = &z_xz_init;
          z_open  = &z_xz_open;
          z_write = &z_xz_write;
          z_close = &z_xz_close;
        }
        else if (!strcmp(optarg,"bz2"))
        {
          z_init  = &z_bz2_init;
          z_open  = &z_bz2_open;
          z_write = &z_bz2_write;
          z_close = &z_bz2_close;
        }
        break;

      case 'i':
        f_i = 1;
        interval = (unsigned long) parse_string_time(optarg);
        if (interval < 1) printUsage();
        break;

      case 's':
        f_s = 1;
        size = (unsigned long) parse_string_size(optarg);
        if (size < 1) printUsage();
        break;

      case 'f':
        f_f = 1;
        break;

      case 'v':
        f_v = 1;
        break;

      case '0': if (!cl_set) {comp_level = 0; cl_set = 1;} break;
      case '1': if (!cl_set) {comp_level = 1; cl_set = 1;} break;
      case '2': if (!cl_set) {comp_level = 2; cl_set = 1;} break;
      case '3': if (!cl_set) {comp_level = 3; cl_set = 1;} break;
      case '4': if (!cl_set) {comp_level = 4; cl_set = 1;} break;
      case '5': if (!cl_set) {comp_level = 5; cl_set = 1;} break;
      case '6': if (!cl_set) {comp_level = 6; cl_set = 1;} break;
      case '7': if (!cl_set) {comp_level = 7; cl_set = 1;} break;
      case '8': if (!cl_set) {comp_level = 8; cl_set = 1;} break;
      case '9': if (!cl_set) {comp_level = 9; cl_set = 1;} break;

      case '?':
        printUsage();

      default:
        abort();
    }

  ////////////////////////////// module + signal initializations ////

  f_init();
  z_init();
  sig_init();

  //////////////////////////////////////// filename preparations ////

  // take first non-option argument as a filename
  
  sprintf(filename_mask,"%s",argv[optind]);
  if (!strcmp(filename_mask,"(null)"))
    printUsage();

  // %i in filename?
  if (strstr(filename_mask,"%i"))
  {
    inc_needed = 1;
    inc = 0;
  }

  while (1) // NEW FILE
  {

    openFile();

    while (1) // NEW LOAD OF BUFFER
    {
      buffer_bytes = 0;
      send = 0;

      // read from STDIN
      f_read();

      // write to the file
      if (buffer_bytes)
      {
        if (f_t) fwrite(buffer,1,buffer_bytes,stdout);
        z_write(buffer,buffer_bytes);
      }

      if (checkRotation()) break;
    }

    // close the file
    closeFile();

    if (f_c)
    {
      pid = fork();

      if (pid < 0) printError("Error while trying to execute user command!");
      else if (pid == 0)
      {
        getcwd(currentpath, sizeof(currentpath));
        sprintf(fullpath,"%s/%s",currentpath,filename);
        execlp(command,command,fullpath,NULL);
        _exit(1);
      }
    }

    // did we read it all?
    if (eof) break;
  }

  return 0;
}
