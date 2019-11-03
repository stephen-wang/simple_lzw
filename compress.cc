#include <algorithm>
#include <cstring>
#include <fstream>

#include "simple_lzw.h"

using namespace simple_lzw;


RawData::RawData(uint8 ch)
{
  len_ = 1;
  buf_ = std::make_shared<uint8>(len_); *buf_ = ch;
}



RawData::RawData(const uint8 *data, int len)
{
  len_ = len;
  buf_ = std::make_shared<uint8>(len_);
  memcpy(buf_.get(), data, len_);
}


void RawData::reset(const uint8* data, int len)
{
  if (len_ != len) {
    buf_ = std::make_shared<uint8>(len);
  }

  memcpy(buf_.get(), data, len);
}


bool RawData::equal_to(const uint8* data, int len)
{
  if (len_ != len) {
    return false;
  }

  return (memcmp(buf_.get(), data, len_) == 0);
}


bool RawData::operator==(const RawData &rhs)
{
  if (&rhs == this || (rhs.len_ == len_ && rhs.buf_ == buf_)) {
    return true;
  } else if (rhs.len_ != len_) {
    return false;
  }

  return (memcpy(buf_.get(), rhs.buf_.get(), len_) == 0);
}


int Dict::find(const uint8* data, int len)
{
  for(auto i = 0; i < buf_.size(); i++) {
    if (buf_[i].equal_to(data, len)) {
      return i;
    }
  }

  return -1;
}


bool Dict::add(const uint8* data, int len)
{
  if (buf_.size() >= size_) {  
    return false;
  }

  std::lock_guard<std::mutex> _lock(mutex_);
  buf_.emplace_back(data, len);
}


void Dict::init()
{
  char digits[] = "0123456789";
  char symbols[] = "+-*/=<>!~|\\()[]{}_,:;.'\"^%$#@ \t\n";
  char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  std::lock_guard<std::mutex> lock(mutex_);

  for (auto i = 0; i < strlen(digits); i++) {
    buf_.emplace_back(uint8(digits[i]));
  }

  for (auto i = 0; i < strlen(digits); i++) {
    buf_.emplace_back(uint8(symbols[i]));
  }

  for (auto i = 0; i < strlen(letters); i++) {
    buf_.emplace_back(uint8(letters[i]));
  }
}


bool Compression::compress(const std::string& in_file,
                           const std::string& out_file)
{
  BinFile ibf(in_file, 'r');
  BinFile obf(out_file, 'w');
  RingBuf rb(4096);
  //CodeBuf cb(100)
  long actual = 0;

  while ((actual = ibf.read(rb)) > 0) {
    auto pos = 0;
    auto size = rb.size();
    while (pos < size) {
      /// FIXME: ringbuf fallback case
      //if (dict_.find(rb.back(), pos) == -1) {
      //  break;
      //}

      pos++;;
    }

    if (pos < size) {
    } else {
    }
  }
}
