CXXFLAGS+=-O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17

srcs:=kernel.cpp graphics.cpp font.cpp
objs:=$(srcs:%.cpp=%.o)
deps:=$(srcs:%.cpp=%.d)

kernel.elf: $(objs) font/mplus.o
	ld.lld $(LDFLAGS) --entry kernel_main -z norelro --image-base 0x100000 --static \
	-z separate-code -o $@ $^
	ctags -R

-include $(deps)

%.o: %.cpp
	clang++ -MMD -MP $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf tags *.d

font/mplus.o: subs

subs:
	make -C font
