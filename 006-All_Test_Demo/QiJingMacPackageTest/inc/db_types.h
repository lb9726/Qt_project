#ifndef __DB_TYPES_H__
#define __DB_TYPES_H__

#include <stdint.h>

#ifndef __packed
#ifdef __GNUC__
    #define __packed __attribute__ ((packed))
#else
    #define __packed
#endif
#endif /* __packed */

#ifndef __cplusplus
typedef enum 
{
    false,
    true 
} bool;
#endif  /* __cplusplus */

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef char  int8;
typedef short int16;
typedef int   int32;

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;

#ifndef NULL
    #define NULL 0
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef RET_OK
    #define RET_OK 0
#endif

#ifndef RET_FAIL
    #define RET_FAIL -1
#endif

#endif /* __DB_TYPES_H__ */
