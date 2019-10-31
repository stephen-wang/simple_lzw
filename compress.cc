#include <algorithm>
#include <cstring>

#include "compress.h"


using namespace easy_lzw;


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


int Dict::get_code(const uint8* data, int len)
{
  for(auto i=0; i < buf_.size(); i++) {
    if (buf[i].equal_to(data, len)) {
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

  std::lock_guard<mutex> _lock(mutex);
  buf_.emplace_back(data, len);
}

