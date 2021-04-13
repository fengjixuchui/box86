/*****************************************************************
 * File automatically generated by rebuild_wrappers.py (v1.2.0.09)
 *****************************************************************/
#ifndef __wrappedgobject2TYPES_H_
#define __wrappedgobject2TYPES_H_

#ifndef LIBNAME
#error You should only #include this file inside a wrapped*.c file
#endif
#ifndef ADDED_FUNCTIONS
#define ADDED_FUNCTIONS() 
#endif

typedef void* (*pFp_t)(void*);
typedef int32_t (*iFpp_t)(void*, void*);
typedef void* (*pFpp_t)(void*, void*);
typedef void (*vFiip_t)(int32_t, int32_t, void*);
typedef int32_t (*iFppp_t)(void*, void*, void*);
typedef void* (*pFipV_t)(int32_t, void*, void*);
typedef void* (*pFppp_t)(void*, void*, void*);
typedef void* (*pFppV_t)(void*, void*, void*);
typedef void (*vFpupp_t)(void*, uint32_t, void*, void*);
typedef void (*vFpppp_t)(void*, void*, void*, void*);
typedef int32_t (*iFippi_t)(int32_t, void*, void*, int32_t);
typedef int32_t (*iFipppi_t)(int32_t, void*, void*, void*, int32_t);
typedef uintptr_t (*LFupppp_t)(uint32_t, void*, void*, void*, void*);
typedef uintptr_t (*LFpppppu_t)(void*, void*, void*, void*, void*, uint32_t);
typedef int32_t (*iFipupupi_t)(int32_t, void*, uint32_t, void*, uint32_t, void*, int32_t);
typedef uint32_t (*uFpiupppp_t)(void*, int32_t, uint32_t, void*, void*, void*, void*);
typedef uintptr_t (*LFpiupppp_t)(void*, int32_t, uint32_t, void*, void*, void*, void*);
typedef uint32_t (*uFpiiupppiuV_t)(void*, int32_t, int32_t, uint32_t, void*, void*, void*, int32_t, uint32_t, void*);
typedef uint32_t (*uFpiippppiup_t)(void*, int32_t, int32_t, void*, void*, void*, void*, int32_t, uint32_t, void*);

#define SUPER() ADDED_FUNCTIONS() \
	GO(g_type_class_peek_parent, pFp_t) \
	GO(g_param_type_register_static, iFpp_t) \
	GO(g_value_array_sort, pFpp_t) \
	GO(g_type_add_interface_static, vFiip_t) \
	GO(g_value_register_transform_func, vFiip_t) \
	GO(g_boxed_type_register_static, iFppp_t) \
	GO(g_object_new, pFipV_t) \
	GO(g_value_array_sort_with_data, pFppp_t) \
	GO(g_object_connect, pFppV_t) \
	GO(g_param_spec_set_qdata_full, vFpupp_t) \
	GO(g_object_set_data_full, vFpppp_t) \
	GO(g_type_register_static, iFippi_t) \
	GO(g_type_register_fundamental, iFipppi_t) \
	GO(g_signal_add_emission_hook, LFupppp_t) \
	GO(g_signal_connect_data, LFpppppu_t) \
	GO(g_type_register_static_simple, iFipupupi_t) \
	GO(g_signal_handlers_block_matched, uFpiupppp_t) \
	GO(g_signal_handlers_disconnect_matched, uFpiupppp_t) \
	GO(g_signal_handlers_unblock_matched, uFpiupppp_t) \
	GO(g_signal_handler_find, LFpiupppp_t) \
	GO(g_signal_new, uFpiiupppiuV_t) \
	GO(g_signal_new_valist, uFpiippppiup_t) \
	GO(g_signal_newv, uFpiippppiup_t)

#endif // __wrappedgobject2TYPES_H_
