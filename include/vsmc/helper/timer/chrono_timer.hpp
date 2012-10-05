#ifndef VSMC_TIMER_CHRONO_TIMER_HPP
#define VSMC_TIMER_CHRONO_TIMER_HPP

#include <vsmc/cxx11/chrono.hpp>

namespace vsmc {

/// \brief A timer use C++11/Boost chrono
/// \ingroup Timer
///
/// \details
/// This timer use the C++11 or Boost \c chrono library. The default behavior
/// is use the Boost version. To select the C++11 version define \c
/// VSMC_USE_STD_CHRONO or define \c VSMC_HAS_CXX11LIB_CHRONO to non-zero value
class ChronoTimer
{
    public :

    ChronoTimer () : time_(0), running_(false) {}

    void start () const
    {
        if (!running_) {
            running_ = true;
            last_ = cxx11::chrono::system_clock::now();
        }
    }

    void stop () const
    {
        if (running_) {
            time_ += cxx11::chrono::duration<double>(
                    cxx11::chrono::system_clock::now() - last_).count();
            running_ = false;
        }
    }

    void reset () const
    {
        time_ = 0;
        running_ = false;
    }

    double duration () const
    {
        return time_;
    }

    private :

    mutable double time_;
    mutable cxx11::chrono::system_clock::time_point last_;
    mutable bool running_;
}; // class ChronoTimer

} // namespace vsmc

#endif // VSMC_TIMER_CHRONO_TIMER_HPP