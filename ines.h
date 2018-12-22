typedef struct {
	FILE file;
	int file_size;
	int mapper;
	int rom_pages;
	int chr_pages;
	unsigned char header[16];

} rom_file


rom_file load_rom(FILE file);
