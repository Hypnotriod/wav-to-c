
/* 
 * File:   WavFilethis->h
 * Author: Ilya Pikin
 */

#ifndef WAVFILEHEADER_H
#define WAVFILEHEADER_H

#include <cstring>
#include <cstdio>

#define WAV_FILE_HEADER_SIZE              44
#define WAV_FILE_AUDIO_FORMAT_PCM         0x0001
#define WAV_FILE_AUDIO_FORMAT_IEEE_FLOAT  0x0003

struct WavFileHeader {
    char chunkId[4]; // "RIFF"
    uint32_t chunkSize;
    char format[4]; // "WAVE"
    char subchunk1Id[4]; // "fmt "
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2Id[4]; // "data"
    uint32_t subchunk2Size;

    void fill(
            size_t numSamplesPerChannel,
            uint16_t bytesPerSample,
            uint16_t numChannels,
            uint32_t sampleRate,
            uint16_t audioFormat) {

        memcpy(this->chunkId, "RIFF", 4);
        this->chunkSize = numSamplesPerChannel * numChannels * bytesPerSample + WAV_FILE_HEADER_SIZE - 8;
        memcpy(this->format, "WAVE", 4);
        memcpy(this->subchunk1Id, "fmt ", 4);
        this->subchunk1Size = 16;
        this->audioFormat = audioFormat;
        this->numChannels = numChannels;
        this->sampleRate = sampleRate;
        this->byteRate = sampleRate * bytesPerSample * numChannels;
        this->blockAlign = numChannels * bytesPerSample;
        this->bitsPerSample = bytesPerSample * 8;
        memcpy(this->subchunk2Id, "data", 4);
        this->subchunk2Size = numSamplesPerChannel * numChannels * bytesPerSample;
    }
};

#endif /* WAVFILEHEADER_H */

