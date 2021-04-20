
/* 
 * File:   main.cpp
 * Author: Ilya Pikin
 */

#include <cstdlib>
#include <iostream>
#include <cstring>

#include "WavToCFileWriter.h"
#include "WavFileReader.h"

#define SAMPLES_IN_ROW_NUM 16

using namespace std;

void generate(WavFileReader * reader, WavToCFileWriter * writer, size_t samplesNumMax) {
    size_t samplesRead;
    float buffer[SAMPLES_IN_ROW_NUM];

    while (true) {
        if (reader->getSamplesLeft())
            reader->read(samplesNumMax > SAMPLES_IN_ROW_NUM ? SAMPLES_IN_ROW_NUM : samplesNumMax, buffer, &samplesRead);
        writer->writePortion(buffer, samplesRead);
        if (!reader->getSamplesLeft() || samplesNumMax <= SAMPLES_IN_ROW_NUM) break;
        samplesNumMax -= samplesRead;
    }
}

int process(const char * input, const char * output, size_t samplesNumMax) {
    WavFileReader reader;
    WavToCFileWriter writer;

    if (reader.open(input) != WavFileReader::Status::OK) {
        cout << "Input file read error!" << endl;
        return -1;
    }

    if (!reader.isSupported()) {
        cout << "This tool only supports 8, 16, 24 or 32 bit PCM or 32 bit IEEE FLOAT audio format." << endl;
        return -1;
    }

    if (writer.open(output) != WavToCFileWriter::Status::OK) {
        cout << "Can't create file " << output << endl;
        return -1;
    }
    writer.writeHeader(reader.getHeader(), input, samplesNumMax);
    generate(&reader, &writer, samplesNumMax);
    writer.writeEOF();

    return 0;
}

int main(int argc, char** argv) {
    size_t samplesNumMax = SIZE_MAX;

    if (argc < 3 || argc > 4) {
        cout << "Usage 1: wav-to-c <path_to_wav_file> <path_to_c_file>" << endl;
        cout << "Usage 2: wav-to-c <path_to_wav_file> <path_to_c_file> <max_samples_num>" << endl;
        return 0;
    }

    if (argc == 4) {
        try {
            samplesNumMax = stoi(argv[3]);
        } catch (invalid_argument e) {
            cout << "Invalid argument!" << endl;
            return -1;
        } catch (out_of_range e) {
            cout << "Invalid argument!" << endl;
            return -1;
        }
    }

    return process(argv[1], argv[2], samplesNumMax);
}

