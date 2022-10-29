#include <stdio.h>
#include <stdlib.h>

#define byte unsigned char
#define LOAD_MEM 0
#define LOAD_VAL 1
#define STORE 2
#define ADD 3
#define SUB 4
#define MUL 5
#define DIV 6
#define INC 7
#define DEC 8
#define AND 9
#define OR 10
#define NOT 11
#define JMP 12
#define JZ 13
#define JNZ 14
#define JG 15
#define JL 16
#define JGE 17
#define JLE 18
#define HLT 19

// define ZeroACC, Carry and Overflow
enum flags
{
	ZeroACC = 1,
	Carry = 2,
	Overflow = 4
};

// updates the status
void uptadeSTAT(byte *STAT, int tof, int ocz)
{
	if (tof)
	{
		*STAT = *STAT | ocz; // set status to overflow, carry or 0
	}
	else
	{
		*STAT = *STAT & (255 - ocz); // set status = 0
	}
}

// checks if the accumulator is zero
void testACC(byte acc, byte *STAT)
{
	byte tof;

	if (acc == 0)
	{
		tof = 1;
	}
	else
	{
		tof = 0;
	}
	uptadeSTAT(STAT, tof, ZeroACC);
}

typedef struct format
{
	byte opcode;
	byte operand;
} instructions;

int main(int argc, char *argv[])
{

	instructions number[256];
	FILE *bin_file;

	// registers
	byte acc = 0;  // accumulator
	byte pc = 0;   // program counter
	byte STAT = 0; // status

	// reads a binary file
	bin_file = fopen(argv[1], "rb");

	if (bin_file == NULL)
	{
		printf("-> Error opening the file %s \n", argv[1]);
		return 1;
	}

	byte instruction[2];

	// reads one line at a time (every 2, 1 byte at a time)
	while (fread(instruction, sizeof(byte), 2, bin_file))
	{
		number[pc].opcode = instruction[0];
		number[pc].operand = instruction[1];
		pc++;
	}

	fclose(bin_file);

	pc = 0;
	byte mem[256];
	byte tof;
	byte opcode;
	byte operand;

	for (;;)
	{
		opcode = number[pc].opcode;
		operand = number[pc].operand;

		STAT = 0;
		switch (opcode)
		{
		// instructions
		case LOAD_MEM:
			acc = mem[operand];
			testACC(acc, &STAT);
			pc++;
			break;

		case LOAD_VAL:
			acc = operand;
			testACC(acc, &STAT);
			pc++;
			break;

		case STORE:
			mem[operand] = acc;
			pc++;
			break;

		case ADD:
			// checks for carry
			if ((int)(acc + mem[operand]) > 255)
			{
				tof = 1;
			}
			else
			{
				tof = 0;
			}

			acc += mem[operand];
			uptadeSTAT(&STAT, tof, Carry);
			testACC(acc, &STAT);
			pc++;
			break;

		case SUB:
			acc -= mem[operand];
			testACC(acc, &STAT);
			pc++;
			break;

		case MUL:
			// checks for overflow
			if ((int)(acc * mem[operand]) > 255)
			{
				tof = 1;
			}
			else
			{
				tof = 0;
			}

			acc *= mem[operand];
			uptadeSTAT(&STAT, tof, Overflow);
			testACC(acc, &STAT);
			pc++;
			break;

		case DIV:
			acc /= mem[operand];
			testACC(acc, &STAT);
			pc++;
			break;

		case INC:
			// checks for carry
			if ((int)(acc + 1) > 255)
			{
				tof = 1;
			}
			else
			{
				tof = 0;
			}

			++acc;
			uptadeSTAT(&STAT, tof, Carry);
			testACC(acc, &STAT);
			pc++;
			break;

		case DEC:
			--acc;
			testACC(acc, &STAT);
			pc++;
			break;

		case AND:
			acc &= mem[operand];
			testACC(acc, &STAT);
			pc++;
			break;

		case OR:
			acc |= mem[operand];
			testACC(acc, &STAT);
			pc++;
			break;

		case NOT:
			acc = ~acc;
			testACC(acc, &STAT);
			pc++;
			break;

		case JMP:
			pc = operand / 2;
			break;

		case JZ:
			if (acc = 0)
			{
				pc = operand / 2;
			}
			else
			{
				pc += 1;
			}
			break;

		case JNZ:
			if (acc != 0)
			{
				pc = operand / 2;
			}
			else
			{
				pc += 1;
			}
			break;

		case JG:
			if (acc > 0)
			{
				pc = operand / 2;
			}
			else
			{
				pc += 1;
			}
			break;

		case JL:
			if (acc < 0)
			{
				pc = operand / 2;
			}
			else
			{
				pc += 1;
			}
			break;

		case JGE:
			if (acc >= 0)
			{
				pc = operand / 2;
			}
			else
			{
				pc += 1;
			}
			break;

		case JLE:
			if (acc <= 0)
			{
				pc = operand / 2;
			}
			else
			{
				pc += 1;
			}
			break;

		case HLT:
			printf("ACC: %d\nSTAT: %.2X\n", acc, STAT);
			return 0;
		}
	}
}
