target=src/bin.efi

run-qemu: disk.img OVMF_CODE.fd OVMF_VARS.fd
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,file=OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=OVMF_VARS.fd \
		-hda $<

#%.fd :
#	cp /usr/share/OVMF/*.fd .
# %.fd : edk2
#	cp edk2/Build/OvmfX64/DEBUG_GCC5/FV/*.fd .
%.fd : osbook
	cp $</devenv/$@ .

disk.img : $(target)
	qemu-img create -f raw $@ 200M
	mkfs.fat -n 'BOXNOS-M' -s 2 -f 2 -R 32 -F 32 $@
	mmd -i $@ EFI
	mmd -i $@ EFI/BOOT
	mcopy -i $@ $< ::EFI/BOOT/BOOTX64.EFI

osbook :
	git clone https://github.com/uchan-nos/mikanos-build.git $@

clean:
	rm -f *.fd
	rm -f disk.img
