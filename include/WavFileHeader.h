
/* 
 * File:   WavFileHeader.h
 * Author: Ilya Pikin
 */

#ifndef WAVFILEHEADER_H
#define WAVFILEHEADER_H

#define WAV_FILE_HEADER_SIZE 44

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
};

#endif /* WAVFILEHEADER_H */

