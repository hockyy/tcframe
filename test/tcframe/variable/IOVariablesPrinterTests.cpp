#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../format/FakeIOSegment.hpp"
#include "../format/MockLineIOSegmentPrinter.hpp"
#include "tcframe/variable/IOVariablesPrinter.hpp"

using ::testing::InSequence;
using ::testing::Test;

using std::ostringstream;

namespace tcframe {

class IOVariablesPrinterTests : public Test {
protected:
    Mock(LineIOSegmentPrinter) lineIOSegmentPrinter;
    IOSegment* segmentA = new FakeIOSegment(IOSegmentType::LINE);
    IOSegment* segmentB = new FakeIOSegment(IOSegmentType::LINE);
    IOFormat ioFormat = IOFormatBuilder()
            .prepareForInputFormat()
            .addIOSegment(segmentA)
            .addIOSegment(segmentB)
            .build();

    ostream* out = new ostringstream();

    IOVariablesPrinter printer = IOVariablesPrinter(&lineIOSegmentPrinter, ioFormat);
};

TEST_F(IOVariablesPrinterTests, Printing) {
    {
        InSequence sequence;

        EXPECT_CALL(lineIOSegmentPrinter, print(reinterpret_cast<LineIOSegment*>(segmentA), out));
        EXPECT_CALL(lineIOSegmentPrinter, print(reinterpret_cast<LineIOSegment*>(segmentB), out));
    }

    printer.printInput(out);
}

}
