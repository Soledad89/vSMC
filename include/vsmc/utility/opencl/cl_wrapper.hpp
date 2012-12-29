#ifndef VSMC_UTILITY_OPENCL_CL_WRAPPER_HPP
#define VSMC_UTILITY_OPENCL_CL_WRAPPER_HPP

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#if __GNUC__ >= 4 && __GNUC_MINIOR__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#endif

#include <cl.hpp>

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#if __GNUC__ >= 4 && __GNUC_MINIOR__ >= 6
#pragma GCC diagnostic pop
#endif
#endif

#endif // VSMC_UTILITY_OPENCL_CL_WRAPPER_HPP
