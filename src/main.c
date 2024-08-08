#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// TODO: Linux version (windows syscalls are too inaccessible).
// TODO: Arguments to program procedure should contain important information
//       (for example, beginning address of written instructions could be passed as
//       first argument, and it can then be grabbed from register when writting hex).

typedef unsigned char u8;

static u8 error = 0;
u8 translate(u8 symbol) {
	switch(symbol) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		error = 0;
		return symbol - '0';
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		error = 0;
		return symbol - 'a' + 10;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		error = 0;
		return symbol - 'A' + 10;
	}
	
	error = 1;
	return -1;
}

int main(int argc, char** argv) {
	char* machine_code_file = "machine_code.mc";
	if(argc == 2) machine_code_file = argv[1];
	FILE* mc_file = mc_file = fopen(machine_code_file, "rb");
	if(!mc_file) return -1;
	int ret = fseek(mc_file, 0, SEEK_END);
	if(ret) return -1;
	long mc_size = ftell(mc_file);
	if(mc_size == -1L) return -1;
	fseek(mc_file, 0, SEEK_SET);
	if(ret) return -1;

	u8* program_instructions = VirtualAlloc(NULL, mc_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	typedef void* (*Program)(void);
	Program program = (Program)program_instructions;

	u8* machine_code = malloc(mc_size);
	fread(machine_code, 1, mc_size, mc_file);
	
	u8 temp = 0;
	u8 byte = 0;
	u8 nibble_is_even = 1;
	u8 comment = 0;
	for(long i = 0; i < mc_size; ++i) {
		if(machine_code[i] == '#') comment = 1;
		if(machine_code[i] == '\n') comment = 0;
		temp = translate(machine_code[i]);
		if(!error && comment == 0) {
			byte |= (u8)((nibble_is_even) ? (temp << 4) : temp);
			nibble_is_even ^= 1;

			if(nibble_is_even) {
				*program_instructions++ = byte;
				byte = 0;
			}
		}
	}

	fclose(mc_file);

	if((void*)program_instructions != (void*)program) {
		long rax = (long)program();
		printf("RAX: %ld\n", rax);
	}
	
	return 0;
}
