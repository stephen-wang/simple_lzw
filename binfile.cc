#include "simple_lzw.h"

using namespace simple_lzw;

BinFile::BinFile(const std::string& filename, char mode)
 : mode_(mode)
{
  if (mode_ == 'r') {
    fs_.open(filename, std::ios::in|std::ios::binary);
    fs_.seekg(0, std::ios::end);
    size_ = fs_.tellg();
    fs_.seekg(0, std::ios::beg);
  } else {
    fs_.open(filename, std::ios::out|std::ios::binary);
    size_ = 0;
  }
}


BinFile::~BinFile()
{
  fs_.close();
}


long BinFile::read(RingBuf &rb)
{

  if (mode_ != 'r' || fs_.eof() || fs_.bad()) {
    return 0;
  }

  const int BUF_SIZE = 4096;
  auto buf_len = rb.available();
  uint8 buf[BUF_SIZE];

  assert(buf_len < BUF_SIZE);

  fs_.read((char *)buf, buf_len);

  auto actual = fs_.gcount();
  rb.add(buf, actual);
  return actual;
}
