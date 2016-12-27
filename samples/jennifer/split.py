#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import wave
import struct

def main():
  solfege = [(0, 'do'), (1, 'ga'), (2, 're'), (3, 'nu'), (4, 'mi'), (5, 'fa'), (6, 'jur'), (7, 'so'), (8, 'ki'), (9, 'la'), (10, 'pe'), (11, 'ti')]
  with wave.open('scale48-83.wav', 'rb') as big_file:
    assert big_file.getnchannels() == 1
    sampwidth = big_file.getsampwidth()
    framerate = big_file.getframerate()
    print(framerate)
    nframes = big_file.getnframes()
    print(nframes)
    note_begin = 48
    note_end = 84
    n_frames_note = 2 * framerate
    n_frames_note_on = int(1.75 * framerate)
    assert nframes == n_frames_note * (note_end - note_begin)
    for pitch in range(note_begin, note_end):
      note_filename = 'note%03d-%s.wav' % (pitch, solfege[pitch % 12][1])
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
