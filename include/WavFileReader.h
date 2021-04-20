
/* 
 * File:   WavFileReader.h
 * Author: Ilya Pikin
 */

#ifndef WAVFILEREADER_H
#define WAVFILEREADER_H

#include <stdio.h>
#include <cstdint>
#include <cstdlib>

#include "WavFileHeader.h"

class WavFileReader {
public:
    WavFileReader();
    virtual ~WavFileReader();

    enum Status {
        OK = 0,
        OPEN_ERROR,
        READ_ERROR,
        PARSE_ERROR,
        END_OF_FILE,
    };

    Status open(const char * path);
    void close();
    Status read(size_t samplesNum, float * buff, size_t * samplesRead);
    size_t getSamplesTotal();
    size_t getSamplesRead();
    size_t getSamplesLeft();
    WavFileHeader * getHeader();
    bool isSupported();

private:
    WavFileHeader header;
    FILE * file = NULL;
    size_t dataSize;

    Status parseHeader();
};

#endif /* WAVFILEREADER_H */

