// This file is part of the Acts project.
//
// Copyright (C) 2023 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "Acts/Utilities/Logger.hpp"

#include <any>
#include <vector>

namespace Acts {

// TODO maybe replace std::any with some kind of variant<unique_ptr<torch>,
// unique_ptr<onnx>>?
// TODO maybe replace input for GraphConstructionBase with some kind of
// boost::multi_array / Eigen::Array

class GraphConstructionBase {
 public:
  /// Perform the graph construction
  ///
  /// @param inputValues Flattened input data
  /// @param logger Logger instance
  ///
  /// @return (node_tensor, edge_tensore)
  virtual std::tuple<std::any, std::any> operator()(
      std::vector<float> &inputValues, const Logger &logger) = 0;
};

class EdgeClassificationBase {
 public:
  /// Perform edge classification
  ///
  /// @param nodes Node tensor with shape (n_nodes, n_node_features)
  /// @param edges Edge-index tensor with shape (2, n_edges)
  /// @param logger Logger instance
  ///
  /// @return (node_tensor, edge_tensor, score_tensor)
  virtual std::tuple<std::any, std::any, std::any> operator()(
      std::any nodes, std::any edges, const Logger &logger) = 0;
};

class TrackBuildingBase {
 public:
  /// Perform track building
  ///
  /// @param nodes Node tensor with shape (n_nodes, n_node_features)
  /// @param edges Edge-index tensor with shape (2, n_edges)
  /// @param edgeWeights Edge-weights of the previous edge classification phase
  /// @param spacepointIDs IDs of the nodes (must have size=n_nodes)
  /// @param logger Logger instance
  ///
  /// @return tracks (as vectors of node-IDs)
  virtual std::vector<std::vector<int>> operator()(
      std::any nodes, std::any edges, std::any edgeWeights,
      std::vector<int> &spacepointIDs, const Logger &logger) = 0;
};

}  // namespace Acts
