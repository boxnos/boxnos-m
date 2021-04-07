run-qemu: disk.img OVMF_CODE.fd OVMF_VARS.fd
	qemu-system-x86_64 \
		-drive if=pflash,file=OVMF_CODE.fd \
		-drive if=pflash,file=OVMF_VARS.fd \
		-hda $<

%.fd : osbook
	cp $</devenv/$@ .

disk.img : BOOTX64.EFI
	qemu-img create -f raw $@ 200M
	mkfs.fat -n 'BOXNOS-M' -s 2 -f 2 -R 32 -F 32 $@
	mmd -i $@ EFI
	mmd -i $@ EFI/BOOT
	mcopy -i $@ $< ::EFI/BOOT

osbook :
	git clone https://github.com/uchan-nos/mikanos-build.git $@

clean:
	rm -f *.fd
	rm -f disk.img
