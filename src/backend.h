/**
 * Copyright (C) 2005, 2006 Christoph Rupp (chris@crupp.de)
 * see file COPYING for licence information
 *
 * a base-"class" for a backend
 *
 */

#ifndef HAM_BACKEND_H__
#define HAM_BACKEND_H__

#ifdef __cplusplus
extern "C" {
#endif 

#include <ham/hamsterdb.h>
#include <ham/hamsterdb_int.h>
#include "txn.h"

/**
 * the backend structure - these functions and members are "inherited"
 * by every other backend (i.e. btree, hashdb etc). 
 */
#define BACKEND_DECLARATIONS(clss)                                      \
    /**                                                                 \
     * create and initialize a new backend                              \
     *                                                                  \
     * @remark this function is called after the ham_db_t structure     \ 
     * and the file were created                                        \
     */                                                                 \
    ham_status_t (*_fun_create)(clss *be, ham_u32_t flags);             \
                                                                        \
    /**                                                                 \
     * open and initialize a backend                                    \
     *                                                                  \
     * @remark this function is called after the ham_db_structure       \
     * was allocated and the file was opened                            \
     */                                                                 \
    ham_status_t (*_fun_open)(clss *be, ham_u32_t flags);               \
                                                                        \
    /**                                                                 \
     * close the backend                                                \
     *                                                                  \
     * @remark this function is called before the file is closed        \
     */                                                                 \
    ham_status_t (*_fun_close)(clss *be);                               \
                                                                        \
    /**                                                                 \
     * find a key in the index                                          \
     */                                                                 \
    ham_status_t (*_fun_find)(clss *be, ham_txn_t *txn, ham_key_t *key, \
            ham_record_t *record, ham_u32_t flags);                     \
                                                                        \
    /**                                                                 \
     * insert (or update) a key in the index                            \
     *                                                                  \
     * the backend is responsible for inserting or updating the         \
     * record. (see blob.h for blob management functions)               \
     */                                                                 \
    ham_status_t (*_fun_insert)(clss *be, ham_txn_t *txn,               \
            ham_key_t *key, ham_record_t *record, ham_u32_t flags);     \
                                                                        \
    /**                                                                 \
     * erase a key in the index                                         \
     *                                                                  \
     * @remark this function returns the record of the key              \
     * in @a rid                                                        \
     */                                                                 \
    ham_status_t (*_fun_erase)(clss *be, ham_txn_t *txn, ham_key_t *key,\
            ham_offset_t *rid, ham_u32_t *intflags, ham_u32_t flags);   \
                                                                        \
    /**                                                                 \
     * dump the whole tree to stdout                                    \
     */                                                                 \
    ham_status_t (*_fun_dump)(clss *be, ham_txn_t *txn,                 \
            ham_dump_cb_t cb);                                          \
                                                                        \
    /**                                                                 \
     * verify the whole tree                                            \
     *                                                                  \
     * @remark this function is available in DEBUG-mode only            \
     */                                                                 \
    ham_status_t (*_fun_check_integrity)(clss *be, ham_txn_t *txn);     \
                                                                        \
    /**                                                                 \
     * free all allocated resources                                     \
     *                                                                  \
     * @remark this function is called after _fun_close()               \
     */                                                                 \
    void (*_fun_delete)(clss *be);                                      \
                                                                        \
    /**                                                                 \
     * pointer to the database object                                   \
     */                                                                 \
    ham_db_t *_db;


/**
 * a generic backend structure, which has the same memory layout as 
 * all other backends
 *
 * @remark we're pre-declaring struct ham_backend_t and the typedef 
 * to avoid syntax errors in BACKEND_DECLARATIONS
 */
struct ham_backend_t;
typedef struct ham_backend_t ham_backend_t;

struct ham_backend_t
{
    BACKEND_DECLARATIONS(ham_backend_t)
};


#ifdef __cplusplus
} // extern "C"
#endif 

#endif /* HAM_BACKEND_H__ */
