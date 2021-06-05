#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>

const CHAR16 * get_memory_type (EFI_MEMORY_TYPE t) {
    // julia> foreach(s -> println("case ", s, ": return L\"", s, "\";"), map(strip,split(match(r"typedef.*{(.*)}.*EFI_MEMORY_TYPE"sm, replace(read("edk2/MdePkg/Include/Uefi/UefiMultiPhase.h", String), r"//.*\n" => ""))[1], ",\r\n")))
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

#define halt_if_error(status, ...) \
    if (EFI_ERROR(status)) { \
        Print(__VA_ARGS__); \
        for (;;) __asm__("hlt");}

EFI_FILE_PROTOCOL * open_root_dir(EFI_HANDLE image_handle) {
    EFI_FILE_PROTOCOL *root;
    EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
    gBS->OpenProtocol(image_handle, &gEfiLoadedImageProtocolGuid, (VOID **) &loaded_image,
                      image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    gBS->OpenProtocol(loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **) &fs,
                      image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    fs->OpenVolume(fs, &root);
    return root;
}

struct memory_map {
    UINTN buffer_size;
    VOID* buffer;
    UINTN map_size;
    UINTN map_key;
    UINTN discriptor_size;
    UINT32 discriptor_version;
};

void get_memory_map(struct memory_map *m) {
    gBS->GetMemoryMap(&m->buffer_size, (EFI_MEMORY_DESCRIPTOR *) &m->buffer, &m->map_key, &m->discriptor_size, &m->discriptor_version);
}

void save_memory_map(struct memory_map *m, EFI_FILE_PROTOCOL *root) {
    EFI_FILE_PROTOCOL *file;
    root->Open(root, &file, L"\\memmap", EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

    get_memory_map(m);

    CHAR8 buf[256];
    CHAR8 *header = "index, type, type(name), physical_start, virtual_start, attribute\n";
    UINTN len = AsciiStrLen(header);
    file->Write(file, &len, header);

    m->buffer_size /= m->discriptor_size;
    for (UINTN i = 0; i < m->buffer_size; i++) {
        EFI_MEMORY_DESCRIPTOR *o = (EFI_MEMORY_DESCRIPTOR *) (m->buffer + i * m->discriptor_size);
        len = AsciiSPrint(buf, sizeof(buf),
                          "%u, %x, %-ls, %08lx, %lx, %lx\n",
                          i, o->Type, get_memory_type(o->Type),
                          o->PhysicalStart, o->VirtualStart, o->NumberOfPages, o->Attribute & 0xffffflu);
        file->Write(file, &len, buf);
    }
    file->Close(file);
}

void load_kernel(EFI_FILE_PROTOCOL *root) {
    EFI_FILE_PROTOCOL *file;
    CHAR16 *filename = L"\\kernel.elf";
    halt_if_error(root->Open(root, &file, filename, EFI_FILE_MODE_READ, 0), L"Can't find %s", filename);

    UINTN info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
    UINT8 buf[info_size];
    file->GetInfo(file, &gEfiFileInfoGuid, &info_size, buf);
    EFI_FILE_INFO *info = (EFI_FILE_INFO *) buf;
    UINTN size = info->FileSize;

    EFI_PHYSICAL_ADDRESS addr = 0x100000;
    gBS->AllocatePages(AllocateAddress, EfiLoaderData,
                       (size + 0xfff) / 0x1000, &addr);
    file->Read(file, &size, (VOID *) addr);
    Print(L"ADDR: 0x%0lx (%lu bytes)", addr, size);
}

void open_gop(EFI_HANDLE ih, EFI_GRAPHICS_OUTPUT_PROTOCOL **gop) {
    UINTN n = 0;
    EFI_HANDLE *h = NULL;
    gBS->LocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &n, &h);
    gBS->OpenProtocol(h[0], &gEfiGraphicsOutputProtocolGuid, (VOID**)gop,
                      ih, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    gBS->FreePool(h);
}

EFI_STATUS EFIAPI uefi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    Print(L"Reset Display...\n");
    gST->ConOut->ClearScreen(gST->ConOut);

    Print(L"Open graphics ... ");
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    open_gop(image_handle, &gop);
    Print(L"[DONE]\n");

    //for (UINTN i = 0; i < gop->Mode->FrameBufferSize; i++)
    //    ((UINT8 *)gop->Mode->FrameBufferBase)[i] = i / 16;

    Print(L"BOOTING BOXNOS-M\n");

    Print(L"Getting memory map... ");
    CHAR8 mm_buf[1024 * 4 * 4];
    struct memory_map mm = {sizeof(mm_buf), mm_buf, 0, 0, 0, 0};
    get_memory_map(&mm);
    Print(L"[DONE]\n");

    Print(L"Opening root... ");
    EFI_FILE_PROTOCOL *root = open_root_dir(image_handle);
    Print(L"[DONE]\n");

    Print(L"Saving memory map... ");
    save_memory_map(&mm, root);
    Print(L"[DONE]\n");

    Print(L"Loading kernel... ");
    load_kernel(root);
    Print(L"[DONE]\n");

    Print(L"[All DONE]\n");

    Print(L"Exiting boot... \n");
    halt_if_error(gBS->ExitBootServices(image_handle, (get_memory_map(&mm), mm.map_key)), L"[FAIL]\n");

    ((void(*)(UINT64, UINT64))*(UINT64 *)(0x100000 + 24))(gop->Mode->FrameBufferBase,
                                                          gop->Mode->FrameBufferSize);

    return 0;
}
