#include  <Uefi.h>
#include  <Library/UefiLib.h>

EFI_STATUS EFIAPI uefi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    for (int i = 0; i < 3; i++)
        Print(L"BOOTING BOXNOS-M ... %d\n", i);
    Print(L"DONE.");
    for (;;)
        ;
    return EFI_SUCCESS;
}
