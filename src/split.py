#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import wave
import struct

def main():
  solfege = [(0, 'do'), (1, 'di'), (1, 'ga'), (1, 'ra'), (2, 're'), (3, 'ri'), (3, 'nu'), (3, 'me'), (4, 'mi'), (5, 'fa'), (6, 'fi'), (6, 'jur'), (6, 'se'), (7, 'so'), (8, 'si'), (8, 'ki'), (8, 'le'), (9, 'la'), (10, 'li'), (10, 'pe'), (10, 'te'), (10, 'se'), (11, 'ti'), (11, 'si'), (11, 'tsi')]
  with wave.open('scale24-120.wav', 'rb') as big_file:
    assert big_file.getnchannels() == 1
    sampwidth = big_file.getsampwidth()
    framerate = big_file.getframerate()
    print(framerate)
    nframes = big_file.getnframes()
    print(nframes)
    note_begin = 24
    note_end = 120
    assert (note_end - note_begin) % 12 == 0
    n_octaves = (note_end - note_begin) // 12
    n_frames_note = 4 * framerate
    n_frames_note_on = int(3.4 * framerate)
    assert nframes == n_frames_note * len(solfege) * n_octaves
    for o in range(n_octaves):
      for i in range(len(solfege)):
        note_filename = 'note%03d-%s.wav' % (note_begin + o * 12 + solfege[i][0], solfege[i][1])
        print(note_filename)
        note_frames = big_file.readframes(n_frames_note_on)
        with wave.open(note_filename, 'wb') as note_file:
          note_file.setnchannels(1)
          note_file.setsampwidth(sampwidth)
          note_file.setframerate(framerate)
          note_file.writeframes(note_frames)
        big_file.readframes(n_frames_note - n_frames_note_on)


if __name__ == '__main__':
  main()
