
// This file is part of the Acts project.
//
// Copyright (C) 2020 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include "ActsExamples/EventData/SimParticle.hpp"
#include "Acts/Vertexing/Vertex.hpp"
#include "Acts/Definitions/Algebra.hpp"
#include "Acts/MagneticField/MagneticFieldProvider.hpp"
#include "ActsExamples/EventData/ProtoVertex.hpp"
#include "ActsExamples/EventData/Trajectories.hpp"
#include "Acts/EventData/TrackParameters.hpp"
#include "ActsExamples/Framework/DataHandle.hpp"
#include "ActsExamples/Framework/IAlgorithm.hpp"
#include "Acts/Definitions/TrackParametrization.hpp"

#include <string>

namespace ActsExamples {

using VerticesContainer = 
  std::vector<Acts::Vertex<Acts::BoundTrackParameters>>;
using SimParticleContainer = 
  std::vector<Acts::SimParticle<ActsExamples::detail::ActsExamples::SimParticleContainer>>;
using TrackParametersContainer=
  std::vector<Acts::TrackParameters<Acts::BoundTrackParameters>>;
class PrimaryVertexFinderAlgorithm final : public IAlgorithm {
 public:
  struct Config {
    /// Optional. Input track parameters collection
    std::string inputTrackParameters;
    std::string inputSelectedTruthParticles;    
/// Optional. Input trajectories container.
    //std::string inputTrajectories;
    /// Output proto vertex collection
    //std::string outputProtoVertices;
    /// The magnetic field
    //std::shared_ptr<Acts::MagneticFieldProvider> bField;
    // Don't need the above lines
    //Do need a slot to pass the input vtx
    std::string inputVertices;
  };

  PrimaryVertexFinderAlgorithm(const Config& cfg, Acts::Logging::Level lvl);

  /// Find vertices.
  ///
  /// @param ctx is the algorithm context with event information
  /// @return a process code indication success or failure
  ProcessCode execute(const AlgorithmContext& ctx) const final;
  ///method for matching truth vertices to reco

 
 /// Get readonly access to the config parameters
  const Config& config() const { return m_cfg; }

 private:
  Config m_cfg;

  ReadDataHandle<TrajectoriesContainer> m_inputTrajectories{
      this, "InputTrajectories"};
  //Need ReadDataHandle for the vertices
  ReadDataHandle<VerticesContainer> m_inputVertices{
    this, "inputVertices"};
  ReadDataHandle<SimParticleContainer> m_inputSelectedTruthParticles{
    this, "inputSelectedTruthParticles"};
  ReadDataHandle<std::vector<Acts::BoundTrackParameters>>
    m_inputTrackParameters{this, "InputTrackParameters"};

  Acts::Vertex* PrimaryVertexFinderAlgorithm::matchVtx<TrackParametersContainer, SimParticleContainer, VerticesContainer>;
};

} // namespace ActsExamples


