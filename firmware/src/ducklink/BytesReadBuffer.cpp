/*
 *  Copyright (C) 2020 Embedded AMS B.V. - All Rights Reserved
 *
 *  This file is part of Embedded Proto.
 *
 *  Embedded Proto is open source software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, version 3 of the license.
 *
 *  Embedded Proto  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Embedded Proto. If not, see <https://www.gnu.org/licenses/>.
 *
 *  For commercial and closed source application please visit:
 *  <https://EmbeddedProto.com/license/>.
 *
 *  Embedded AMS B.V.
 *  Info:
 *    info at EmbeddedProto dot com
 *
 *  Postal adress:
 *    Johan Huizingalaan 763a
 *    1066 VH, Amsterdam
 *    the Netherlands
 */

#include "BytesReadBuffer.h"

BytesReadBuffer::BytesReadBuffer()
  : data_{0},
    write_index_(0),
    read_index_(0)
{

}

uint32_t BytesReadBuffer::get_size() const
{
  return write_index_;
}

uint32_t BytesReadBuffer::get_max_size() const
{
  return MAX_SIZE;
}

bool BytesReadBuffer::peek(uint8_t& byte) const
{
  bool return_value = write_index_ > read_index_;
  if(return_value)
  {
    byte = data_[read_index_];
  }
  return return_value;
}

bool BytesReadBuffer::advance()
{
  bool return_value = write_index_ > read_index_;
  if(return_value)
  {
    ++read_index_;
  }
  return return_value;
}

bool BytesReadBuffer::advance(const uint32_t N)
{
  bool return_value = write_index_ >= (read_index_ + N);
  if(return_value)
  {
    read_index_ += N;
  }
  return return_value;
}

bool BytesReadBuffer::pop(uint8_t& byte)
{
  bool return_value = write_index_ > read_index_;
  if(return_value) {
    byte = data_[read_index_];
    ++read_index_;
  }
  return return_value;
}

uint8_t* BytesReadBuffer::get_data_array()
{
  return data_;
}

uint32_t& BytesReadBuffer::get_bytes_written()
{
  return write_index_;
}

void BytesReadBuffer::clear()
{
  read_index_ = 0;
  write_index_ = 0;
}

bool BytesReadBuffer::push(uint8_t& byte)
{
  bool return_value = MAX_SIZE > write_index_;
  if(return_value)
  {
    data_[write_index_] = byte;
    ++write_index_;
  }
  return return_value;
}

