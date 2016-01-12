/*
 * Copyright (C) 2005-2016 Christoph Rupp (chris@crupp.de).
 * All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See the file COPYING for License information.
 */

#ifndef UPS_BTREE_VISITOR_H
#define UPS_BTREE_VISITOR_H

#include "0root/root.h"

#include "ups/upscaledb_uqi.h"

// Always verify that a file of level N does not include headers > N!

#ifndef UPS_ROOT_H
#  error "root.h was not included"
#endif

namespace upscaledb {

struct SelectStatement;

//
// The ScanVisitor is the callback implementation for the scan call.
// It will either receive single keys or multiple keys in an array.
//
struct ScanVisitor {
  // Constructor
  ScanVisitor(SelectStatement *stmt = 0)
    : statement(stmt) {
  }

  // Operates on a single key/value pair
  virtual void operator()(const void *key_data, uint16_t key_size, 
                  const void *record_data, uint32_t record_size,
                  size_t duplicate_count) = 0;

  // Operates on an array of keys and/or records
  virtual void operator()(const void *key_array, const void *record_array,
                  size_t key_count) = 0;

  // Assigns the internal result to |result|
  virtual void assign_result(uqi_result_t *result) = 0;

  // The select statement
  SelectStatement *statement;
};

struct Context;
class BtreeNodeProxy;

//
// The BtreeVisitor is the callback implementation for the visit call.
// It will visit each node instead of each key.
//
struct BtreeVisitor {
  // Specifies if the visitor modifies the node
  virtual bool is_read_only() const = 0;

  // called for each node
  virtual void operator()(Context *context, BtreeNodeProxy *node) = 0;
};

} // namespace upscaledb

#endif /* UPS_BTREE_VISITOR_H */
