# wav-to-c
Simple utility to convert wav files to c byte-array  
Supported 8, 16, 24 or 32 bit PCM or 32 bit IEEE FLOAT audio format

## Usage
```console
./wav-to-c <path_to_wav_file> <path_to_c_file>  
./wav-to-c <path_to_wav_file> <path_to_c_file> <max_samples_num>
./wav-to-c -sh <path_to_wav_file> <path_to_c_file> <max_samples_num>

-s: swap bytes (16, 24 or 32 bit PCM only)
-h: hexadecimal format (8, 16, 24 or 32 bit PCM only)
```
