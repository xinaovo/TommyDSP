#!/usr/bin/env python3
import wave
import struct
import sys
import os

DEFAULT_SAMPLE_RATE = 48000

def convert_wav_to_header(wav_path, header_path):
    # ---- Open WAV file ----
    try:
        wav = wave.open(wav_path, 'rb')
    except wave.Error as e:
        print(f"Error: Unable to read WAV file: {e}")
        return

    channels = wav.getnchannels()
    sample_width = wav.getsampwidth()
    sample_rate = wav.getframerate()
    sample_count = wav.getnframes()

    # ---- Validate format ----
    if channels != 2:
        print("Error: WAV file must be stereo (2 channels).")
        return

    if sample_width != 2:
        print("Error: WAV file must be 16-bit.")
        return

    if sample_rate != DEFAULT_SAMPLE_RATE:
        print(f"Error: WAV file must be {DEFAULT_SAMPLE_RATE} Hz sample rate.")
        return

    print(f"Input WAV OK: stereo, 16-bit, {sample_rate} Hz, {sample_count} frames")

    # ---- Read raw samples ----
    raw_data = wav.readframes(sample_count)
    wav.close()

    # Each frame = 2ch × 2 bytes = 4 bytes
    total_samples = sample_count * 2  # L+R per frame
    samples = struct.unpack('<' + ('h' * total_samples), raw_data)

    # ---- Generate header ----
    with open(header_path, "w") as f:
        f.write("// Auto-generated from WAV file\n")
        f.write("#pragma once\n\n")
        f.write("#include <stdint.h>\n\n")

        f.write(f"const uint32_t SAMPLE_RATE = {DEFAULT_SAMPLE_RATE};\n")
        f.write(f"const uint32_t SAMPLE_TOTAL_COUNT = {total_samples};\n\n")

        f.write("const int16_t SAMPLES[] = {\n")

        # Write interleaved L R L R ...
        for i, s in enumerate(samples):
            if i % 8 == 0:
                f.write("    ")
            f.write(f"{s}, ")
            if i % 8 == 7:
                f.write("\n")

        f.write("\n};\n")

    print(f"Header file written: {header_path}")


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 wav_to_header.py input.wav output.h")
        return

    wav_path = sys.argv[1]
    header_path = sys.argv[2]

    if not os.path.isfile(wav_path):
        print("Error: Input WAV does not exist.")
        return

    convert_wav_to_header(wav_path, header_path)


if __name__ == "__main__":
    main()
