#include <cstring>
#include "simple_lzw.h"

using namespace simple_lzw;


void RingBuf::advance_front(int step=0)
{
  front_ = (front_ + step) % capacity_;
  size_ += step;
}


void RingBuf::advance_back(int step=0)
{
  back_ = (back_ + step) % capacity_;
  size_ -= step;
}


int RingBuf::add(uint8 ch)
{
  if (is_full()) {
    return 0;
  }

  buf_[front_] = ch;
  advance_front();
  return 1;
}


long RingBuf::add(const uint8 *buf, long len)
{
  if (is_full()) {
    return 0;
  }

  long avail = available();
  auto actual = (avail <= len ? avail : len);
  auto part_one = capacity_ - front_;

  if (part_one >= actual) {
    memcpy(buf_ + front_, buf, actual);
  } else {
    memcpy(buf_ + front_, buf, part_one);
    auto part_two = actual - part_one;
    memcpy(buf_, buf+part_one, part_two);
  }
  advance_front(actual);
  return actual;
}


long RingBuf::remove(long len)
{
  if (is_empty()) {
    return 0;
  }

  long actual = (size_ <= len ? size_ : len);
  advance_back(actual);
  return actual;
}
