#ifndef _HELPERS_H
#define _HELPERS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * A simple macro used for functions that not implemented yet. Like TODO
 */
#define UNIMPLEMENTED()                                                        \
  do                                                                           \
  {                                                                            \
    fprintf(stderr,                                                            \
            "Error: Function %s is not implemented yet. File: %s, Line: %d\n", \
            __func__, __FILE__, __LINE__);                                     \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#include <stdio.h>

void resize_terminal(int width, int height);

#endif