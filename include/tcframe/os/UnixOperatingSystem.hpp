#pragma once

#include <fstream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include "tcframe/os/ExecutionResult.hpp"
#include "tcframe/os/OperatingSystem.hpp"

using std::ifstream;
using std::istream;
using std::istringstream;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::string;

namespace tcframe {

class UnixOperatingSystem : public OperatingSystem {
public:
    istream* openForReading(const string& filename) {
        ifstream* file = new ifstream();
        file->open(filename);
        return file;
    }

    ostream* openForWriting(const string& filename) {
        ofstream* file = new ofstream();
        file->open(filename);
        return file;
    }

    void closeOpenedWritingStream(ostream* out) {
        delete out;
    }

    void forceMakeDir(const string& dirName) {
        system(("rm -rf " + dirName).c_str());
        system(("mkdir -p " + dirName).c_str());
    }

    void removeFile(const string& filename) {
        system(("rm -f " + filename).c_str());
    }

    ExecutionResult execute(
            const string& command,
            const string& inputFilename,
            const string& outputFilename,
            const string& errorFilename) {

        ostringstream sout;

        sout << command;

        if (!inputFilename.empty()) {
            sout << " < " << inputFilename;
        }
        if (outputFilename.empty()) {
            sout << " > /dev/null";
        } else {
            sout << " > " << outputFilename;
        }
        if (errorFilename.empty()) {
            sout << " 2> /dev/null";
        } else {
            sout << " 2> " << errorFilename;
        }

        int exitCode = system(sout.str().c_str());
        int exitStatus = WEXITSTATUS(exitCode);

        istream* outputStream;
        istream* errorStream;

        if (outputFilename.empty()) {
            outputStream = new istringstream();
        } else {
            outputStream = openForReading(outputFilename);
        }

        if (errorFilename.empty()) {
            errorStream = new istringstream();
        } else {
            errorStream = openForReadingAsStringStream(errorFilename);
        }

        return ExecutionResult(exitCode, outputStream, errorStream);
    }

private:
    istringstream* openForReadingAsStringStream(const string& filename) {
        ifstream file(filename);

        ostringstream buffer;
        buffer << file.rdbuf();

        removeFile(filename);

        return new istringstream(buffer.str());
    }
};

}