////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source
// License.  See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by:
// Ye Luo, yeluo@anl.gov,
//    Argonne National Laboratory
//
// File created by:
// Ye Luo, yeluo@anl.gov,
//    Argonne National Laboratory
////////////////////////////////////////////////////////////////////////////////
// -*- C++ -*-

/**
 * @file Mover.h
 * @brief Declaration of Mover class
 *
 */

#ifndef QMCPLUSPLUS_MOVER_HPP
#define QMCPLUSPLUS_MOVER_HPP

#include <Utilities/Configuration.h>
#include <Utilities/RandomGenerator.h>
#include <Particle/ParticleSet.h>
#include "QMCWaveFunctions/SPOSet.h"
#include <QMCWaveFunctions/WaveFunction.h>
#include <Particle/ParticleSet_builder.hpp>
#include <Input/pseudo.hpp>
#include <algorithm>
#include <memory>
#include <iterator>

namespace qmcplusplus
{
/**
   * @brief Container class includes all the classes required to operate walker moves
   *
   * Movers are distinct from walkers. Walkers are lightweight in memory, while
   * Movers carry additional data needed to evolve the walkers efficiently.
   * In a memory capacity limited scenario, a limited number of movers can be used to
   * handle a large amount of walkers.
   *
   * This class is used only by QMC drivers.
   */
struct Mover
{
  using RealType = QMCTraits::RealType;

  /// random number generator
  RandomGenerator<RealType> rng;
  /// electrons
  ParticleSet els;
  /// single particle orbitals
  std::unique_ptr<SPOSet> spo;
  /// wavefunction container
  WaveFunction wavefunction;
  /// non-local pseudo-potentials
  NonLocalPP<RealType> nlpp;

  /// constructor
  Mover(const uint32_t myPrime, const ParticleSet& ions) : spo(nullptr), rng(myPrime), nlpp(rng)
  {
    build_els(els, ions, rng);
  }
};

template<class T, typename TBOOL>
const std::vector<T*>
    filtered_list(const std::vector<T*>& input_list, const std::vector<TBOOL>& chosen)
{
  using filtered_type = std::vector<T*>;
  using size_type = typename filtered_type::size_type;

  filtered_type final_list;
  final_list.reserve(input_list.size());
  for (size_type iw = 0; iw != input_list.size(); ++iw)
    if (chosen[iw])
      final_list.push_back(input_list[iw]);
  final_list.shrink_to_fit();
  return final_list;
}

const std::vector<ParticleSet*> extract_els_list(const std::vector<Mover*>& mover_list)
{
  std::vector<ParticleSet*> els_list;
  els_list.reserve(mover_list.size());
  std::transform(std::begin(mover_list), std::end(mover_list), std::back_inserter(els_list),
                 [](Mover* m){ return &m->els; });
  return els_list;
}

const std::vector<SPOSet*> extract_spo_list(const std::vector<Mover*>& mover_list)
{
  std::vector<SPOSet*> spo_list;
  spo_list.reserve(mover_list.size());
  std::transform(std::begin(mover_list), std::end(mover_list), std::back_inserter(spo_list),
                 [](Mover* m){ return m->spo.get(); });
  return spo_list;
}

const std::vector<WaveFunction*> extract_wf_list(const std::vector<Mover*>& mover_list)
{
  std::vector<WaveFunction*> wf_list;
  wf_list.reserve(mover_list.size());
  std::transform(std::begin(mover_list), std::end(mover_list), std::back_inserter(wf_list),
                 [](Mover* m){ return &m->wavefunction; });
  return wf_list;
}

} // namespace qmcplusplus

#endif
