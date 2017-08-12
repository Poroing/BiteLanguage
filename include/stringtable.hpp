#ifndef _H_BRAXSTRINGTABLE_H_
#define _H_BRAXSTRINGTABLE_H_

#include <unordered_map>
#include <string>
#include <initializer_list>
#include <utility>
#include <type_traits>
#include <vector>
#include <iterator>
#include <algorithm>


//Create an hash table wich associate std::string to a given type
//and that can be created with cstring initializer list
template<typename Value>
class StringTable
{

    typedef std::pair<char const*, Value> cstring_pair_type;

    template<typename Iterator>
    struct is_iterator_value_type_cstring_pair :
        std::is_same<typename std::iterator_traits<Iterator>::value_type,
            cstring_pair_type >
    {};

public:
    StringTable(std::initializer_list<cstring_pair_type> init):
        internal_map(
            getInternalMapInitialValueFromCstringPair(init.begin(),
                init.end()))
    {};

    const Value& getValueAssociatedOrdefault(const std::string& key,
        const Value& default_value) const
    {
        auto value_iterator = internal_map.find(key);
        if (value_iterator == internal_map.end())
            return default_value;
        return value_iterator->second;
    };

    bool isKeyAssociatedToValue(const std::string& key, const Value& value) const
    {
        auto value_iterator = internal_map.find(key);
        return value_iterator != internal_map.end()
            && value_iterator->second == value;
    };

private:

    template<typename InputIterator>
    typename std::enable_if<
        is_iterator_value_type_cstring_pair<InputIterator>::value,
        std::unordered_map<std::string, Value>
    >::type
    getInternalMapInitialValueFromCstringPair(InputIterator begin,
        InputIterator end)
    {
        std::vector<std::pair<std::string, Value> > converted_range;
        converted_range = convertCstringPairRangeToStringPairRange(begin, end);
        return std::unordered_map<std::string, Value>(
            converted_range.begin(), converted_range.end());
    };

    template<typename InputIterator>
    typename std::enable_if<
        is_iterator_value_type_cstring_pair<InputIterator>::value,
        std::vector<std::pair<std::string, Value> >
    >::type
    convertCstringPairRangeToStringPairRange(InputIterator begin,
        InputIterator end)
    {
        std::vector<std::pair<std::string, Value> > converted_range;
        std::transform(begin, end, std::back_inserter(converted_range),
            &StringTable<Value>::convertCstringPairToStringPair);
        return converted_range;
    };

    static std::pair<std::string, Value> convertCstringPairToStringPair(
        const cstring_pair_type& cstring_pair)
    {
        return std::make_pair(std::string(cstring_pair.first), cstring_pair.second);
    };


    const std::unordered_map<std::string, Value> internal_map;
};

#endif
