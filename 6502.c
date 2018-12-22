#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
//#include <SDL2/SDL.h>
#include <stdio.h>

#include "6502.h"


// CPU LDX
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

int run_cycle( cpu6502 * cpu)
{
	address a;
	int done = 0;
	cpu -> opcode = cpu -> RAM[cpu -> pc];
	print_state(cpu);
	switch( cpu -> opcode )
	{
		case 0x02: // END INSTRUCTION
			done = 1;
			break;
		// ADC
		case 0x69: // CHECKED
		case 0x65: // CHECKED
		case 0x75: // CHECKED
		case 0x6d: // CHECKED
		case 0x7d: // CHECKED
		case 0x79: // CHECKED
		case 0x61: // CHECKED
		case 0x71: // CHECKED
			cpu_adc(cpu);
			break;

		// AND
		case 0x29:
		case 0x25:
		case 0x35:
		case 0x2d:
		case 0x3d:
		case 0x39:
		case 0x21:
		case 0x31:
			cpu_and(cpu);
			break;

		// ASL
		case 0x0a:
		case 0x06:
		case 0x16:
		case 0x0e:
		case 0x1e:
			cpu_asl(cpu);
			break;

		// BRANCHES
		case 0x10:
		case 0x30:
		case 0x50:
		case 0x70:
		case 0x90:
		case 0xd0:
		case 0xb0:
		case 0xf0:
			cpu_branch(cpu);
			break;

		// BIT
		case 0x24:
		case 0x2c:
			cpu_bit(cpu);
			break;
		// BRK
		case 0x00:
			cpu_break(cpu);
			break;

		// CMP
		case 0xc9:
		case 0xc5:
		case 0xd5:
		case 0xcd:
		case 0xdd:
		case 0xd9:
		case 0xc1:
		case 0xd1:
			cpu_cmp(cpu);
			break;

		// CPX
		case 0xe0:
		case 0xe4:
		case 0xec:
			cpu_cpx(cpu);
			break;
		// CPY
		case 0xc0:
		case 0xc4:
		case 0xcc:
			cpu_cpy(cpu);
			break;
		
		// DEC
		case 0xc6:
		case 0xd6:
		case 0xce:
		case 0xde:
			cpu_dec(cpu);
			break;
		// EOR
		case 0x49:
		case 0x45:
		case 0x55:
		case 0x4d:
		case 0x5d:
		case 0x59:
		case 0x41:
		case 0x51:
			cpu_eor(cpu);
			break;


		// CLEAR / SET FLAGS
		case 0x18:
			cpu -> flags[FLAG_C] = 0;
			cpu -> cycles += 2;
			cpu -> pc += 1;
			break;
		case 0x38:
			cpu -> flags[FLAG_C] = 1;
			cpu -> cycles += 2;
			cpu -> pc += 1;
			break;
		case 0x58:
			cpu -> flags[FLAG_I] = 0;
			cpu -> cycles += 2;
			cpu -> pc += 1;
			break;
		case 0x78:
			cpu -> flags[FLAG_I] = 1;
			cpu -> cycles += 2;
			cpu -> pc += 1;
			break;
		case 0xb8:
			cpu -> flags[FLAG_V] = 0;
			cpu -> cycles += 2;
			cpu -> pc += 1;
			break;
		case 0xd8:
			cpu -> flags[FLAG_D] = 0;
			cpu -> cycles += 2;
			cpu -> pc += 1;
			break;
		case 0xf8:
			cpu -> flags[FLAG_D] = 1;
			cpu -> cycles += 2;
			cpu -> pc += 1;
			break;

		// INC
		case 0xe6:
		case 0xf6:
		case 0xee:
		case 0xfe:
			cpu_inc(cpu);
			break;
		// JMP
		case 0x4c:
		case 0x6c:
			cpu_jmp(cpu);
			break;

		// JSR
		case 0x20:
			a = absolute_read(cpu)  - 1;
			cpu -> RAM[cpu -> sp + 1] = (unsigned char) cpu -> pc + 2 >> 8;
			cpu -> RAM[cpu -> sp + 2] = (unsigned char) cpu -> pc + 2 && 0xff;

			cpu -> sp += 2;
			cpu -> pc = a;
			cpu -> cycles += 6;
			break;
		// LDA
		case 0xa9: // CHECKED
		case 0xa5: // CHEKCED
		case 0xb5: // CHECKED
		case 0xad: // CHECKED
		case 0xbd: // CHECKED
		case 0xb9: // CHECKED
		case 0xa1: // CHECKED
		case 0xb1: // CHECKED
			cpu_lda(cpu);
			break;

		// LDX 
		case 0xa2: // CHECKED
		case 0xa6: // CHECKED
		case 0xb6: // CHECKED
		case 0xae: // CHECKED
		case 0xbe: // CHECKED
			cpu_ldx(cpu);
			break;

		// LDY
		case 0xa0: // CHECKED
		case 0xa4: // CHECKED
		case 0xb4: // CHECKED
		case 0xac: // CHECKED
		case 0xbc: // CHECKED
			cpu_ldy(cpu);
			break;

		// LSR
		case 0x4a:
		case 0x46:
		case 0x56:
		case 0x4e:
		case 0x5e:
			cpu_lsr(cpu);
			break;
		// NOP
		case 0xea:
			cpu -> pc += 1;
			cpu -> cycles += 2;
			break;
		// ORA
		case 0x09:
		case 0x05:
		case 0x15:
		case 0x0d:
		case 0x1d:
		case 0x19:
		case 0x01:
		case 0x11:
			cpu_ora(cpu);
			break;
		// REGISTERS OPERATIONS
		case 0xaa:
		case 0x8a:
		case 0xca:
		case 0xe8:
		case 0xa8:
		case 0x98:
		case 0x88:
		case 0xc8:
			cpu_reg(cpu);
			break;

		// ROL
		case 0x2a:
		case 0x26:
		case 0x36:
		case 0x2e:
		case 0x3e:
			cpu_rol(cpu);
			break;
		// ROR 
		case 0x6a:
		case 0x66:
		case 0x76:
		case 0x6e:
		case 0x7e:
			cpu_ror(cpu);
			break;

		// RTI
		case 0x40:
			cpu_rti(cpu);
			break;
		// SBC
		case 0xe9:
		case 0xe5:
		case 0xf5:
		case 0xed:
		case 0xfd:
		case 0xf9:
		case 0xe1:
		case 0xf1:
			cpu_sbc(cpu);
			break;
		// STA
		case 0x85: // CHECKED
		case 0x95: // CHECKED
		case 0x8d: // CHECKED
		case 0x9d: // CHECKED
		case 0x99: // CHECKED
		case 0x81: // CHECKED
		case 0x91: // CHECKED
			cpu_sta(cpu);
			break;
		// STACK INSTRUCTIONS

		case 0x9a:
		case 0xba:
		case 0x48:
		case 0x68:
		case 0x08:
		case 0x28:
			cpu_stk(cpu);
			break;
		default:
			printf("OPCODE ERROR: %x\n", cpu -> opcode);
			done = 1;
	}

	return done;
}

int console( cpu6502 * cpu )
{
	fprintf(stdout,"DEBUG\n");
	return 1;
}


void print_state( cpu6502 * cpu)
{
	printf("==============================\n");
	printf("Registers:\n");
	printf("X : %x\n", cpu -> X);
	printf("Y : %x\n", cpu -> Y);
	printf("A : %x\n", cpu -> A);
	printf("Cycles : %d\n",cpu -> cycles);
	printf("Opcode : 0x%x\n", cpu -> RAM[cpu -> pc]);

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
//	for (int i = 0x0; i < 0x2000; i ++)
//		fprintf(stderr,"%c", cpu -> RAM[i]);
}

void clear_flag( cpu6502 * cpu, int FLAG )
{
	cpu -> flags[FLAG] = 0;
}

void set_flag( cpu6502 * cpu, int FLAG )
{
	cpu -> flags[FLAG] = 1;
}




void cpu_ldx(cpu6502 * cpu)
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
			a = zero_page_read_Y(cpu);
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
	cpu -> flags[FLAG_Z] = (cpu -> X == 0)?1:0;
	cpu -> flags[FLAG_N] = (cpu -> X & 0x80) >> 7;
}
void cpu_ldy(cpu6502 * cpu)
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
			zero_page_read_X(cpu);
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
	cpu -> flags[FLAG_Z] = (cpu -> Y == 0)?1:0;
	cpu -> flags[FLAG_N] = (cpu -> Y & 0x80) >> 7;
}



void cpu_lda(cpu6502 * cpu)
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
	cpu -> flags[FLAG_N] = cpu -> A >> 7;
	cpu -> flags[FLAG_Z] = (cpu -> A == 0)?1:0;
}

void cpu_sta(cpu6502 * cpu)
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


void cpu_and( cpu6502 * cpu)
{
	address a;
	switch(cpu -> opcode)
	{
		case 0x29:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0x25:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 3;
			break;
		case 0x35:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 4;
			break;
		case 0x2d:
			a =absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x3d:
			a = absolute_read_X(cpu);
			cpu -> cycles += 3;
			cpu -> pc += 4;
			break;
		case 0x39:
			a = absolute_read_Y(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x21:
			a = indirect_X_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 6;
			break;
		case 0x31:
			a = indirect_Y_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 5;
			break;
	}
	cpu -> A &= cpu -> RAM[a];
	cpu -> flags[FLAG_N] = 0;
	cpu -> flags[FLAG_Z] = 0;
	if (cpu -> A >> 7) cpu -> flags[FLAG_N] = 1;
	if (!cpu -> A) cpu -> flags[FLAG_N] = 1;
}

void cpu_adc( cpu6502 * cpu)
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
	cpu -> flags[FLAG_V] = (cpu -> A < tmp)?1:0;
	cpu -> flags[FLAG_N] = cpu -> A >> 7;
	cpu -> flags[FLAG_C] = (tmp > 255)?1:0;



	cpu -> A = tmp % 256;
	if(tmp > 255)
	{
		cpu -> flags[FLAG_C] = 1;
		cpu -> flags[FLAG_N] = 1;
	}
	if(128 <= tmp < 255)
		cpu -> flags[FLAG_V] = 1;


	if( !cpu -> A )set_flag(cpu, FLAG_Z);
}	

void cpu_asl( cpu6502 * cpu)
{
	/// ATTENTION AUX CYCLES -> REPRENDRE LES ROUTINES DE LECTURE
	/// Ajouter des outines sans cycles supplémentaires
	address a;
	int carry = 0;
	int tmp;
	switch(cpu -> opcode)
	{
		case 0x0a:
			cpu -> flags[FLAG_C] = cpu -> A >> 7;
			tmp = cpu -> A << 1;
			cpu -> A <= 1;
			
			cpu -> pc += 1;
			cpu -> cycles += 2;
			break;
		case 0x06:
			a = zero_page_read(cpu);
			tmp = cpu -> RAM[a] << 1;
			cpu -> pc += 2;
			cpu -> cycles += 5;
			break;
		case 0x16:
			a = zero_page_read_X(cpu);
			tmp = cpu -> RAM[a] << 1;
			cpu -> pc += 2;
			cpu -> cycles += 6;
			break;
		case 0x0e:
			a = absolute_read(cpu);
			tmp = cpu -> RAM[a] << 1;
			cpu -> pc += 3;
			cpu -> cycles += 6;
			break;
		case 0x1e:
			a = absolute_read_X_no_c(cpu);
			tmp = cpu -> RAM[a] << 1;
			cpu -> pc += 3;
			cpu -> cycles += 7;
			break;
	}

	if(cpu -> opcode != 0x0a)
	{
		cpu -> flags[FLAG_C] = cpu -> RAM[a] >> 7;
		cpu -> RAM[a] = tmp;
	}
	cpu -> flags[FLAG_Z] = 0;
	cpu -> flags[FLAG_N] = (tmp & 0xff) >> 7;
	if (! tmp % 256)
		cpu -> flags[FLAG_Z] = 1;

}

void break_boundary(cpu6502 * cpu)
{
	uint16_t tmp = cpu -> RAM[cpu -> pc + 1];
	// boundaries break
	if (tmp + ((cpu -> pc) & 0x00ff) > 256)
		cpu -> cycles += 1;
}


void cpu_branch(cpu6502 * cpu)
{
	uint16_t tmp;
	int flag = 0;
	switch(cpu -> opcode)
	{
		case 0x10:
			if(!cpu -> flags[FLAG_N]) flag = 1;
			break;
		case 0x30:
			if(cpu -> flags[FLAG_N]) flag = 1;
			break;
		case 0x50:
			if(!cpu -> flags[FLAG_V]) flag = 1;
			break;
		case 0x70:
			if(cpu -> flags[FLAG_V]) flag = 1;
			break;
		case 0x90:
			if(!cpu -> flags[FLAG_C]) flag = 1;
			break;
		case 0xb0:
			if(cpu -> flags[FLAG_C]) flag = 1;
			break;
		case 0xd0:
			if(!cpu -> flags[FLAG_Z]) flag = 1;
			break;
		case 0xf0:
			if(cpu -> flags[FLAG_Z]) flag = 1;
			break;
	}
	if(flag)
	{
		tmp = cpu -> RAM[cpu -> pc + 1];
		break_boundary(cpu);
		cpu -> pc += tmp;
	}
	else
		cpu -> pc += 2;
	cpu -> cycles += 2;
}
void cpu_bit(cpu6502 * cpu)
{
	address a;
	unsigned int tmp;
	switch(cpu -> opcode)
	{
		case 0x24:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 3;
			break;
		case 0x2c:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
	}
	tmp = cpu -> A & cpu -> RAM[a];
	cpu -> flags[FLAG_N] = (cpu -> RAM[a] & 0x80) >> 7; // On récupère le 7eme bit
	cpu -> flags[FLAG_V] = (cpu -> RAM[a] & 0x40) >> 6; // On récupère le 7eme bit
	if (!tmp)
		cpu -> flags[FLAG_Z] = 1;

}

void cpu_break(cpu6502 * cpu)
{
	cpu -> pc += 3;
	cpu -> cycles += 7;
	cpu -> flags[FLAG_B] = 1;
	cpu -> flags[FLAG_B2] = 1;
	cpu -> flags[FLAG_I] = 1;
}

void cpu_cmp(cpu6502 * cpu)
{
	address a;
	int tmp;
	switch(cpu -> opcode)
	{
		case 0xc9:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xc5:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 3;
			break;
		case 0xd5: 
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 4;
			break;
		case 0xcd:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0xdd:
			a = absolute_read_X(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0xd9:
			a = absolute_read_Y(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0xc1:
			a = indirect_X_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 6;
			break;
		case 0xd1:
			a = indirect_Y_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 5;
			break;
	}
	tmp = cpu -> A - cpu -> RAM[a];
	cpu -> flags[FLAG_C] = 0;
	cpu -> flags[FLAG_Z] = 0;
	cpu -> flags[FLAG_N] = 0;
	if (cpu -> A >= cpu -> RAM[a])
		cpu -> flags[FLAG_C] = 1;
	if (!tmp)
		cpu -> flags[FLAG_Z] = 1;
	cpu -> flags[FLAG_N] = tmp & 0x80 >> 7;
}




void cpu_cpx(cpu6502  * cpu)
{
	address a;
	int tmp;
	switch(cpu -> opcode)
	{
		case 0xe0:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xe4:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xec:
			a = absolute_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
	}
	tmp = cpu -> X - cpu -> RAM[a];

	cpu -> flags[FLAG_C] = 0;
	cpu -> flags[FLAG_Z] = 0;
	cpu -> flags[FLAG_N] = 0;

	if (cpu -> X >= cpu -> RAM[a])
		cpu -> flags[FLAG_C] = 1;
	if (!tmp)
		cpu -> flags[FLAG_Z] = 1;
	cpu -> flags[FLAG_N] = tmp & 0x80 >> 7;
}

void cpu_cpy(cpu6502  * cpu)
{
	address a;
	int tmp;
	switch(cpu -> opcode)
	{
		case 0xe0:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xe4:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xec:
			a = absolute_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
	}
	tmp = cpu -> Y - cpu -> RAM[a];

	cpu -> flags[FLAG_C] = 0;
	cpu -> flags[FLAG_Z] = 0;
	cpu -> flags[FLAG_N] = 0;

	if (cpu -> Y >= cpu -> RAM[a])
		cpu -> flags[FLAG_C] = 1;
	if (!tmp)
		cpu -> flags[FLAG_Z] = 1;
	cpu -> flags[FLAG_N] = tmp & 0x80 >> 7;
}

void cpu_dec(cpu6502 * cpu)
{
	address a;
	switch(cpu -> opcode)
	{
		case 0xc6:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles = 5;
			break;
		case 0xd6:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles = 5;
			break;
		case 0xce:
			a = absolute_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles = 5;
			break;
		case 0xde:
			a = absolute_read_X_no_c(cpu);
			cpu -> pc += 2;
			cpu -> cycles = 5;
			break;
	}
	cpu -> RAM[a] -= 1;

	cpu -> flags[FLAG_Z] = 0;
	cpu -> flags[FLAG_N] = 0;
	if (!cpu -> RAM[a])
		cpu -> flags[FLAG_Z] = 1;
	cpu -> flags[FLAG_N] = cpu -> RAM[a] & 0x80 >> 7;
}

void cpu_eor(cpu6502 * cpu)
{
	address a;
	unsigned int tmp;
	switch(cpu -> opcode)
	{
		case 0x49:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0x45:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 3;
			break;
		case 0x55:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 4;
			break;
		case 0x4d:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x5d:
			a = absolute_read_X(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x59:
			a = absolute_read_Y(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x41:
			a = indirect_X_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 6;
			break;
		case 0x51:
			a = indirect_Y_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 5;
			break;
	}
	cpu -> flags[FLAG_Z] = 0;
	cpu -> A = cpu -> A ^ cpu -> RAM[a];
	if (!cpu -> A)
		cpu -> flags[FLAG_Z] = 1;

	cpu -> flags[FLAG_N] = cpu -> A >> 7;
}
void cpu_inc(cpu6502 * cpu)
{
	address a;
	switch(cpu -> pc)
	{

		case 0xe6:
			a = zero_page_read(cpu);
			cpu -> cycles += 2;
			cpu -> pc += 5;
			break;
		case 0xf6:
			a = zero_page_read_X(cpu);
			cpu -> cycles += 2;
			cpu -> pc += 5;
			break;
		case 0xee:
			a = absolute_read(cpu);
			cpu -> cycles += 2;
			cpu -> pc += 5;
			break;
		case 0xfe:
			a = absolute_read_X(cpu);
			cpu -> cycles += 2;
			cpu -> pc += 5;
			break;
	}
	cpu -> RAM[a] += 1;
	cpu -> flags[FLAG_Z] = 0;

	if (!cpu -> RAM[a])
		cpu -> flags[FLAG_Z] = 1;

	cpu -> flags[FLAG_N] = cpu -> RAM[a] >> 7;
}

void cpu_jmp(cpu6502 * cpu)
{
	address a;
	switch(cpu -> opcode)
	{
		case 0x4c:
			a = absolute_read(cpu);
			cpu -> cycles += 3;
		case 0x6c:
			a = indirect_read(cpu);
			cpu -> cycles += 5;
	}
	cpu -> pc = a;
}

void cpu_lsr(cpu6502 * cpu)
{
	address a;

	switch(cpu -> opcode)
	{

		case 0x4a:
			// Accumulator
			cpu -> flags[FLAG_C] = cpu -> A >> 7;
			cpu -> A <<= 1; 
			cpu -> flags[FLAG_N] = cpu -> A >> 7;
			cpu -> flags[FLAG_Z] = (cpu -> A == 0)?1:0;
			cpu -> pc += 1;
			cpu -> cycles += 2;
			return;
		case 0x46:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 5;
			break;
		case 0x56:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 6;
			break;
		case 0x4e:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 6;
			break;
		case 0x5e:
			a = absolute_read_X(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 7;
			break;
	}
	cpu -> flags[FLAG_C] = cpu -> RAM[a] >> 7;
	cpu -> RAM[a] <= 1; 
	cpu -> flags[FLAG_N] = cpu -> RAM[a] >> 7;
	cpu -> flags[FLAG_Z] = (cpu -> RAM[a] == 0)?1:0;
}


void cpu_ora(cpu6502 * cpu)
{

	address a;
	switch(cpu -> opcode)
	{
		case 0x09:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0x05:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0x15:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 3;
			break;
		case 0x0d:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x1d:
			a = absolute_read_X(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x19:
			a = absolute_read_Y(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 4;
			break;
		case 0x01:
			a = indirect_X_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 6;
			break;
		case 0x11:
			a = indirect_Y_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 5;
			break;
	}
	cpu -> A |= cpu -> RAM[a];
	cpu -> flags[FLAG_N] = cpu -> A >> 7;
	cpu -> flags[FLAG_Z] = (cpu -> A == 0)?1:0;
}

void cpu_reg(cpu6502 * cpu)
{
	address a;
	unsigned char tmp;
	switch(cpu -> opcode)
	{
		case 0xaa:
			cpu -> X = cpu -> A;
			tmp = cpu -> X;
			break;
		case 0x8a:
			cpu -> A = cpu -> X;
			tmp = cpu -> A;
			break;
		case 0xca:
			cpu -> X --;
			tmp = cpu -> X;
			break;
		case 0xe8:
			cpu -> X ++;
			tmp = cpu -> X;
			break;
		case 0xa8:
			cpu -> Y = cpu -> A;
			tmp = cpu -> Y;
			break;
		case 0x98:
			cpu -> A = cpu -> Y;
			tmp = cpu -> A;
			break;
		case 0x88:
			cpu -> Y --;
			tmp = cpu -> Y;
			break;
		case 0xc8:
			cpu -> Y ++;
			tmp = cpu -> Y;
			break;
	}
	cpu -> flags[FLAG_N] = tmp >> 7;
	cpu -> flags[FLAG_Z] = (tmp == 0)?1:0;

	cpu -> cycles += 2;
	cpu -> pc += 1;
}
void cpu_rol(cpu6502 * cpu)
{
	address a;
	int tmp;
	switch(cpu -> opcode)
	{
		case 0x2a:

			// Accumulator
			tmp = cpu -> A >> 7;
			cpu -> A <<= 1;
			cpu -> A += cpu -> flags[FLAG_C];
			cpu -> flags[FLAG_C] = tmp;
			cpu -> flags[FLAG_Z] = (cpu -> A == 0)?1:0;
			cpu -> flags[FLAG_N] = cpu -> A >> 7;
			
			cpu -> cycles += 2;
			cpu -> pc += 1;
			return;
		case 0x26:
			a =zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 5;
		case 0x36:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 6;
		case 0x2e:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 6;
		case 0x3e:
			a = absolute_read_X_no_c(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 7;
	}
	tmp = cpu -> RAM[a] >> 7;
	cpu -> RAM[a] <<= 1;
	cpu -> RAM[a] += cpu -> flags[FLAG_C];
	cpu -> flags[FLAG_C] = tmp;
	cpu -> flags[FLAG_Z] = (cpu -> RAM[a] == 0)?1:0;
	cpu -> flags[FLAG_N] = cpu -> RAM[a] >> 7;
	
}

void cpu_ror(cpu6502 * cpu)
{
	address a;
	int tmp;
	switch(cpu -> opcode)
	{
		case 0x6a:

			// Accumulator
			tmp = cpu -> A & 0x1;
			cpu -> A >>= 1;
			cpu -> A |= cpu -> flags[FLAG_C] << 7;
			cpu -> flags[FLAG_C] = tmp;
			cpu -> flags[FLAG_Z] = (cpu -> A == 0)?1:0;
			cpu -> flags[FLAG_N] =  cpu -> A >> 7;
			
			cpu -> cycles += 2;
			cpu -> pc += 1;
			return;
		case 0x66:
			a =zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 5;
		case 0x76:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 6;
		case 0x6e:
			a = absolute_read(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 6;
		case 0x7e:
			a = absolute_read_X_no_c(cpu);
			cpu -> pc += 3;
			cpu -> cycles += 7;
	}
	tmp = cpu -> RAM[a] & 0x1;
	cpu -> RAM[a] >>= 1;
	cpu -> RAM[a] |= cpu -> flags[FLAG_C] << 7;
	cpu -> flags[FLAG_C] = tmp;
	cpu -> flags[FLAG_Z] = (cpu -> RAM[a] == 0)?1:0;
	cpu -> flags[FLAG_N] = cpu -> RAM[a] >> 7;
	
}

void cpu_rti(cpu6502 * cpu)
{
	// get flag word
	unsigned char word = cpu -> RAM[cpu -> sp];
	cpu -> sp --;
	cpu -> flags[FLAG_C]  = word & 0x1;
	cpu -> flags[FLAG_Z]  = (word & 0x2)  >> 1 ;
	cpu -> flags[FLAG_I]  = (word & 0x4)  >> 2 ;
	cpu -> flags[FLAG_D]  = (word & 0x8)  >> 3 ;
	cpu -> flags[FLAG_B]  = (word & 0x10) >> 4 ;
	cpu -> flags[FLAG_B2] = (word & 0x20) >> 5 ;
	cpu -> flags[FLAG_V]  = (word & 0x40) >> 6 ;
	cpu -> flags[FLAG_N]  = (word & 0x80) >> 7 ;
	// get pc 
	int pc = cpu -> RAM[cpu -> sp] << 8 | cpu -> RAM[cpu -> sp -1] ;
	cpu -> sp -= 2;
	cpu -> pc = pc;
	cpu -> cycles += 6;

}
void cpu_sbc(cpu6502 * cpu)
{
	address a;
	int newC = 0;
	int tmp;
	switch( cpu -> opcode)
	{

		case 0xe9:
			a = immediate_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xe5:
			a = zero_page_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xf5:
			a = zero_page_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xed:
			a = absolute_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xfd:
			a = absolute_read_X(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xf9:
			a = absolute_read_Y(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xe1:
			a = indirect_X_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
		case 0xf1:
			a = indirect_Y_read(cpu);
			cpu -> pc += 2;
			cpu -> cycles += 2;
			break;
	}
	if (cpu -> A >= cpu -> RAM[a])
		newC = 1;
	tmp = cpu -> A;

	cpu -> A = cpu -> A - cpu -> RAM[a] - (1 -  cpu -> flags[FLAG_C]);

	cpu -> flags[FLAG_V] = (cpu -> A > tmp)? 1 : 0;
	cpu -> flags[FLAG_C] = newC;	
	cpu -> flags[FLAG_Z] = (cpu -> A == 0)?1:0;
	cpu -> flags[FLAG_N] = cpu -> A >> 7;

}

void cpu_stk(cpu6502 * cpu)
{
	unsigned char word;

	switch(cpu -> opcode)
	{
		case 0x9a:
			//TXS
			cpu -> sp ++;
			cpu -> RAM[cpu -> sp] = cpu -> X;
			cpu -> cycles += 2; 
			break;

		case 0xba:
			// TSX
			cpu -> X = cpu -> RAM[cpu -> sp];
			cpu -> sp --;
			cpu -> cycles += 2; 
			cpu -> flags[FLAG_Z] = (cpu -> X == 0)?1:0;
			cpu -> flags[FLAG_N] =  cpu -> X >> 7;
			break;

		case 0x48:
			// PHA
			cpu -> sp ++;
			word = cpu -> A & 0xff00 >> 8;
			cpu -> RAM[cpu -> sp] = word;
			cpu -> sp ++;
			word = cpu -> A & 0xff;
			cpu -> RAM[cpu -> sp] = word;
			cpu -> cycles += 3; 
			break;

		case 0x68:
			// PLA
			cpu -> A = cpu -> RAM[cpu -> sp - 1] << 8 | cpu -> RAM[cpu -> sp];
			cpu -> sp -= 2;
			cpu -> cycles += 4; 

			cpu -> flags[FLAG_Z] = (cpu -> A == 0)?0:1;
			break;
		case 0x08:
			//PHP
			word = 0;
			word |= cpu -> flags[FLAG_C];
			word |= cpu -> flags[FLAG_Z] << 1;
			word |= cpu -> flags[FLAG_I] << 2;
			word |= cpu -> flags[FLAG_D] << 3;
			word |= cpu -> flags[FLAG_B] << 4;
			word |= cpu -> flags[FLAG_B2] << 5;
			word |= cpu -> flags[FLAG_V] << 6;
			word |= cpu -> flags[FLAG_N] << 7;
			cpu -> sp ++;
			cpu -> RAM[cpu -> sp] = word;
			cpu -> cycles +=2; 
			break;
		case 0x28:
			//PLP
			word = cpu -> RAM[cpu -> sp];
			cpu -> sp --;
			cpu -> flags[FLAG_C]  = word & 0x1;
			cpu -> flags[FLAG_Z]  = (word & 0x2)  >> 1 ;
			cpu -> flags[FLAG_I]  = (word & 0x4)  >> 2 ;
			cpu -> flags[FLAG_D]  = (word & 0x8)  >> 3 ;
			cpu -> flags[FLAG_B]  = (word & 0x10) >> 4 ;
			cpu -> flags[FLAG_B2] = (word & 0x20) >> 5 ;
			cpu -> flags[FLAG_V]  = (word & 0x40) >> 6 ;
			cpu -> flags[FLAG_N]  = (word & 0x80) >> 7 ;
			// get pc 
			break;
	}
	cpu -> pc += 1;
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

address absolute_read_X_no_c(cpu6502 * cpu)
{
	return (cpu -> RAM[cpu -> pc + 2] << 8|cpu -> RAM[cpu -> pc + 1])  + cpu -> X;
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

address indirect_read(cpu6502 * cpu)
{
	return cpu -> RAM[cpu -> pc + 2] << 8 | cpu -> RAM[cpu -> pc + 1];
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



//int main(int argc, char * argv[])
//{
//	int done = 0;
//	unsigned int tmp = 0;
//	cpu6502 * cpu;
//	cpu = init_cpu6502();
//
//	FILE *ptr;
//	ptr = fopen("ROM","rb");
//
//	fread(cpu -> RAM + 0x200, 1,sizeof(cpu -> RAM) - 0x200,ptr);
//	
//	while ( ! done)
//	{
//		done = run_cycle(cpu);
//	}
//	print_state(cpu);
//}
