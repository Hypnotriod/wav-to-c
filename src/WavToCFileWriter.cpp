
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
    this->path = path;
    return OK;
}

WavToCFileWriter::Status WavToCFileWriter::writeHeader(WavFileHeader * header) {
    bytesPerSample = header->bitsPerSample / 8;

    file << "/****************" << std::endl;
    file << " * " << std::experimental::filesystem::path(this->path).filename() << std::endl;
    file << " * sampleRate: " << header->sampleRate << std::endl;
    file << " * numChannels: " << header->numChannels << std::endl;
    file << " * bytesPerSample: " << bytesPerSample << std::endl;
    file << "****************/" << std::endl << std::endl;

    switch (bytesPerSample) {
        case 8: file << "const int8_t DATA[] = ";
            break;
        case 16: file << "const int16_t DATA[] = ";
            break;
        default: file << "const int32_t DATA[] = ";
            break;
    }
    return OK;
}

WavToCFileWriter::Status WavToCFileWriter::writePortion(float * data, size_t samplesNum) {
    switch (bytesPerSample) {
        case 8:
            while (samplesNum--)
                file << (int8_t) (*data++ * INT8_MAX) << ", " << std::endl;
            break;
        case 16:
            while (samplesNum--)
                file << (int16_t) (*data++ * INT16_MAX) << ", " << std::endl;
            break;
        case 24:
            while (samplesNum--)
                file << (int32_t) (*data++ * INT24_MAX) << ", " << std::endl;
            break;
        default:
            while (samplesNum--)
                file << (int32_t) (*data++ * INT32_MAX) << ", " << std::endl;
            break;
    }

    return OK;
}

WavToCFileWriter::Status WavToCFileWriter::writeEOF() {
    file << "];" << std::endl;
    return OK;
}

void WavToCFileWriter::close() {

}
