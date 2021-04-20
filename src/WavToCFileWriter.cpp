
/* 
 * File:   WavToCFileWriter.cpp
 * Author: Ilya Pikin
 */

#include <cstring>
#include <experimental/filesystem>
#include <iomanip>

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
    audioFormat = header->audioFormat;
    totalSamplesNum = header->subchunk2Size / bytesPerSample;
    std::string audioFormatDesc = " (unsupported)";

    if (header->audioFormat == WAV_FILE_AUDIO_FORMAT_PCM)
        audioFormatDesc = " (PCM)";
    if (header->audioFormat == WAV_FILE_AUDIO_FORMAT_IEEE_FLOAT)
        audioFormatDesc = " (IEEE_FLOAT)";

    file << "/*********************" << std::endl;
    file << " * " << std::experimental::filesystem::path(fileName).filename() << std::endl;
    file << " * audioFormat: " << header->audioFormat << audioFormatDesc << std::endl;
    file << " * sampleRate: " << header->sampleRate << std::endl;
    file << " * numChannels: " << header->numChannels << std::endl;
    file << " * bytesPerSample: " << bytesPerSample << std::endl;
    file << " * samplesNum: " << (totalSamplesNum < samplesNumMax ? totalSamplesNum : samplesNumMax) << std::endl;
    file << "*********************/" << std::endl << std::endl;

    if (audioFormat == WAV_FILE_AUDIO_FORMAT_PCM) {
        if (bytesPerSample == 1)
            file << "const uint8_t DATA[] = [" << std::endl;
        else if (bytesPerSample == 2)
            file << "const int16_t DATA[] = [" << std::endl;
        else if (bytesPerSample == 3 || bytesPerSample == 4)
            file << "const int32_t DATA[] = [" << std::endl;
    } else if (audioFormat == WAV_FILE_AUDIO_FORMAT_IEEE_FLOAT) {
        if (bytesPerSample == 4)
            file << "const float DATA[] = [" << std::endl;
    }
    return OK;
}

WavToCFileWriter::Status WavToCFileWriter::writePortion(float * data, size_t samplesNum) {
    file << "  ";
    if (audioFormat == WAV_FILE_AUDIO_FORMAT_PCM) {
        if (bytesPerSample == 1)
            while (samplesNum--)
                file << (int32_t) (*data++ * INT8_MAX) << ", ";
        else if (bytesPerSample == 2)
            while (samplesNum--)
                file << (int32_t) (*data++ * INT16_MAX) << ", ";
        else if (bytesPerSample == 3)
            while (samplesNum--)
                file << (int32_t) (*data++ * INT24_MAX) << ", ";
        else if (bytesPerSample == 4)
            while (samplesNum--)
                file << (int32_t) (*data++ * INT32_MAX) << ", ";
    } else if (audioFormat == WAV_FILE_AUDIO_FORMAT_IEEE_FLOAT) {
        if (bytesPerSample == 4)
            while (samplesNum--)
                file << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10)
                << *data++ << "f, ";
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
