//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2017 Jeongnim Kim and QMCPACK developers.
//
// File developed by: Mark Dewing, mdewing@anl.gov, Argonne National Laboratory
//
// File created by: Mark Dewing, mdewing@anl.gov, Argonne National Laboratory
//////////////////////////////////////////////////////////////////////////////////////


/** @file InfoStream.h
 * @brief Declaration of InfoStream class.
 */

#ifndef INFOSTREAM_H
#define INFOSTREAM_H

#include <fstream>
#include <memory>
#include <ostream>

/**
 *  Interface to output streams.  Can redirect output to stdout/stderr, a file, or a null stream.
 */

class InfoStream
{
public:
  explicit InfoStream(std::ostream* output_stream)
      : currStream(output_stream)
  {}

  explicit InfoStream(InfoStream& in)
  {
    redirectToSameStream(in);
  }

  std::ostream& getStream() const { return *currStream; }

  void setStream(std::ostream* output_stream) { currStream = output_stream; }

  void flush() const { currStream->flush(); }

  /// Stop output (redirect to a null stream)
  void pause();

  ///  Continue output on the stream used before pausing
  void resume();

  /// Open a file and output to that file
  void redirectToFile(const std::string& fname);

  /// Copy a stream
  void redirectToSameStream(InfoStream& info);

  ///  Permanently turn off the stream
  void shutOff();

private:
  // Keep track of whether we should delete the stream or not
  std::unique_ptr<std::ofstream> ownStream;

  std::ostream* currStream = nullptr;

  // save stream during pause
  std::ostream* prevStream = nullptr;

  // Used during pause
  static std::ostream nullStream;

  template<typename T>
  friend InfoStream& operator<<(InfoStream& o, const T& val)
  { return o.getStream() << val; }
};

#endif

