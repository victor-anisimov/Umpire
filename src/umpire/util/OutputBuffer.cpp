//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2019, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory
//
// Created by David Beckingsale, david@llnl.gov
// LLNL-CODE-747640
//
// All rights reserved.
//
// This file is part of Umpire.
//
// For details, see https://github.com/LLNL/Umpire
// Please also see the LICENSE file for MIT license.
//////////////////////////////////////////////////////////////////////////////

#include "umpire/util/OutputBuffer.hpp"

#include <ostream>

namespace umpire {
namespace util {

void
OutputBuffer::setConsoleStream(std::ostream* stream)
{
  if (stream) {
    d_console_stream = stream->rdbuf();
  } else {
    d_console_stream = nullptr;
  }
}

void
OutputBuffer::setFileStream(std::ostream* stream)
{
  if (stream) {
    d_file_stream = stream->rdbuf();
  } else {
    d_file_stream = nullptr;
  }
}

int
OutputBuffer::overflow(int ch)
{
  if (ch == EOF)
  {
    return !EOF;
  } else {
    auto r_console = ch;
    auto r_file = ch;

    if (d_console_stream) {
      r_console = d_console_stream->sputc(ch);
    }

    if (d_file_stream) {
      r_file = d_file_stream->sputc(ch);
    }

    return r_console == EOF || r_file == EOF ? EOF : ch;
  }

}

int
OutputBuffer::sync()
{
  auto ret = 0;

  if (d_console_stream) {
    ret = d_console_stream->pubsync();
  }

  if (d_file_stream) {
    ret += d_file_stream->pubsync();
  }

  return ret == 0 ? 0 : -1;
}

OutputBuffer::~OutputBuffer()
{
  if (d_console_stream) {
    d_console_stream->pubsync();
  }

  if (d_file_stream) {
    d_file_stream->pubsync();
  }
}

} // end of namespace util
} // end of namespace umpire
