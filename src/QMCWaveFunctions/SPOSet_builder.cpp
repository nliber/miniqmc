// This file is distributed under the University of Illinois/NCSA Open Source
// License. See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by: Ye Luo, yeluo@anl.gov, Argonne National Laboratory
//
// File created by: Ye Luo, yeluo@anl.gov, Argonne National Laboratory
//////////////////////////////////////////////////////////////////////////////////////


#include "QMCWaveFunctions/SPOSet_builder.h"
#include <Utilities/RandomGenerator.h>
#include "QMCWaveFunctions/einspline_spo.hpp"
#include "QMCWaveFunctions/einspline_spo_ref.hpp"

namespace qmcplusplus
{
namespace
{
    template<typename SPOSetType>
    std::unique_ptr<SPOSet>
    build_SPOSet(int nx,
                 int ny,
                 int nz,
                 int num_splines,
                 int nblocks,
                 const Tensor<OHMMS_PRECISION, 3>& lattice_b,
                 bool init_random)
    {
        std::unique_ptr<SPOSetType> spo_main(new SPOSetType);
        spo_main->set(nx, ny, nz, num_splines, nblocks);
        spo_main->Lattice.set(lattice_b);
        return spo_main;
    }
} // namespace

std::unique_ptr<SPOSet> build_SPOSet(bool useRef,
                                     int nx,
                                     int ny,
                                     int nz,
                                     int num_splines,
                                     int nblocks,
                                     const Tensor<OHMMS_PRECISION, 3>& lattice_b,
                                     bool init_random)
{
  return useRef ?
      build_SPOSet<miniqmcreference::einspline_spo_ref<OHMMS_PRECISION>>(nx, ny, nz, num_splines, nblocks, lattice_b, init_random) :
      build_SPOSet<miniqmcreference::einspline_spo    <OHMMS_PRECISION>>(nx, ny, nz, num_splines, nblocks, lattice_b, init_random);
}

namespace
{
    template<typename SPOSetType>
    std::unique_ptr<SPOSet>
    build_SPOSet_view(const SPOSet* SPOSet_main, int team_size, int member_id)
    {
        return std::unique_ptr<SPOSetType>(new SPOSetType(*static_cast<const SPOSetType*>(SPOSet_main), team_size, member_id));
    }
}

std::unique_ptr<SPOSet> build_SPOSet_view(bool useRef, const SPOSet* SPOSet_main, int team_size, int member_id)
{
    return useRef ?
        build_SPOSet_view<miniqmcreference::einspline_spo_ref<OHMMS_PRECISION>>(SPOSet_main, team_size, member_id) :
        build_SPOSet_view<miniqmcreference::einspline_spo    <OHMMS_PRECISION>>(SPOSet_main, team_size, member_id);
}

} // namespace qmcplusplus

