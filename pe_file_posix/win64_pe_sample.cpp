#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

struct pe_file_t
{
	IMAGE_DOS_HEADER		dos;
	uint8_t					dos_warning[64] =
	{
		0x0e, 0x1f, 0xba, 0x0e, 0x00, 0xb4, 0x09, 0xcd, 0x21, 0xb8, 0x01, 0x4c, 0xcd, 0x21, 0x54, 0x68,
		0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 0x61, 0x6d, 0x20, 0x63, 0x61, 0x6e, 0x6e, 0x6f,
		0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x44, 0x4f, 0x53, 0x20,
		0x6d, 0x6f, 0x64, 0x65, 0x2e, 0x0d, 0x0a, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	IMAGE_NT_HEADERS64		nt;
	IMAGE_SECTION_HEADER	stext;
	IMAGE_SECTION_HEADER	sdata;
	IMAGE_SECTION_HEADER	sidata;

	size_t header_rsize = sizeof(dos) + sizeof(dos_warning) + sizeof(nt) + sizeof(stext) + sizeof(sdata) + sizeof(sidata);
	size_t header_size				= 0x200;
	size_t header_vsize				= 0x1000;
	size_t file_alignment			= 0x200;
	size_t segment_alignment		= 0x1000;
	size_t image_vbase				= 0x400000;

	size_t code_padding = 0;

	uint8_t *	code = nullptr;
	size_t		code_size = 0;
	size_t		code_rsize = 0;
	size_t		code_vsize = 0;

	uint8_t *	data = nullptr;
	size_t		data_size = 0;
	size_t		data_rsize = 0;
	size_t		data_vsize = 0;

	uint8_t *	import = nullptr;
	size_t		import_size = 0;
	size_t		import_rsize = 0;
	size_t		import_vsize = 0;

	void link_code(const uint8_t * bytes, size_t count);
	void link_data(const uint8_t * bytes, size_t count);
	void link_import(const uint8_t * bytes, size_t count);

	void fill_dos();
	void fill_nt();
	void fill_stext();
	void fill_sdata();
	void fill_sidata();

	void write(FILE * f);

	void clear();
};

size_t aligned_size(size_t size, size_t align)
{
	return size + align - size % align;
}

void pe_file_t::link_code(const uint8_t * bytes, size_t count)
{
	code_padding = 0x200 - header_rsize;

	code_size = aligned_size(count, file_alignment);
	code_rsize = count;
	code_vsize = aligned_size(code_size, segment_alignment);
	code = (uint8_t*)malloc(code_size);
	memset(code, 0, code_size);
	memcpy(code, bytes, count);
}

void pe_file_t::link_data(const uint8_t * bytes, size_t count)
{
	data_size = aligned_size(count, file_alignment);
	data_rsize = count;
	data_vsize = aligned_size(data_size, segment_alignment);
	data = (uint8_t*)malloc(data_size);
	memset(data, 0, data_size);
	memcpy(data, bytes, count);
}

void pe_file_t::link_import(const uint8_t * bytes, size_t count)
{
	import_size = aligned_size(count, file_alignment);
	import_rsize = count;
	import_vsize = aligned_size(import_size, segment_alignment);
	import = (uint8_t*)malloc(import_size);
	memset(import, 0, import_size);
	memcpy(import, bytes, count);
}

void pe_file_t::fill_dos()
{
	memset(&dos, 0, sizeof(dos));
	dos.e_magic		= 0x5a4d;
	dos.e_cblp		= 0x80;
	dos.e_cp		= 0x01;
	dos.e_cparhdr	= 0x04;
	dos.e_minalloc	= 0x10;
	dos.e_maxalloc	= 0xffff;
	dos.e_sp		= 0x140;
	dos.e_lfarlc	= 0x40;
	dos.e_lfanew	= 0x80;
}

void pe_file_t::fill_nt()
{
	memset(&nt, 0, sizeof(nt));
	nt.Signature = IMAGE_NT_SIGNATURE;

	nt.FileHeader.Machine				= IMAGE_FILE_MACHINE_AMD64;
	nt.FileHeader.NumberOfSections		= 0x3;
	nt.FileHeader.TimeDateStamp			= 0x0; // TODO add timestamp
	nt.FileHeader.PointerToSymbolTable	= 0x0;
	nt.FileHeader.NumberOfSymbols		= 0x0;
	nt.FileHeader.SizeOfOptionalHeader	= sizeof(nt.OptionalHeader);
	nt.FileHeader.Characteristics		= IMAGE_FILE_RELOCS_STRIPPED | IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LINE_NUMS_STRIPPED | IMAGE_FILE_LOCAL_SYMS_STRIPPED | IMAGE_FILE_LARGE_ADDRESS_AWARE;

	nt.OptionalHeader.Magic							= IMAGE_NT_OPTIONAL_HDR64_MAGIC;
	nt.OptionalHeader.MajorLinkerVersion			= 0x01;
	nt.OptionalHeader.MinorLinkerVersion			= 0x47;
	nt.OptionalHeader.SizeOfCode					= code_size;
	nt.OptionalHeader.SizeOfInitializedData			= data_size + import_size;
	nt.OptionalHeader.SizeOfUninitializedData		= 0x0;
	nt.OptionalHeader.AddressOfEntryPoint			= header_vsize;
	nt.OptionalHeader.BaseOfCode					= header_vsize;
	nt.OptionalHeader.ImageBase						= image_vbase;
	nt.OptionalHeader.SectionAlignment				= segment_alignment;
	nt.OptionalHeader.FileAlignment					= file_alignment;
	nt.OptionalHeader.MajorOperatingSystemVersion	= 0x1;
	nt.OptionalHeader.MinorOperatingSystemVersion	= 0x0;
	nt.OptionalHeader.MajorImageVersion				= 0x0;
	nt.OptionalHeader.MinorImageVersion				= 0x0;
	nt.OptionalHeader.MajorSubsystemVersion			= 0x5;
	nt.OptionalHeader.MinorSubsystemVersion			= 0x0;
	nt.OptionalHeader.Win32VersionValue				= 0x0;
	nt.OptionalHeader.SizeOfImage					= header_vsize + code_vsize + data_vsize + import_vsize;
	nt.OptionalHeader.SizeOfHeaders					= header_size;
	nt.OptionalHeader.CheckSum						= 0x0; // TODO add checksum
	nt.OptionalHeader.Subsystem						= IMAGE_SUBSYSTEM_WINDOWS_CUI;
	nt.OptionalHeader.DllCharacteristics			= 0x0;
	nt.OptionalHeader.SizeOfStackReserve			= 0x1000;
	nt.OptionalHeader.SizeOfStackCommit				= 0x1000;
	nt.OptionalHeader.SizeOfHeapReserve				= 0x10000;
	nt.OptionalHeader.SizeOfHeapCommit				= 0x0;
	nt.OptionalHeader.LoaderFlags					= 0x0;
	nt.OptionalHeader.NumberOfRvaAndSizes			= 0x10;

	nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress	= header_vsize + code_vsize + data_vsize;
	nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size				= import_rsize;
}

void pe_file_t::fill_stext()
{
	memset(&stext, 0, sizeof(stext));

	strcpy((char*)stext.Name, ".text");
	stext.Misc.VirtualSize		= code_rsize;
	stext.VirtualAddress		= header_vsize;
	stext.SizeOfRawData			= code_size;
	stext.PointerToRawData		= header_size;
	stext.Characteristics		= IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE;
}

void pe_file_t::fill_sdata()
{
	memset(&sdata, 0, sizeof(sdata));

	strcpy((char*)sdata.Name, ".data");
	sdata.Misc.VirtualSize		= data_rsize;
	sdata.VirtualAddress		= header_vsize + code_vsize;
	sdata.SizeOfRawData			= data_size;
	sdata.PointerToRawData		= header_size + code_size;
	sdata.Characteristics		= IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA;
}

void pe_file_t::fill_sidata()
{
	memset(&sidata, 0, sizeof(sidata));

	strcpy((char*)sidata.Name, ".idata");
	sidata.Misc.VirtualSize		= import_rsize;
	sidata.VirtualAddress		= header_vsize + code_vsize + data_vsize;
	sidata.SizeOfRawData		= import_size;
	sidata.PointerToRawData		= header_size + code_size + data_size;
	sidata.Characteristics		= IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA;
}

void pe_file_t::write(FILE * f)
{
	fwrite(&dos, sizeof(dos), 1, f);
	fwrite(dos_warning, sizeof(dos_warning), 1, f);
	fwrite(&nt, sizeof(nt), 1, f);
	fwrite(&stext, sizeof(stext), 1, f);
	fwrite(&sdata, sizeof(sdata), 1, f);
	fwrite(&sidata, sizeof(sidata), 1, f);

	for(size_t i = 0; i < code_padding; ++i)
		fputc(0, f);

	if(code)
		fwrite(code, code_size, 1, f);

	if(data)
		fwrite(data, data_size, 1, f);

	if(import)
		fwrite(import, import_size, 1, f);
}

void pe_file_t::clear()
{
	if(code)
		free(code);

	if(data)
		free(data);

	if(import)
		free(import);

	code = nullptr;
	code_size = 0;
	code_rsize = 0;
	code_vsize = 0;

	data = nullptr;
	data_size = 0;
	data_rsize = 0;
	data_vsize = 0;

	import = nullptr;
	import_size = 0;
	import_rsize = 0;
	import_vsize = 0;
}

int main()
{
	pe_file_t pe;

	// implement win64_posix_test.asm code

	uint8_t code[] = {
		0x48, 0x83, 0xec, 0x28,						// sub rsp, 0x28
		0x48, 0xc7, 0xc1, 0x01, 0x00, 0x00, 0x00,	// mov rcx, 1
		0x48, 0xc7, 0xc2, 0x01, 0x00, 0x00, 0x00,	// mov rdx, 1
		0x4c, 0x8d, 0x05, 0xe7, 0x0f, 0x00, 0x00,	// lea r8, [_message]
		0x49, 0xc7, 0xc1, 0x05, 0x00, 0x00, 0x00,	// mov r9, 5
		0xff, 0x15, 0x1a, 0x20, 0x00, 0x00,			// call [syscall3]

		0x48, 0x8d, 0x0d, 0xd3, 0x0f, 0x00, 0x00,	// lea rcx, [_message]
		0xff, 0x15, 0x2d, 0x20, 0x00, 0x00,			// call [syscall_msg]

		0x48, 0xc7, 0xc1, 0x3c, 0x00, 0x00, 0x00,	// mov rcx, 60; //exit
		0x48, 0xc7, 0xc2, 0x00, 0x00, 0x00, 0x00,	// mov rdx, 0;
		0xff, 0x15, 0xe1, 0x1f, 0x00, 0x00			// call [syscall0]
	};
	pe.link_code(code, sizeof(code));

	uint8_t data[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0x00};
	pe.link_data(data, sizeof(data));

	const uint8_t import[] =
	{
		// check IMAGE_IMPORT_DESCRIPTOR for this

		0x00, 0x00, 0x00, 0x00, // dd 0, 0, 0
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x70, 0x30, 0x00, 0x00, // RVA syscall_name
		0x28, 0x30, 0x00, 0x00, // RVA syscall_table

		0x00, 0x00, 0x00, 0x00, // dd 0, 0, 0, 0, 0
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,

		// syscall_table:
		0x80, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall0 dq RVA _syscall0
		0x8b, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall1 dq RVA _syscall1
		0x96, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall2 dq RVA _syscall2
		0xa1, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall3 dq RVA _syscall3
		0xac, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall4 dq RVA _syscall4
		0xb7, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall5 dq RVA _syscall5
		0xc2, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall6 dq RVA _syscall6
		0xcd, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // syscall_msg dq RVA _syscall_msg
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // dq 0

		// syscall_name db 'win64_posix.dll', 0
		0x77, 0x69, 0x6e, 0x36, 0x34, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x78, 0x2e, 0x64, 0x6c, 0x6c, 0x00,

		// _syscall0 dw 0
		// db 'syscall0',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x30, 0x00,

		// _syscall1 dw 0
		// db 'syscall1',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x31, 0x00,

		// _syscall2 dw 0
		// db 'syscall2',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x32, 0x00,

		// _syscall3 dw 0
		// db 'syscall3',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x33, 0x00,

		// _syscall4 dw 0
		// db 'syscall4',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x34, 0x00,

		// _syscall5 dw 0
		// db 'syscall5',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x35, 0x00,

		// _syscall6 dw 0
		// db 'syscall6',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x36, 0x00,

		// _syscall_msg dw 0
		// db 'syscall_msg',0
		0x00, 0x00, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x5f, 0x6d, 0x73, 0x67, 0x00
	};
	pe.link_import(import, sizeof(import));

	pe.fill_dos();
	pe.fill_nt();
	pe.fill_stext();
	pe.fill_sdata();
	pe.fill_sidata();

	FILE * f = fopen("test.exe", "wb+");
	pe.write(f);
	fclose(f);

	pe.clear();

	printf("done\n");
	Sleep(20);
	return 0;
}
