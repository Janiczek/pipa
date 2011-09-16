//
//  helper.h
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 26.8.2011
//  Last Updated: 9.9.2011
//

#include "pipa.h"

#include <errno.h>
#include <libgen.h>
#include <sys/stat.h> 
#include <sys/types.h>

#define NONBLOCK_SET()   fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);
#define NONBLOCK_UNSET() fcntl(STDIN_FILENO,F_SETFL,fcntl(STDIN_FILENO,F_GETFL) & ~O_NONBLOCK);

void    print              (char *message);
void    printError         (char *message);
void    printUsage         ();

int     mkpath             (const char *s, mode_t mode);

char   *replace_str        (char *str, char *orig, char *rep);

double  parse_string_size  (char *value);
double  parse_string_time  (char *value);

void    openFile           ();
void    closeFile          ();
int     checkRotation      ();

size_t  fread_nb           (void *location, size_t nbytes);
