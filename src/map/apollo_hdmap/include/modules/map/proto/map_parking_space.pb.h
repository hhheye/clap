// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modules/map/proto/map_parking_space.proto

#ifndef PROTOBUF_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto__INCLUDED
#define PROTOBUF_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "modules/map/proto/map_geometry.pb.h"
#include "modules/map/proto/map_id.pb.h"
// @@protoc_insertion_point(includes)

namespace apollo {
namespace hdmap {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();
void protobuf_AssignDesc_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();
void protobuf_ShutdownFile_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();

class ParkingLot;
class ParkingSpace;

// ===================================================================

class ParkingSpace : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:apollo.hdmap.ParkingSpace) */ {
 public:
  ParkingSpace();
  virtual ~ParkingSpace();

  ParkingSpace(const ParkingSpace& from);

  inline ParkingSpace& operator=(const ParkingSpace& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ParkingSpace& default_instance();

  void Swap(ParkingSpace* other);

  // implements Message ----------------------------------------------

  inline ParkingSpace* New() const { return New(NULL); }

  ParkingSpace* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ParkingSpace& from);
  void MergeFrom(const ParkingSpace& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(ParkingSpace* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .apollo.hdmap.Id id = 1;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 1;
  const ::apollo::hdmap::Id& id() const;
  ::apollo::hdmap::Id* mutable_id();
  ::apollo::hdmap::Id* release_id();
  void set_allocated_id(::apollo::hdmap::Id* id);

  // optional .apollo.hdmap.Polygon polygon = 2;
  bool has_polygon() const;
  void clear_polygon();
  static const int kPolygonFieldNumber = 2;
  const ::apollo::hdmap::Polygon& polygon() const;
  ::apollo::hdmap::Polygon* mutable_polygon();
  ::apollo::hdmap::Polygon* release_polygon();
  void set_allocated_polygon(::apollo::hdmap::Polygon* polygon);

  // repeated .apollo.hdmap.Id overlap_id = 3;
  int overlap_id_size() const;
  void clear_overlap_id();
  static const int kOverlapIdFieldNumber = 3;
  const ::apollo::hdmap::Id& overlap_id(int index) const;
  ::apollo::hdmap::Id* mutable_overlap_id(int index);
  ::apollo::hdmap::Id* add_overlap_id();
  ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >*
      mutable_overlap_id();
  const ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >&
      overlap_id() const;

  // optional double heading = 4;
  bool has_heading() const;
  void clear_heading();
  static const int kHeadingFieldNumber = 4;
  double heading() const;
  void set_heading(double value);

  // @@protoc_insertion_point(class_scope:apollo.hdmap.ParkingSpace)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_polygon();
  inline void clear_has_polygon();
  inline void set_has_heading();
  inline void clear_has_heading();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::apollo::hdmap::Id* id_;
  ::apollo::hdmap::Polygon* polygon_;
  ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id > overlap_id_;
  double heading_;
  friend void  protobuf_AddDesc_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();
  friend void protobuf_AssignDesc_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();
  friend void protobuf_ShutdownFile_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();

  void InitAsDefaultInstance();
  static ParkingSpace* default_instance_;
};
// -------------------------------------------------------------------

class ParkingLot : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:apollo.hdmap.ParkingLot) */ {
 public:
  ParkingLot();
  virtual ~ParkingLot();

  ParkingLot(const ParkingLot& from);

  inline ParkingLot& operator=(const ParkingLot& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ParkingLot& default_instance();

  void Swap(ParkingLot* other);

  // implements Message ----------------------------------------------

  inline ParkingLot* New() const { return New(NULL); }

  ParkingLot* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ParkingLot& from);
  void MergeFrom(const ParkingLot& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(ParkingLot* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .apollo.hdmap.Id id = 1;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 1;
  const ::apollo::hdmap::Id& id() const;
  ::apollo::hdmap::Id* mutable_id();
  ::apollo::hdmap::Id* release_id();
  void set_allocated_id(::apollo::hdmap::Id* id);

  // optional .apollo.hdmap.Polygon polygon = 2;
  bool has_polygon() const;
  void clear_polygon();
  static const int kPolygonFieldNumber = 2;
  const ::apollo::hdmap::Polygon& polygon() const;
  ::apollo::hdmap::Polygon* mutable_polygon();
  ::apollo::hdmap::Polygon* release_polygon();
  void set_allocated_polygon(::apollo::hdmap::Polygon* polygon);

  // repeated .apollo.hdmap.Id overlap_id = 3;
  int overlap_id_size() const;
  void clear_overlap_id();
  static const int kOverlapIdFieldNumber = 3;
  const ::apollo::hdmap::Id& overlap_id(int index) const;
  ::apollo::hdmap::Id* mutable_overlap_id(int index);
  ::apollo::hdmap::Id* add_overlap_id();
  ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >*
      mutable_overlap_id();
  const ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >&
      overlap_id() const;

  // @@protoc_insertion_point(class_scope:apollo.hdmap.ParkingLot)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_polygon();
  inline void clear_has_polygon();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::apollo::hdmap::Id* id_;
  ::apollo::hdmap::Polygon* polygon_;
  ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id > overlap_id_;
  friend void  protobuf_AddDesc_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();
  friend void protobuf_AssignDesc_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();
  friend void protobuf_ShutdownFile_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto();

  void InitAsDefaultInstance();
  static ParkingLot* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// ParkingSpace

// optional .apollo.hdmap.Id id = 1;
inline bool ParkingSpace::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ParkingSpace::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ParkingSpace::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ParkingSpace::clear_id() {
  if (id_ != NULL) id_->::apollo::hdmap::Id::Clear();
  clear_has_id();
}
inline const ::apollo::hdmap::Id& ParkingSpace::id() const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.ParkingSpace.id)
  return id_ != NULL ? *id_ : *default_instance_->id_;
}
inline ::apollo::hdmap::Id* ParkingSpace::mutable_id() {
  set_has_id();
  if (id_ == NULL) {
    id_ = new ::apollo::hdmap::Id;
  }
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.ParkingSpace.id)
  return id_;
}
inline ::apollo::hdmap::Id* ParkingSpace::release_id() {
  // @@protoc_insertion_point(field_release:apollo.hdmap.ParkingSpace.id)
  clear_has_id();
  ::apollo::hdmap::Id* temp = id_;
  id_ = NULL;
  return temp;
}
inline void ParkingSpace::set_allocated_id(::apollo::hdmap::Id* id) {
  delete id_;
  id_ = id;
  if (id) {
    set_has_id();
  } else {
    clear_has_id();
  }
  // @@protoc_insertion_point(field_set_allocated:apollo.hdmap.ParkingSpace.id)
}

// optional .apollo.hdmap.Polygon polygon = 2;
inline bool ParkingSpace::has_polygon() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ParkingSpace::set_has_polygon() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ParkingSpace::clear_has_polygon() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ParkingSpace::clear_polygon() {
  if (polygon_ != NULL) polygon_->::apollo::hdmap::Polygon::Clear();
  clear_has_polygon();
}
inline const ::apollo::hdmap::Polygon& ParkingSpace::polygon() const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.ParkingSpace.polygon)
  return polygon_ != NULL ? *polygon_ : *default_instance_->polygon_;
}
inline ::apollo::hdmap::Polygon* ParkingSpace::mutable_polygon() {
  set_has_polygon();
  if (polygon_ == NULL) {
    polygon_ = new ::apollo::hdmap::Polygon;
  }
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.ParkingSpace.polygon)
  return polygon_;
}
inline ::apollo::hdmap::Polygon* ParkingSpace::release_polygon() {
  // @@protoc_insertion_point(field_release:apollo.hdmap.ParkingSpace.polygon)
  clear_has_polygon();
  ::apollo::hdmap::Polygon* temp = polygon_;
  polygon_ = NULL;
  return temp;
}
inline void ParkingSpace::set_allocated_polygon(::apollo::hdmap::Polygon* polygon) {
  delete polygon_;
  polygon_ = polygon;
  if (polygon) {
    set_has_polygon();
  } else {
    clear_has_polygon();
  }
  // @@protoc_insertion_point(field_set_allocated:apollo.hdmap.ParkingSpace.polygon)
}

// repeated .apollo.hdmap.Id overlap_id = 3;
inline int ParkingSpace::overlap_id_size() const {
  return overlap_id_.size();
}
inline void ParkingSpace::clear_overlap_id() {
  overlap_id_.Clear();
}
inline const ::apollo::hdmap::Id& ParkingSpace::overlap_id(int index) const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.ParkingSpace.overlap_id)
  return overlap_id_.Get(index);
}
inline ::apollo::hdmap::Id* ParkingSpace::mutable_overlap_id(int index) {
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.ParkingSpace.overlap_id)
  return overlap_id_.Mutable(index);
}
inline ::apollo::hdmap::Id* ParkingSpace::add_overlap_id() {
  // @@protoc_insertion_point(field_add:apollo.hdmap.ParkingSpace.overlap_id)
  return overlap_id_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >*
ParkingSpace::mutable_overlap_id() {
  // @@protoc_insertion_point(field_mutable_list:apollo.hdmap.ParkingSpace.overlap_id)
  return &overlap_id_;
}
inline const ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >&
ParkingSpace::overlap_id() const {
  // @@protoc_insertion_point(field_list:apollo.hdmap.ParkingSpace.overlap_id)
  return overlap_id_;
}

// optional double heading = 4;
inline bool ParkingSpace::has_heading() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ParkingSpace::set_has_heading() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ParkingSpace::clear_has_heading() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ParkingSpace::clear_heading() {
  heading_ = 0;
  clear_has_heading();
}
inline double ParkingSpace::heading() const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.ParkingSpace.heading)
  return heading_;
}
inline void ParkingSpace::set_heading(double value) {
  set_has_heading();
  heading_ = value;
  // @@protoc_insertion_point(field_set:apollo.hdmap.ParkingSpace.heading)
}

// -------------------------------------------------------------------

// ParkingLot

// optional .apollo.hdmap.Id id = 1;
inline bool ParkingLot::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ParkingLot::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ParkingLot::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ParkingLot::clear_id() {
  if (id_ != NULL) id_->::apollo::hdmap::Id::Clear();
  clear_has_id();
}
inline const ::apollo::hdmap::Id& ParkingLot::id() const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.ParkingLot.id)
  return id_ != NULL ? *id_ : *default_instance_->id_;
}
inline ::apollo::hdmap::Id* ParkingLot::mutable_id() {
  set_has_id();
  if (id_ == NULL) {
    id_ = new ::apollo::hdmap::Id;
  }
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.ParkingLot.id)
  return id_;
}
inline ::apollo::hdmap::Id* ParkingLot::release_id() {
  // @@protoc_insertion_point(field_release:apollo.hdmap.ParkingLot.id)
  clear_has_id();
  ::apollo::hdmap::Id* temp = id_;
  id_ = NULL;
  return temp;
}
inline void ParkingLot::set_allocated_id(::apollo::hdmap::Id* id) {
  delete id_;
  id_ = id;
  if (id) {
    set_has_id();
  } else {
    clear_has_id();
  }
  // @@protoc_insertion_point(field_set_allocated:apollo.hdmap.ParkingLot.id)
}

// optional .apollo.hdmap.Polygon polygon = 2;
inline bool ParkingLot::has_polygon() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ParkingLot::set_has_polygon() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ParkingLot::clear_has_polygon() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ParkingLot::clear_polygon() {
  if (polygon_ != NULL) polygon_->::apollo::hdmap::Polygon::Clear();
  clear_has_polygon();
}
inline const ::apollo::hdmap::Polygon& ParkingLot::polygon() const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.ParkingLot.polygon)
  return polygon_ != NULL ? *polygon_ : *default_instance_->polygon_;
}
inline ::apollo::hdmap::Polygon* ParkingLot::mutable_polygon() {
  set_has_polygon();
  if (polygon_ == NULL) {
    polygon_ = new ::apollo::hdmap::Polygon;
  }
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.ParkingLot.polygon)
  return polygon_;
}
inline ::apollo::hdmap::Polygon* ParkingLot::release_polygon() {
  // @@protoc_insertion_point(field_release:apollo.hdmap.ParkingLot.polygon)
  clear_has_polygon();
  ::apollo::hdmap::Polygon* temp = polygon_;
  polygon_ = NULL;
  return temp;
}
inline void ParkingLot::set_allocated_polygon(::apollo::hdmap::Polygon* polygon) {
  delete polygon_;
  polygon_ = polygon;
  if (polygon) {
    set_has_polygon();
  } else {
    clear_has_polygon();
  }
  // @@protoc_insertion_point(field_set_allocated:apollo.hdmap.ParkingLot.polygon)
}

// repeated .apollo.hdmap.Id overlap_id = 3;
inline int ParkingLot::overlap_id_size() const {
  return overlap_id_.size();
}
inline void ParkingLot::clear_overlap_id() {
  overlap_id_.Clear();
}
inline const ::apollo::hdmap::Id& ParkingLot::overlap_id(int index) const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.ParkingLot.overlap_id)
  return overlap_id_.Get(index);
}
inline ::apollo::hdmap::Id* ParkingLot::mutable_overlap_id(int index) {
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.ParkingLot.overlap_id)
  return overlap_id_.Mutable(index);
}
inline ::apollo::hdmap::Id* ParkingLot::add_overlap_id() {
  // @@protoc_insertion_point(field_add:apollo.hdmap.ParkingLot.overlap_id)
  return overlap_id_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >*
ParkingLot::mutable_overlap_id() {
  // @@protoc_insertion_point(field_mutable_list:apollo.hdmap.ParkingLot.overlap_id)
  return &overlap_id_;
}
inline const ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >&
ParkingLot::overlap_id() const {
  // @@protoc_insertion_point(field_list:apollo.hdmap.ParkingLot.overlap_id)
  return overlap_id_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace hdmap
}  // namespace apollo

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_modules_2fmap_2fproto_2fmap_5fparking_5fspace_2eproto__INCLUDED
