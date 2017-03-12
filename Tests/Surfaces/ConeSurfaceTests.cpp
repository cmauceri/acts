// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#define BOOST_TEST_MODULE ConeSurface Tests

#include <boost/test/included/unit_test.hpp>
// leave blank line

#include <boost/test/data/test_case.hpp>
// leave blank line

#include <boost/test/output_test_stream.hpp>
// leave blank line

//
#include <limits>
#include "ACTS/Surfaces/ConeSurface.hpp"
#include "ACTS/Utilities/Definitions.hpp"
#include "ACTS/Surfaces/RectangleBounds.hpp"

namespace tt    = boost::test_tools;
using boost::test_tools::output_test_stream;
namespace utf    = boost::unit_test;
const double inf = std::numeric_limits<double>::infinity();
const double NaN = std::numeric_limits<double>::quiet_NaN();

namespace Acts {

namespace Test {
  BOOST_AUTO_TEST_SUITE(ConeSurfaces);
  /// Unit test for creating compliant/non-compliant ConeSurface object
  BOOST_AUTO_TEST_CASE(ConeSurfaceConstruction)
  {
    //ConeSurface default constructor is deleted
    //
    ///Constructor with transform pointer, null or valid, alpha and symmetry indicator
    double  alpha{M_PI/8.}, halfPhiSector{M_PI/16.}, zMin{1.0}, zMax{10.};
    bool symmetric(false);
    Translation3D translation{0., 1., 2.};
    auto pTransform = std::make_shared<Transform3D>(translation);
    auto pNullTransform = std::make_shared<Transform3D>();
    BOOST_TEST(ConeSurface(pNullTransform, alpha, symmetric).type() == Surface::Cone);
    BOOST_TEST(ConeSurface(pTransform, alpha, symmetric).type() == Surface::Cone);
    //
    ///Constructor with transform pointer, alpha,z min and max, halfPhiSector
    BOOST_TEST(ConeSurface(pTransform, alpha, zMin, zMax, halfPhiSector).type() == Surface::Cone);
    //
    
    ///Constructor with transform and ConeBounds pointer
    //ConeBounds (double alpha, double zmin, double zmax, double halfphi=M_PI, double avphi=0.)
    auto pConeBounds = std::make_shared<ConeBounds>(alpha, zMin, zMax, halfPhiSector, 0.);
    BOOST_TEST(ConeSurface(pTransform, pConeBounds).type() == Surface::Cone);
    //
    //
    ///Copy constructor
    ConeSurface coneSurfaceObject(pTransform, alpha, symmetric);
    ConeSurface copiedConeSurface(coneSurfaceObject);
    BOOST_TEST(copiedConeSurface.type() == Surface::Cone);
    BOOST_TEST(copiedConeSurface == coneSurfaceObject);
    //
    ///Copied and transformed
    ConeSurface copiedTransformedConeSurface(coneSurfaceObject, *pTransform);
    BOOST_TEST(copiedTransformedConeSurface.type() == Surface::Cone);
  }
  //
  /// Unit test for testing ConeSurface properties
  BOOST_AUTO_TEST_CASE(ConeSurfaceProperties)
  {
    double withinOnePercent=0.01;
    ///Test clone method
    double  alpha{M_PI/8.} /*,halfPhiSector{M_PI/16.}, zMin{1.0}, zMax{10.}*/;
    bool symmetric(false);
    Translation3D translation{0., 1., 2.};
    auto pTransform = std::make_shared<Transform3D>(translation);
    //auto pNullTransform = std::make_shared<Transform3D>();
    ConeSurface coneSurfaceObject(pTransform, alpha, symmetric);
    //
    auto pClonedConeSurface = coneSurfaceObject.clone();
    BOOST_TEST(pClonedConeSurface->type() == Surface::Cone);
    delete pClonedConeSurface;
    //
    ///Test type (redundant)
    BOOST_TEST(coneSurfaceObject.type()  == Surface::Cone);
    //
    ///Test binningPosition
    Vector3D binningPosition{0.,1.,2.};
    BOOST_TEST(coneSurfaceObject.binningPosition(BinningValue::binPhi) == binningPosition);
    //
    ///Test measurementFrame
    Vector3D globalPosition{2.0,2.0,2.0};
    Vector3D momentum{1.e6, 1.e6, 1.e6};
    double rootHalf=std::sqrt(0.5);
    RotationMatrix3D expectedFrame;
    expectedFrame << -rootHalf, 0., rootHalf, rootHalf, 0., rootHalf, 0.,1.,0.;
    BOOST_TEST(coneSurfaceObject.measurementFrame(globalPosition, momentum).isApprox(expectedFrame));
    //
    ///Test normal, given 3D position
    Vector3D origin{0.,0.,0.};
    Vector3D normal3D = {0.,-1.,0.};
    BOOST_TEST(coneSurfaceObject.normal(origin) == normal3D);
    //
    ///Test normal given 2D rphi position
    Vector2D positionPiBy2(1.0,M_PI/2.);
    Vector3D normalAtPiBy2{0.0312768, 0.92335,-0.382683};
    
    BOOST_TEST(coneSurfaceObject.normal(positionPiBy2).isApprox(normalAtPiBy2, withinOnePercent));
    //
    ///Test rotational symmetry axis
    Vector3D symmetryAxis{0.,0.,1.};
    BOOST_TEST(coneSurfaceObject.rotSymmetryAxis() == symmetryAxis);
    //
    ///Test bounds
    BOOST_TEST(coneSurfaceObject.bounds().type() == SurfaceBounds::Cone);
    //
    ///Test localToGlobal
    Vector2D localPosition{1.0,M_PI/2.0};
    coneSurfaceObject.localToGlobal(localPosition, momentum, globalPosition);
    //std::cout<<globalPosition<<std::endl;
    Vector3D expectedPosition{0.0220268,1.65027,3.5708};
    
    BOOST_TEST(globalPosition.isApprox(expectedPosition, withinOnePercent));
    //
    ///Testing globalToLocal
    coneSurfaceObject.globalToLocal(globalPosition, momentum,localPosition);
    //std::cout<<localPosition<<std::endl;
    Vector2D expectedLocalPosition{1.0,M_PI/2.0};
    
    BOOST_TEST(localPosition == expectedLocalPosition,"Testing globalToLocal");
    //
    ///Test isOnSurface
    Vector3D offSurface{100,1,2};
    BOOST_TEST(coneSurfaceObject.isOnSurface(globalPosition,true));
    BOOST_TEST(coneSurfaceObject.isOnSurface(offSurface,true) == false);
    //
    ///intersectionEstimate
    Vector3D direction{-1.,0,0};
    auto intersect =coneSurfaceObject.intersectionEstimate(offSurface, direction);
    Intersection expectedIntersect{Vector3D{0,1,2},100.,true,0};
    BOOST_TEST(intersect.valid);
    BOOST_TEST(intersect.position == expectedIntersect.position);
    BOOST_TEST(intersect.pathLength == expectedIntersect.pathLength);
    BOOST_TEST(intersect.distance == expectedIntersect.distance);
    //
    ///Test pathCorrection
    BOOST_TEST(coneSurfaceObject.pathCorrection(offSurface, momentum) == 0.40218866453252877, tt::tolerance(0.01));
    //
    ///Test name
    BOOST_TEST(coneSurfaceObject.name() == std::string("Acts::ConeSurface"));
    //
    ///Test dump
    boost::test_tools::output_test_stream dumpOuput;
    coneSurfaceObject.dump(dumpOuput);
    BOOST_TEST(dumpOuput.is_equal("Acts::ConeSurface\n\
     Center position  (x, y, z) = (0.0000, 1.0000, 2.0000)\n\
     Rotation:             colX = (1.000000, 0.000000, 0.000000)\n\
                           colY = (0.000000, 1.000000, 0.000000)\n\
                           colZ = (0.000000, 0.000000, 1.000000)\n\
     Bounds  : Acts::ConeBounds: (tanAlpha, minZ, maxZ, averagePhi, halfPhiSector) = (0.4142136, 0.0000000, 100000000000.0000000, 0.0000000, 3.1415927)"));
  }

  BOOST_AUTO_TEST_CASE(EqualityOperators, *utf::expected_failures(1))
  {
    double  alpha{M_PI/8.} /*, halfPhiSector{M_PI/16.}, zMin{1.0}, zMax{10.}*/;
    bool symmetric(false);
    Translation3D translation{0., 1., 2.};
    auto pTransform = std::make_shared<Transform3D>(translation);
    ConeSurface coneSurfaceObject(pTransform, alpha, symmetric);
    //
    ConeSurface coneSurfaceObject2(pTransform, alpha, symmetric);
    //
    ///Test equality operator
    BOOST_TEST(coneSurfaceObject == coneSurfaceObject2);
    //
    BOOST_TEST_CHECKPOINT("Create and then assign a ConeSurface object to the existing one");
    ///Test assignment (will fail at the equality test)
    ConeSurface assignedConeSurface(nullptr, NaN, NaN);
    assignedConeSurface = coneSurfaceObject;
    ///Test equality of assigned to original
    BOOST_TEST(assignedConeSurface == coneSurfaceObject);
  }
  BOOST_AUTO_TEST_SUITE_END();

}  // end of namespace Test

}  // end of namespace Acts
