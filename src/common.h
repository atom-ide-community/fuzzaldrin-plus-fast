#ifndef fuzzaldrin_common_h__
#define fuzzaldrin_common_h__

#include <vector>
#include <set>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <utility>

#include <napi.h>

using namespace std;

constexpr size_t kMaxThreads = 16;

#ifdef ENABLE_DEBUG
// Safe string class that logs error when index is accessed outside the string.
class SafeString : public std::string {
  public:
    SafeString() = default;
    SafeString(const std::string &s) : std::string(s) {}
    const char &operator[](size_t i) const {
        if (i >= size())
            printf("ERROR string index access index=%zu str=%s\n", i, c_str());
        return at(i);
    }
};

using Element = SafeString;
using CandidateString = SafeString;
#else
using Element = string;
using CandidateString = string;
#endif

using CandidateIndex = size_t;

using Score = float;

struct Options;

struct PreparedQuery {
    Element query;
    Element query_lw;
    Element core;
    Element core_lw;
    Element core_up;
    int depth = 0;
    Element ext;
    std::set<char> charCodes{};

    PreparedQuery(const Element &q, const char pathSeparator);
};

struct Options {
    size_t max_results = 0;
    bool allowErrors = false;
    bool usePathScoring = true;
    bool useExtensionBonus = false;
#ifdef _WIN32
    char pathSeparator = '\\';
#else
    char pathSeparator = '/';
#endif
    const PreparedQuery preparedQuery;

    Options(const Element &_query, size_t _maxResults, bool _usePathScoring, bool _useExtensionBonus) : max_results(_maxResults), usePathScoring(_usePathScoring), useExtensionBonus(_useExtensionBonus), preparedQuery(_query, pathSeparator) {}
    Options(const Element &_query, char _pathSeparator) : pathSeparator(_pathSeparator), preparedQuery(_query, _pathSeparator) {}
};

struct AcronymResult {
    Score score;
    float pos;
    int count;

    AcronymResult(Score s, float p, int c) noexcept : score(s), pos(p), count(c) {}
};

extern Element ToLower(const Element &s);
extern Element ToUpper(const Element &s);

extern bool isMatch(const CandidateString &subject, const Element &query_lw, const Element &query_up);
extern bool isWordStart(const size_t pos, const CandidateString &subject, const CandidateString &subject_lw) noexcept;
extern Score scoreCharacter(const int i, const int j, const bool start, const Score acro_score, const Score csc_score);
extern Score scoreConsecutives(const CandidateString &subject, const CandidateString &subject_lw, const Element &query, const Element &query_lw, unsigned i, unsigned j, const bool startOfWord);
extern AcronymResult scoreAcronyms(const CandidateString &subject, const CandidateString &subject_lw, const Element &query, const Element &query_lw);

extern Score computeScore(const CandidateString &subject, const CandidateString &subject_lw, const PreparedQuery &preparedQuery);

extern Score scorer_score(const CandidateString &string, const Element &query, const Options &options);
extern Score scoreSize(const Score n, const Score m);

extern Score path_scorer_score(const CandidateString &string, const Element &query, const Options &options);
extern int countDir(const CandidateString &path, const size_t end, const char pathSeparator) noexcept;
extern CandidateString getExtension(const CandidateString &str);

extern const std::vector<CandidateIndex> filter(const vector<std::vector<CandidateString>> &candidates, const Element &query, const Options &options);

extern std::vector<size_t> matcher_match(const CandidateString &string, const Element &query, const Options &options);
extern void get_wrap(const CandidateString &string, const Element &query, const Options &options, std::string *out);

#endif// fuzzaldrin_common_h__
