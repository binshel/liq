// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test2.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "test2.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace test2 {

namespace {

const ::google::protobuf::Descriptor* A_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  A_reflection_ = NULL;
const ::google::protobuf::Descriptor* SearchRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SearchRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* SearchResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SearchResponse_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_test2_2eproto() {
  protobuf_AddDesc_test2_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "test2.proto");
  GOOGLE_CHECK(file != NULL);
  A_descriptor_ = file->message_type(0);
  static const int A_offsets_[1] = {
  };
  A_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      A_descriptor_,
      A::default_instance_,
      A_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(A, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(A, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(A));
  SearchRequest_descriptor_ = file->message_type(1);
  static const int SearchRequest_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchRequest, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchRequest, a_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchRequest, num_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchRequest, nums_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchRequest, strs_),
  };
  SearchRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SearchRequest_descriptor_,
      SearchRequest::default_instance_,
      SearchRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SearchRequest));
  SearchResponse_descriptor_ = file->message_type(2);
  static const int SearchResponse_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchResponse, reply_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchResponse, a_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchResponse, num_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchResponse, nums_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchResponse, strs_),
  };
  SearchResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SearchResponse_descriptor_,
      SearchResponse::default_instance_,
      SearchResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SearchResponse));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_test2_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    A_descriptor_, &A::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SearchRequest_descriptor_, &SearchRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SearchResponse_descriptor_, &SearchResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_test2_2eproto() {
  delete A::default_instance_;
  delete A_reflection_;
  delete SearchRequest::default_instance_;
  delete SearchRequest_reflection_;
  delete SearchResponse::default_instance_;
  delete SearchResponse_reflection_;
}

void protobuf_AddDesc_test2_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013test2.proto\022\005test2\"\003\n\001A\"[\n\rSearchReque"
    "st\022\014\n\004name\030\001 \002(\t\022\023\n\001a\030\002 \002(\0132\010.test2.A\022\013\n"
    "\003num\030\003 \002(\005\022\014\n\004nums\030\004 \003(\005\022\014\n\004strs\030\005 \003(\t\"]"
    "\n\016SearchResponse\022\r\n\005reply\030\001 \002(\t\022\023\n\001a\030\002 \002"
    "(\0132\010.test2.A\022\013\n\003num\030\003 \002(\005\022\014\n\004nums\030\004 \003(\005\022"
    "\014\n\004strs\030\005 \003(\t2\?\n\006Search\0225\n\006search\022\024.test"
    "2.SearchRequest\032\025.test2.SearchResponse", 278);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "test2.proto", &protobuf_RegisterTypes);
  A::default_instance_ = new A();
  SearchRequest::default_instance_ = new SearchRequest();
  SearchResponse::default_instance_ = new SearchResponse();
  A::default_instance_->InitAsDefaultInstance();
  SearchRequest::default_instance_->InitAsDefaultInstance();
  SearchResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_test2_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_test2_2eproto {
  StaticDescriptorInitializer_test2_2eproto() {
    protobuf_AddDesc_test2_2eproto();
  }
} static_descriptor_initializer_test2_2eproto_;

// ===================================================================

#ifndef _MSC_VER
#endif  // !_MSC_VER

A::A()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void A::InitAsDefaultInstance() {
}

A::A(const A& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void A::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

A::~A() {
  SharedDtor();
}

void A::SharedDtor() {
  if (this != default_instance_) {
  }
}

void A::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* A::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return A_descriptor_;
}

const A& A::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_test2_2eproto();
  return *default_instance_;
}

A* A::default_instance_ = NULL;

A* A::New() const {
  return new A;
}

void A::Clear() {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool A::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
        ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
      return true;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, mutable_unknown_fields()));
  }
  return true;
#undef DO_
}

void A::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* A::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int A::ByteSize() const {
  int total_size = 0;

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void A::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const A* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const A*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void A::MergeFrom(const A& from) {
  GOOGLE_CHECK_NE(&from, this);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void A::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void A::CopyFrom(const A& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool A::IsInitialized() const {

  return true;
}

void A::Swap(A* other) {
  if (other != this) {
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata A::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = A_descriptor_;
  metadata.reflection = A_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int SearchRequest::kNameFieldNumber;
const int SearchRequest::kAFieldNumber;
const int SearchRequest::kNumFieldNumber;
const int SearchRequest::kNumsFieldNumber;
const int SearchRequest::kStrsFieldNumber;
#endif  // !_MSC_VER

SearchRequest::SearchRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void SearchRequest::InitAsDefaultInstance() {
  a_ = const_cast< ::test2::A*>(&::test2::A::default_instance());
}

SearchRequest::SearchRequest(const SearchRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void SearchRequest::SharedCtor() {
  _cached_size_ = 0;
  name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  a_ = NULL;
  num_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SearchRequest::~SearchRequest() {
  SharedDtor();
}

void SearchRequest::SharedDtor() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (this != default_instance_) {
    delete a_;
  }
}

void SearchRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SearchRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SearchRequest_descriptor_;
}

const SearchRequest& SearchRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_test2_2eproto();
  return *default_instance_;
}

SearchRequest* SearchRequest::default_instance_ = NULL;

SearchRequest* SearchRequest::New() const {
  return new SearchRequest;
}

void SearchRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_name()) {
      if (name_ != &::google::protobuf::internal::kEmptyString) {
        name_->clear();
      }
    }
    if (has_a()) {
      if (a_ != NULL) a_->::test2::A::Clear();
    }
    num_ = 0;
  }
  nums_.Clear();
  strs_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SearchRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string name = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_a;
        break;
      }

      // required .test2.A a = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_a:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_a()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_num;
        break;
      }

      // required int32 num = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_num:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_)));
          set_has_num();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_nums;
        break;
      }

      // repeated int32 nums = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nums:
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 1, 32, input, this->mutable_nums())));
        } else if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag)
                   == ::google::protobuf::internal::WireFormatLite::
                      WIRETYPE_LENGTH_DELIMITED) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitiveNoInline<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, this->mutable_nums())));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_nums;
        if (input->ExpectTag(42)) goto parse_strs;
        break;
      }

      // repeated string strs = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_strs:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_strs()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->strs(this->strs_size() - 1).data(),
            this->strs(this->strs_size() - 1).length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_strs;
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SearchRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string name = 1;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->name(), output);
  }

  // required .test2.A a = 2;
  if (has_a()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->a(), output);
  }

  // required int32 num = 3;
  if (has_num()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->num(), output);
  }

  // repeated int32 nums = 4;
  for (int i = 0; i < this->nums_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(
      4, this->nums(i), output);
  }

  // repeated string strs = 5;
  for (int i = 0; i < this->strs_size(); i++) {
  ::google::protobuf::internal::WireFormat::VerifyUTF8String(
    this->strs(i).data(), this->strs(i).length(),
    ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->strs(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SearchRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string name = 1;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }

  // required .test2.A a = 2;
  if (has_a()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->a(), target);
  }

  // required int32 num = 3;
  if (has_num()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->num(), target);
  }

  // repeated int32 nums = 4;
  for (int i = 0; i < this->nums_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteInt32ToArray(4, this->nums(i), target);
  }

  // repeated string strs = 5;
  for (int i = 0; i < this->strs_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->strs(i).data(), this->strs(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(5, this->strs(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SearchRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string name = 1;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // required .test2.A a = 2;
    if (has_a()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->a());
    }

    // required int32 num = 3;
    if (has_num()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->num());
    }

  }
  // repeated int32 nums = 4;
  {
    int data_size = 0;
    for (int i = 0; i < this->nums_size(); i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::
        Int32Size(this->nums(i));
    }
    total_size += 1 * this->nums_size() + data_size;
  }

  // repeated string strs = 5;
  total_size += 1 * this->strs_size();
  for (int i = 0; i < this->strs_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->strs(i));
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SearchRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SearchRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SearchRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SearchRequest::MergeFrom(const SearchRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  nums_.MergeFrom(from.nums_);
  strs_.MergeFrom(from.strs_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_name()) {
      set_name(from.name());
    }
    if (from.has_a()) {
      mutable_a()->::test2::A::MergeFrom(from.a());
    }
    if (from.has_num()) {
      set_num(from.num());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SearchRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SearchRequest::CopyFrom(const SearchRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SearchRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void SearchRequest::Swap(SearchRequest* other) {
  if (other != this) {
    std::swap(name_, other->name_);
    std::swap(a_, other->a_);
    std::swap(num_, other->num_);
    nums_.Swap(&other->nums_);
    strs_.Swap(&other->strs_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SearchRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SearchRequest_descriptor_;
  metadata.reflection = SearchRequest_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int SearchResponse::kReplyFieldNumber;
const int SearchResponse::kAFieldNumber;
const int SearchResponse::kNumFieldNumber;
const int SearchResponse::kNumsFieldNumber;
const int SearchResponse::kStrsFieldNumber;
#endif  // !_MSC_VER

SearchResponse::SearchResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void SearchResponse::InitAsDefaultInstance() {
  a_ = const_cast< ::test2::A*>(&::test2::A::default_instance());
}

SearchResponse::SearchResponse(const SearchResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void SearchResponse::SharedCtor() {
  _cached_size_ = 0;
  reply_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  a_ = NULL;
  num_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SearchResponse::~SearchResponse() {
  SharedDtor();
}

void SearchResponse::SharedDtor() {
  if (reply_ != &::google::protobuf::internal::kEmptyString) {
    delete reply_;
  }
  if (this != default_instance_) {
    delete a_;
  }
}

void SearchResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SearchResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SearchResponse_descriptor_;
}

const SearchResponse& SearchResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_test2_2eproto();
  return *default_instance_;
}

SearchResponse* SearchResponse::default_instance_ = NULL;

SearchResponse* SearchResponse::New() const {
  return new SearchResponse;
}

void SearchResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_reply()) {
      if (reply_ != &::google::protobuf::internal::kEmptyString) {
        reply_->clear();
      }
    }
    if (has_a()) {
      if (a_ != NULL) a_->::test2::A::Clear();
    }
    num_ = 0;
  }
  nums_.Clear();
  strs_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SearchResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string reply = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_reply()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->reply().data(), this->reply().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_a;
        break;
      }

      // required .test2.A a = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_a:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_a()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_num;
        break;
      }

      // required int32 num = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_num:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_)));
          set_has_num();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_nums;
        break;
      }

      // repeated int32 nums = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nums:
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 1, 32, input, this->mutable_nums())));
        } else if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag)
                   == ::google::protobuf::internal::WireFormatLite::
                      WIRETYPE_LENGTH_DELIMITED) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitiveNoInline<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, this->mutable_nums())));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_nums;
        if (input->ExpectTag(42)) goto parse_strs;
        break;
      }

      // repeated string strs = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_strs:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_strs()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->strs(this->strs_size() - 1).data(),
            this->strs(this->strs_size() - 1).length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_strs;
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SearchResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string reply = 1;
  if (has_reply()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->reply().data(), this->reply().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->reply(), output);
  }

  // required .test2.A a = 2;
  if (has_a()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->a(), output);
  }

  // required int32 num = 3;
  if (has_num()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->num(), output);
  }

  // repeated int32 nums = 4;
  for (int i = 0; i < this->nums_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(
      4, this->nums(i), output);
  }

  // repeated string strs = 5;
  for (int i = 0; i < this->strs_size(); i++) {
  ::google::protobuf::internal::WireFormat::VerifyUTF8String(
    this->strs(i).data(), this->strs(i).length(),
    ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->strs(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SearchResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string reply = 1;
  if (has_reply()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->reply().data(), this->reply().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->reply(), target);
  }

  // required .test2.A a = 2;
  if (has_a()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->a(), target);
  }

  // required int32 num = 3;
  if (has_num()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->num(), target);
  }

  // repeated int32 nums = 4;
  for (int i = 0; i < this->nums_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteInt32ToArray(4, this->nums(i), target);
  }

  // repeated string strs = 5;
  for (int i = 0; i < this->strs_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->strs(i).data(), this->strs(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(5, this->strs(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SearchResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string reply = 1;
    if (has_reply()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->reply());
    }

    // required .test2.A a = 2;
    if (has_a()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->a());
    }

    // required int32 num = 3;
    if (has_num()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->num());
    }

  }
  // repeated int32 nums = 4;
  {
    int data_size = 0;
    for (int i = 0; i < this->nums_size(); i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::
        Int32Size(this->nums(i));
    }
    total_size += 1 * this->nums_size() + data_size;
  }

  // repeated string strs = 5;
  total_size += 1 * this->strs_size();
  for (int i = 0; i < this->strs_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->strs(i));
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SearchResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SearchResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SearchResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SearchResponse::MergeFrom(const SearchResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  nums_.MergeFrom(from.nums_);
  strs_.MergeFrom(from.strs_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_reply()) {
      set_reply(from.reply());
    }
    if (from.has_a()) {
      mutable_a()->::test2::A::MergeFrom(from.a());
    }
    if (from.has_num()) {
      set_num(from.num());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SearchResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SearchResponse::CopyFrom(const SearchResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SearchResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void SearchResponse::Swap(SearchResponse* other) {
  if (other != this) {
    std::swap(reply_, other->reply_);
    std::swap(a_, other->a_);
    std::swap(num_, other->num_);
    nums_.Swap(&other->nums_);
    strs_.Swap(&other->strs_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SearchResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SearchResponse_descriptor_;
  metadata.reflection = SearchResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace test2

// @@protoc_insertion_point(global_scope)
