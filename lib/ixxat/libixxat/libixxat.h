#ifndef LIB_IXXAT_H
#define LIB_IXXAT_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

size_t __stdcall ixxat_enumerate(void);
void __stdcall ixxat_get_name(size_t index, char *name, int max_size);
/*
"10K",
"20K",
"50K",
"100K",
"125K",
"250K",
"500K",
"800K",
"1M",

Returns 1 if OK
        0 if failure
*/

void __stdcall init_server(void);
void __stdcall stop_server(void);

int __stdcall ixxat_init(const char *name, const char *bitRate);
void __stdcall ixxat_stop(const char *name);

// Max data size is 8
int __stdcall ixxat_send(const char *name, unsigned long id, const char* data, int size);

// Return the length of the message read
int __stdcall ixxat_read(const char *name, unsigned long *id, char* data);

// Pointer functions for DLL access
typedef void (*init_server_t)(void);
typedef void (*stop_server_t)(void);

typedef size_t (*ixxat_enumerate_t)(void);
typedef void (*ixxat_get_name_t)(size_t index, char *name, int max_size);
typedef int (*ixxat_init_t)(const char *name, const char *bitRate);
typedef void (*ixxat_stop_t)(const char *name);
typedef int (*ixxat_send_t)(const char *name, unsigned long id, const char* data, int size);
typedef int (*ixxat_read_t)(const char *name, unsigned long *id, char* data);

#ifdef __cplusplus
}
#endif

#endif // LIB_IXXAT_H
