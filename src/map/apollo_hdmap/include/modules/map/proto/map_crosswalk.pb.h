// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modules/map/proto/map_crosswalk.proto

#ifndef PROTOBUF_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto__INCLUDED
#define PROTOBUF_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto__INCLUDED

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
void protobuf_AddDesc_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto();
void protobuf_AssignDesc_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto();
void protobuf_ShutdownFile_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto();

class Crosswalk;

// ===================================================================

class Crosswalk : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:apollo.hdmap.Crosswalk) */ {
 public:
  Crosswalk();
  virtual ~Crosswalk();

  Crosswalk(const Crosswalk& from);

  inline Crosswalk& operator=(const Crosswalk& from) {
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
  static const Crosswalk& default_instance();

  void Swap(Crosswalk* other);

  // implements Message ----------------------------------------------

  inline Crosswalk* New() const { return New(NULL); }

  Crosswalk* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Crosswalk& from);
  void MergeFrom(const Crosswalk& from);
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
  void InternalSwap(Crosswalk* other);
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

  // @@protoc_insertion_point(class_scope:apollo.hdmap.Crosswalk)
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
  friend void  protobuf_AddDesc_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto();
  friend void protobuf_AssignDesc_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto();
  friend void protobuf_ShutdownFile_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto();

  void InitAsDefaultInstance();
  static Crosswalk* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Crosswalk

// optional .apollo.hdmap.Id id = 1;
inline bool Crosswalk::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Crosswalk::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Crosswalk::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Crosswalk::clear_id() {
  if (id_ != NULL) id_->::apollo::hdmap::Id::Clear();
  clear_has_id();
}
inline const ::apollo::hdmap::Id& Crosswalk::id() const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.Crosswalk.id)
  return id_ != NULL ? *id_ : *default_instance_->id_;
}
inline ::apollo::hdmap::Id* Crosswalk::mutable_id() {
  set_has_id();
  if (id_ == NULL) {
    id_ = new ::apollo::hdmap::Id;
  }
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.Crosswalk.id)
  return id_;
}
inline ::apollo::hdmap::Id* Crosswalk::release_id() {
  // @@protoc_insertion_point(field_release:apollo.hdmap.Crosswalk.id)
  clear_has_id();
  ::apollo::hdmap::Id* temp = id_;
  id_ = NULL;
  return temp;
}
inline void Crosswalk::set_allocated_id(::apollo::hdmap::Id* id) {
  delete id_;
  id_ = id;
  if (id) {
    set_has_id();
  } else {
    clear_has_id();
  }
  // @@protoc_insertion_point(field_set_allocated:apollo.hdmap.Crosswalk.id)
}

// optional .apollo.hdmap.Polygon polygon = 2;
inline bool Crosswalk::has_polygon() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Crosswalk::set_has_polygon() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Crosswalk::clear_has_polygon() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Crosswalk::clear_polygon() {
  if (polygon_ != NULL) polygon_->::apollo::hdmap::Polygon::Clear();
  clear_has_polygon();
}
inline const ::apollo::hdmap::Polygon& Crosswalk::polygon() const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.Crosswalk.polygon)
  return polygon_ != NULL ? *polygon_ : *default_instance_->polygon_;
}
inline ::apollo::hdmap::Polygon* Crosswalk::mutable_polygon() {
  set_has_polygon();
  if (polygon_ == NULL) {
    polygon_ = new ::apollo::hdmap::Polygon;
  }
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.Crosswalk.polygon)
  return polygon_;
}
inline ::apollo::hdmap::Polygon* Crosswalk::release_polygon() {
  // @@protoc_insertion_point(field_release:apollo.hdmap.Crosswalk.polygon)
  clear_has_polygon();
  ::apollo::hdmap::Polygon* temp = polygon_;
  polygon_ = NULL;
  return temp;
}
inline void Crosswalk::set_allocated_polygon(::apollo::hdmap::Polygon* polygon) {
  delete polygon_;
  polygon_ = polygon;
  if (polygon) {
    set_has_polygon();
  } else {
    clear_has_polygon();
  }
  // @@protoc_insertion_point(field_set_allocated:apollo.hdmap.Crosswalk.polygon)
}

// repeated .apollo.hdmap.Id overlap_id = 3;
inline int Crosswalk::overlap_id_size() const {
  return overlap_id_.size();
}
inline void Crosswalk::clear_overlap_id() {
  overlap_id_.Clear();
}
inline const ::apollo::hdmap::Id& Crosswalk::overlap_id(int index) const {
  // @@protoc_insertion_point(field_get:apollo.hdmap.Crosswalk.overlap_id)
  return overlap_id_.Get(index);
}
inline ::apollo::hdmap::Id* Crosswalk::mutable_overlap_id(int index) {
  // @@protoc_insertion_point(field_mutable:apollo.hdmap.Crosswalk.overlap_id)
  return overlap_id_.Mutable(index);
}
inline ::apollo::hdmap::Id* Crosswalk::add_overlap_id() {
  // @@protoc_insertion_point(field_add:apollo.hdmap.Crosswalk.overlap_id)
  return overlap_id_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >*
Crosswalk::mutable_overlap_id() {
  // @@protoc_insertion_point(field_mutable_list:apollo.hdmap.Crosswalk.overlap_id)
  return &overlap_id_;
}
inline const ::google::protobuf::RepeatedPtrField< ::apollo::hdmap::Id >&
Crosswalk::overlap_id() const {
  // @@protoc_insertion_point(field_list:apollo.hdmap.Crosswalk.overlap_id)
  return overlap_id_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace hdmap
}  // namespace apollo

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_modules_2fmap_2fproto_2fmap_5fcrosswalk_2eproto__INCLUDED
