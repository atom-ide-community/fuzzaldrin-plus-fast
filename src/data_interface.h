#ifndef Zadeh_data_interface_h_
#define Zadeh_data_interface_h_

#include "common.h"

namespace zadeh {


// Data Interface

template<typename ArrayType, typename ElementType, typename IndexType = size_t>
ElementType get_at(const ArrayType &candidates, const IndexType j);

template<typename ArrayType, typename ElementType, typename IndexType = size_t>
ElementType create(const ArrayType &candidates, const IndexType j);

template<typename ArrayType, typename SizeType = size_t>
SizeType get_size(const ArrayType &candidates);

template<typename ArrayType, typename SizeType = size_t>
ArrayType init(const SizeType len);

template<typename ArrayType, typename ElementType>
void push_back(ArrayType &candidates, ElementType value);

// vector<E>
template<>
CandidateString get_at(const vector<CandidateString> &candidates, const size_t j) {
    return candidates[j];
}

template<>
size_t get_size(const vector<CandidateString> &candidates) {
    return candidates.size();
}

template<typename ParentType, typename ChildType>
optional<ChildType> get_children(const ParentType &tree_object, const string &children_key);


template<typename ElementType>
vector<ElementType> init(const size_t len) {
    auto out = vector<ElementType>{};
    out.reserve(len);
    return out;
}

template<typename ElementType>
void push_back(vector<ElementType> &candidates, ElementType value) {
    candidates.push_back(value);
}

}    // namespace zadeh
#endif
