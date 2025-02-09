// This file is part of the Acts project.
//
// Copyright (C) 2017-2021 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ActsExamples/DDG4/DDG4DetectorConstruction.hpp"

#include <DD4hep/DetElement.h>
#include <DD4hep/Detector.h>
#include <DDG4/Geant4Converter.h>
#include <DDG4/Geant4GeometryInfo.h>
#include <DDG4/Geant4Mapping.h>
#include <Parsers/Printout.h>

class G4VPhysicalVolume;

ActsExamples::DDG4DetectorConstruction::DDG4DetectorConstruction(
    dd4hep::Detector& detector)
    : G4VUserDetectorConstruction(), m_detector(detector) {}

// See DD4hep::Simulation::Geant4DetectorConstruction::Construct()
G4VPhysicalVolume* ActsExamples::DDG4DetectorConstruction::Construct() {
  if (m_world == nullptr) {
    dd4hep::sim::Geant4Mapping& g4map = dd4hep::sim::Geant4Mapping::instance();
    dd4hep::DetElement world = m_detector.world();
    dd4hep::sim::Geant4Converter conv(m_detector, dd4hep::PrintLevel::VERBOSE);
    dd4hep::sim::Geant4GeometryInfo* geo_info = conv.create(world).detach();
    g4map.attach(geo_info);
    // All volumes are deleted in ~G4PhysicalVolumeStore()
    m_world = geo_info->world();
    m_detector.apply("DD4hepVolumeManager", 0, nullptr);
    // Create Geant4 volume manager
    g4map.volumeManager();
  }
  return m_world;
}
