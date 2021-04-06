disk.img : BOOTX64.EFI
	qemu-img create -f raw disk.img 200M
	mkfs.fat -n 'BOXNOS-M' -s 2 -f 2 -R 32 -F 32 disk.img
	mmd -i disk.img EFI
	mmd -i disk.img EFI/BOOT
	mcopy -i disk.img BOOTX64.EFI ::EFI/BOOT

clean:
	rm -f disk.img
