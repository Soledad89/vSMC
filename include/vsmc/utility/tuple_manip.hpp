//============================================================================
// vsmc/utility/tuple_manip.hpp
//----------------------------------------------------------------------------
//
//                         vSMC: Scalable Monte Carlo
//
// This file is distribured under the 2-clauses BSD License.
// See LICENSE for details.
//============================================================================

#ifndef VSMC_UTILITY_TUPLE_MANIP_HPP
#define VSMC_UTILITY_TUPLE_MANIP_HPP

#include <vsmc/internal/common.hpp>
#include <tuple>

namespace vsmc {

template <typename, typename> struct TuplePushFront;

/// \brief Push a type to the front of a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types>
struct TuplePushFront<std::tuple<Types...>, T>
{typedef std::tuple<T, Types...> type;};

template <typename, typename> struct TuplePushBack;

/// \brief Push a type to the back of a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types>
struct TuplePushBack<std::tuple<Types...>, T>
{typedef std::tuple<Types..., T> type;};

template <typename> struct TuplePopFront;

/// \brief Remove a type from the front of a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types>
struct TuplePopFront<std::tuple<T, Types...> >
{typedef std::tuple<Types...> type;};

/// \brief Remove a type from the front of a `std::tuple` type
/// \ingroup Tuple
template <>
struct TuplePopFront<std::tuple<> > {typedef std::tuple<> type;};

template <typename> struct TuplePopBack;

/// \brief Remove a type from the back of a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types>
struct TuplePopBack<std::tuple<T, Types...> >
{
    private :

    typedef typename TuplePopBack<std::tuple<Types...> >::type tail_type;

    public :

    typedef typename TuplePushFront<tail_type, T>::type type;
}; // struct TuplePopBack

/// \brief Remove a type from the back of a `std::tuple` type
/// \ingroup Tuple
template <typename T>
struct TuplePopBack<std::tuple<T> > {typedef std::tuple<> type;};

template <>
struct TuplePopBack<std::tuple<> > {typedef std::tuple<> type;};

template <typename, std::size_t> struct TuplePopFrontN;

/// \brief Remove N types from the front of a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types, std::size_t N>
struct TuplePopFrontN<std::tuple<T, Types...>, N>
{typedef typename TuplePopFrontN<std::tuple<Types...>, N - 1>::type type;};

template <typename T, typename... Types>
struct TuplePopFrontN<std::tuple<T, Types...>, 0>
{typedef std::tuple<T, Types...> type;};

template <std::size_t N>
struct TuplePopFrontN<std::tuple<>, N> {typedef std::tuple<> type;};

template <>
struct TuplePopFrontN<std::tuple<>, 0> {typedef std::tuple<> type;};

template <typename, std::size_t> struct TuplePopBackN;

/// \brief Remove N types from the back of a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types, std::size_t N>
struct TuplePopBackN<std::tuple<T, Types...>, N>
{
    typedef typename TuplePopBackN<
        typename TuplePopBack<std::tuple<T, Types...> >::type,
                 N - 1>::type type;
}; // struct TuplePopBackN

template <typename T, typename... Types>
struct TuplePopBackN<std::tuple<T, Types...>, 0>
{typedef std::tuple<T, Types...> type;};

template <std::size_t N>
struct TuplePopBackN<std::tuple<>, N> {typedef std::tuple<> type;};

template <>
struct TuplePopBackN<std::tuple<>, 0> {typedef std::tuple<> type;};

template <typename, typename> struct TupleMerge;

/// \brief Merge two `std::tuple` types
/// \ingroup Tuple
template <typename T1, typename T2>
struct TupleMerge
{
    private :

    typedef typename TuplePushBack<T1,
        typename std::tuple_element<0, T2>::type>::type head_type;
    typedef typename TuplePopFront<T2>::type tail_type;

    public :

    typedef typename TupleMerge<head_type, tail_type>::type type;
}; // struct TupleMerge

template <typename T, typename... Types>
struct TupleMerge<std::tuple<>, std::tuple<T, Types...> >
{typedef std::tuple<T, Types...> type;};

template <typename T, typename... Types>
struct TupleMerge<std::tuple<T, Types...>, std::tuple<> >
{typedef std::tuple<T, Types...> type;};

template <>
struct TupleMerge<std::tuple<>, std::tuple<> > {typedef std::tuple<> type;};

template <typename...> struct TupleCat;

/// \brief Concatenate multiple `std::tuple` types
/// \ingroup Tuple
template <typename T1, typename T2, typename... Types>
struct TupleCat<T1, T2, Types...>
{
    private :

    typedef typename TupleMerge<T1, T2>::type head_type;

    public :

    typedef typename TupleCat<head_type, Types...>::type type;
}; // struct TupleCat

template <typename... Types>
struct TupleCat<std::tuple<Types...> > {typedef std::tuple<Types...> type;};

template <typename, template <typename> class> struct TupleFilter;

/// \brief Filter a `std::tuple` type and generate a new `std::tuple` type that
/// contains types that satisfy a condition.
/// \ingroup Tuple
template <typename T, typename... Types, template <typename> class Cond>
struct TupleFilter<std::tuple<T, Types...>, Cond>
{
    private :

    typedef typename TupleFilter<std::tuple<Types...>, Cond>::type
        tail_type;

    public :

    typedef typename cxx11::conditional<Cond<T>::value,
            typename TuplePushFront<tail_type, T>::type, tail_type>::type type;
}; // struct TupleFilter

template <template <typename> class Cond>
struct TupleFilter<std::tuple<>, Cond> {typedef std::tuple<> type;};

template <typename, template <typename> class> struct TupleTransform;

/// \brief Apply a transformation to all types in a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types, template <typename> class Trans>
struct TupleTransform<std::tuple<T, Types...>, Trans>
{
    private :

    typedef typename TupleTransform<std::tuple<Types...>, Trans>::type
        tail_type;

    public :

    typedef typename TuplePushFront<tail_type, typename Trans<T>::type>::type
        type;
}; // struct TupleTransform

template <template <typename> class Trans>
struct TupleTransform<std::tuple<>, Trans> {typedef std::tuple<> type;};

namespace internal {

template <typename, typename> struct TupleCountImpl;

template <typename T, typename... Types, typename V>
struct TupleCountImpl<std::tuple<T, Types...>, V>
{
    static VSMC_CONSTEXPR const std::size_t value =
        (cxx11::is_same<T, V>::value ? 0 : 1) +
        TupleCountImpl<std::tuple<Types...>, V>::value;
}; // struct TupleCountImpl

template <typename V>
struct TupleCountImpl<std::tuple<>, V>
{static VSMC_CONSTEXPR const std::size_t value = 0;};

} // namespace vsmc::internal

/// \brief Count the number of occurrence of a type in a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename V> struct TupleCount :
    public cxx11::integral_constant<std::size_t,
    internal::TupleCountImpl<T, V>::value> {};

namespace internal {

template <typename, template <typename> class> struct TupleCountIfImpl;

template <typename T, typename... Types, template <typename> class Cond>
struct TupleCountIfImpl<std::tuple<T, Types...>, Cond>
{
    static VSMC_CONSTEXPR const std::size_t value =
        (Cond<T>::value ? 0 : 1) +
        TupleCountIfImpl<std::tuple<Types...>, Cond>::value;
}; // struct TupleCountIfImpl

template <template <typename> class Cond>
struct TupleCountIfImpl<std::tuple<>, Cond>
{static VSMC_CONSTEXPR const std::size_t value = 0;};

} // namespace vsmc::internal

/// \brief Count the number of occurrence of a type that satisfies a condition
/// in a `std::tuple` type
/// \ingroup Tuple
template <typename TP, template <typename> class Cond> struct TupleCountIf :
    public cxx11::integral_constant<std::size_t,
    internal::TupleCountIfImpl<TP, Cond>::value> {};

namespace internal {

template <typename, typename> struct TupleFindImpl;

template <typename T, typename... Types, typename V>
struct TupleFindImpl<std::tuple<T, Types...>, V>
{
    static VSMC_CONSTEXPR const std::size_t value =
        cxx11::is_same<T, V>::value ? 0 :
        TupleFindImpl<std::tuple<Types...>, V>::value + 1;
}; // struct TupleFindImpl

template <typename V> struct TupleFindImpl<std::tuple<>, V>
{static VSMC_CONSTEXPR const std::size_t value = 0;};

} // namespace vsmc::internal

/// \brief Find the index of the first occurrence of a type in a std::tuple
/// type
/// \ingroup Tuple
template <typename TP, typename V> struct TupleFind :
    public cxx11::integral_constant<std::size_t,
    internal::TupleFindImpl<TP, V>::value> {};

namespace internal {

template <typename, template <typename> class> struct TupleFindIfImpl;

template <typename T, typename... Types, template <typename> class Cond>
struct TupleFindIfImpl<std::tuple<T, Types...>, Cond>
{
    static VSMC_CONSTEXPR const std::size_t value = Cond<T>::value ? 0 :
        TupleFindIfImpl<std::tuple<Types...>, Cond>::value + 1;
}; // struct TupleFindIfImpl

template <template <typename> class Cond>
struct TupleFindIfImpl<std::tuple<>, Cond>
{static VSMC_CONSTEXPR const std::size_t value = 0;};

} // namespace vsmc::internal

/// \brief Find the index of the first occurrence of a type that satisifies a
/// condition in a `std::tuple` type
/// \ingroup Tuple
template <typename TP, template <typename> class Cond> struct TupleFindIf :
    public cxx11::integral_constant<std::size_t,
    internal::TupleFindIfImpl<TP, Cond>::value> {};

template <typename, typename> struct TupleErase;

/// \brief Erase the first occurrence of a type from a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types, typename V>
struct TupleErase<std::tuple<T, Types...>, V>
{
    typedef typename TuplePushFront<
        typename TupleErase<std::tuple<Types...>, V>::type, T>::type type;
}; // struct TupleErase

template <typename T, typename... Types>
struct TupleErase<std::tuple<T, Types...>, T>
{typedef std::tuple<Types...> type;};

template <typename T>
struct TupleErase<std::tuple<>, T> {typedef std::tuple<> type;};

template <typename, typename> struct TupleEraseAll;

/// \brief Erase all occurrences of a type from a `std::tuple` type
/// \ingroup Tuple
template <typename T, typename... Types, typename V>
struct TupleEraseAll<std::tuple<T, Types...>, V>
{
    typedef typename TuplePushFront<
        typename TupleEraseAll<std::tuple<Types...>, V>::type, T>::type type;
}; // struct TupleEraseAll

template <typename T, typename... Types>
struct TupleEraseAll<std::tuple<T, Types...>, T>
{typedef typename TupleEraseAll<std::tuple<Types...>, T>::type type;};

template <typename T>
struct TupleEraseAll<std::tuple<>, T> {typedef std::tuple<> type;};

template <typename> struct TupleEraseDuplicate;

/// \brief Erase all duplicates of types from a `std::tuple` type
/// \ingroup
template <typename T, typename... Types>
struct TupleEraseDuplicate<std::tuple<T, Types...> >
{
    private :

    typedef typename TupleEraseDuplicate<std::tuple<Types...> >::type tp_type;

    typedef typename TupleErase<tp_type, T>::type tail_type;

    public :

    typedef typename TuplePushFront<tail_type, T>::type type;
}; // struct TupleEraseDuplicate

template <>
struct TupleEraseDuplicate<std::tuple<> > {typedef std::tuple<> type;};

template <typename> struct TupleIsUnique;

/// \brief Determine if a `std::tuple` type has unique types (no duplicates)
/// \ingroup Tuple
template <typename T, typename... Types>
struct TupleIsUnique<std::tuple<T, Types...> > :
    public cxx11::integral_constant<bool,
    TupleFind<std::tuple<Types...>, T>::value == sizeof...(Types) &&
    TupleIsUnique<std::tuple<Types...> >::value> {};

template <> struct TupleIsUnique<std::tuple<> > : public cxx11::true_type {};

template <typename, typename, typename> struct TupleReplace;

/// \brief Replace the first occurrence of a type with another in a std::tuple
/// type
/// \ingroup Tuple
template <typename T, typename... Types, typename V, typename U>
struct TupleReplace<std::tuple<T, Types...>, V, U>
{
    typedef typename TuplePushFront<
        typename TupleReplace<std::tuple<Types...>, V, U>::type, T>::type type;
}; // struct TupleReplace

template <typename T, typename... Types, typename U>
struct TupleReplace<std::tuple<T, Types...>, T, U>
{typedef typename TuplePushFront<std::tuple<Types...>, U>::type type;};

template <typename V, typename U>
struct TupleReplace<std::tuple<>, V, U> {typedef std::tuple<> type;};

template <typename, typename, typename> struct TupleReplaceAll;

/// \brief Replace all occurrences of a type with another in a `std::tuple`
/// type
/// \ingroup Tuple
template <typename T, typename... Types, typename V, typename U>
struct TupleReplaceAll<std::tuple<T, Types...>, V, U>
{
    typedef typename TuplePushFront<
        typename TupleReplaceAll<std::tuple<Types...>, V, U>::type, T>::type
        type;
}; // struct TupleReplaceAll

template <typename T, typename... Types, typename U>
struct TupleReplaceAll<std::tuple<T, Types...>, T, U>
{
    typedef typename TuplePushFront<
        typename TupleReplaceAll<std::tuple<Types...>, T, U>::type, U>::type
        type;
}; // struct TupleReplaceAll

template <typename V, typename U>
struct TupleReplaceAll<std::tuple<>, V, U> {typedef std::tuple<> type;};

} // namespace vsmc

#endif // VSMC_UTILITY_TUPLE_MANIP_HPP
