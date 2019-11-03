#ifndef _SIMPLE_LZW_COMPRESS_H_
#define _SIMPLE_LZW_COMPRESS_H_

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>

namespace simple_lzw {

using uint8 = unsigned char;

template<typename T>
void LOG(T param)
{
    std::cout << param << std::endl;
}


template<typename T1, typename... T2>
void LOG(T1 param1, T2... param)
{
    std::cout << param1;
    LOG(std::forward<T2>(param)...);
}


class RingBuf {
public:
  RingBuf(long size)
    : front_(0), back_(size-1), size_(0), capacity_(size)
  { buf_ = new uint8(capacity_ + 1); }
  ~RingBuf() { delete [] buf_; }

  bool is_empty() { return front_ == back_; }
  bool is_full() { return ((front_ + 1) % size_ == back_); }

  long size() const { return size_; }
  long available() const { return capacity_ - 1 - size_; }
  const uint8* base() const { return buf_; }

  void advance_front(int);
  void advance_back(int);
  int add(uint8);
  long add(const uint8 *, long);
  long remove(long);

private:
  uint8* buf_;
  long front_;
  long back_;
  long size_;
  long capacity_;
};


class BinFile {
public:
  BinFile(const std::string&, char);
  ~BinFile();
  long read(RingBuf&);

private:
  char mode_;
  long size_;
  std::string file_path_;
  std::fstream fs_;
};


class RawData {
public:
  explicit RawData(int size);
  explicit RawData(uint8 ch);
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
  explicit Dict(int size=4096) : size_(size) {}
  ~Dict() { buf_.clear(); }

  int find(const uint8* data, int len) ;
  bool add(const uint8* data, int len);
  void init();

private:
  std::mutex mutex_;
  std::vector<RawData> buf_;
  int size_;
};


// compression class
class Compression {
public:
  bool compress(const std::string& in_file, const std::string& out_file);

private:
  Dict dict_;
};

}

#endif
