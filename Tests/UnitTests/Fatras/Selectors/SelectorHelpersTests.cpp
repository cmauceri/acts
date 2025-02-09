// This file is part of the Acts project.
//
// Copyright (C) 2018-2020 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <boost/test/unit_test.hpp>

#include "ActsFatras/Selectors/KinematicCasts.hpp"
#include "ActsFatras/Selectors/SelectorHelpers.hpp"

#include <string>

#include "Dataset.hpp"

namespace {
const auto& backward = Dataset::backwardPion;
const auto& central = Dataset::centralPion;
const auto& forward = Dataset::forwardPion;

// mock-ups for input objects (particles)

struct Object {
  int feature = 0;
  std::string name = "";
};

/// Selector that selects on the feature. Only acts on the object.
struct FeatureSelector {
  int select_on = 0;

  bool operator()(const Object& object) const {
    return object.feature == select_on;
  }
};

/// Selector that selects on the name. Only acts on the object.
struct NameSelector {
  std::string select_on = "";

  bool operator()(const Object& object) const {
    return object.name == select_on;
  }
};

struct CombineFixture {
  FeatureSelector selectObjectFeature = {1};
  NameSelector selectObjectName = {"same_name"};
  Object obj = {1, "same_name"};
  Object objWrongFeature = {2, "same_name"};
  Object objWrongName = {1, "another_name"};
};

}  // namespace

BOOST_AUTO_TEST_SUITE(FatrasSelectorHelpers)

BOOST_AUTO_TEST_CASE(Min) {
  // require a minimum eta value of 0.5
  ActsFatras::Min<ActsFatras::Casts::Eta> minEta{0.5};
  BOOST_CHECK(not minEta(backward));
  BOOST_CHECK(not minEta(central));
  BOOST_CHECK(minEta(forward));

  // require a mininum absolute eta value of 0.5
  ActsFatras::Min<ActsFatras::Casts::AbsEta> minAbsEta{0.5};
  BOOST_CHECK(minAbsEta(backward));
  BOOST_CHECK(not minAbsEta(central));
  BOOST_CHECK(minAbsEta(forward));
}

BOOST_AUTO_TEST_CASE(Max) {
  // require a maximum eta value of 0.5
  ActsFatras::Max<ActsFatras::Casts::Eta> maxEta{0.5};
  BOOST_CHECK(maxEta(backward));
  BOOST_CHECK(maxEta(central));
  BOOST_CHECK(not maxEta(forward));

  // require a maximum absolute eta value of 0.5
  ActsFatras::Max<ActsFatras::Casts::AbsEta> maxAbsEta{0.5};
  BOOST_CHECK(not maxAbsEta(backward));
  BOOST_CHECK(maxAbsEta(central));
  BOOST_CHECK(not maxAbsEta(forward));
}

BOOST_AUTO_TEST_CASE(Range) {
  ActsFatras::Range<ActsFatras::Casts::Eta> rangeEta{-6.0, -0.5};
  BOOST_CHECK(rangeEta(backward));
  BOOST_CHECK(not rangeEta(central));
  BOOST_CHECK(not rangeEta(forward));

  ActsFatras::Range<ActsFatras::Casts::AbsEta> rangeAbsEta{0.5, 6.0};
  BOOST_CHECK(rangeAbsEta(backward));
  BOOST_CHECK(not rangeAbsEta(central));
  BOOST_CHECK(rangeAbsEta(forward));
}

BOOST_AUTO_TEST_CASE(And1) {
  CombineFixture f;
  ActsFatras::CombineAnd<FeatureSelector> select;
  select.get<FeatureSelector>() = f.selectObjectFeature;
  BOOST_CHECK(select(f.obj));
  BOOST_CHECK(not select(f.objWrongFeature));
  BOOST_CHECK(select(f.objWrongName));
}

BOOST_AUTO_TEST_CASE(And2) {
  CombineFixture f;
  ActsFatras::CombineAnd<FeatureSelector, NameSelector> select;
  select.get<FeatureSelector>() = f.selectObjectFeature;
  select.get<NameSelector>() = f.selectObjectName;
  BOOST_CHECK(select(f.obj));
  BOOST_CHECK(not select(f.objWrongFeature));
  BOOST_CHECK(not select(f.objWrongName));
}

BOOST_AUTO_TEST_CASE(Or1) {
  CombineFixture f;
  ActsFatras::CombineOr<FeatureSelector> select;
  select.get<FeatureSelector>() = f.selectObjectFeature;
  BOOST_CHECK(select(f.obj));
  BOOST_CHECK(not select(f.objWrongFeature));
  BOOST_CHECK(select(f.objWrongName));
}

BOOST_AUTO_TEST_CASE(Or2) {
  CombineFixture f;
  ActsFatras::CombineOr<FeatureSelector, NameSelector> select;
  select.get<FeatureSelector>() = f.selectObjectFeature;
  select.get<NameSelector>() = f.selectObjectName;
  BOOST_CHECK(select(f.obj));
  BOOST_CHECK(select(f.objWrongFeature));
  BOOST_CHECK(select(f.objWrongName));
}

BOOST_AUTO_TEST_SUITE_END()
