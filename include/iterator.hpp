#ifndef _H_BITEITERATOR_H_
#define _H_BITEITERATOR_H_

template<typename InternalIteratorType, typename OutType>
class MapIterator
{
    typedef iterator_traits<InternalIteratorType>::difference_type
        difference_type;
    typedef OutType value_type;
    typedef OutType* pointer;
    typedef OutType& reference;
    typedef iterator_traits<InternalIteratorType>::iterator_category
        iterator_category;

    typedef InType iterator_traits<InternalIteratorType>::value_type

public:
    MapIterator(const MapIterator<InternalIteratorType, OutType>& other):
        internal_iterator(other.internal_iterator),
        function(other.function)
    {
        initializeCurrentOut();
    };

    MapIterator(MapIterator<InternalIteratorType, OutType>&& other):
        internal_iterator(std::move(other.internal_iterator)),
        function(std::move(other.function))
    {
        initializeCurrentOut();
    };

    MapIterator(InternalIteratorType& internal_iterator,
            const std::function<OutType(InType)>& function):
        internal_iterator(internal_iterator),
        function(function)
    {
        initializeCurrentOut();
    };

    MapIterator(InternalIteratorTypeType& internal_iterator,
            std::function<OutType(InType)>&& function):
        internal_iterator(internal_iterator),
        function(std::move(function))
    {
        initializeCurrentOut();
    };

    MapIterator<InternalIteratorType, OutType>& operator=(
        MapIterator<InternalIteratorType, OutType>& other)
    {
        internal_iterator = other.internal_iterator;
        function = other.function;
        invalidateCurrentOut();
    };

    MapIterator<InternalIteratorType, OutType>& operator==(
        const MapIterator<InternalIteratorType, OutType>& other) const
    {
        return internal_iterator == other.internal_iterator
            && function == other.function;
    };

    MapIterator<InternalIteratorType, OutType>& operator!=(
        const MapIterator<InternalIteratorType, OutType>& other) const
    {
        return !operator==(other);
    };

    pointer operator->() const
    {
        return &current_out
    }

    reference operator*() const
    {
        if (!isCurrentOutValid()) {
            updateCurrentout();
        }
        return current_out;
    };

    MapIterator<InternalIteratorType, OutType>& operator++()
    {
        ++internal_iterator;
        invalidateCurrentOut();
        return this;
    };

    MapIterator<InternalIteratorType, OutType>& operator++(int)
    {
        MapIterator<InternalIteratorType, OutType>& copy = *this;
        operator++();
        return copy;
    };
        

    ~MapIterator<InternalIteratorType, OutType>()
    {};

private

    void updateCurrentout() const
    {
        current_out = function(*internal_iterator);
        is_current_out_valid = true;
    };

    void isCurrentOutValid() const
    {
        return is_current_out_valid;
    };

    void invalidateCurrentOut() const
    {
        is_current_out_valid = false;
    };

    void initializeCurrentOut() const
    {
        is_current_out_valid = false;
    };
        

    InternalIteratorType internal_iterator;
    std::function<OutType(InType)> function;
    mutable bool is_current_out_valid;
    mutable OutType current_out;
}
#endif
