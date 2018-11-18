typedef uint16_t address;
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



void immediate_write(cpu6502 * cpu, unsigned int value);
void zero_page_write(cpu6502 * cpu, unsigned int value);
void zero_page_write_X(cpu6502 * cpu, unsigned int value);
void absolute_write(cpu6502 * cpu, unsigned int value);
void absolute_write_X(cpu6502 * cpu, unsigned int value);
void absolute_write_Y(cpu6502 * cpu, unsigned int value);
void indirect_X_write(cpu6502 * cpu, unsigned int value);
void indirect_Y_write(cpu6502 * cpu, unsigned int value);


address immediate_read(cpu6502 * cpu);
address zero_page_read(cpu6502 * cpu);
address zero_page_read_X(cpu6502 * cpu);
address zero_page_read_Y(cpu6502 * cpu);
address absolute_read(cpu6502 * cpu);
address absolute_read_X(cpu6502 * cpu);
address absolute_read_Y(cpu6502 * cpu);
address indirect_X_read(cpu6502 * cpu);
address indirect_Y_read(cpu6502 * cpu);


static cpu6502 * init_cpu6502( );
static void print_state( cpu6502 * cpu );
static int console( cpu6502 * cpu );
static void set_flag( cpu6502 * cpu, int FLAG );
static void clear_flag( cpu6502 * cpu, int FLAG );

// OPCODE FUNCTIONS
static void cpu_adc( cpu6502 * cpu );
static void cpu_lda( cpu6502 * cpu );
static void cpu_sta( cpu6502 * cpu );
static void cpu_ldx( cpu6502 * cpu );
static void cpu_ldy( cpu6502 * cpu );


const int FLAG_C = 0x0;
const int FLAG_Z = 0x1;
const int FLAG_I = 0x2;
const int FLAG_D = 0x3;
const int FLAG_B = 0x4;
const int FLAG_V = 0x5;
const int FLAG_N = 0x6;
