#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
//#include <SDL2/SDL.h>
#include <stdio.h>

#include"6502.h"







cpu6502 * init_cpu6502()
{
	int i;
	cpu6502 *cpu;
	cpu = malloc(sizeof(cpu6502));

	cpu -> cycles = 0;
	cpu -> pc = 0x200;
	cpu -> sp = 0x0;

	// Registers
	cpu -> A = 0x0;
	cpu -> X = 0x0;
	cpu -> Y = 0x0;

	// Ram
	for(i = 0; i < 0x10000;i++ )
		cpu -> RAM[i] = 0;

	// Flags
	for (i = 0; i < 8; i ++)
		cpu -> flags[i] = 0;
	
	return cpu;
}


static int console( cpu6502 * cpu )
{
	fprintf(stdout,"DEBUG\n");
	return 1;
}


static void print_state( cpu6502 * cpu)
{
	printf("==============================\n");
	printf("Registers:\n");
	printf("X : %x\n", cpu -> X);
	printf("Y : %x\n", cpu -> Y);
	printf("A : %x\n", cpu -> A);
	printf("Cycles : %d\n",cpu -> cycles);
	printf("Opcode : 0x%x\n", cpu -> opcode);

	printf("\nPointers : \n");
	printf("pc : %x\n", cpu -> pc);
	printf("sp : %x\n", cpu -> sp);

	printf("\nFlags : \n");
	printf("C : %x\n", cpu -> flags[FLAG_C] );
	printf("Z : %x\n", cpu -> flags[FLAG_Z] );
	printf("I : %x\n", cpu -> flags[FLAG_I] );
	printf("D : %x\n", cpu -> flags[FLAG_D] );
	printf("V : %x\n", cpu -> flags[FLAG_V] );
	printf("B : %x\n", cpu -> flags[FLAG_B] );
	printf("N : %x\n", cpu -> flags[FLAG_N] );

	printf("\n");
	for (int i = 0x0; i < 0x2000; i ++)
		fprintf(stderr,"%c", cpu -> RAM[i]);
}

static void clear_flag( cpu6502 * cpu, int FLAG )
{
	cpu -> flags[FLAG] = 0;
}

static void set_flag( cpu6502 * cpu, int FLAG )
{
	cpu -> flags[FLAG] = 1;
}




static void cpu_ldx(cpu6502 * cpu)
{
	address a;
	switch(cpu -> opcode)
	{
		case 0xa2:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xa6:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 3;
			break;
		case 0xb6:
			zero_page_read_Y(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 4;
			break;
		case 0xae:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0xbe:
			a = absolute_read_Y(cpu);
			cpu -> pc += 3;
			cpu -> cycles +=4;
			break;

	}
	cpu -> X = cpu -> RAM[a];
}
static void cpu_ldy(cpu6502 * cpu)
{
	address a;
	switch(cpu -> opcode)
	{
		case 0xa0:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xa4:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 3;
			break;
		case 0xb4:
			zero_page_read_Y(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 4;
			break;
		case 0xac:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0xbc:
			a = absolute_read_Y(cpu);
			cpu -> pc += 3;
			cpu -> cycles +=4;
			break;

	}
	cpu -> Y = cpu -> RAM[a];
}



static void cpu_lda(cpu6502 * cpu)
{
	address a;
	switch(cpu -> opcode)
	{
		case 0xa9:
			// Immediate
			a = immediate_read(cpu);
			cpu -> cycles +=2;
			cpu -> pc += 2;
			break;
		case 0xa5:
			// Zero page
			a = zero_page_read(cpu);
			cpu -> cycles +=3;
			cpu -> pc += 2;
			break;
		case 0xb5:
			//Zero Page X
			a = zero_page_read_X(cpu);
			cpu -> cycles +=4;
			cpu -> pc += 2;
			break;
		case 0xad:
			// Absolute
			a = absolute_read(cpu);
			cpu -> cycles +=4;
			cpu -> pc += 3;
			break;
		case 0xbd:
			// Absolute X
			a = absolute_read_X(cpu);
			cpu -> cycles +=4;
			cpu -> pc += 3;
			break;
		case 0xb9:
			// Absolute Y
			a = absolute_read_Y(cpu);
			cpu -> cycles +=4;
			cpu -> pc += 3;
			break;
		case 0xa1:
			// Indirect read X
			a = indirect_X_read(cpu);
			cpu -> cycles +=6;
			cpu -> pc += 2;
			break;
		case 0xb1:
			// Indirect read Y
			a = indirect_Y_read(cpu);
			cpu -> cycles +=5;
			cpu -> pc += 2;
			break;
	}
	cpu -> A = cpu -> RAM[a];

}

static void cpu_sta(cpu6502 * cpu)
{
	switch(cpu -> opcode)
	{
		case 0x85:
			// Zero page
			zero_page_write(cpu, cpu -> A);
			cpu -> cycles += 3;
			cpu -> pc += 2;
			break;
		case 0x95:
			//Zero Page X
			zero_page_write_X(cpu, cpu -> A);
			cpu -> cycles += 4;
			cpu -> pc += 2;
			break;
		case 0x8d:
			// Absolute
			absolute_write(cpu, cpu -> A);
			cpu -> cycles += 4;
			cpu -> pc += 3;
			break;
		case 0x9d:
			// Absolute X
			absolute_write_X(cpu, cpu -> A);
			cpu -> cycles += 5;
			cpu -> pc += 3;
			break;
		case 0x99:
			// Absolute Y
			absolute_write_Y(cpu, cpu -> A);
			cpu -> cycles +=5;
			cpu -> pc += 3;
			break;
		case 0x81:
			// Indirect read X
			indirect_X_write(cpu, cpu -> A);
			cpu -> cycles +=6;
			cpu -> pc += 2;
			break;
		case 0x91:
			// Indirect read Y
			indirect_Y_write(cpu, cpu -> A);
			cpu -> cycles +=6;
			cpu -> pc += 2;
			break;
	}
}


static void cpu_adc( cpu6502 * cpu)
{
	address a;
	unsigned int tmp;
	switch(cpu -> opcode)
	{
		case 0x69:
			a = immediate_read(cpu);
			// Immediate
			cpu -> cycles += 2;
			cpu -> pc += 2;
			break;
		case 0x65:
			a = zero_page_read(cpu);
			cpu -> cycles += 3;
			cpu -> pc += 2;
			//Zero Page
			break;
		case 0x75:
			a = zero_page_read_X(cpu);
			cpu -> cycles += 4;
			cpu -> pc += 2;
			//Zero Page X
			break;
		case 0x6d:
			//Absolute
			a = absolute_read(cpu);
			cpu -> cycles += 4;
			cpu -> pc += 3;
			break;
		case 0x7d:
			//Absolute,X
			a = absolute_read_X(cpu);
			cpu -> cycles += 4;
			cpu -> pc += 3;
			break;
		case 0x79:
			a = absolute_read_Y(cpu);
			cpu -> cycles += 4;
			cpu -> pc += 3;
			//Absolute,Y
			break;

		case 0x61:
			//Indirect,X
			a = indirect_X_read(cpu);
			cpu -> cycles += 6;
			cpu -> pc += 2;
			break;
		case 0x71:
			// Indirect,Y
			a = indirect_Y_read(cpu);
			cpu -> cycles += 5;
			cpu -> pc += 2;
			break;
	}
	tmp = cpu -> A + cpu -> RAM[a] + cpu -> flags[FLAG_C];
	if(tmp > 255)
	{
		set_flag(cpu, FLAG_C);
		set_flag(cpu, FLAG_V);
		set_flag(cpu, FLAG_N);
	}

	cpu -> A = tmp % 256;
	if( !cpu -> A )set_flag(cpu, FLAG_Z);
}	

// Write ROUTINES
void immediate_write(cpu6502 * cpu, unsigned int value)
{
	cpu -> RAM [cpu -> pc + 1] = value;
}
void zero_page_write(cpu6502 * cpu, unsigned int value)
{
	cpu -> RAM[cpu -> RAM[cpu -> pc + 1]] = value;
}

void zero_page_write_X(cpu6502 * cpu, unsigned int value)
{
	cpu -> RAM[(cpu -> RAM[cpu -> pc + 1] + cpu -> X) % 256] = value;
}

void absolute_write(cpu6502 * cpu, unsigned int value)
{
	int a =  cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1] ;
	cpu -> RAM[ cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1] ] = value;
}

void absolute_write_X(cpu6502 * cpu, unsigned int value)
{
	cpu -> RAM[ (cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1])  + cpu -> X] = value;
}

void absolute_write_Y(cpu6502 * cpu, unsigned int value)
{
	cpu -> RAM[ (cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1] )+ cpu -> Y] = value;
}

void indirect_X_write(cpu6502 * cpu, unsigned int value)
{
	unsigned char a =  cpu -> X + cpu -> RAM[cpu -> pc + 1];
	cpu -> RAM[ cpu -> RAM[a + 1] << 8 | cpu -> RAM[a] ] = value;
}

void indirect_Y_write(cpu6502 * cpu, unsigned int value)
{
	unsigned char a = cpu -> RAM[cpu -> pc + 1];
	address ad = (cpu -> RAM[a + 1] << 8 | cpu -> RAM[a] ) + cpu -> Y;
	cpu -> RAM[ad] = value;
}




address immediate_read(cpu6502 * cpu)
{
	return cpu -> pc + 1;
}
address zero_page_read(cpu6502 * cpu)
{
	return cpu -> RAM[cpu -> pc + 1]; 
}

address zero_page_read_Y(cpu6502 * cpu)
{
	// No Carry
	return  (cpu -> RAM[cpu -> pc + 1] + cpu -> Y) % 256;
}

address zero_page_read_X(cpu6502 * cpu)
{
	// No Carry
	return  (cpu -> RAM[cpu -> pc + 1] + cpu -> X) % 256;
}

address absolute_read(cpu6502 * cpu)
{
	return cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1] ;
}

address absolute_read_X(cpu6502 * cpu)
{
	if(cpu -> RAM[cpu -> pc + 1] + cpu -> X > 255)cpu -> cycles ++;
	return (cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1])  + cpu -> X;
}

address absolute_read_Y(cpu6502 * cpu)
{
	if(cpu -> RAM[cpu -> pc + 1] + cpu -> Y > 255)cpu -> cycles ++;
	return (cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1] )+ cpu -> Y;
}

address indirect_X_read(cpu6502 * cpu)
{
	unsigned char a =  cpu -> X + cpu -> RAM[cpu -> pc + 1];
	return cpu -> RAM[a + 1] << 8 | cpu -> RAM[a]; 
}

address indirect_Y_read(cpu6502 * cpu)
{
	unsigned char a = cpu -> RAM[cpu -> pc + 1];
	if(cpu -> RAM[a] + cpu -> Y > 255) cpu -> cycles ++; // OOPS cycle
	address ad = (cpu -> RAM[a + 1] << 8 | cpu -> RAM[a] ) + cpu -> Y;
	return ad;
}

/*
 *         
 *
 *          MAIN CODE
 *
 *
 *
 */



int main(int argc, char * argv[])
{
	int done = 0;
	unsigned int tmp = 0;
	cpu6502 * cpu;
	cpu = init_cpu6502();




	FILE *ptr;
	ptr = fopen("ROM","rb");

	fread(cpu -> RAM + 0x200, 1,sizeof(cpu -> RAM) - 0x200,ptr);

	while ( !done )
	{
		cpu -> opcode = cpu -> RAM[cpu -> pc];
		switch( cpu -> opcode )
		{
			case 0x00:
				done = 1;
			// ADC
			case 0x69:
			case 0x65:
			case 0x75:
			case 0x6d:
			case 0x7d:
			case 0x79:
			case 0x61:
			case 0x71:
				cpu_adc(cpu);
				break;
			// LDA
			case 0xa9: // CHECKED
			case 0xa5: // CHEKCED
			case 0xb5:
			case 0xad: // CHECKED
			case 0xbd:
			case 0xb9:
			case 0xa1:
			case 0xb1:
				cpu_lda(cpu);
				break;
			// STA
			case 0x85: // CHECKED
			case 0x95: // CHECKED
			case 0x8d: // CHECKED
			case 0x9d: // CHECKED
			case 0x99: // CHECKED
			case 0x81: // CHECKED
			case 0x91:
				cpu_sta(cpu);
				break;

			// LDX 
			case 0xa2: // CHECKED
			case 0xa6: // CHECKED
			case 0xb6:
			case 0xae: // CHECKED
			case 0xbe:
				cpu_ldx(cpu);
				break;

			// LDY
			case 0xa0: // CHECKED
			case 0xa4: // CHECKED
			case 0xb4:
			case 0xac:// CHECKED
			case 0xbc:
				cpu_ldy(cpu);
				break;
			
		}
	}
	print_state(cpu);
}
