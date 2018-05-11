// CommandLineParser - parse command line for SuperB1
//
// Copyright (c) 2012 by Ben Morgan <bmorgan.warwick@gmail.com> 
// Copyright (c) 2012 by The University of Warwick
//
// Distributed under the OSI-approved BSD 3-Clause License (the "License");
// see accompanying file License.txt for details.
//
// This software is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the License for more information.

/// \file CommandLineParser.hh
/// \brief Definition of the CommandLineParser class

#ifndef COMMANDLINEPARSER_HH
#define COMMANDLINEPARSER_HH
// Standard Library
#include <string>

// Third Party
// - A

// This Project

/// Class providing interface to parse and query command line args
class CommandLineParser {
 public:
  CommandLineParser(int argc, char** argv);
  ~CommandLineParser();

  /// perform the parsing of the command line
  void parse();

  /// return requested session name
  std::string session_name();

 private:
  int local_argc_;
  char** local_argv_;

  class ParserImpl;
  ParserImpl* pimpl_;
};

#endif // COMMANDLINEPARSER_HH

