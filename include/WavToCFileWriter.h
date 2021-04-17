
/* 
 * File:   WavToCFileWriter.cpp
 * Author: Ilya Pikin
 */

#ifndef WAVTOCFILEWRITER_H
#define WAVTOCFILEWRITER_H

#include <fstream>

#include "WavFileHeader.h"

class WavToCFileWriter {
public:
    WavToCFileWriter();
    virtual ~WavToCFileWriter();

    enum Status {
        OK = 0,
        OPEN_ERROR,
    };

    Status open(const char * path);
    Status writeHeader(WavFileHeader * header);
    Status writePortion(float * data, size_t samplesNum);
    Status writeEOF();
    void close();

private:
    size_t bytesPerSample = 0;
    std::string path;
    std::ofstream file;
private:

};

#endif /* WAVTOCFILEWRITER_H */

