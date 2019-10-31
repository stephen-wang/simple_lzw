#ifndef _SIMPLE_LZW_COMPRESS_H_
#define _SIMPLE_LZW_COMPRESS_H_

#include <cassert>
#include <memory>
#include <mutex>
#include <vector>


namespace easy_lzw {
  using uint8 = unsigned char;

class RawData {
public:
  explicit RawData(int size);
  RawData(const uint8 *data, int len);

  int length() const { return len_; }
  uint8 get(int i) const {
    assert(i < len_);
    return buf_.get()[i];
  }
  void reset(const uint8 *, int len);
  bool equal_to(const uint8 *data, int len);
  bool operator==(const RawData&);

private:
  int len_;
  std::shared_ptr<uint8> buf_;
};


// compress/decompress dictionary
class Dict{
public:
  explicit Dict::Dict(int size=4096) : size_(size) {}
  ~Dict() { buf_.clear(); }

  int get_code(const uint8* data, int len) ;
  bool add(const uint8* data, int len);

private:
  std::mutex mutex_;
  std::vector<RawData> buf_;
  int size_;
};

}


#endif
