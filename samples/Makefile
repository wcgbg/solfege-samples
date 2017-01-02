CHENGU_SONGS = minuet-in-g-major twinkle
DAISY_SONGS = minuet-in-g-major twinkle
JENNIFER_SONGS = minuet-in-g-major twinkle
KATY_SONGS = twinkle

ALL_OGGS = $(patsubst %,chengu/songs/%.ogg,$(CHENGU_SONGS)) \
           $(patsubst %,daisy/songs/%.ogg,$(DAISY_SONGS)) \
           $(patsubst %,jennifer/songs/%.ogg,$(JENNIFER_SONGS)) \
           $(patsubst %,katy/songs/%.ogg,$(KATY_SONGS))

.PHONY: all
all: $(ALL_OGGS)

%.ogg: %.wav
	sox $< -C 8 --comment "Title=$(@:.ogg=)" $@ remix 1,2 norm

CHENGU_SF2 = chengu/solfege-chengu.12.sf2
JENNIFER_SF2 = jennifer/solfege-jennifer.sf2
KATY_SF2 = katy/solfege-katy.sf2

chengu/songs/%.wav: ../midi/%.mid
	fluidsynth -g 0.7 -F $@ $< chengu/solfege-chengu.12.sf2

daisy/songs/%.wav: ../midi/%.mid
	fluidsynth -g 0.7 -F $@ $< daisy/solfege-daisy.sf2

jennifer/songs/%.wav: ../midi/%.mid
	fluidsynth -g 0.7 -F $@ $< jennifer/solfege-jennifer.sf2

katy/songs/%.wav: ../midi/%.mid
	fluidsynth -g 0.7 -F $@ $< katy/solfege-katy.sf2

.PHONY: clean
clean:
	rm -f $(ALL_OGGS) $(ALL_OGGS:.ogg=.wav)
