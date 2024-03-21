// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: radar.proto

#ifndef PROTOBUF_INCLUDED_radar_2eproto
#define PROTOBUF_INCLUDED_radar_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3006001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_radar_2eproto

// Internal implementation detail -- do not use these members.
struct TableStruct_radar_2eproto {
  static const ::google::protobuf::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors_radar_2eproto();
namespace radarPackage {
class Timestamp;
class TimestampDefaultTypeInternal;
extern TimestampDefaultTypeInternal _Timestamp_default_instance_;
class radarMsg;
class radarMsgDefaultTypeInternal;
extern radarMsgDefaultTypeInternal _radarMsg_default_instance_;
class targetMessage;
class targetMessageDefaultTypeInternal;
extern targetMessageDefaultTypeInternal _targetMessage_default_instance_;
}  // namespace radarPackage
namespace google {
namespace protobuf {
template<> ::radarPackage::Timestamp* Arena::CreateMaybeMessage<::radarPackage::Timestamp>(Arena*);
template<> ::radarPackage::radarMsg* Arena::CreateMaybeMessage<::radarPackage::radarMsg>(Arena*);
template<> ::radarPackage::targetMessage* Arena::CreateMaybeMessage<::radarPackage::targetMessage>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace radarPackage {

// ===================================================================

class targetMessage final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:radarPackage.targetMessage) */ {
 public:
  targetMessage();
  virtual ~targetMessage();

  targetMessage(const targetMessage& from);

  inline targetMessage& operator=(const targetMessage& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  targetMessage(targetMessage&& from) noexcept
    : targetMessage() {
    *this = ::std::move(from);
  }

  inline targetMessage& operator=(targetMessage&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const targetMessage& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const targetMessage* internal_default_instance() {
    return reinterpret_cast<const targetMessage*>(
               &_targetMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(targetMessage* other);
  friend void swap(targetMessage& a, targetMessage& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline targetMessage* New() const final {
    return CreateMaybeMessage<targetMessage>(nullptr);
  }

  targetMessage* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<targetMessage>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const targetMessage& from);
  void MergeFrom(const targetMessage& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(targetMessage* other);
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::google::protobuf::StringPiece FullMessageName() {
    return "radarPackage.targetMessage";
  }
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 id = 1;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 1;
  ::google::protobuf::uint32 id() const;
  void set_id(::google::protobuf::uint32 value);

  // required int32 longitudinalX = 2;
  bool has_longitudinalx() const;
  void clear_longitudinalx();
  static const int kLongitudinalXFieldNumber = 2;
  ::google::protobuf::int32 longitudinalx() const;
  void set_longitudinalx(::google::protobuf::int32 value);

  // required int32 lateralY = 3;
  bool has_lateraly() const;
  void clear_lateraly();
  static const int kLateralYFieldNumber = 3;
  ::google::protobuf::int32 lateraly() const;
  void set_lateraly(::google::protobuf::int32 value);

  // required int32 velocity = 4;
  bool has_velocity() const;
  void clear_velocity();
  static const int kVelocityFieldNumber = 4;
  ::google::protobuf::int32 velocity() const;
  void set_velocity(::google::protobuf::int32 value);

  // required uint32 heading = 5;
  bool has_heading() const;
  void clear_heading();
  static const int kHeadingFieldNumber = 5;
  ::google::protobuf::uint32 heading() const;
  void set_heading(::google::protobuf::uint32 value);

  // required int32 RCS = 6;
  bool has_rcs() const;
  void clear_rcs();
  static const int kRCSFieldNumber = 6;
  ::google::protobuf::int32 rcs() const;
  void set_rcs(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:radarPackage.targetMessage)
 private:
  class HasBitSetters;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::uint32 id_;
  ::google::protobuf::int32 longitudinalx_;
  ::google::protobuf::int32 lateraly_;
  ::google::protobuf::int32 velocity_;
  ::google::protobuf::uint32 heading_;
  ::google::protobuf::int32 rcs_;
  friend struct ::TableStruct_radar_2eproto;
};
// -------------------------------------------------------------------

class Timestamp final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:radarPackage.Timestamp) */ {
 public:
  Timestamp();
  virtual ~Timestamp();

  Timestamp(const Timestamp& from);

  inline Timestamp& operator=(const Timestamp& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Timestamp(Timestamp&& from) noexcept
    : Timestamp() {
    *this = ::std::move(from);
  }

  inline Timestamp& operator=(Timestamp&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const Timestamp& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Timestamp* internal_default_instance() {
    return reinterpret_cast<const Timestamp*>(
               &_Timestamp_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Timestamp* other);
  friend void swap(Timestamp& a, Timestamp& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Timestamp* New() const final {
    return CreateMaybeMessage<Timestamp>(nullptr);
  }

  Timestamp* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Timestamp>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Timestamp& from);
  void MergeFrom(const Timestamp& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Timestamp* other);
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::google::protobuf::StringPiece FullMessageName() {
    return "radarPackage.Timestamp";
  }
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int64 seconds = 1;
  bool has_seconds() const;
  void clear_seconds();
  static const int kSecondsFieldNumber = 1;
  ::google::protobuf::int64 seconds() const;
  void set_seconds(::google::protobuf::int64 value);

  // required int32 nanos = 2;
  bool has_nanos() const;
  void clear_nanos();
  static const int kNanosFieldNumber = 2;
  ::google::protobuf::int32 nanos() const;
  void set_nanos(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:radarPackage.Timestamp)
 private:
  class HasBitSetters;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::int64 seconds_;
  ::google::protobuf::int32 nanos_;
  friend struct ::TableStruct_radar_2eproto;
};
// -------------------------------------------------------------------

class radarMsg final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:radarPackage.radarMsg) */ {
 public:
  radarMsg();
  virtual ~radarMsg();

  radarMsg(const radarMsg& from);

  inline radarMsg& operator=(const radarMsg& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  radarMsg(radarMsg&& from) noexcept
    : radarMsg() {
    *this = ::std::move(from);
  }

  inline radarMsg& operator=(radarMsg&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const radarMsg& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const radarMsg* internal_default_instance() {
    return reinterpret_cast<const radarMsg*>(
               &_radarMsg_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(radarMsg* other);
  friend void swap(radarMsg& a, radarMsg& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline radarMsg* New() const final {
    return CreateMaybeMessage<radarMsg>(nullptr);
  }

  radarMsg* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<radarMsg>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const radarMsg& from);
  void MergeFrom(const radarMsg& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(radarMsg* other);
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::google::protobuf::StringPiece FullMessageName() {
    return "radarPackage.radarMsg";
  }
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .radarPackage.targetMessage msg = 2;
  int msg_size() const;
  void clear_msg();
  static const int kMsgFieldNumber = 2;
  ::radarPackage::targetMessage* mutable_msg(int index);
  ::google::protobuf::RepeatedPtrField< ::radarPackage::targetMessage >*
      mutable_msg();
  const ::radarPackage::targetMessage& msg(int index) const;
  ::radarPackage::targetMessage* add_msg();
  const ::google::protobuf::RepeatedPtrField< ::radarPackage::targetMessage >&
      msg() const;

  // required .radarPackage.Timestamp time = 1;
  bool has_time() const;
  void clear_time();
  static const int kTimeFieldNumber = 1;
  const ::radarPackage::Timestamp& time() const;
  ::radarPackage::Timestamp* release_time();
  ::radarPackage::Timestamp* mutable_time();
  void set_allocated_time(::radarPackage::Timestamp* time);

  // @@protoc_insertion_point(class_scope:radarPackage.radarMsg)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::radarPackage::targetMessage > msg_;
  ::radarPackage::Timestamp* time_;
  friend struct ::TableStruct_radar_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// targetMessage

// required uint32 id = 1;
inline bool targetMessage::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void targetMessage::clear_id() {
  id_ = 0u;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::google::protobuf::uint32 targetMessage::id() const {
  // @@protoc_insertion_point(field_get:radarPackage.targetMessage.id)
  return id_;
}
inline void targetMessage::set_id(::google::protobuf::uint32 value) {
  _has_bits_[0] |= 0x00000001u;
  id_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.targetMessage.id)
}

// required int32 longitudinalX = 2;
inline bool targetMessage::has_longitudinalx() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void targetMessage::clear_longitudinalx() {
  longitudinalx_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::google::protobuf::int32 targetMessage::longitudinalx() const {
  // @@protoc_insertion_point(field_get:radarPackage.targetMessage.longitudinalX)
  return longitudinalx_;
}
inline void targetMessage::set_longitudinalx(::google::protobuf::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  longitudinalx_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.targetMessage.longitudinalX)
}

// required int32 lateralY = 3;
inline bool targetMessage::has_lateraly() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void targetMessage::clear_lateraly() {
  lateraly_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::google::protobuf::int32 targetMessage::lateraly() const {
  // @@protoc_insertion_point(field_get:radarPackage.targetMessage.lateralY)
  return lateraly_;
}
inline void targetMessage::set_lateraly(::google::protobuf::int32 value) {
  _has_bits_[0] |= 0x00000004u;
  lateraly_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.targetMessage.lateralY)
}

// required int32 velocity = 4;
inline bool targetMessage::has_velocity() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void targetMessage::clear_velocity() {
  velocity_ = 0;
  _has_bits_[0] &= ~0x00000008u;
}
inline ::google::protobuf::int32 targetMessage::velocity() const {
  // @@protoc_insertion_point(field_get:radarPackage.targetMessage.velocity)
  return velocity_;
}
inline void targetMessage::set_velocity(::google::protobuf::int32 value) {
  _has_bits_[0] |= 0x00000008u;
  velocity_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.targetMessage.velocity)
}

// required uint32 heading = 5;
inline bool targetMessage::has_heading() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void targetMessage::clear_heading() {
  heading_ = 0u;
  _has_bits_[0] &= ~0x00000010u;
}
inline ::google::protobuf::uint32 targetMessage::heading() const {
  // @@protoc_insertion_point(field_get:radarPackage.targetMessage.heading)
  return heading_;
}
inline void targetMessage::set_heading(::google::protobuf::uint32 value) {
  _has_bits_[0] |= 0x00000010u;
  heading_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.targetMessage.heading)
}

// required int32 RCS = 6;
inline bool targetMessage::has_rcs() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void targetMessage::clear_rcs() {
  rcs_ = 0;
  _has_bits_[0] &= ~0x00000020u;
}
inline ::google::protobuf::int32 targetMessage::rcs() const {
  // @@protoc_insertion_point(field_get:radarPackage.targetMessage.RCS)
  return rcs_;
}
inline void targetMessage::set_rcs(::google::protobuf::int32 value) {
  _has_bits_[0] |= 0x00000020u;
  rcs_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.targetMessage.RCS)
}

// -------------------------------------------------------------------

// Timestamp

// required int64 seconds = 1;
inline bool Timestamp::has_seconds() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Timestamp::clear_seconds() {
  seconds_ = PROTOBUF_LONGLONG(0);
  _has_bits_[0] &= ~0x00000001u;
}
inline ::google::protobuf::int64 Timestamp::seconds() const {
  // @@protoc_insertion_point(field_get:radarPackage.Timestamp.seconds)
  return seconds_;
}
inline void Timestamp::set_seconds(::google::protobuf::int64 value) {
  _has_bits_[0] |= 0x00000001u;
  seconds_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.Timestamp.seconds)
}

// required int32 nanos = 2;
inline bool Timestamp::has_nanos() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Timestamp::clear_nanos() {
  nanos_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::google::protobuf::int32 Timestamp::nanos() const {
  // @@protoc_insertion_point(field_get:radarPackage.Timestamp.nanos)
  return nanos_;
}
inline void Timestamp::set_nanos(::google::protobuf::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  nanos_ = value;
  // @@protoc_insertion_point(field_set:radarPackage.Timestamp.nanos)
}

// -------------------------------------------------------------------

// radarMsg

// required .radarPackage.Timestamp time = 1;
inline bool radarMsg::has_time() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void radarMsg::clear_time() {
  if (time_ != nullptr) time_->Clear();
  _has_bits_[0] &= ~0x00000001u;
}
inline const ::radarPackage::Timestamp& radarMsg::time() const {
  const ::radarPackage::Timestamp* p = time_;
  // @@protoc_insertion_point(field_get:radarPackage.radarMsg.time)
  return p != nullptr ? *p : *reinterpret_cast<const ::radarPackage::Timestamp*>(
      &::radarPackage::_Timestamp_default_instance_);
}
inline ::radarPackage::Timestamp* radarMsg::release_time() {
  // @@protoc_insertion_point(field_release:radarPackage.radarMsg.time)
  _has_bits_[0] &= ~0x00000001u;
  ::radarPackage::Timestamp* temp = time_;
  time_ = nullptr;
  return temp;
}
inline ::radarPackage::Timestamp* radarMsg::mutable_time() {
  _has_bits_[0] |= 0x00000001u;
  if (time_ == nullptr) {
    auto* p = CreateMaybeMessage<::radarPackage::Timestamp>(GetArenaNoVirtual());
    time_ = p;
  }
  // @@protoc_insertion_point(field_mutable:radarPackage.radarMsg.time)
  return time_;
}
inline void radarMsg::set_allocated_time(::radarPackage::Timestamp* time) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete time_;
  }
  if (time) {
    ::google::protobuf::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      time = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, time, submessage_arena);
    }
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  time_ = time;
  // @@protoc_insertion_point(field_set_allocated:radarPackage.radarMsg.time)
}

// repeated .radarPackage.targetMessage msg = 2;
inline int radarMsg::msg_size() const {
  return msg_.size();
}
inline void radarMsg::clear_msg() {
  msg_.Clear();
}
inline ::radarPackage::targetMessage* radarMsg::mutable_msg(int index) {
  // @@protoc_insertion_point(field_mutable:radarPackage.radarMsg.msg)
  return msg_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::radarPackage::targetMessage >*
radarMsg::mutable_msg() {
  // @@protoc_insertion_point(field_mutable_list:radarPackage.radarMsg.msg)
  return &msg_;
}
inline const ::radarPackage::targetMessage& radarMsg::msg(int index) const {
  // @@protoc_insertion_point(field_get:radarPackage.radarMsg.msg)
  return msg_.Get(index);
}
inline ::radarPackage::targetMessage* radarMsg::add_msg() {
  // @@protoc_insertion_point(field_add:radarPackage.radarMsg.msg)
  return msg_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::radarPackage::targetMessage >&
radarMsg::msg() const {
  // @@protoc_insertion_point(field_list:radarPackage.radarMsg.msg)
  return msg_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace radarPackage

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // PROTOBUF_INCLUDED_radar_2eproto
