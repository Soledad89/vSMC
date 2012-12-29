#ifndef VSMC_INTERNAL_TRAITS_HPP
#define VSMC_INTERNAL_TRAITS_HPP

#include <vsmc/cxx11/random.hpp>
#include <vsmc/cxx11/type_traits.hpp>

#include <vsmc/internal/config.hpp>
#include <vsmc/internal/forward.hpp>

// Type dispatcher
#define VSMC_DEFINE_TYPE_DISPATCH_TRAIT(OuterType, InnerType, DefaultType) \
namespace vsmc { namespace traits {                                          \
                                                                             \
template <typename T>                                                        \
struct Has##OuterType##Impl                                                  \
{                                                                            \
    private :                                                                \
                                                                             \
    struct char2 {char c1; char c2;};                                        \
    template <typename U> static char test (typename U::InnerType *);        \
    template <typename U> static char2 test (...);                           \
                                                                             \
    public :                                                                 \
                                                                             \
    enum {value = sizeof(test<T>(VSMC_NULLPTR)) == sizeof(char)};            \
};                                                                           \
                                                                             \
template <typename T>                                                        \
struct Has##OuterType :                                                      \
    public cxx11::integral_constant <bool, Has##OuterType##Impl<T>::value>   \
{};                                                                          \
                                                                             \
template <typename T, bool> struct OuterType##Dispatch;                      \
                                                                             \
template <typename T> struct OuterType##Dispatch<T, true>                    \
{typedef typename T::InnerType type;};                                       \
                                                                             \
template <typename T> struct OuterType##Dispatch<T, false>                   \
{typedef DefaultType type;};                                                 \
                                                                             \
                                                                             \
template <typename T> struct OuterType##Trait                                \
{                                                                            \
    enum {value = traits::Has##OuterType<T>::value};                         \
    typedef typename traits::OuterType##Dispatch<T, value>::type type;       \
};                                                                           \
                                                                             \
} }

#define VSMC_DEFINE_MF_CHECKER(OuterMF, InnerMF, RT, Args) \
namespace vsmc { namespace traits {                                          \
                                                                             \
template <typename T>                                                        \
struct Has##OuterMF##Impl                                                    \
{                                                                            \
    private :                                                                \
                                                                             \
    struct char2 {char c1; char c2;};                                        \
    template <typename U, RT (U::*) Args> struct sfinae_;                    \
    template <typename U> static char test (sfinae_<U, &U::InnerMF> *);      \
    template <typename U> static char2 test(...);                            \
                                                                             \
    public :                                                                 \
                                                                             \
    enum {value = sizeof(test<T>(VSMC_NULLPTR)) == sizeof(char)};            \
};                                                                           \
                                                                             \
template <typename T>                                                        \
struct Has##OuterMF :                                                        \
    public cxx11::integral_constant<bool, Has##OuterMF##Impl<T>::value>      \
{};                                                                          \
                                                                             \
} }

#define VSMC_DEFINE_STATIC_MF_CHECKER(OuterMF, InnerMF, RT, Args) \
namespace vsmc { namespace traits {                                          \
                                                                             \
template <typename T>                                                        \
struct HasStatic##OuterMF##Impl                                              \
{                                                                            \
    private :                                                                \
                                                                             \
    struct char2 {char c1; char c2;};                                        \
    template <typename U, RT (*) Args> struct sfinae_;                       \
    template <typename U> static char test (sfinae_<U, &U::InnerMF> *);      \
    template <typename U> static char2 test(...);                            \
                                                                             \
    public :                                                                 \
                                                                             \
    enum {value = sizeof(test<T>(VSMC_NULLPTR)) == sizeof(char)};            \
};                                                                           \
                                                                             \
template <typename T>                                                        \
struct HasStatic##OuterMF :                                                  \
    public cxx11::integral_constant<bool, HasStatic##OuterMF##Impl<T>::value>\
{};                                                                          \
                                                                             \
} }

VSMC_DEFINE_TYPE_DISPATCH_TRAIT(SizeType, size_type, VSMC_SIZE_TYPE);
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(StateType, state_type, void);
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(DDotType, ddot_type, cxxblas::DDot<T>);
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(DGemvType, dgemv_type, cxxblas::DGemv<T>);
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(RngSetType, rng_set_type, RngSetPrl);
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(ResampleRngType, resample_rng_type,
        cxx11::mt19937);
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(OpenCLDeviceType, opencl_device_type,
        cxx11::false_type);

VSMC_DEFINE_STATIC_MF_CHECKER(CheckOpenCLVendor, check_opencl_vendor,
        bool, (const std::string &));
VSMC_DEFINE_STATIC_MF_CHECKER(CheckOpenCLDevice, check_opencl_device,
        bool, (const std::string &));

namespace vsmc { namespace traits {

// IsBaseOfState

template <template <unsigned, typename> class State, typename D>
struct IsBaseOfStateImpl
{
    private :

    struct char2 {char c1; char c2;};
    typedef typename cxx11::remove_cv<D>::type derived_type;

    template <unsigned Dim, typename T>
    static char test (State<Dim, T> *);
    static char2 test (...);

    public :

   enum {value = sizeof(test(static_cast<derived_type *>(0))) == sizeof(char)};
};

template <template <unsigned, typename> class State, typename D>
struct IsBaseOfState :
    public cxx11::integral_constant<bool, IsBaseOfStateImpl<State, D>::value>
{};

// IsBaseOfStateCL

template <typename D>
struct IsBaseOfStateCLImpl
{
    private :

    struct char2 {char c1; char c2;};
    typedef typename cxx11::remove_cv<D>::type derived_type;

    template <unsigned Dim, typename T, typename ID>
    static char test (StateCL<Dim, T, ID> *);
    static char2 test (...);

    public :

   enum {value = sizeof(test(static_cast<derived_type *>(0))) == sizeof(char)};
};

template <typename D>
struct IsBaseOfStateCL :
    public cxx11::integral_constant<bool, IsBaseOfStateCLImpl<D>::value>
{};

// CheckOpenCLVendorTrait

template <typename ID, bool> struct CheckOpenCLVendorDispatch;

template <typename ID>
struct CheckOpenCLVendorDispatch<ID, true>
{
    static bool check (const std::string &name)
    {return ID::check_opencl_vendor(name);}
};

template <typename ID>
struct CheckOpenCLVendorDispatch<ID, false>
{static bool check (const std::string &name) {return true;}};

template <typename ID>
struct CheckOpenCLVendorTrait :
    public CheckOpenCLVendorDispatch<ID, HasStaticCheckOpenCLVendor<ID>::value>
{};

// CheckOpenCLDeviceTrait

template <typename ID, bool> struct CheckOpenCLDeviceDispatch;

template <typename ID>
struct CheckOpenCLDeviceDispatch<ID, true>
{
    static bool check (const std::string &name)
    {return ID::check_opencl_device(name);}
};

template <typename ID>
struct CheckOpenCLDeviceDispatch<ID, false>
{static bool check (const std::string &name) {return true;}};

template <typename ID>
struct CheckOpenCLDeviceTrait :
    public CheckOpenCLDeviceDispatch<ID, HasStaticCheckOpenCLDevice<ID>::value>
{};

// AdaptImplTrait

template <typename, template <typename, typename> class, template <typename,
         template <typename, typename> class, typename> class, typename>
struct AdapImplTrait;

template <typename T, template <typename, typename> class Impl,
         template <typename, template <typename, typename> class, typename>
             class Adapter>
struct AdapImplTrait<T, Impl, Adapter, VBase>
{
    typedef Impl<T, VBase> type;
};

template <typename T, template <typename, typename> class Impl,
         template <typename, template <typename, typename> class, typename>
         class Adapter>
struct AdapImplTrait<T, Impl, Adapter, CBase>
{
    typedef Impl<T, Adapter<T, Impl, CBase> > type;
};

template <template <typename, typename> class>
struct IsInitializeImpl : public cxx11::false_type {};

template <template <typename, typename> class>
struct IsMoveImpl : public cxx11::false_type {};

template <template <typename, typename> class>
struct IsMonitorEvalImpl : public cxx11::false_type {};

template <template <typename, typename> class>
struct IsPathEvalImpl : public cxx11::false_type {};

// SEQ
template <>
struct IsInitializeImpl<InitializeSEQ> : public cxx11::true_type {};

template <>
struct IsMoveImpl<MoveSEQ> : public cxx11::true_type {};

template <>
struct IsMonitorEvalImpl<MonitorEvalSEQ> : public cxx11::true_type {};

template <>
struct IsPathEvalImpl<PathEvalSEQ> : public cxx11::true_type {};

// CILK
template <>
struct IsInitializeImpl<InitializeCILK> : public cxx11::true_type {};

template <>
struct IsMoveImpl<MoveCILK> : public cxx11::true_type {};

template <>
struct IsMonitorEvalImpl<MonitorEvalCILK> : public cxx11::true_type {};

template <>
struct IsPathEvalImpl<PathEvalCILK> : public cxx11::true_type {};

// OMP
template <>
struct IsInitializeImpl<InitializeOMP> : public cxx11::true_type {};

template <>
struct IsMoveImpl<MoveOMP> : public cxx11::true_type {};

template <>
struct IsMonitorEvalImpl<MonitorEvalOMP> : public cxx11::true_type {};

template <>
struct IsPathEvalImpl<PathEvalOMP> : public cxx11::true_type {};

// STD
template <>
struct IsInitializeImpl<InitializeSTD> : public cxx11::true_type {};

template <>
struct IsMoveImpl<MoveSTD> : public cxx11::true_type {};

template <>
struct IsMonitorEvalImpl<MonitorEvalSTD> : public cxx11::true_type {};

template <>
struct IsPathEvalImpl<PathEvalSTD> : public cxx11::true_type {};

// TBB
template <>
struct IsInitializeImpl<InitializeTBB> : public cxx11::true_type {};

template <>
struct IsMoveImpl<MoveTBB> : public cxx11::true_type {};

template <>
struct IsMonitorEvalImpl<MonitorEvalTBB> : public cxx11::true_type {};

template <>
struct IsPathEvalImpl<PathEvalTBB> : public cxx11::true_type {};

} } // namespace vsmc::traits

#endif // VSMC_INTERNAL_TRAITS_HPP
