/*****************************************************************
 * File automatically generated by rebuild_wrappers.py (v1.2.0.09)
 *****************************************************************/
#ifndef __wrappedlibxcbdri2TYPES_H_
#define __wrappedlibxcbdri2TYPES_H_

#ifndef LIBNAME
#error You should only #include this file inside a wrapped*.c file
#endif
#ifndef ADDED_FUNCTIONS
#define ADDED_FUNCTIONS() 
#endif

typedef my_xcb_cookie_t (*XFpuu_t)(void*, uint32_t, uint32_t);

#define SUPER() ADDED_FUNCTIONS() \
	GO(xcb_dri2_authenticate, XFpuu_t) \
	GO(xcb_dri2_connect, XFpuu_t) \
	GO(xcb_dri2_query_version, XFpuu_t)

#endif // __wrappedlibxcbdri2TYPES_H_
