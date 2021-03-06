//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//
// The following only applies to changes made to this file as part of YugaByte development.
//
// Portions Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "yb/rocksdb/utilities/leveldb_options.h"
#include "yb/rocksdb/cache.h"
#include "yb/rocksdb/comparator.h"
#include "yb/rocksdb/env.h"
#include "yb/rocksdb/filter_policy.h"
#include "yb/rocksdb/options.h"
#include "yb/rocksdb/table.h"

namespace rocksdb {

LevelDBOptions::LevelDBOptions()
    : comparator(BytewiseComparator()),
      create_if_missing(false),
      error_if_exists(false),
      paranoid_checks(false),
      env(Env::Default()),
      info_log(nullptr),
      write_buffer_size(4 << 20),
      max_open_files(1000),
      block_cache(nullptr),
      block_size(4096),
      block_restart_interval(16),
      compression(kSnappyCompression),
      filter_policy(nullptr) {}

Options ConvertOptions(const LevelDBOptions& leveldb_options) {
  Options options = Options();
  options.create_if_missing = leveldb_options.create_if_missing;
  options.error_if_exists = leveldb_options.error_if_exists;
  options.paranoid_checks = leveldb_options.paranoid_checks;
  options.env = leveldb_options.env;
  options.info_log.reset(leveldb_options.info_log);
  options.write_buffer_size = leveldb_options.write_buffer_size;
  options.max_open_files = leveldb_options.max_open_files;
  options.compression = leveldb_options.compression;

  BlockBasedTableOptions table_options;
  table_options.block_cache.reset(leveldb_options.block_cache);
  table_options.block_size = leveldb_options.block_size;
  table_options.block_restart_interval = leveldb_options.block_restart_interval;
  table_options.filter_policy.reset(leveldb_options.filter_policy);
  options.table_factory.reset(NewBlockBasedTableFactory(table_options));

  return options;
}

}  // namespace rocksdb
