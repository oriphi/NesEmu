#include <stdio.h>
#include <stdlib.h>

typedef struct {
	FILE  * file;
	int file_size;
	int mapper;
	int rom_pages;
	int chr_pages;
	unsigned char header[16];
} rom_file;




void rom_info(rom_file  * rom);
rom_file * load_rom(FILE * file);




rom_file * load_rom( FILE * file)
{
	rom_file * rom;
	rom = malloc(sizeof(rom_file));
	int file_size;
	unsigned char mapper;
	unsigned char header[16];

	fseek(file, 0L, SEEK_END);
	file_size = ftell(file);
	rewind(file);
	rom -> file = file;
	rom -> file_size = file_size;
	// On récupère le header
	fread(rom -> header , 1, 16, file );
	
	mapper = rom -> header[6] & 0xf0 | (rom -> header[7] & 0xf0 << 4);
	rom -> rom_pages = rom -> header[4];
	rom -> chr_pages = rom -> header[5];
	rom -> mapper = mapper;
	rom_info(rom);
	return rom;
}

void rom_info( rom_file *rom)
{
	printf("File Size %d\n", rom -> file_size);
	// On récupère le header
	printf("Number of ROM pages [ 16384 byte ] : %d\n", rom -> rom_pages);

	if(rom -> chr_pages == 0)
		printf("CHR RAM used\n");

	else
		printf("Number of CHR pages [ 8192 byte ] : %d\n", rom -> chr_pages);

	printf("Mapper : %d\n", rom -> mapper);
	printf("Four screen : %s\n", ((rom -> header[6] & 0x08 >>3) == 1)?"yes":"no");
	printf("Trainer : %s\n", ((rom -> header[6] & 0x04 >> 2) == 1)?"yes":"no");
	printf("Battery backed : %s\n", ((rom -> header[6] & 0x02 >> 0) == 1)?"yes":"no");
	printf("Mirroring: %s\n", ((rom -> header[6] & 0x01 ) == 1)?"yes":"no");
}

int main(int argc, char * argv[])
{
	FILE * RomFile;
	if(argc < 2)
		printf("You need to specify a rom file\n");
	if(argc  == 2)
	{
		printf("Load File : %s\n", argv[1]);
		RomFile = fopen(argv[1],"rb");
		rom_file * rom;
		rom = load_rom(RomFile);

	}
}
