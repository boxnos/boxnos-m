SHELL=/bin/bash
# target=hello.efi
target=edk2/Build/loaderX64/DEBUG_CLANG38/X64/Loader.efi

.PHONEY:
run-qemu: OVMF_CODE.fd OVMF_VARS.fd disk.img
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,file=OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=OVMF_VARS.fd \
		-hda disk.img

# %.fd: /usr/share/OVMF
#	cp /usr/share/OVMF/*.fd .
%.fd: edk2/Build/OvmfX64
	cp $</DEBUG_GCC5/FV/*.fd .
#%.fd: osbook
#	cp $</devenv/$@ .

hello.efi: src/hello.c
	clang -target x86_64-pc-win32-coff \
		-mno-red-zone -fno-stack-protector \
		-fshort-wchar -Wall \
		-c $<
	lld-link /subsystem:efi_application /entry:EfiMain /out:$@ hello.o

disk.img: $(target)
	qemu-img create -f raw $@ 200M
	mkfs.fat -n 'BOXNOS-M' -s 2 -f 2 -R 32 -F 32 $@
	mmd -i $@ EFI
	mmd -i $@ EFI/BOOT
	mcopy -i $@ $< ::EFI/BOOT/BOOTX64.EFI

define build
	cd edk2
	source edksetup.sh --reconfig
	patch -n Conf/target.txt < ../$1
	build |& grep -v Build.*time:
endef

.ONESHELL:
edk2/Build/OvmfX64: ovmf.patch edk2
	$(call build,$<)

.ONESHELL:
edk2/Build/loaderX64/DEBUG_CLANG38/X64/Loader.efi: loader.patch edk2 edk2/pkg/loader
	$(call build,$<)

edk2/pkg/loader: loader edk2
	cd edk2/pkg; ln -s ../../$< loader

edk2: edk2_bak
	cp -r $< $@
	mkdir $@/pkg

osbook: osbook_bak
	cp -r $< $@

edk2_bak:
	git clone --recursive https://github.com/tianocore/edk2.git -b edk2-stable202102 $@
	cd $@; make -C BaseTools

osbook_bak:
	git clone https://github.com/uchan-nos/mikanos-build.git $@

clean:
	rm -f *.o
	rm -f disk.img
	rm -f edk2/pkg/loader
	rm -rf edk2/Build/loaderX64

clean_all: clean
	rm -f *.fd
	rm -rf edk2
	rm -rf osbook

destroy: clean_all
	rm -rf *_bak
