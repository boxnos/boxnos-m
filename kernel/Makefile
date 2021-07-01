flags:=-O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone
CFLAGS+=$(flags)
CXXFLAGS+=$(flags) -fno-exceptions -fno-rtti -std=c++17

srcc:=newlib.c
srcpp:=kernel.cpp graphics.cpp font.cpp
objs:=$(srcc:%.c=%.o) $(srcpp:%.cpp=%.o)
deps:=$(srcc:%.c=%.d) $(srcpp:%.cpp=%.d)

kernel.elf: $(objs) font/mplus.o
	ld.lld $(LDFLAGS) --entry kernel_main -z norelro --image-base 0x100000 --static \
	-z separate-code -o $@ $^
	ctags -R

-include $(deps)

%.o: %.cpp
	clang++ -MMD -MP $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	clang -MMD -MP $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf tags *.d

font/mplus.o: subs

subs:
	make -C font
