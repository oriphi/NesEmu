#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "6502.c"
#include "ines.c"


int main(int argc, char * argv[])
{
	int done = 0;
	unsigned int tmp = 0;
	cpu6502 * cpu;
	cpu = init_cpu6502();

	FILE *ptr;
	ptr = fopen("ROM","rb");

	fread(cpu -> RAM + 0x200, 1,sizeof(cpu -> RAM) - 0x200,ptr);
	
	while ( ! done)
	{
		done = run_cycle(cpu);
	}
	print_state(cpu);
}

