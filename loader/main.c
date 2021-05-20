#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/UefiBootServicesTableLib.h>

EFI_STATUS EFIAPI uefi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    for (int i = 0; i < 3; i++)
        Print(L"BOOTING BOXNOS-M ... %d\n", i);

    CHAR8 buf[1024 * 4 * 4];
    UINTN buf_size = sizeof(buf), map_key, discriptor_size;
    UINT32 discriptor_version;

    gBS->GetMemoryMap(&buf_size, (EFI_MEMORY_DESCRIPTOR *) buf, &map_key, &discriptor_size, &discriptor_version);
    Print(L"buf_size %d\n", buf_size);
    Print(L"Discriptor size %d\n", sizeof(EFI_MEMORY_DESCRIPTOR));
    Print(L"discriptor_size %d\n", discriptor_size);

    buf_size /= discriptor_size;
    for (UINTN i = 0; i < buf_size; i++) {
        EFI_MEMORY_DESCRIPTOR *o = (EFI_MEMORY_DESCRIPTOR *) (buf + i * discriptor_size);
        Print(L"Type:%d PhysicalStart:%x VirtualStart:%x NumberOfPages:%d Attribute:%d\n", o->Type, o->PhysicalStart, o->VirtualStart, o->NumberOfPages, o->Attribute);
    }


    Print(L"DONE.");
    for (;;)
        ;
    return EFI_SUCCESS;
}
