#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "wrappedlibs.h"

#include "debug.h"
#include "wrapper.h"
#include "bridge.h"
#include "librarian/library_private.h"
#include "x86emu.h"
#include "emu/x86emu_private.h"
#include "callback.h"
#include "librarian.h"
#include "box86context.h"
#include "emu/x86emu_private.h"
#include "callback.h"

const char* gnutlsName =
#if ANDROID
    "libgnutls.so"
#else
    "libgnutls.so.30"
#endif
    ;
#define LIBNAME gnutls

#include "generated/wrappedgnutlstypes.h"

#include "wrappercallback.h"

void freeGnutlsMy(void* lib)
{
    (void)lib;
    //gnutls_my_t *my = (gnutls_my_t *)lib;
}

// utility functions
#define SUPER() \
GO(0)   \
GO(1)   \
GO(2)   \
GO(3)   \
GO(4)

// gnutls_log
#define GO(A)   \
static uintptr_t my_gnutls_log_fct_##A = 0;                       \
static void my_gnutls_log_##A(int level, const char* p)           \
{                                                                 \
    RunFunction(my_context, my_gnutls_log_fct_##A, 2, level, p);  \
}
SUPER()
#undef GO
static void* find_gnutls_log_Fct(void* fct)
{
    if(!fct) return NULL;
    void* p;
    if((p = GetNativeFnc((uintptr_t)fct))) return p;
    #define GO(A) if(my_gnutls_log_fct_##A == (uintptr_t)fct) return my_gnutls_log_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_gnutls_log_fct_##A == 0) {my_gnutls_log_fct_##A = (uintptr_t)fct; return my_gnutls_log_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for libgnutls.so.30 gnutls_log callback\n");
    return NULL;
}

// pullpush
#define GO(A)   \
static uintptr_t my_pullpush_fct_##A = 0;                                   \
static long my_pullpush_##A(void* p, void* d, size_t l)                     \
{                                                                           \
    return (long)RunFunction(my_context, my_pullpush_fct_##A, 3, p, d, l);  \
}
SUPER()
#undef GO
static void* find_pullpush_Fct(void* fct)
{
    if(!fct) return NULL;
    void* p;
    if((p = GetNativeFnc((uintptr_t)fct))) return p;
    #define GO(A) if(my_pullpush_fct_##A == (uintptr_t)fct) return my_pullpush_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_pullpush_fct_##A == 0) {my_pullpush_fct_##A = (uintptr_t)fct; return my_pullpush_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for libgnutls.so.30 pullpush callback\n");
    return NULL;
}


// pulltimeout
#define GO(A)   \
static uintptr_t my_pulltimeout_fct_##A = 0;                                \
static int my_pulltimeout_##A(void* p, uint32_t t)                          \
{                                                                           \
    return (int)RunFunction(my_context, my_pulltimeout_fct_##A, 2, p, t);   \
}
SUPER()
#undef GO
static void* find_pulltimeout_Fct(void* fct)
{
    if(!fct) return NULL;
    void* p;
    if((p = GetNativeFnc((uintptr_t)fct))) return p;
    #define GO(A) if(my_pulltimeout_fct_##A == (uintptr_t)fct) return my_pulltimeout_##A;
    SUPER()
    #undef GO
    #define GO(A) if(my_pulltimeout_fct_##A == 0) {my_pulltimeout_fct_##A = (uintptr_t)fct; return my_pulltimeout_##A; }
    SUPER()
    #undef GO
    printf_log(LOG_NONE, "Warning, no more slot for libgnutls.so.30 pulltimeout callback\n");
    return NULL;
}
#undef SUPER


EXPORT void my_gnutls_global_set_log_function(x86emu_t* emu, void* f)
{
    (void)emu;
    my->gnutls_global_set_log_function(find_gnutls_log_Fct(f));
}

EXPORT void my_gnutls_transport_set_pull_function(x86emu_t* emu, void* session, void* f)
{
    (void)emu;
    my->gnutls_transport_set_pull_function(session, find_pullpush_Fct(f));
}
EXPORT void my_gnutls_transport_set_push_function(x86emu_t* emu, void* session, void* f)
{
    (void)emu;
    my->gnutls_transport_set_push_function(session, find_pullpush_Fct(f));
}

EXPORT void my_gnutls_transport_set_pull_timeout_function(x86emu_t* emu, void* session, void* f)
{
    (void)emu;
    my->gnutls_transport_set_pull_timeout_function(session, find_pulltimeout_Fct(f));
}

#define CUSTOM_INIT \
    getMy(lib);

#define CUSTOM_FINI \
    freeMy();

#include "wrappedlib_init.h"
