// CommandLineParser - parser for 
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

#include "CommandLineParser.hh"


//----------------------------------------------------------------------
// ParserImpl to hide concrete implementation.
// It uses boost::program_options for now, but as this is hidden it can
// easily be changed later
//
class CommandLineParser::ParserImpl {
}; 


//----------------------------------------------------------------------
// CommandLineParser itself
//
CommandLineParser::CommandLineParser(int argc, char** argv)
   : local_argc_(argc),
     local_argv_(argv),
     pimpl_(new ParserImpl) {}

     
CommandLineParser::~CommandLineParser() {
  delete pimpl_;
}


void CommandLineParser::parse() {
  // __stub method__
}


std::string CommandLineParser::session_name() {
  // __stub method__
  return std::string("qt");
}

