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
	cpu -> pc = 0x0;
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
	fprintf(stderr,"%s", cpu -> RAM);
	printf("Registers:\n");
	printf("X : %x\n", cpu -> X);
	printf("Y : %x\n", cpu -> Y);
	printf("A : %x\n", cpu -> A);

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
}

static void clear_flag( cpu6502 * cpu, int FLAG )
{
	cpu -> flags[FLAG] = 0;
}

static void set_flag( cpu6502 * cpu, int FLAG )
{
	cpu -> flags[FLAG] = 1;
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


	while ( !done )
	{
		cpu -> opcode = cpu -> RAM[cpu -> pc];
		switch( cpu -> opcode )
		{
			// ADC
			case 0x69:
				tmp = cpu -> A + cpu -> RAM[cpu -> pc + 1] + cpu -> flags[FLAG_C];
				if(tmp > 255)
				{
					set_flag(cpu, FLAG_C);
					set_flag(cpu, FLAG_V);
					set_flag(cpu, FLAG_N);
				}

				cpu -> A = tmp % 256;
				if( !cpu -> A )set_flag(cpu, FLAG_Z);
				break;

			case 0x65:
				break;
			case 0x75:
				break;
			case 0x6d:
				break;
			case 0x7d:
				break;
				break;
			case 0x79:
				break;
			case 0x61:
				break;
			case 0x71:
				break;
		}
	}
}
