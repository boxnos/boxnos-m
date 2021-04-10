target=hello.efi

run-qemu: disk.img OVMF_CODE.fd OVMF_VARS.fd
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,file=OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=OVMF_VARS.fd \
		-hda $<

%.fd : /usr/share/OVMF
	cp /usr/share/OVMF/*.fd .
# %.fd : edk2
#	cp edk2/Build/OvmfX64/DEBUG_GCC5/FV/*.fd .
#%.fd : osbook
#	cp $</devenv/$@ .

hello.efi : src/hello.c Makefile
	clang -target x86_64-pc-win32-coff \
		-mno-red-zone -fno-stack-protector \
		-fshort-wchar -Wall \
		-c $<
	lld-link /subsystem:efi_application /entry:EfiMain /out:$@ hello.o

disk.img : $(target) Makefile
	qemu-img create -f raw $@ 200M
	mkfs.fat -n 'BOXNOS-M' -s 2 -f 2 -R 32 -F 32 $@
	mmd -i $@ EFI
	mmd -i $@ EFI/BOOT
	mcopy -i $@ $< ::EFI/BOOT/BOOTX64.EFI

osbook :
	git clone https://github.com/uchan-nos/mikanos-build.git $@

clean:
	rm -f *.fd
	rm -f *.o
	rm -f disk.img
