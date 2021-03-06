//============================================================================
// vSMC/include/vsmc/internal/forward.hpp
//----------------------------------------------------------------------------
//                         vSMC: Scalable Monte Carlo
//----------------------------------------------------------------------------
// Copyright (c) 2013-2015, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#ifndef VSMC_INTERNAL_FORWARD_HPP
#define VSMC_INTERNAL_FORWARD_HPP

#include <vsmc/internal/config.hpp>
#include <vsmc/internal/defines.hpp>

#define VSMC_DEFINE_SMP_FORWARD(Name) \
template <typename T, typename = Virtual> class Initialize##Name;            \
template <typename T, typename = Virtual> class Move##Name;                  \
template <typename T, typename = Virtual> class MonitorEval##Name;           \
template <typename T, typename = Virtual> class PathEval##Name;

namespace vsmc {

// Template default arguments
struct Virtual;
struct NullType;

// Core classes
template <typename> class Sampler;
template <typename> class Particle;
template <typename> class Monitor;
template <typename> class Path;
template <typename> class SingleParticle;
template <typename> class ConstSingleParticle;
template <typename> class SingleParticleBase;
template <typename> class ConstSingleParticleBase;
class WeightSet;
class NormalizingConstant;

// SMP
template <MatrixOrder, std::size_t, typename> class StateMatrix;
#if VSMC_HAS_CXX11LIB_TUPLE
template <MatrixOrder, typename, typename...> class StateTuple;
#endif

// MPI
struct MPIDefault;
template <typename = MPIDefault> class MPICommunicator;
template <typename, typename = MPIDefault> class WeightSetMPI;
template <typename, typename = MPIDefault> class StateMPI;

// OpenCL
struct CLDefault;
template <std::size_t, typename, typename = CLDefault> class StateCL;

} // namesapce vsmc

#endif // VSMC_INTERNAL_FORWARD_HPP
