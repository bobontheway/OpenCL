#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

/**
 * 将文件中的数据加载到缓冲区中，或则将缓冲区中的数据存入文件
 */
int load_data(const char *file, uint8_t *addr, uint32_t size);
int store_data(const char *file, void *addr, uint32_t size);
void time_start(void);
void time_end(const char *str);

#endif /* _UTIL_H_ */
