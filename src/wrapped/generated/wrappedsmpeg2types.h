/*****************************************************************
 * File automatically generated by rebuild_wrappers.py (v1.2.0.09)
 *****************************************************************/
#ifndef __wrappedsmpeg2TYPES_H_
#define __wrappedsmpeg2TYPES_H_

#ifndef LIBNAME
#error You should only #include this file inside a wrapped*.c file
#endif
#ifndef ADDED_FUNCTIONS
#define ADDED_FUNCTIONS() 
#endif

typedef void (*vFpp_t)(void*, void*);
typedef void* (*pFipi_t)(int32_t, void*, int32_t);
typedef void* (*pFppi_t)(void*, void*, int32_t);
typedef void (*vFpppp_t)(void*, void*, void*, void*);
typedef void* (*pFpipi_t)(void*, int32_t, void*, int32_t);
typedef void* (*pFppii_t)(void*, void*, int32_t, int32_t);

#define SUPER() ADDED_FUNCTIONS() \
	GO(SMPEG_getinfo, vFpp_t) \
	GO(SMPEG_new_descr, pFipi_t) \
	GO(SMPEG_new, pFppi_t) \
	GO(SMPEG_setdisplay, vFpppp_t) \
	GO(SMPEG_new_data, pFpipi_t) \
	GO(SMPEG_new_rwops, pFppii_t)

#endif // __wrappedsmpeg2TYPES_H_
