
/* 
 * File:   WavToCFileWriter.cpp
 * Author: Ilya Pikin
 */

#include <cstring>
#include <experimental/filesystem>

#include "WavToCFileWriter.h"

#define INT24_MAX 8388607

WavToCFileWriter::WavToCFileWriter() {
}

WavToCFileWriter::~WavToCFileWriter() {
}

WavToCFileWriter::Status WavToCFileWriter::open(const char * path) {
    file.open(path);
    if (!file.is_open())
        return OPEN_ERROR;
    return OK;
}

WavToCFileWriter::Status WavToCFileWriter::writeHeader(WavFileHeader * header, const char * fileName, size_t samplesNumMax) {
    bytesPerSample = header->bitsPerSample / 8;
    totalSamplesNum = header->subchunk2Size / bytesPerSample;

    file << "/*********************" << std::endl;
    file << " * " << std::experimental::filesystem::path(fileName).filename() << std::endl;
    file << " * sampleRate: " << header->sampleRate << std::endl;
    file << " * numChannels: " << header->numChannels << std::endl;
    file << " * bytesPerSample: " << bytesPerSample << std::endl;
    file << " * samplesNum: " << (totalSamplesNum < samplesNumMax ? totalSamplesNum : samplesNumMax) << std::endl;
    file << "*********************/" << std::endl << std::endl;

    switch (bytesPerSample) {
        case 1: file << "const int8_t DATA[] = [" << std::endl;
            break;
        case 2: file << "const int16_t DATA[] = [" << std::endl;
            break;
        default: file << "const int32_t DATA[] = [" << std::endl;
            break;
    }
    return OK;
}

WavToCFileWriter::Status WavToCFileWriter::writePortion(float * data, size_t samplesNum) {
    file << "  ";
    switch (bytesPerSample) {
        case 1:
            while (samplesNum--)
                file << (int8_t) (*data++ * INT8_MAX) << ", ";
            break;
        case 2:
            while (samplesNum--)
                file << (int16_t) (*data++ * INT16_MAX) << ", ";
            break;
        case 3:
            while (samplesNum--)
                file << (int32_t) (*data++ * INT24_MAX) << ", ";
            break;
    }

    file << std::endl;

    return OK;
}

WavToCFileWriter::Status WavToCFileWriter::writeEOF() {
    file << "];" << std::endl;
    return OK;
}

void WavToCFileWriter::close() {

}
