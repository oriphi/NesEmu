typedef struct{
	unsigned char RAM[0x10000];
	uint16_t pc;
	unsigned char sp;
	unsigned char X;
	unsigned char Y;
	unsigned char A;
	unsigned int cycles;

	// Flags
	unsigned char flags[7];
	

	unsigned char opcode;
} cpu6502;



static cpu6502 * init_cpu6502( );
static void print_state( cpu6502 * cpu );
static int console( cpu6502 * cpu );
static void set_flag( cpu6502 * cpu, int FLAG );
static void clear_flag( cpu6502 * cpu, int FLAG );

// OPCODE FUNCTIONS
static void cpu_adc( cpu6502 * cpu );


const int FLAG_C = 0x0;
const int FLAG_Z = 0x1;
const int FLAG_I = 0x2;
const int FLAG_D = 0x3;
const int FLAG_B = 0x4;
const int FLAG_V = 0x5;
const int FLAG_N = 0x6;
