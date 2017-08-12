#ifndef _H_BRAXTYPE_H_
#define _H_BRAXTYPE_H_

#include <memory>

//Remove when C++17 is available
template<typename... Bs>
struct conjunction : std::true_type {};

template<typename B1, typename... Bs>
struct conjunction<B1, Bs...>
    : std::conditional<B1::value, conjunction<Bs...>, std::false_type>::type
{};

template<typename... Bs>
constexpr bool conjunction_v = conjunction<Bs...>::value;


template<typename... Bs>
struct disjunction : std::false_type {};

template<typename B1, typename... Bs>
struct disjunction<B1, Bs...>
    : std::conditional<B1::value, std::true_type, disjunction<Bs...>>::type
{};

template<typename... Bs>
constexpr bool disjunction_v = disjunction<Bs...>::value;


template<typename... Ts> struct make_void { typedef void type; };
template<typename... Ts> using void_t = typename make_void<Ts...>::type;


template<typename T>
struct identity
{
    using type = T;
};


template<typename T, typename = void_t<>>
struct hasValueTypeMember : std::false_type {};

template<typename T>
struct hasValueTypeMember<T, void_t<typename T::value_type>>
    : std::true_type {};

template<typename T>
constexpr bool hasValueTypeMember_v = hasValueTypeMember<T>::value;

    
template<typename T, typename = void_t<>>
struct hasDifferenceTypeMember : std::false_type {};

template<typename T>
struct hasDifferenceTypeMember<T, void_t<typename T::difference_type>>
    : std::true_type {};

template<typename T>
constexpr bool hasDifferenceTypeMember_v = hasDifferenceTypeMember<T>::value;


template<typename T, typename = void_t<>>
struct hasReferenceMember : std::false_type {};

template<typename T>
struct hasReferenceMember<T, void_t<typename T::reference>>
    : std::true_type {};

template<typename T>
constexpr bool hasReferenceMember_v = hasReferenceMember<T>::value;


template<typename T, typename = void_t<>>
struct hasConstReferenceMember : std::false_type {};

template<typename T>
struct hasConstReferenceMember<T, void_t<typename T::const_reference>>
    : std::true_type {};

template<typename T>
constexpr bool hasConstReferenceMember_v = hasConstReferenceMember<T>::value;


template<typename T, typename = void_t<>>
struct hasPointerMember : std::false_type {};

template<typename T>
struct hasPointerMember<T, void_t<typename T::pointer>>
    : std::true_type {};

template<typename T>
constexpr bool hasPointerMember_v = hasPointerMember<T>::value;


//Every Type seem to be erasable
template<typename T, typename Allocator, typename = void_t<>>
struct isErasable : std::false_type {};

template<typename T, typename Allocator>
struct isErasable<
        T,
        Allocator,
        void_t<decltype(std::allocator_traits<Allocator>::destroy(
            std::declval<Allocator&>(),
            std::declval<T*>()))>>
    : std::true_type {};

template<typename T, typename Allocator>
constexpr bool isErasable_v = isErasable<T, Allocator>::value;


template<typename T>
using Dereferenciation = decltype(*std::declval<T>());

template<typename T, typename = void_t<>>
struct isDereferenceable : std::false_type {};

template<typename T>
struct isDereferenceable<T, void_t<Dereferenciation<T>>>
    : std::true_type {};

template<typename T>
constexpr bool isDereferenceable_v = isDereferenceable<T>::value;


//TODO: Find why int ++std::declval<int>() is ill-formed
//(expression is not assignable)
template<typename T>
using LeftIncrementation = decltype(++std::declval<T>());

template<typename T, typename = void_t<>>
struct isLeftIncrementable : std::false_type {};

template<typename T>
struct isLeftIncrementable<T, void_t<LeftIncrementation<T>>>
    : std::true_type {};

template<typename T>
constexpr bool isLeftIncrementable_v = isLeftIncrementable<T>::value;


template<typename T, typename = void>
struct isCoherentlyLeftIncrementable : std::false_type {};

template<typename T>
struct isCoherentlyLeftIncrementable<
        T,
        std::enable_if_t<isLeftIncrementable_v<T>>>
    : std::is_same<std::add_lvalue_reference_t<T>, LeftIncrementation<T>>
{};

template<typename T>
constexpr bool isCoherentlyLeftIncrementable_v
    = isCoherentlyLeftIncrementable<T>::value;


template<typename T>
using RightIncrementation = decltype(std::declval<T>()++);

template<typename T, typename = void_t<>>
struct isRightIncrementable : std::false_type {};

template<typename T>
struct isRightIncrementable<T, void_t<RightIncrementation<T>>>
    : std::true_type {}; 

template<typename T>
constexpr bool isRightIncrementable_v = isRightIncrementable<T>::value;

template<typename T, typename = void_t<>>
struct isCoherentlyRightIncrementable : std::false_type {};

template<typename T>
struct isCoherentlyRightIncrementable<
        T,
        std::enable_if_t<isRightIncrementable_v<T>>>
    : std::is_same<std::remove_reference_t<T>, RightIncrementation<T>>
{};

template<typename T>
constexpr bool isCoherentlyRightIncrementable_v
    = isCoherentlyRightIncrementable<T>::value;


template<typename T, typename U = T>
using Swap = decltype(std::swap(std::declval<T&>(), std::declval<U&>()));

template<typename T, typename U = T, typename = void_t<>>
struct isSwappable : std::false_type {};

template<typename T, typename U>
struct isSwappable<T, U, void_t<Swap<T, U>>> : std::true_type {};

template<typename T, typename U = T>
constexpr bool isSwappable_v = isSwappable<T, U>::value;


template<typename T, typename U = T>
using EqualityComparison = decltype(std::declval<T>() == std::declval<U>());

template<typename T, typename U = T, typename = void_t<>>
struct isEqualityComparable : std::false_type {};

template<typename T, typename U>
struct isEqualityComparable<T, U, void_t<EqualityComparison<T, U>>>
    : std::is_convertible<EqualityComparison<T, U>, bool> {};

template<typename T, typename U = T>
constexpr bool isEqualityComparable_v = isEqualityComparable<T>::value;


template<typename T, typename U = T>
using InequalityComparison = decltype(std::declval<T>() != std::declval<U>());

template<typename T, typename U = T, typename = void_t<>>
struct isInequalityComparable : std::false_type {};

template<typename T, typename U>
struct isInequalityComparable<T, U, void_t<InequalityComparison<T, U>>>
    : std::is_convertible<InequalityComparison<T, U>, bool> {};

template<typename T, typename U = T>
constexpr bool isInequalityComparable_v = isInequalityComparable<T, U>::value;


template<typename T, typename = void_t<>>
struct hasOperatorArrow : std::false_type {};

template<typename T>
struct hasOperatorArrow<T, void_t<decltype(std::declval<T>().operator->())>>
    : std::true_type {};

template<typename T>
constexpr bool hasOperatorArrow_v = hasOperatorArrow<T>::value;


template<typename T>
struct isArrowable : disjunction<std::is_pointer<T>, hasOperatorArrow<T>> {};

template<typename T>
constexpr bool isArrowable_v = isArrowable<T>::value;


template<typename T, typename = void_t<>>
struct hasIteratorCategoryMember : std::false_type {};

template<typename T>
struct hasIteratorCategoryMember<T, void_t<typename T::iterator_category>>
    : std::true_type {};

template<typename T>
constexpr bool hasIteratorCategoryMember_v
    = hasIteratorCategoryMember<T>::value;


template<typename T>
struct hasIteratorTraitsValueTypeMember
    : hasValueTypeMember<std::iterator_traits<T>> {};

template<typename T>
constexpr bool hasIteratorTraitsValueTypeMember_v
    = hasIteratorTraitsValueTypeMember<T>::value;


template<typename T>
struct getIteratorTraitsValueType :
    identity<typename std::iterator_traits<T>::value_type> {};

template<typename T>
using getIteratorTraitsValueType_t
    = typename getIteratorTraitsValueType<T>::type;


template<typename T>
struct hasIteratorTraitsReferenceMember
    : hasReferenceMember<std::iterator_traits<T>> {};

template<typename T>
constexpr bool hasIteratorTraitsReferenceMember_v
    = hasIteratorTraitsReferenceMember<T>::value;


template<typename T>
struct getIteratorTraitsReference
    : identity<typename std::iterator_traits<T>::reference> {};

template<typename T>
using getIteratorTraitsReference_t
    = typename getIteratorTraitsReference<T>::type;


template<typename T>
struct hasIteratorTraitsIteratorCategoryMember
    : hasIteratorCategoryMember<std::iterator_traits<T>> {};

template<typename T>
constexpr bool hasIteratorTraitsIteratorCategoryMember_v
    = hasIteratorTraitsIteratorCategoryMember<T>::value;


template<typename T>
struct getIteratorTraitsIteratorCategory
    : identity<typename std::iterator_traits<T>::iterator_category> {};

template<typename T>
using getIteratorTraitsIteratorCategory_t
    = typename getIteratorTraitsIteratorCategory<T>::type;


template<typename T>
struct hasIteratorTraitsDifferenceTypeMember
    : hasDifferenceTypeMember<std::iterator_traits<T>> {};

template<typename T>
constexpr bool hasIteratorTraitsDifferenceTypeMember_v
    = hasIteratorTraitsDifferenceTypeMember<T>::value;


template<typename T>
struct getIteratorTraitsDifferenceType
    : identity<typename std::iterator_traits<T>::difference_type> {};

template<typename T>
using getIteratorTraitsDifferenceType_t
    = typename getIteratorTraitsDifferenceType<T>::type;


template<typename T>
struct hasIteratorTraitsPointerMember
    : hasPointerMember<std::iterator_traits<T>> {};

template<typename T>
constexpr bool hasIteratorTraitsPointerMember_v
    = hasIteratorTraitsPointerMember<T>::value;


template<typename T>
struct getIteratorTraitsPointer
    : identity<typename std::iterator_traits<T>::pointer> {};

template<typename T>
using getIteratorTraitsPointer_t = typename getIteratorTraitsPointer<T>::type;


template<typename T, typename = void_t<>>
struct isIteratorRightIncrementable : std::false_type {};

template<typename T>
struct isIteratorRightIncrementable<T, void_t<
        std::enable_if_t<isRightIncrementable_v<T>>,
        std::enable_if_t<isDereferenceable_v<RightIncrementation<T>>>,
        std::enable_if_t<hasIteratorTraitsValueTypeMember_v<T>>>>
    : std::is_convertible<
        Dereferenciation<RightIncrementation<T>>,
        getIteratorTraitsValueType_t<T>>
{};


template<typename T, typename = void>
struct isIteratorDereferenceable : std::false_type {};

template<typename T>
struct isIteratorDereferenceable<
        T,
        std::enable_if_t<conjunction_v<
            isDereferenceable<T>,
            hasIteratorTraitsReferenceMember<T>,
            hasIteratorTraitsValueTypeMember<T>>>>
    : conjunction<
        std::is_same<
            Dereferenciation<T>,
            getIteratorTraitsReference_t<T>>,
        std::is_convertible<
            getIteratorTraitsReference_t<T>,
            getIteratorTraitsValueType_t<T>>>
{};

template<typename T>
constexpr bool isIteratorDereferenceable_v
    = isIteratorDereferenceable<T>::value;


template<typename T>
struct isIterator : conjunction<
    std::is_copy_constructible<T>,
    std::is_copy_assignable<T>,
    std::is_destructible<T>,
    isSwappable<T>,
    hasIteratorTraitsValueTypeMember<T>,
    hasIteratorTraitsDifferenceTypeMember<T>,
    hasIteratorTraitsReferenceMember<T>,
    hasIteratorTraitsPointerMember<T>,
    hasIteratorTraitsIteratorCategoryMember<T>,
    isDereferenceable<T>,
    isLeftIncrementable<T> >
{};

template<typename T>
constexpr bool isIterator_v = isIterator<T>::value;


template<typename T>
struct isInputIterator : conjunction<
    isIterator<T>,
    isEqualityComparable<T>,
    isInequalityComparable<T>,
    isIteratorDereferenceable<T>,
    isArrowable<T>,
    isCoherentlyLeftIncrementable<T>,
    isIteratorRightIncrementable<T>>
{};

template<typename T>
constexpr bool isInputIterator_v = isInputIterator<T>::value;


template<typename T, typename = void>
struct isForwardIteratorRightIncrementable : std::false_type {};

template<typename T>
struct isForwardIteratorRightIncrementable<T, void_t<
        std::enable_if_t<isRightIncrementable_v<T>>,
        std::enable_if_t<isDereferenceable_v<T>>,
        std::enable_if_t<hasIteratorTraitsReferenceMember_v<T>>>>
    : conjunction<
        std::is_same<
            Dereferenciation<T>,
            getIteratorTraitsReference_t<T>>,
        std::is_same<
            RightIncrementation<T>,
            T>>
{};

template<typename T>
constexpr bool isForwardIteratorRightIncrementable_v
    = isForwardIteratorRightIncrementable<T>::value;


template<typename T, typename = void>
struct hasValidForwardIteratorTraits : std::false_type {};

template<typename T>
struct hasValidForwardIteratorTraits<T, void_t<std::enable_if_t<conjunction_v<
        hasIteratorTraitsReferenceMember<T>,
        hasIteratorTraitsValueTypeMember<T>>>>>
    : std::is_same<
        std::add_lvalue_reference_t<getIteratorTraitsValueType_t<T>>,
        std::remove_const_t<getIteratorTraitsReference_t<T>>>
{};

template<typename T>
constexpr bool hasValidForwardIteratorTraits_v
    = hasValidForwardIteratorTraits<T>::value;
        

template<typename T>
struct isForwardIterator : conjunction<
    isInputIterator<T>,
    hasValidForwardIteratorTraits<T>,
    isForwardIteratorRightIncrementable<T>>
{};

template<typename T>
constexpr bool isForwardIterator_v = isForwardIterator<T>::value;


template<typename T, typename = void>
struct hasAllocatorTypeMember : std::false_type {};

template<typename T>
struct hasAllocatorTypeMember<T, void_t<typename T::allocator_type>>
    : std::true_type
{};

template<typename T>
constexpr bool hasAllocatorTypeMember_v = hasAllocatorTypeMember<T>::value;
    
    
template<typename T, typename X, typename = void>
struct isMoveInsertable : std::false_type {};


template<typename A, typename T>
using MoveConstruction = decltype(std::allocator_traits<A>::construct(
    std::declval<A&>(),
    std::declval<T*>(),
    std::declval<T&&>()));

template<typename T, typename X>
struct isMoveInsertable<
        T,
        X,
        void_t<
            std::enable_if_t<hasAllocatorTypeMember_v<X>>,
            MoveConstruction<typename X::allocator_type, T>>>
    : std::true_type
{};

template<typename T, typename X>
struct isMoveInsertable<
        T,
        X,
        void_t<
            std::enable_if_t<!hasAllocatorTypeMember_v<X>>,
            MoveConstruction<std::allocator<T>, T>>>
    : std::true_type
{};

template<typename T, typename X>
constexpr bool isMoveInsertable_v = isMoveInsertable<T, X>::value;
        

template<typename A, typename T>
using CopyConstruction = decltype(std::allocator_traits<A>::construct(
    std::declval<A&>(),
    std::declval<T*>(),
    std::declval<const T&>()));

template<typename T, typename X, typename = void>
struct isCopyInsertable : std::false_type {};

template<typename T, typename X>
struct isCopyInsertable<
        T,
        X,
        void_t<
            std::enable_if_t<hasAllocatorTypeMember_v<X>>,
            CopyConstruction<typename X::allocator_type, T>>>
    : std::true_type
{};

template<typename T, typename X>
struct isCopyInsertable<
        T,
        X,
        void_t<
            std::enable_if_t<!hasAllocatorTypeMember_v<X>>,
            CopyConstruction<std::allocator<T>, T>>>
    : std::true_type
{};

template<typename T, typename X>
constexpr bool isCopyInsertable_v = isCopyInsertable<T, X>::value;


template<typename T>
using Begin = decltype(std::declval<T>().begin());

template<typename T>
using End = decltype(std::declval<T>().end());


template<typename T, typename = void>
struct hasBeginEndForwardIterator : std::false_type {};

template<typename T>
struct hasBeginEndForwardIterator<
        T,
        void_t<
            Begin<T>,
            End<T>,
            std::enable_if_t<isForwardIterator_v<Begin<T>>>,
            std::enable_if_t<isForwardIterator_v<End<T>>>>>
    : std::is_same<Begin<T>, End<T>>
{};

template<typename T>
constexpr bool hasBeginEndForwardIterator_v = hasBeginEndForwardIterator<T>::value;


template<typename T, typename = void>
struct isOutStreamable : std::false_type {};

template<typename T>
struct isOutStreamable<
        T,
        void_t<decltype(std::declval<std::ostream&>(), std::declval<T>())>>
    : std::true_type
{};

template<typename T>
constexpr bool isOutStreamable_v = isOutStreamable<T>::value;

    
#endif
