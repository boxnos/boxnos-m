kernel.elf: kernel.o
	ld.lld --entry kernel_main -z norelro --image-base 0x100000 --static \
		-o $@ $<

kernel.o: kernel.cpp
	clang++ -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
		-fno-rtti -std=c++17 -c $< -o $@

clean:
	rm -f *.o *.elf
