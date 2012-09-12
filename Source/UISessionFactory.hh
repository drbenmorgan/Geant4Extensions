// UISessionFactory.hh - Alexandrescu style factory for UI sessions.
//                       Includes builder for builtin sessions based on
//                       availability in Geant4 toolkit.
//
// Copyright (c) 2012, Ben Morgan <bmorgan.warwick@gmail.com>
//
// Distributed under the OSI-approved BSD 3-Clause License (the "License");
// see accompanying file License.txt for details.
//
// This software is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the License for more information.

#ifndef UISESSIONFACTORY_HH
#define UISESSIONFACTORY_HH

#include "G4UIsession.hh"
#include "G4UIterminal.hh"
#include <map>
#include <string>

namespace geant4 {
class UISessionFactory {
 public:
  typedef G4String IdentifierType;
  typedef G4UIsession AbstractProduct;
  typedef AbstractProduct* (*ProductCreator)(int, char**);

 public:
  UISessionFactory() {;}
  ~UISessionFactory() {;}

  bool Register(const IdentifierType& id, ProductCreator creator) {
    return associations_.insert(IdToProductMap::value_type(id,creator)).second != 0;
  }

  AbstractProduct* CreateProduct(const IdentifierType& id, 
                                 int argc, char** argv) {
    IdToProductMap::iterator iter = associations_.find(id);

    if( iter != associations_.end() ) {
      return (iter->second)(argc, argv);
    } else {
      return 0; //ErrorPolicy::OnUnknownType(id);
    }
  }

 private:
  typedef std::map<IdentifierType, ProductCreator> IdToProductMap;
  IdToProductMap associations_;
};

template<typename ConcreteType>
G4UIsession* UISessionCreator(int argc, char** argv) {
  return new ConcreteType(argc, argv);
}

template<typename ShellType>
G4UIsession* UITerminalSessionCreator(int, char**) {
  return new G4UIterminal(new ShellType);
}

}
//Now we can sort of follow G4UIExecutive...
// We can *probably* put this into a .cc eventually because the UIs
// available at Geant4 build time are known exactly. Our builder interface
// simply returns an initial instance of the factory, which we can then
// add our own types to.
#include "G4UIcsh.hh"
#ifdef G4UI_USE_TCSH
#include "G4UItcsh.hh"
#endif
#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif
#ifdef G4UI_USE_QT
#include "G4UIQt.hh"
#endif
#ifdef G4UI_USE_WIN32
#include "G4UIWin32.hh"
#endif

namespace geant4 {
UISessionFactory BuildUISessionFactory() {
  UISessionFactory f;
  f.Register("csh", UITerminalSessionCreator<G4UIcsh>);
#ifdef G4UI_USE_TCSH
  f.Register("tcsh", UITerminalSessionCreator<G4UItcsh>);
#endif
#ifdef G4UI_USE_XM
  f.Register("xm", UISessionCreator<G4UIXm>);
#endif
#ifdef G4UI_USE_QT
  f.Register("qt", UISessionCreator<G4UIQt>);
#endif
#ifdef G4UI_USE_WIN32
  f.Register("win32", UISessionCreator<G4UIWin32>);
#endif

  return f;
}
} // namespace geant4

#endif // UISESSIONFACTORY_HH

