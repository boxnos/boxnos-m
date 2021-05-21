#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/UefiBootServicesTableLib.h>
const CHAR16 * get_memory_type (EFI_MEMORY_TYPE t) {
    // julia> foreach(s -> println("case ", s, ": return L\"", s, "\";"), split("EfiReservedMemoryType,EfiLoaderCode,EfiLoaderData,EfiBootServicesCode,EfiBootServicesData,EfiRuntimeServicesCode,EfiRuntimeServicesData,EfiConventionalMemory,EfiUnusableMemory,EfiACPIReclaimMemory,EfiACPIMemoryNVS,EfiMemoryMappedIO,EfiMemoryMappedIOPortSpace,EfiPalCode,EfiPersistentMemory,EfiMaxMemoryType", ","))
    switch (t) {
    case EfiReservedMemoryType: return L"EfiReservedMemoryType";
    case EfiLoaderCode: return L"EfiLoaderCode";
    case EfiLoaderData: return L"EfiLoaderData";
    case EfiBootServicesCode: return L"EfiBootServicesCode";
    case EfiBootServicesData: return L"EfiBootServicesData";
    case EfiRuntimeServicesCode: return L"EfiRuntimeServicesCode";
    case EfiRuntimeServicesData: return L"EfiRuntimeServicesData";
    case EfiConventionalMemory: return L"EfiConventionalMemory";
    case EfiUnusableMemory: return L"EfiUnusableMemory";
    case EfiACPIReclaimMemory: return L"EfiACPIReclaimMemory";
    case EfiACPIMemoryNVS: return L"EfiACPIMemoryNVS";
    case EfiMemoryMappedIO: return L"EfiMemoryMappedIO";
    case EfiMemoryMappedIOPortSpace: return L"EfiMemoryMappedIOPortSpace";
    case EfiPalCode: return L"EfiPalCode";
    case EfiPersistentMemory: return L"EfiPersistentMemory";
    case EfiMaxMemoryType: return L"EfiMaxMemoryType";
    default : return L"unknown.";
    }
}

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
        Print(L"Type:%s PhysicalStart:%x VirtualStart:%x NumberOfPages:%d Attribute:%d\n", get_memory_type(o->Type), o->PhysicalStart, o->VirtualStart, o->NumberOfPages, o->Attribute);
    }


    Print(L"DONE.");
    for (;;)
        ;
    return EFI_SUCCESS;
}
