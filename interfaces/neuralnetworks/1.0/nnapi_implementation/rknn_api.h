/****************************************************************************
*
*    Copyright (c) 2017 - 2021 by Rockchip Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Rockchip Corporation. This is proprietary information owned by
*    Rockchip Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Rockchip Corporation.
*
*****************************************************************************/


#ifndef _RKNN_API_H
#define _RKNN_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
    Definition of extended flag for rknn_init.
*/
/* set high priority context. */
#define RKNN_FLAG_PRIOR_HIGH                    0x00000000

/* set medium priority context */
#define RKNN_FLAG_PRIOR_MEDIUM                  0x00000001

/* set low priority context. */
#define RKNN_FLAG_PRIOR_LOW                     0x00000002

/* asynchronous mode.
   when enable, rknn_outputs_get will not block for too long because it directly retrieves the result of
   the previous frame which can increase the frame rate on single-threaded mode, but at the cost of
   rknn_outputs_get not retrieves the result of the current frame.
   in multi-threaded mode you do not need to turn this mode on. */
#define RKNN_FLAG_ASYNC_MASK                    0x00000004

/* collect performance mode.
   when enable, you can get detailed performance reports via rknn_query(ctx, RKNN_QUERY_PERF_DETAIL, ...),
   but it will reduce the frame rate. */
#define RKNN_FLAG_COLLECT_PERF_MASK             0x00000008

/* allocate all memory in outside, includes weight/internal/inputs/outputs */
#define RKNN_FLAG_MEM_ALLOC_OUTSIDE             0x00000010

/*
    Error code returned by the RKNN API.
*/
#define RKNN_SUCC                               0       /* execute succeed. */
#define RKNN_ERR_FAIL                           -1      /* execute failed. */
#define RKNN_ERR_TIMEOUT                        -2      /* execute timeout. */
#define RKNN_ERR_DEVICE_UNAVAILABLE             -3      /* device is unavailable. */
#define RKNN_ERR_MALLOC_FAIL                    -4      /* memory malloc fail. */
#define RKNN_ERR_PARAM_INVALID                  -5      /* parameter is invalid. */
#define RKNN_ERR_MODEL_INVALID                  -6      /* model is invalid. */
#define RKNN_ERR_CTX_INVALID                    -7      /* context is invalid. */
#define RKNN_ERR_INPUT_INVALID                  -8      /* input is invalid. */
#define RKNN_ERR_OUTPUT_INVALID                 -9      /* output is invalid. */
#define RKNN_ERR_DEVICE_UNMATCH                 -10     /* the device is unmatch, please update rknn sdk
                                                           and npu driver/firmware. */
#define RKNN_ERR_INCOMPATILE_PRE_COMPILE_MODEL  -11     /* This RKNN model use pre_compile mode, but not compatible with current driver. */
#define RKNN_ERR_INCOMPATILE_OPTIMIZATION_LEVEL_VERSION  -12     /* This RKNN model set optimization level, but not compatible with current driver. */
#define RKNN_ERR_TARGET_PLATFORM_UNMATCH        -13     /* This RKNN model set target platform, but not compatible with current platform. */

/*
    Definition for tensor
*/
#define RKNN_MAX_DIMS                           16      /* maximum dimension of tensor. */
#define RKNN_MAX_NUM_CHANNEL                    15      /* maximum channel number of input tensor. */
#define RKNN_MAX_NAME_LEN                       256     /* maximum name lenth of tensor. */


#ifdef __arm__
typedef uint32_t rknn_context;
#else
typedef uint64_t rknn_context;
#endif


/*
    The query command for rknn_query
*/
typedef enum _rknn_query_cmd {
    RKNN_QUERY_IN_OUT_NUM = 0,                          /* query the number of input & output tensor. */
    RKNN_QUERY_INPUT_ATTR,                              /* query the attribute of input tensor. */
    RKNN_QUERY_OUTPUT_ATTR,                             /* query the attribute of output tensor. */
    RKNN_QUERY_PERF_DETAIL,                             /* query the detail performance, need set
                                                           RKNN_FLAG_COLLECT_PERF_MASK when call rknn_init,
                                                           this query needs to be valid after rknn_outputs_get. */
    RKNN_QUERY_PERF_RUN,                                /* query the time of run,
                                                           this query needs to be valid after rknn_outputs_get. */
    RKNN_QUERY_SDK_VERSION,                             /* query the sdk & driver version */

    RKNN_QUERY_MEM_SIZE,                                /* query the weight & internal memory size */
    RKNN_QUERY_CUSTOM_STRING,                           /* query the custom string */

    RKNN_QUERY_CMD_MAX
} rknn_query_cmd;

/*
    the tensor data type.
*/
typedef enum _rknn_tensor_type {
    RKNN_TENSOR_FLOAT32 = 0,                            /* data type is float32. */
    RKNN_TENSOR_FLOAT16,                                /* data type is float16. */
    RKNN_TENSOR_INT8,                                   /* data type is int8. */
    RKNN_TENSOR_UINT8,                                  /* data type is uint8. */
    RKNN_TENSOR_INT16,                                  /* data type is int16. */
    RKNN_TENSOR_UINT16,                                 /* data type is uint16. */
    RKNN_TENSOR_INT32,                                  /* data type is int32. */
    RKNN_TENSOR_UINT32,                                 /* data type is uint32. */
    RKNN_TENSOR_INT64,                                  /* data type is int64. */

    RKNN_TENSOR_TYPE_MAX
} rknn_tensor_type;

inline const char* get_type_string(rknn_tensor_type type)
{
    switch(type) {
    case RKNN_TENSOR_FLOAT32: return "FP32";
    case RKNN_TENSOR_FLOAT16: return "FP16";
    case RKNN_TENSOR_INT8: return "INT8";
    case RKNN_TENSOR_UINT8: return "UINT8";
    case RKNN_TENSOR_INT16: return "INT16";
    case RKNN_TENSOR_UINT16: return "UINT16";
    case RKNN_TENSOR_INT32: return "INT32";
    case RKNN_TENSOR_UINT32: return "UINT32";
    case RKNN_TENSOR_INT64: return "UINT64";
    default: return "UNKNOW";
    }
}

/*
    the quantitative type.
*/
typedef enum _rknn_tensor_qnt_type {
    RKNN_TENSOR_QNT_NONE = 0,                           /* none. */
    RKNN_TENSOR_QNT_DFP,                                /* dynamic fixed point. */
    RKNN_TENSOR_QNT_AFFINE_ASYMMETRIC,                  /* asymmetric affine. */

    RKNN_TENSOR_QNT_MAX
} rknn_tensor_qnt_type;

inline const char* get_qnt_type_string(rknn_tensor_qnt_type type)
{
    switch(type) {
    case RKNN_TENSOR_QNT_NONE: return "NONE";
    case RKNN_TENSOR_QNT_DFP: return "DFP";
    case RKNN_TENSOR_QNT_AFFINE_ASYMMETRIC: return "AFFINE";
    default: return "UNKNOW";
    }
}

/*
    the tensor data format.
*/
typedef enum _rknn_tensor_format {
    RKNN_TENSOR_NCHW = 0,                               /* data format is NCHW. */
    RKNN_TENSOR_NHWC,                                   /* data format is NHWC. */

    RKNN_TENSOR_FORMAT_MAX
} rknn_tensor_format;

inline const char* get_format_string(rknn_tensor_format fmt)
{
    switch(fmt) {
    case RKNN_TENSOR_NCHW: return "NCHW";
    case RKNN_TENSOR_NHWC: return "NHWC";
    default: return "UNKNOW";
    }
}

/*
    the information for RKNN_QUERY_IN_OUT_NUM.
*/
typedef struct _rknn_input_output_num {
    uint32_t n_input;                                   /* the number of input. */
    uint32_t n_output;                                  /* the number of output. */
} rknn_input_output_num;

/*
    the information for RKNN_QUERY_INPUT_ATTR / RKNN_QUERY_OUTPUT_ATTR.
*/
typedef struct _rknn_tensor_attr {
    uint32_t index;                                     /* input parameter, the index of input/output tensor,
                                                           need set before call rknn_query. */

    uint32_t n_dims;                                    /* the number of dimensions. */
    uint32_t dims[RKNN_MAX_DIMS];                       /* the dimensions array. */
    char name[RKNN_MAX_NAME_LEN];                       /* the name of tensor. */

    uint32_t n_elems;                                   /* the number of elements. */
    uint32_t size;                                      /* the bytes size of tensor. */

    rknn_tensor_format fmt;                             /* the data format of tensor. */
    rknn_tensor_type type;                              /* the data type of tensor. */
    rknn_tensor_qnt_type qnt_type;                      /* the quantitative type of tensor. */
    int8_t fl;                                          /* fractional length for RKNN_TENSOR_QNT_DFP. */
    int32_t zp;                                         /* zero point for RKNN_TENSOR_QNT_AFFINE_ASYMMETRIC. */
    float scale;                                        /* scale for RKNN_TENSOR_QNT_AFFINE_ASYMMETRIC. */

    uint32_t stride;                                    /* the stride of tensor, 0 means equal to width. */
    uint32_t size_with_stride;                          /* the bytes size of tensor with stride. */

    uint8_t pass_through;                               /* pass through mode, for rknn_set_io_mem interface.
                                                           if TRUE, the buf data is passed directly to the input node of the rknn model
                                                                    without any conversion. the following variables do not need to be set.
                                                           if FALSE, the buf data is converted into an input consistent with the model
                                                                     according to the following type and fmt. so the following variables
                                                                     need to be set.*/
} rknn_tensor_attr;

/*
    the information for RKNN_QUERY_PERF_DETAIL.
*/
typedef struct _rknn_perf_detail {
    char* perf_data;                                    /* the string pointer of perf detail. don't need free it by user. */
    uint64_t data_len;                                  /* the string length. */
} rknn_perf_detail;

/*
    the information for RKNN_QUERY_PERF_RUN.
*/
typedef struct _rknn_perf_run {
    int64_t run_duration;                               /* real inference time (us) */
} rknn_perf_run;

/*
    the information for RKNN_QUERY_SDK_VERSION.
*/
typedef struct _rknn_sdk_version {
    char api_version[256];                              /* the version of rknn api. */
    char drv_version[256];                              /* the version of rknn driver. */
} rknn_sdk_version;

/*
    the information for RKNN_QUERY_MEM_SIZE.
*/
typedef struct _rknn_mem_size {
    uint32_t total_weight_size;                         /* the weight memory size */
    uint32_t total_internal_size;                       /* the internal memory size, exclude inputs/outputs */
} rknn_mem_size;

/*
    the information for RKNN_QUERY_CUSTOM_STRING.
*/
typedef struct _rknn_custom_string {
    char string[1024];                                  /* the string of custom, lengths max to 1024 bytes */
} rknn_custom_string;

/*
   The flags of rknn_tensor_mem. 
*/
typedef enum _rknn_tensor_mem_flags {
    RKNN_TENSOR_MEMORY_FLAGS_ALLOC_INSIDE = 1,           /*Used to mark in rknn_destory_mem() whether it is necessary to release the "mem" pointer itself.
                                                         If the flag RKNN_TENSOR_MEMORY_FLAGS_ALLOC_INSIDE is set, rknn_destory_mem() will call free(mem).*/
    RKNN_TENSOR_MEMORY_FLAGS_UNKNOWN
} rknn_tensor_mem_flags;

/*
    the memory information of tensor.
*/
typedef struct _rknn_tensor_memory {
    void*            virt_addr;                         /* the virtual address of tensor buffer. */
    uint64_t         phys_addr;                         /* the physical address of tensor buffer. */
    int32_t          fd;                                /* the fd of tensor buffer. */
    int32_t          offset;                            /* indicates the offset of the memory. */
    uint32_t         size;                              /* the size of tensor buffer. */
    uint32_t         flags;                             /* the flags of tensor buffer, reserved */
    void *           priv_data;                         /* the private data of tensor buffer. */
} rknn_tensor_mem;

/*
    the input information for rknn_input_set.
*/
typedef struct _rknn_input {
    uint32_t index;                                     /* the input index. */
    void* buf;                                          /* the input buf for index. */
    uint32_t size;                                      /* the size of input buf. */
    uint8_t pass_through;                               /* pass through mode.
                                                           if TRUE, the buf data is passed directly to the input node of the rknn model
                                                                    without any conversion. the following variables do not need to be set.
                                                           if FALSE, the buf data is converted into an input consistent with the model
                                                                     according to the following type and fmt. so the following variables
                                                                     need to be set.*/
    rknn_tensor_type type;                              /* the data type of input buf. */
    rknn_tensor_format fmt;                             /* the data format of input buf.
                                                           currently the internal input format of NPU is NCHW by default.
                                                           so entering NCHW data can avoid the format conversion in the driver. */
} rknn_input;

/*
    the output information for rknn_outputs_get.
*/
typedef struct _rknn_output {
    uint8_t want_float;                                 /* want transfer output data to float */
    uint8_t is_prealloc;                                /* whether buf is pre-allocated.
                                                           if TRUE, the following variables need to be set.
                                                           if FALSE, the following variables do not need to be set. */
    uint32_t index;                                     /* the output index. */
    void* buf;                                          /* the output buf for index.
                                                           when is_prealloc = FALSE and rknn_outputs_release called,
                                                           this buf pointer will be free and don't use it anymore. */
    uint32_t size;                                      /* the size of output buf. */
} rknn_output;

/*
    the extend information for rknn_init.
*/
typedef struct _rknn_init_extend {
    int32_t core_id;                                    /* npu core id */
    uint8_t reserved[128];                              /* reserved */
} rknn_init_extend;

/*
    the extend information for rknn_run.
*/
typedef struct _rknn_run_extend {
    uint64_t frame_id;                                  /* output parameter, indicate current frame id of run. */
    int32_t non_block;                                  /* block flag of run, 0 is block else 1 is non block */
    int32_t timeout_ms;                                 /* timeout for block mode, in milliseconds */
} rknn_run_extend;

/*
    the extend information for rknn_outputs_get.
*/
typedef struct _rknn_output_extend {
    uint64_t frame_id;                                  /* output parameter, indicate the frame id of outputs, corresponds to
                                                           struct rknn_run_extend.frame_id.*/
} rknn_output_extend;


/*  rknn_init

    initial the context and load the rknn model.

    input:
        rknn_context* context       the pointer of context handle.
        void* model                 if size > 0, pointer to the rknn model, if size = 0, filepath to the rknn model.
        uint32_t size               the size of rknn model.
        uint32_t flag               extend flag, see the define of RKNN_FLAG_XXX_XXX.
        rknn_init_extend* extend    the extend information of init.
    return:
        int                         error code.
*/
int rknn_init(rknn_context* context, void* model, uint32_t size, uint32_t flag, rknn_init_extend* extend);


/*  rknn_destroy

    unload the rknn model and destroy the context.

    input:
        rknn_context context        the handle of context.
    return:
        int                         error code.
*/
int rknn_destroy(rknn_context context);


/*  rknn_query

    query the information about model or others. see rknn_query_cmd.

    input:
        rknn_context context        the handle of context.
        rknn_query_cmd cmd          the command of query.
        void* info                  the buffer point of information.
        uint32_t size               the size of information.
    return:
        int                         error code.
*/
int rknn_query(rknn_context context, rknn_query_cmd cmd, void* info, uint32_t size);


/*  rknn_inputs_set

    set inputs information by input index of rknn model.
    inputs information see rknn_input.

    input:
        rknn_context context        the handle of context.
        uint32_t n_inputs           the number of inputs.
        rknn_input inputs[]         the arrays of inputs information, see rknn_input.
    return:
        int                         error code
*/
int rknn_inputs_set(rknn_context context, uint32_t n_inputs, rknn_input inputs[]);


/*  rknn_run

    run the model to execute inference.

    input:
        rknn_context context        the handle of context.
        rknn_run_extend* extend     the extend information of run.
    return:
        int                         error code.
*/
int rknn_run(rknn_context context, rknn_run_extend* extend);


/*  rknn_wait

    wait the model after execute inference.

    input:
        rknn_context context        the handle of context.
        rknn_run_extend* extend     the extend information of run.
    return:
        int                         error code.
*/
int rknn_wait(rknn_context context, rknn_run_extend* extend);


/*  rknn_outputs_get

    wait the inference to finish and get the outputs.
    this function will block until inference finish.
    the results will set to outputs[].

    input:
        rknn_context context        the handle of context.
        uint32_t n_outputs          the number of outputs.
        rknn_output outputs[]       the arrays of output, see rknn_output.
        rknn_output_extend*         the extend information of output.
    return:
        int                         error code.
*/
int rknn_outputs_get(rknn_context context, uint32_t n_outputs, rknn_output outputs[], rknn_output_extend* extend);


/*  rknn_outputs_release

    release the outputs that get by rknn_outputs_get.
    after called, the rknn_output[x].buf get from rknn_outputs_get will
    also be free when rknn_output[x].is_prealloc = FALSE.

    input:
        rknn_context context        the handle of context.
        uint32_t n_ouputs           the number of outputs.
        rknn_output outputs[]       the arrays of output.
    return:
        int                         error code
*/
int rknn_outputs_release(rknn_context context, uint32_t n_ouputs, rknn_output outputs[]);


/* new api for zero copy */

/*  rknn_create_mem_from_phys (memory allocated outside)

    initialize tensor memory from physical address.

    input:
        rknn_context ctx            the handle of context.
        uint64_t phys_addr          physical address.
        void *virt_addr             virtual address.
        uint32_t size               the size of tensor buffer.
    return: 
        rknn_tensor_mem             the pointer of tensor memory information.
*/
rknn_tensor_mem* rknn_create_mem_from_phys(rknn_context ctx, uint64_t phys_addr, void *virt_addr, uint32_t size);


/*  rknn_create_mem_from_fd (memory allocated outside)

    initialize tensor memory from file description.

    input:
        rknn_context ctx            the handle of context.
        int32_t fd                  file description.
        void *virt_addr             virtual address.
        uint32_t size               the size of tensor buffer.
        int32_t offset              indicates the offset of the memory (virt_addr without offset).
    return:
        rknn_tensor_mem             the pointer of tensor memory information.
*/
rknn_tensor_mem* rknn_create_mem_from_fd(rknn_context ctx, int32_t fd, void *virt_addr, uint32_t size, int32_t offset);


/*  rknn_create_mem_from_mb_blk (memory allocated outside)

    create tensor memory from mb_blk.

    input:
        rknn_context ctx            the handle of context.
        void *mb_blk                mb_blk allocate from system api.
        int32_t offset              indicates the offset of the memory.
    return:
        rknn_tensor_mem             the pointer of tensor memory information.
*/
rknn_tensor_mem* rknn_create_mem_from_mb_blk(rknn_context ctx, void *mb_blk, int32_t offset);


/*  rknn_create_mem (memory allocated inside)

    create tensor memory.

    input:
        rknn_context ctx            the handle of context.
        uint32_t size               the size of tensor buffer.
    return:
        rknn_tensor_mem             the pointer of tensor memory information.
*/
rknn_tensor_mem* rknn_create_mem(rknn_context ctx, uint32_t size);


/*  rknn_destory_mem (support allocate inside and outside)

    destory tensor memory.

    input:
        rknn_context ctx            the handle of context.
        rknn_tensor_mem *mem        the pointer of tensor memory information.
    return:
        int                         error code
*/
int rknn_destory_mem(rknn_context ctx, rknn_tensor_mem *mem);


/*  rknn_set_weight_mem

    set the weight memory.

    input:
        rknn_context ctx            the handle of context.
        rknn_tensor_mem *mem        the array of tensor memory information
    return:
        int                         error code.
*/
int rknn_set_weight_mem(rknn_context ctx, rknn_tensor_mem *mem);


/*  rknn_set_internal_mem

    set the internal memory.

    input:
        rknn_context ctx            the handle of context.
        rknn_tensor_mem *mem        the array of tensor memory information
    return:
        int                         error code.
*/
int rknn_set_internal_mem(rknn_context ctx, rknn_tensor_mem *mem);


/*  rknn_set_io_mem

    set the input and output tensors buffer.

    input:
        rknn_context ctx            the handle of context.
        rknn_tensor_mem *mem        the array of tensor memory information.
        rknn_tensor_attr *attr      the attribute of input or output tensor buffer.
    return:
        int                         error code.
*/
int rknn_set_io_mem(rknn_context ctx, rknn_tensor_mem *mem, rknn_tensor_attr *attr);


#ifdef __cplusplus
} //extern "C"
#endif

#endif  //_RKNN_API_H
