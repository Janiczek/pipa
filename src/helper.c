//
//  helper.c
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

#include "helper.h"

// print to stderr
void print      (char *message) { fprintf(stderr,"%s",message); }
void printError (char *message) { print(message); exit(EXIT_FAILURE); }
void printUsage (void)
{
  printError("Usage: pipa [-x format (none | text | pcap)]\n"
             "            [-t]\n"
             "            [-i interval[s | m | h | d | w]]\n"
             "            [-s size[KB | MB | GB | TB]]\n"
             "            [-c user-command]\n"
             "            [-z compression format (gz | bz2 | xz | lzo)]\n"
             "            [-# compression level (0..9)]\n"
             "            [-v]\n"
             "            [-f]\n"
             "            [-n socketport]\n"
             "            filename\n");
}

// recursive mkdir
int mkpath (const char *s, mode_t mode)
{
    char *q, *r = NULL, *path = NULL, *up = NULL;
    int ret;    
    ret = -1;
    if (strcmp(s, ".") == 0 || strcmp(s, "/") == 0)     return (0);
    if ((path = strdup(s)) == NULL)                     exit(1);    
    if ((q = strdup(s)) == NULL)                        exit(1);
    if ((r = dirname(q)) == NULL)                       goto out;
    if ((up = strdup(r)) == NULL)                       exit(1);
    if ((mkpath(up, mode) == -1) && (errno != EEXIST))  goto out;
    if ((mkdir(path, mode) == -1) && (errno != EEXIST)) ret = -1;
    else ret = 0;
  out:
    if (up != NULL) free(up);
    free(q);
    free(path);
    return ret;
}

// duplicate string - this should live in string.h or stdlib.h
// but it's not standardized, so there's no guarantee
// we'll have it here then :)
char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if (dup) strcpy(dup, str);
    return dup;
}

// replace substring
char *replace_str(char *str, char *orig, char *rep)
{
  static char buffer[STRSIZE];
  char *p;
  if(!(p = strstr(str, orig))) return str; // does the substring exist?
  strncpy(buffer, str, p-str); // copy the part before the substring
  buffer[p-str] = '\0'; // cut it off
  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig)); // print the rest after
  return buffer;
}

double parse_string_size(char *value)
{
  // parse the number
  double v = 0;
  while (*value != '\0' && *value >= '0' && *value <= '9')
  {
    v = v * 10 + (*value - '0');
    value++;
  }
  if (*value == ',' || *value == '.')
  {
    value++;
    double v2 = 0;
    while (*value != '\0' && *value >= '0' && *value <= '9')
    {
      v2 = v2 / 10 + (double) (*value - '0') / 10.0;
      value++;
    }
    v += v2;
  }

  // parse the suffix
  if (*value == '\0'
   || strcmp(value, "b") == 0
   || strcmp(value, "B") == 0) return v;

  if (strcmp(value, "k") == 0
   || strcmp(value, "K") == 0
   || strcmp(value, "kb") == 0
   || strcmp(value, "kB") == 0
   || strcmp(value, "Kb") == 0
   || strcmp(value, "KB") == 0) return v * 1024;

  if (strcmp(value, "m") == 0
   || strcmp(value, "M") == 0
   || strcmp(value, "mb") == 0
   || strcmp(value, "mB") == 0
   || strcmp(value, "Mb") == 0
   || strcmp(value, "MB") == 0) return v * 1024 * 1024;

  if (strcmp(value, "g") == 0
   || strcmp(value, "G") == 0
   || strcmp(value, "gb") == 0
   || strcmp(value, "gB") == 0
   || strcmp(value, "Gb") == 0
   || strcmp(value, "GB") == 0) return v * 1024 * 1024 * 1024;

  if (strcmp(value, "t") == 0
   || strcmp(value, "T") == 0
   || strcmp(value, "tb") == 0
   || strcmp(value, "tB") == 0
   || strcmp(value, "Tb") == 0
   || strcmp(value, "TB") == 0) return v * 1024 * 1024 * 1024 * 1024;

  printUsage(); // error, shouldn't have arrived here!
  return 0;
}

double parse_string_time(char *value)
{
  // parse the number
  double v = 0;
  while (*value != '\0' && *value >= '0' && *value <= '9')
  {
    v = v * 10 + (*value - '0');
    value++;
  }
  if (*value == ',' || *value == '.')
  {
    value++;
    double v2 = 0;
    while (*value != '\0' && *value >= '0' && *value <= '9')
    {
      v2 = v2 / 10 + (double) (*value - '0') / 10.0;
      value++;
    }
    v += v2;
  }

  // parse the suffix
  if (*value == '\0'
   || strcmp(value, "s") == 0
   || strcmp(value, "sec") == 0
   || strcmp(value, "second") == 0
   || strcmp(value, "seconds") == 0)
    return v;

  if (strcmp(value, "m") == 0
   || strcmp(value, "min") == 0
   || strcmp(value, "minute") == 0
   || strcmp(value, "minutes") == 0)
    return v * 60;

  if (strcmp(value, "h") == 0
   || strcmp(value, "hour") == 0
   || strcmp(value, "hours") == 0)
    return v * 60 * 60;

  if (strcmp(value, "d") == 0
   || strcmp(value, "day") == 0
   || strcmp(value, "days") == 0)
    return v * 60 * 60 * 24;

  if (strcmp(value, "w") == 0
   || strcmp(value, "week") == 0
   || strcmp(value, "weeks") == 0)
    return v * 60 * 60 * 24 * 7;

  printUsage(); // error, shouldn't have arrived here!
  return 0;
}

void openFile(void)
{
  // opens a new file and writes a header

  filename_tmp = strdup(filename_mask);

  // if %i
  if (inc_needed)
  {
    // replace it with a number
    sprintf(inc_str,"%d",++inc);
    filename_tmp2 = replace_str(filename_tmp,"%i",inc_str);

    // because valgrind complains
    free(filename_tmp);
    filename_tmp = filename_tmp2;
  }

  // make a filename from current time
  // first time will usually be misaligned
  if (f_i)
  {
    if (t_first)
    {
      time(&t_file);
      t_file_st = localtime(&t_file);
      strftime(filename, STRSIZE-1, filename_tmp, t_file_st);    
      t_divided = time(NULL) / interval;
      t_first = 0;
    }
    else
    {
      // t_file += interval; <-- made aligned times, but misaligned filenames
      time(&t_file);
      t_file_st = localtime(&t_file);
      strftime(filename, STRSIZE-1, filename_tmp, t_file_st);    
    }
  }
  else
  {
    time(&t_file);
    t_file_st = localtime(&t_file);
    strftime(filename,STRSIZE-1,filename_tmp,t_file_st);
  }

  // free the memory
  // because valgrind complains
  if (!inc_needed) free(filename_tmp);

  // recursively make directories
  filename_dir = strdup(filename);
  mkpath(dirname(filename_dir), 0777); // some implementations of dirname
  free(filename_dir);                  // change the contents of the argument
                                       // some don't ...

  // does the file exist?
  if (!stat(filename,&st))
  {
    if (f_f) // -f: force overwrite
      fprintf(stderr,"%s: Filename exists - overwriting\n",filename);
    else // abort
    {
      fprintf(stderr,"%s: Filename exists - aborting\n",filename);
      exit(1);
    }
  }

  // open the file
  if (f_v) fprintf(stderr,"%s: Opening\n",filename);
  z_open();
  state = file_open;

  if (uses_header)
  {
    eof = 0;
    // get header into the string
    f_header();
    // write it - it's once per file! and once per tee output!
    if (header_bytes)
    {
      if (f_t)
      {
        if (f_init == &f_pcap_init)
        {
          if (!tee_wrote_pcap_header)
          {
            tee_wrote_pcap_header = 1;
            fwrite(header,1,header_bytes,stdout);
          }
        }
        else
          fwrite(header,1,header_bytes,stdout);
      }
      z_write(header,header_bytes);
    }
  }
}

void closeFile(void)
{
  if (f_v) fprintf(stderr,"%s: Closing\n",filename);
  z_close();
  state = file_not_open;
}

int checkRotation(void)
{
  // check size
  if (f_s)
  {
    stat(filename,&st);
    if (st.st_size >= size) return 1;
  }
  // check time
  if (f_i && t_divided != time(NULL) / interval)
  {
    t_divided = time(NULL) / interval;
    return 1;
  }
  // check EOF
  if (eof) return 1;

  return 0;
}

/*

UNTESTED!!! BUT SHOULD WORK
-- TODO - what about normal, non-socket fds? how to make *them* NB?

int make_socket_non_blocking (int sfd)
{
  int flags;
  int s;

  flags = fcntl (sfd, F_GETFL, 0);

  if (flags == -1)
    printError("fcntl error when getting\n");

  flags |= O_NONBLOCK;
  s = fcntl (sfd, F_SETFL, flags);

  if (s == -1)
    printError("fcntl error when setting\n");

  return 0;
}

*/
