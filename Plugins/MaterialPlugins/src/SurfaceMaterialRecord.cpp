// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// SurfaceMaterialRecord.cpp, ACTS project
///////////////////////////////////////////////////////////////////

#include "ACTS/Plugins/MaterialPlugins/SurfaceMaterialRecord.hpp"

Acts::SurfaceMaterialRecord::SurfaceMaterialRecord(const Surface& surface,
                                                   const BinUtility& binUtility)
  : m_surface(&surface)
  , m_binUtility(std::make_unique<BinUtility>(binUtility))
  , m_mappedMaterial()
{
  // reserve
  m_mappedMaterial.reserve(m_binUtility->bins(1));
  for (size_t ibin1 = 0; ibin1 < m_binUtility->bins(1); ++ibin1) {
    // create the vector for the push_back
    RecordVector mappedVector;
    mappedVector.reserve(m_binUtility->bins(0));
    for (size_t ibin0 = 0; ibin0 < m_binUtility->bins(0); ++ibin0)
      mappedVector.push_back(RecordBin(MaterialProperties(),0));
    m_mappedMaterial.push_back(mappedVector);
  }
}

Acts::SurfaceMaterialRecord::SurfaceMaterialRecord(
    const SurfaceMaterialRecord& lmrecord)
  : m_surface(lmrecord.m_surface)
  , m_binUtility(std::make_unique<BinUtility>(*lmrecord.m_binUtility))
  , m_mappedMaterial(lmrecord.m_mappedMaterial)
{
}

Acts::SurfaceMaterialRecord&
Acts::SurfaceMaterialRecord::operator=(const SurfaceMaterialRecord& lmrecord)
{
  if (this != &lmrecord) {
    m_surface = lmrecord.m_surface;
    m_binUtility = std::make_unique<BinUtility>(*lmrecord.m_binUtility);
    m_mappedMaterial = lmrecord.m_mappedMaterial;
  }
  return (*this);
}

void
Acts::SurfaceMaterialRecord::assignMaterialSteps(
    const Vector3D&                  pos,
    const std::vector<MaterialStep>& materialSteps)
{
  // sum up all material at this point for this surface
  float tThickness = 0.;
  float tRho       = 0.;
  float tA         = 0.;
  float tZ         = 0.;
  float ttInX0     = 0.;
  float ttInL0     = 0.;

  // loop over the steps and add it up
  for (auto& layerStep : materialSteps) {
    // thickness and density
    float t       = layerStep.materialProperties().thickness();
    float density = layerStep.materialProperties().averageRho();
    
    // sum it up
    tThickness += t;
    tRho += density * t;
    tA += layerStep.materialProperties().averageA() * density * t;
    tZ += layerStep.materialProperties().averageZ() * density * t;
    // add the thickness in x0
    ttInX0 += layerStep.materialProperties().thicknessInX0();
    ttInL0 += layerStep.materialProperties().thicknessInL0();
  }
  // checks before normalisation
  tA /= (tRho != 0.) ? tRho : 1.;
  tZ /= (tRho != 0.) ? tRho : 1.;
  tRho /= (tThickness != 0.) ? tThickness : 1.;

  // now add it at the corresponding assigned position
  // get the bins corresponding to the position
  size_t bin0 = m_binUtility->bin(pos, 0);
  size_t bin1 = m_binUtility->bin(pos, 1);
  
  // get the material which might be there already, add new material and
  // weigh it
  auto materialBin = m_mappedMaterial[bin1][bin0];
  float                    thickness = 0.;
  float                    rho       = 0.;
  float                    tInX0     = 0.;
  float                    tInL0     = 0.;
  float                    A         = 0.;
  float                    Z         = 0.;
  // access the old material properties
  if (materialBin.first) {
    thickness += materialBin.first.thickness();
    rho += materialBin.first.averageRho();
    A += materialBin.first.averageA();
    Z += materialBin.first.averageZ();
    tInX0 += materialBin.first.thicknessInX0();
    tInL0 += materialBin.first.thicknessInL0();
  }
  // sum up material properties (different MaterialTracks)
  thickness += tThickness;
  rho       += tRho;
  A         += tA;
  Z         += tZ;
  tInX0     += ttInX0;
  tInL0     += ttInL0;

  // x0 and l0
  float x0 = (thickness != 0. && tInX0 != 0.) ? thickness / tInX0 : 0.;
  float l0 = (thickness != 0. && tInL0 != 0.) ? thickness / tInL0 : 0.;

  // set the new current material (not averaged yet)
  Material updatedMaterial(x0, l0, A, Z, rho);
  m_mappedMaterial[bin1][bin0].first = MaterialProperties(updatedMaterial, thickness);
  // increase the number of entries for this material
  m_mappedMaterial[bin1][bin0].second++;

}

