CXXFLAGS+=-O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17

kernel.elf: kernel.o graphics.o font.o
	ld.lld $(LDFLAGS) --entry kernel_main -z norelro --image-base 0x100000 --static \
	-z separate-code -o $@ $^
	ctags -R

kernel.o: kernel.cpp range.hpp graphics.hpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

graphics.o: graphics.cpp graphics.hpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

font.o: font.cpp font.hpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf tags
