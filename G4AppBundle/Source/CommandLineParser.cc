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
#include <boost/program_options.hpp>
namespace bpo = boost::program_options;

class CommandLineParser::ParserImpl {
 public:
  ParserImpl() : options_("SuperB1 Options"), variables_() {
    options_.add_options()
        ("help,h", "print help message")
        ("session,s", bpo::value<std::string>()->default_value("qt"), "start interactive session");
  }

  ~ParserImpl() {}

  /// parse input command line args
  void parse(int argc, char** argv) {
    // - Parse command line, and fail if we don't recognise an option
    try {
      bpo::store(bpo::parse_command_line(argc, argv, options_), variables_);
    }
    catch (bpo::unknown_option) {
      this->display_help();
      exit(EXIT_FAILURE);
    }

    bpo::notify(variables_);
    this->post_process();
  }

  /// return value of session option
  std::string session() const {
    return variables_["session"].as<std::string>();
  }

  /// display help message to stderr
  void display_help() {
    std::cerr << options_ << std::endl;
  }

  /// perform any post-parsing processing
  void post_process() {
    if(variables_.count("help")) {
      this->display_help();
      exit(EXIT_SUCCESS);
    }
  }

 private:
  bpo::options_description options_;
  bpo::variables_map variables_;
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
  pimpl_->parse(local_argc_, local_argv_);
}


std::string CommandLineParser::session_name() {
  return pimpl_->session();
}

