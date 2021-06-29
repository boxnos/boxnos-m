mplus.o: mplus.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

mplus.bin: mplus_f12r.bdf bdf2bin
	./bdf2bin < $< > $@

bdf2bin: bdf2bin.cpp
	g++ -o $@ $<

bdf2txt: bdf2txt.cpp
	g++ -o $@ $<

clean:
	rm -f bdf2txt bdf2bin mplus.bin mpulus.o
