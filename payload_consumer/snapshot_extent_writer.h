//
// Copyright (C) 2020 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <cstdint>
#include <vector>

#include <libsnapshot/cow_writer.h>

#include "update_engine/payload_consumer/extent_writer.h"

namespace chromeos_update_engine {
class SnapshotExtentWriter : public chromeos_update_engine::ExtentWriter {
 public:
  explicit SnapshotExtentWriter(android::snapshot::ICowWriter* cow_writer);
  ~SnapshotExtentWriter();
  // Returns true on success.
  bool Init(FileDescriptorPtr fd,
            const google::protobuf::RepeatedPtrField<Extent>& extents,
            uint32_t block_size) override;
  // Returns true on success.
  // This will construct a COW_REPLACE operation and forward it to CowWriter. It
  // is important that caller does not perform SOURCE_COPY operation on this
  // class, otherwise raw data will be stored. Caller should find ways to use
  // COW_COPY whenever possible.
  bool Write(const void* bytes, size_t count) override;

 private:
  // It's a non-owning pointer, because PartitionWriter owns the CowWruter. This
  // allows us to use a single instance of CowWriter for all operations applied
  // to the same partition.
  [[maybe_unused]] android::snapshot::ICowWriter* cow_writer_;
  [[maybe_unused]] google::protobuf::RepeatedPtrField<Extent> extents_;
  [[maybe_unused]] std::vector<uint8_t> buffer_;
};
}  // namespace chromeos_update_engine
