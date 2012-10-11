//
//  pipa.h
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

#ifndef _PIPA_H
#define _PIPA_H

#include <ctype.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// functions
#include "helper.h"
#include "sighandler.h"
#include "read.h"

// formats
#include "f_none.h"
#include "f_text.h"
#include "f_pcap.h"
#include "f_pcapng.h"

// compression formats
#include "z_none.h"
#include "z_gz.h"
#include "z_lzo.h"
#include "z_xz.h"
#include "z_bz2.h"

// constants
#define DEFAULT_COMP_LEVEL 2

#define STRSIZE 1024               // for paths, filenames, ...
#define HDRSIZE 1024
#define BUFSIZE 256*1024

// global variables

char arg;
char f_x;
char f_t;
char f_z;
char f_c;
char f_n;
char f_i;
char f_s;
char f_f;
char f_v;

char *command;

int  port;
int  sock_serv_fd;
int  sock_conn_fd;
socklen_t client_len;

static const struct sockaddr_in zero_sockaddr_in; // all zeros automatically
struct sockaddr_in addr_server; // me
struct sockaddr_in addr_client; // whoever

int  input_fd;

int  cl_set;
int  comp_level;
char parameters[4];

int  inc;
int  inc_needed;
char inc_str[64];

double size;
struct stat st;

unsigned long  interval;
time_t         t_file;
struct tm     *t_file_st;
int            t_first;
unsigned long  t_divided;

enum {file_not_open, file_open} state;

int            eof;
int            uses_header;
size_t         header_bytes;
size_t         buffer_bytes;
unsigned char  header[HDRSIZE];
unsigned char  buffer[BUFSIZE];

int tee_wrote_pcap_header;

char  filename[STRSIZE];
char  filename_mask[STRSIZE];
char *filename_dir;
char *filename_tmp;
char *filename_tmp2;

char  currentpath[FILENAME_MAX];
char  fullpath[STRSIZE];
pid_t pid;

// FUNCTION POINTERS

// file format
void (*f_init)   (void);
void (*f_header) (void);
void (*f_read)   (void);

// compression format
void (*z_init)  (void);
void (*z_open)  (void);
void (*z_write) (void *location, size_t nbytes);
void (*z_close) (void);

#endif
