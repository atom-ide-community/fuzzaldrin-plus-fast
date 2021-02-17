#ifndef Zadeh_TreeFilterer_H
#define Zadeh_TreeFilterer_H

#include "common.h"
#include "data_interface.h"
#include "options.h"
#include "filter.h"

namespace zadeh {

template<typename ArrayType, typename ElementType = CandidateString>
class ArrayFilterer {
  private:
    vector<std::vector<CandidateString>> partitioned_candidates{};
    ArrayType candidates_view;    // TODO use a reference or a raw pointer?

  public:
    ArrayFilterer() = default;

    ArrayFilterer(ArrayType &&candidates) {
        set_candidates(forward<ArrayType>(candidates));
    }

    auto set_candidates(const ArrayType &candidates) {

        const auto N = get_size(candidates);
        const auto num_chunks = get_num_chunks(N);


        partitioned_candidates.clear();
        partitioned_candidates.resize(num_chunks);

        auto cur_start = 0u;
        for (auto i = 0u; i < num_chunks; i++) {

            auto chunk_size = N / num_chunks;
            // Distribute remainder among the chunks.
            if (i < N % num_chunks) {
                chunk_size++;
            }
            for (auto j = cur_start; j < cur_start + chunk_size; j++) {
                partitioned_candidates[i].emplace_back(get_at<ArrayType, ElementType>(candidates, j));
            }
            cur_start += chunk_size;
        }

        // store a view of candidates in case filter was called
        candidates_view = candidates;
    }


    auto filter_indices(const std::string &query, const size_t maxResults = 0, const bool usePathScoring = true, const bool useExtensionBonus = false) {
        // optimization for no candidates
        if (partitioned_candidates.empty()) {
            return vector<size_t>();
        }

        const Options options(query, maxResults, usePathScoring, useExtensionBonus);
        return zadeh::filter(partitioned_candidates, query, options);
    }

    auto filter(const std::string &query, const size_t maxResults = 0, const bool usePathScoring = true, const bool useExtensionBonus = false) {
        if (candidates_view == nullptr) {
            return init<ElementType>(0);    // return an empty vector (should we throw?)
        }

        const auto filtered_indices = filter_indices(query, maxResults, usePathScoring, useExtensionBonus);

        const auto filter_indices_length = filtered_indices.size();
        auto res = init<ElementType>(filter_indices_length);
        for (size_t i = 0; i < filter_indices_length; i++) {
            push_back(res, get_at<ArrayType, ElementType>(candidates_view, filtered_indices[i]));
        }
        return res;
    }
};


}    // namespace zadeh
#endif
