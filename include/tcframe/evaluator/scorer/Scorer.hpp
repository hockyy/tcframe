#pragma once

#include <string>

#include "ScoringResult.hpp"

using std::string;

namespace tcframe {

class Scorer {
public:
    static constexpr const char* SCORING_OUT_FILENAME = "_scoring.out";

    virtual ~Scorer() {}

    virtual ScoringResult score(
            const string& inputFilename,
            const string& outputFilename,
            const string& evaluationFilename) = 0;
};

}
