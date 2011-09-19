//
//  pipa.h
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 24.8.2011
//  Last Updated: 16.9.2011
//

#ifndef _PIPA_H
#define _PIPA_H

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// functions
#include "helper.h"

// formats
#include "f_none.h"
#include "f_text.h"
#include "f_pcap.h"

// compression formats
#include "z_none.h"
#include "z_gz.h"
#include "z_lzo.h"
#include "z_xz.h"
#include "z_bz2.h"

// constants
#define DEFAULT_COMP_LEVEL 2

#define STRSIZE 1024          // for paths, filenames, ...
#define HDRSIZE PCAP_HDR_SIZE // right now only format that uses header is PCAP
#define BUFSIZE 256*1024

// global variables

char arg;
char f_x;
char f_t;
char f_z;
char f_c;
char f_i;
char f_s;
char f_f;
char f_v;

char *command;

int  cl_set;
int  comp_level;
char parameters[4];

int  inc;
int  inc_needed;
char inc_str[64];

double size;
struct stat st;

unsigned long interval;
time_t        t_file;
struct tm    *t_file_st;
int           t_first;
unsigned long t_divided;

int            eof;
int            uses_header;
size_t         header_bytes;
size_t         buffer_bytes;
unsigned char  header[HDRSIZE];
unsigned char  buffer[BUFSIZE];

char           filename[STRSIZE];
char           filename_mask[STRSIZE];
char          *filename_dir;
char          *filename_tmp;
char          *filename_tmp2;

char  currentpath[FILENAME_MAX];
char  fullpath[STRSIZE];
pid_t pid;

void (*f_init)  ();
void (*f_header)();
void (*f_read)  ();

void (*z_init) ();
void (*z_open) ();
void (*z_write)(void *location, size_t nbytes);
void (*z_close)();

#endif
