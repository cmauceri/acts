
// This file is part of the Acts project.
//
// Copyright (C) 2020 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ActsExamples/Vertexing/PrimaryVertexFinderAlgorithm.hpp"

#include "Acts/Definitions/Units.hpp"
#include "Acts/MagneticField/MagneticFieldContext.hpp"
#include "Acts/Propagator/EigenStepper.hpp"
#include "Acts/Propagator/Propagator.hpp"
#include "Acts/Surfaces/PerigeeSurface.hpp"
#include "Acts/Vertexing/AdaptiveMultiVertexFinder.hpp"
#include "Acts/Vertexing/AdaptiveMultiVertexFitter.hpp"
#include "Acts/Vertexing/HelicalTrackLinearizer.hpp"
#include "Acts/Vertexing/ImpactPointEstimator.hpp"
#include "Acts/Vertexing/LinearizedTrack.hpp"
#include "Acts/Vertexing/TrackDensityVertexFinder.hpp"
#include "Acts/Vertexing/VertexFinderConcept.hpp"
#include "Acts/Vertexing/VertexingOptions.hpp"
#include "ActsExamples/Framework/RandomNumbers.hpp"
#include "ActsExamples/Framework/WhiteBoard.hpp"
#include "VertexingHelpers.hpp"

ActsExamples::PrimaryVertexFinderAlgorithm::PrimaryVertexFinderAlgorithm(
    const Config& cfg, Acts::Logging::Level lvl)
    : ActsExamples::IAlgorithm("PrimaryVertexFinder", lvl), m_cfg(cfg) {
  // if (m_cfg.inputTrackParameters.empty() == m_cfg.inputTrajectories.empty()) {
  //throw std::invalid_argument(
  //    "You have to either provide track parameters or trajectories");
  // }
  


  //m_inputTrajectories.maybeInitialize(m_cfg.inputTrajectories);
  // Adapt previous line to get vertices
  m_inputVertices.maybeInitialize(m_cfg.inputVertices);
}

ActsExamples::ProcessCode ActsExamples::PrimaryVertexFinderAlgorithm::execute(
    const ActsExamples::AlgorithmContext& ctx) const {
  // retrieve input tracks and convert into the expected format
  //const auto& inputTrackParameters = m_inputTrackParameters(ctx);
  //const auto& inputTrackPointers =
  //  makeTrackParametersPointerContainer(inputTrackParameters);
  //* Do not change the code above this line *//

  //loop over the vertices retrieved in the constructor(Vt), and print the number of track,
  // vtx location, 
  //so i need to check how to get the vertex object information from the vertex class that
  //is in Core/include/Acts/Vertexing/Vertex
  /*Sketch:
    Create Vtx object??

    Get Vertex   Vertex()
    Get Tracks at vertex
    
    For Vertex:
        For Track: 
	    Get pt^2
	   sumpt^2 =pt^2++
	write sumpt^2 to array?
	Get max from array?
	   Need to find memory position of max to find which vtx it is
     return vtx  
  */
  
  if (m_inputVertices.isInitialized()) {
    
    const auto& vertices = m_inputVertices(ctx);

    std::cout<<"Total number of Vertices: " << vertices.size()<<std::endl;
    int size=vertices.size();
    std::vector<double> vtxPts(size);
    int i=0;
    
    int j = 0;
    for (auto& vtx :  vertices) {
      // Looping through vertices
      double pt2 = 0;
      // initializing pt^2
      auto& tracks=vtx.tracks();
      // initializing vtx associated tracks
      for (auto& track: tracks) {
	// Looping through tracks
	pt2 = pt2+(track.fittedParams.transverseMomentum())*(track.fittedParams.transverseMomentum());
	// Calculating sum (pt^2)
      }
      vtxPts.at(j)=pt2;
      // Storing sum(pt^2) for each vertex
      j++;
      // Increasing vertex index
    }
 
    
    int p_vtx=0;
    //initializing primary vertex index
    for (i=1; i<=size; i++){
      // Loop through Pt^2 
      std::cout<<"Vtx "<<i<<"Pt^2: "<<(vtxPts[i])<<std::endl;
      if(vtxPts[i]>vtxPts[i-1]){
	p_vtx=i;
	// updates primary vertex index
      }
    }
    std::cout<<"Maximum pt squared: " <<vtxPts[p_vtx]<<std::endl;
    const auto& primVtx = vertices.at(p_vtx);
    //primVtx=vertices[p_vtx];
    std::cout<<"Primary Vertex location: "<<std::endl;
    std::cout<<primVtx.position()<<std::endl;
    

  }
 
 // vertex container initialized

    ///////////////////////////////////////////////
  /*     Add the tutorial example code here    */
  ///////////////////////////////////////////////
  ///////////////////////////////////////////////
  /*  For the full tutorial code please refer  */
  /* to AdaptiveMultiVertexFinderAlgorithm.cpp */
  ///////////////////////////////////////////////

  //* Do not change the code below this line *//
  return ActsExamples::ProcessCode::SUCCESS;
}
