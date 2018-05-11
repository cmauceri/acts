// This file is part of the ACTS project.
//
// Copyright (C) 2018 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ACTS_TOOLS_TWOHITSSPACEPOINTBUILDER_H
#define ACTS_TOOLS_TWOHITSSPACEPOINTBUILDER_H

#include "ACTS/Digitization/CartesianSegmentation.hpp"
#include "ACTS/Digitization/DigitizationModule.hpp"
#include "ACTS/Tools/ISpacePointBuilder.hpp"
#include "ACTS/Utilities/Units.hpp"

namespace Acts {

/// @class TwoHitsSpacePointBuilder
///
/// After the particle interaction with surfaces are recorded and digitized
/// the hits strip detectors need further treatment. This class takes
/// the digitized hits and combines them on two different detector elements to a
/// result of the combined detector element. The class is intended to handle
/// strip detector elements in particular.
///
/// @note Used abbreviation: "Strip Detector Element" -> SDE
///
class TwoHitsSpacePointBuilder : public ISpacePointBuilder
{
public:
  /// @brief Configuration of the class to steer its behaviour
  struct Config
  {
    /// Accepted difference in eta for two hits
    double diffTheta2 = 1.;
    /// Accepted difference in phi for two hits
    double diffPhi2 = 1.;
    /// Accepted distance between two hits
    double diffDist = 100. * units::_mm;
    /// Allowed increase of strip length
    double stripLengthTolerance = 0.01;
    /// Allowed increase of strip length wrt gaps between strips
    double stripLengthGapTolerance = 0.01;
    /// Assumed position of the vertex
    Vector3D vertex = {0., 0., 0.};
    /// Perform the perpendicular projection for space point finding
    bool usePerpProj = false;
  };

  /// Constructor
  /// @param cfg is the configuration class
  TwoHitsSpacePointBuilder(const Config& cfg);

  /// @brief Searches possible combinations of two hits on different surfaces
  /// that may come from the same particles
  /// @param vec1 vector of hits on the first surface
  /// @param vec2 vector of hits on the second surface
  void
  addHits(const std::vector<std::vector<PlanarModuleCluster const*>>& hits);

  /// @brief Calculates the space points out of a given collection of hits
  /// on several strip detectors and stores the data
  void
  calculateSpacePoints() override;

  /// @brief Returns the list of all resolved space points
  /// @note This function is a pure getter of the current state
  /// @return full collection of all resolved space points
  const std::vector<SpacePoint>&
  spacePoints() override;

protected:
  /// @brief Getter method for the local coordinates of a hit
  /// on its corresponding surface
  /// @param hit object related to the hit that holds the necessary information
  /// @return vector of the local coordinates of the hit on the surface
  Vector2D
  localCoords(const PlanarModuleCluster& hit) const override;

  /// @brief Getter method for the global coordinates of a hit
  /// @param hit object related to the hit that holds the necessary information
  /// @return vector of the global coordinates of the hit
  Vector3D
  globalCoords(const PlanarModuleCluster& hit) const override;

private:
  /// @brief Storage container for variables related to the calculation of space
  /// points
  struct SpacePointParameters
  {
    /// Vector pointing from bottom to top end of first SDE
    Vector3D q;
    /// Vector pointing from bottom to top end of second SDE
    Vector3D r;
    /// Twice the vector pointing from vertex to to midpoint of first SDE
    Vector3D s;
    /// Twice the vector pointing from vertex to to midpoint of second SDE
    Vector3D t;
    /// Cross product between SpacePointParameters::q and
    /// SpacePointParameters::s
    Vector3D qs;
    /// Cross product between SpacePointParameters::r and
    /// SpacePointParameters::t
    Vector3D rt;
    /// Magnitude of SpacePointParameters::q
    double qmag = 0.;
    /// Parameter that determines the hit position on the first SDE
    double m = 0.;
    /// Parameter that determines the hit position on the second SDE
    double n = 0.;
    /// Regular limit of the absolut values of SpacePointParameters::m and
    /// SpacePointParameters::n
    double limit = 1.;
    /// Limit of SpacePointParameters::m and SpacePointParameters::n in case of
    /// variable vertex
    double limitExtended = 1.;

    /// @brief reset structure and allows to reuse the same structure
    void
    reset()
    {
      // Set every vector to nullvector. This allows checks, if a variable was
      // already set.
      q  = {0., 0., 0.};
      r  = {0., 0., 0.};
      s  = {0., 0., 0.};
      t  = {0., 0., 0.};
      qs = {0., 0., 0.};
      rt = {0., 0., 0.};
      // Set every double to default values
      qmag          = 0;
      m             = 0;
      n             = 0;
      limit         = 1.;
      limitExtended = 1.;
    }
  };

  /// Storage of the user defined configuration of the class
  Config m_cfg;
  /// Storage of all resolved data
  std::vector<SpacePoint> m_allCombSpacePoints;

  /// @brief Calculates (Delta theta)^2 + (Delta phi)^2 between two hits
  /// @param hit1 the first hit
  /// @param hit2 the second hit
  /// @return the squared sum in case of success, otherwise -1
  double
  differenceOfHits(const PlanarModuleCluster& hit1,
                   const PlanarModuleCluster& hit2) const;

  /// @brief Calculates the top and bottom ends of a SDE
  /// that corresponds to a given hit
  /// @param hit object that stores the information about the hit
  /// @return vectors to the top and bottom end of the SDE
  std::pair<Vector3D, Vector3D>
  endsOfStrip(const PlanarModuleCluster& hit) const;

  /// @brief Calculates a space point whithout using the vertex
  /// @note This is mostly to resolve space points from cosmic data
  /// @param a vector to the top end of the first SDE
  /// @param c vector to the top end of the second SDE
  /// @param q vector from the bottom to the top end of the first SDE
  /// @param r vector from the bottom to the top end of the second SDE
  /// @return parameter that indicates the location of the space point; returns
  /// 1. if it failed
  /// @note The meaning of the parameter is explained in more detail in the
  /// function body
  double
  calcPerpProj(const Vector3D& a,
               const Vector3D& c,
               const Vector3D& q,
               const Vector3D& r) const;

  /// @brief This function tests if a space point can be estimated by a more
  /// tolerant treatment of construction. In fact, this function indirectly
  /// allows shifts of the vertex.
  /// @param spaPoPa container that stores geometric parameters and rules of the
  /// space point formation
  /// @return indicator if the test was successful
  bool
  recoverSpacePoint(SpacePointParameters& spaPoPa) const;
};

}  // namespace Acts

#endif  // ACTS_TOOLS_TWOHITSSPACEPOINTBUILDER_H
