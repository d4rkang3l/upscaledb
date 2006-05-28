/**
 * Copyright (C) 2005, 2006 Christoph Rupp (chris@crupp.de)
 * see file LICENSE for license and copyright information
 *
 * freelist structures, functions and macros
 *
 */

#ifndef HAM_FREELIST_H__
#define HAM_FREELIST_H__

#ifdef __cplusplus
extern "C" {
#endif 

/**
 * the freelist structure is a linked list of pages, which track
 * the deleted pages and blobs
 */

#include "txn.h"
#include "packstart.h"

/**
 * an entry in a freelist-page
 */
typedef HAM_PACK_0 struct HAM_PACK_1 freel_entry_t
{
    /**
     * the pointer of this entry
     */
    ham_offset_t _address;

    /**
     * the size of this entry
     */
    ham_size_t _size;
    
} HAM_PACK_2 freel_entry_t;

/*
 * get the address of a freelist entry
 */
#define freel_get_address(fl)       (ham_db2h_offset((fl)->_address))

/*
 * set the address of a freelist entry
 */
#define freel_set_address(fl, a)    ((fl)->_address=ham_h2db_offset(a))

/*
 * get the size of a freelist entry
 */
#define freel_get_size(fl)          (ham_db2h_size((fl)->_size))

/*
 * set the size of a freelist entry
 */
#define freel_set_size(fl, s)       ((fl)->_size=ham_h2db_size(s))


/**
 * a freelist-payload; it spans the persistent part of a ham_page_t:
 * freel_payload_t *fl=(freel_payload_t *)page->_u._pers;
 */
typedef HAM_PACK_0 struct HAM_PACK_1 freel_payload_t
{
    /**
     * number of used entries in the page - unused TODO
     */
    ham_u16_t _count;

    /**
     * address of the next freelist page
     */
    ham_offset_t _overflow;

    /**
     * freelist entries
     */
    freel_entry_t _entries[1];

} HAM_PACK_2 freel_payload_t;

#include "packstop.h"

/**
 * get the number of entries of a freelist-page
 */
#define freel_page_get_count(fl)          (ham_db2h16((fl)->_count))

/**
 * set the number of entries of a freelist-page
 */
#define freel_page_set_count(fl, c)       (fl)->_count=ham_h2db16(c)

/**
 * get the address of the next overflow page
 */
#define freel_page_get_overflow(fl)   (ham_db2h_offset((fl)->_overflow))

/**
 * set the address of the next overflow page
 */
#define freel_page_set_overflow(fl, o) (fl)->_overflow=ham_h2db_offset(o)

/**
 * get a freel_payload_t from a ham_page_t
 */
#define page_get_freel_payload(p)     ((freel_payload_t *)p->_pers->_s._payload)

/**
 * get the array of freelist-entries
 */
#define freel_page_get_entries(fl)    (fl)->_entries

/**
 * get maximum number of freelist entries in an overflow page
 */
extern ham_size_t
freel_get_max_elements(ham_db_t *db);

/**
 * search for a free freelist entry which has space for @a size bytes,
 * and return the address of this area. the freelist entry is removed
 * from the list.
 *
 * @remark you can set the HAM_NO_PAGE_ALIGN as a flag
 */
extern ham_offset_t
freel_alloc_area(ham_db_t *db, ham_txn_t *txn, ham_size_t size, 
        ham_u32_t flags);

/**
 * add a new entry to the freelist; the freelist will automatically
 * allocate an overflow page if necessary
 */
extern ham_status_t 
freel_add_area(ham_db_t *db, ham_txn_t *txn, ham_offset_t address, 
        ham_size_t size);


#ifdef __cplusplus
} // extern "C"
#endif 

#endif /* HAM_FREELIST_H__ */
