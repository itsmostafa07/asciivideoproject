#ifndef _HELPERS_H
#define _HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <libavutil/avutil.h>

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

#define ERROR(s, ...) av_log(NULL, AV_LOG_ERROR, "[ERROR] " s "\n", ##__VA_ARGS__)

#define DEBUG(s, ...) av_log(NULL, AV_LOG_DEBUG, "[DEBUG] " s "\n", ##__VA_ARGS__)

#define INFO(s, ...) av_log(NULL, AV_LOG_INFO, "[INFO] " s "\n", ##__VA_ARGS__)

#define WARN(s, ...) av_log(NULL, AV_LOG_WARNING, "[WARNING] " s "\n", ##__VA_ARGS__)

#define FATAL(s, ...) av_log(NULL, AV_LOG_FATAL, "[FATAl] " s "\n", ##__VA_ARGS__)
#endif