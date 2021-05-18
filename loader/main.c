#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/UefiBootServicesTableLib.h>

EFI_STATUS EFIAPI uefi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    for (int i = 0; i < 3; i++)
        Print(L"BOOTING BOXNOS-M ... %d\n", i);

    EFI_MEMORY_DESCRIPTOR buf[400];
    UINTN buf_size = sizeof(buf), map_key, discriptor_size;
    UINT32 discriptor_version;

    Print(L"Discriptor size %d\n", sizeof(EFI_MEMORY_DESCRIPTOR));
    Print(L"buf_size %d\n", buf_size);

    gBS->GetMemoryMap(&buf_size, buf, &map_key, &discriptor_size, &discriptor_version);

    Print(L"buf_size %d\n", buf_size);

    Print(L"DONE.");
    for (;;)
        ;
    return EFI_SUCCESS;
}
