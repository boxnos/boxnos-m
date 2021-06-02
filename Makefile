SHELL=/bin/bash
# target=hello.efi
ovmf=edk2/Build/OvmfX64/DEBUG_CLANG38/FV/
loader=edk2/Build/loaderX64/DEBUG_CLANG38/X64/loader.efi
kernel=kernel/kernel.elf
lib=x86_64-elf
export CPPFLAGS=-I$(PWD)/$(lib)/include/c++/v1 -I$(PWD)/$(lib)/include -I$(PWD)/$(lib)/freetype -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL
export LDFLAGS=-L$(PWD)/$(lib)/lib -lc++ -lm

run-qemu: $(ovmf)/OVMF_CODE.fd $(ovmf)/OVMF_VARS.fd subs disk.img
	qemu-system-x86_64 \
		-monitor stdio \
		-drive if=pflash,format=raw,file=$(ovmf)OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=$(ovmf)OVMF_VARS.fd \
		-hda disk.img

# %.fd: /usr/share/OVMF
#	cp /usr/share/OVMF/*.fd .
#%.fd: osbook
#	cp $</devenv/$@ .

hello.efi: src/hello.c
	clang -target x86_64-pc-win32-coff \
		-mno-red-zone -fno-stack-protector \
		-fshort-wchar -Wall \
		-c $<
	lld-link /subsystem:efi_application /entry:EfiMain /out:$@ hello.o

disk.img: $(loader) $(kernel)
	qemu-img create -f raw $@ 200M
	mkfs.fat -n 'BOXNOS-M' -s 2 -f 2 -R 32 -F 32 $@
	mmd -i $@ EFI
	mmd -i $@ EFI/BOOT
	mcopy -i $@ $(loader) ::EFI/BOOT/BOOTX64.EFI
	mcopy -i $@ $(kernel) ::

subs: $(lib)
	make -e -C kernel

define build
	cd edk2
	source edksetup.sh --reconfig
	build -p $1 -a X64 -t CLANG38 |& grep -v Build.*time:
	exit $${PIPESTATUS[0]}
endef

.ONESHELL:
$(ovmf)%.fd: edk2
	$(call build,OvmfPkg/OvmfPkgX64.dsc)

.ONESHELL:
edk2/Build/loaderX64/DEBUG_CLANG38/X64/loader.efi: edk2 edk2/pkg/loader loader/loader.dsc loader/loader.inf loader/main.c
	$(call build,pkg/loader/loader.dsc)

edk2/pkg/loader: loader edk2
	cd edk2/pkg; ln -s ../../$< loader

edk2: edk2_bak
	cp -r $< $@
	mkdir $@/pkg

osbook: osbook_bak
	cp -r $< $@

$(lib): lib_bak
	tar -zxvf lib_bak/x86_64-elf.tar.gz
	touch $(lib)

edk2_bak:
	git clone --recursive https://github.com/tianocore/edk2.git -b edk2-stable202102 $@
	cd $@; make -C BaseTools

osbook_bak:
	git clone https://github.com/uchan-nos/mikanos-build.git $@

lib_bak:
	mkdir $@
	cd $@; wget -q https://github.com/uchan-nos/mikanos-build/releases/download/v2.0/x86_64-elf.tar.gz

clean:
	rm -f *.o
	rm -f disk.img
	rm -f edk2/pkg/loader
	rm -rf edk2/Build/loaderX64
	make -C kernel clean

clean-all: clean
	rm -rf edk2
	rm -rf osbook
	rm -rf $(lib)

destroy: clean_all
	rm -rf *_bak
