#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* name;
	int address;
} label;

typedef struct{
	label** arr;
	int count;

	int parse_mode;
} label_c;

typedef struct{
	int codes[8];
	char type;
} instr_set;

void parse_label(label_c* labels, char *program, int line);
void parse_instr(instr_set* instr_values, char* program, int line);

int set_typecode(instr_set* instr_values, char* str);

void reset_codes(instr_set* instr_values);
void add_label(label_c* labels, char *arg, int line);

void pc_hex(int x);
int hexcode(instr_set* instr_values);
void get_r(instr_set* instr_values, char* arg);
void print(label_c* labels);

int main(int argc, char * argv[]){

	// Parsing Section
	// parsing variables
	label_c labels;
	labels.arr = NULL;
	labels.count = 0;
	labels.parse_mode = -1;

	instr_set instr_values;

	char readline[81];
	char** program = NULL;
	char* tmp;
	int line = 0;

	// Stores stdin
	while(fgets(readline, sizeof(readline), stdin) != NULL){

		if(program == NULL){
			program = (char**) malloc(sizeof(char*));
		} else{
			program = (char**) realloc(program, (line+1) * sizeof(char*));
		}

		program[line] = (char*) malloc((strlen(readline)+1) * sizeof(char));
		strcpy(program[line], readline);

		// pass 1 - get labels with pc address
		parse_label(&labels, readline, line++);
	}

	// int op;	// R/I/J	0
	// int rs, rt;	// R/I		1, 2
	// int rd, shamt, func;	// R	3, 4, 5
	// int immed;	// I		6
	// int targaddr;	// J	7

	// pass 2 - get instruction -> decimal -> machine code
	int line_count;
	for(line_count = 0; line_count < line; ++line_count){
		reset_codes(&instr_values);
		parse_instr(&instr_values, program[line_count], line_count);
	}

	print(&labels);	
	printf("Exiting\n");
	free(labels.arr);
	labels.arr = NULL;
	return 0;
}

void reset_codes(instr_set* instr_values){
	
	int i;
	for(i = 0; i < 8; ++i)
		instr_values->codes[i] = 0;
	instr_values->type = 157;

}

void parse_label(label_c* labels, char *program, int line){

	char* delim = " \n\t";	
	char* arg;
	arg = strtok(program, delim);

	do{
		if(arg == NULL) break;

		if(arg[strlen(arg)-1] == ':'){
			add_label(labels, arg, line);
			++labels->count;
		}
		return;
	} while(arg != NULL);
}

void parse_instr(instr_set* instr_values, char* program, int line){

	char* delim = ", \n\t";
	char* arg;
	arg = strtok(program, delim);
	int op_result;
	int first_line = -1;

	do{
		if(arg == NULL) break;

		// checks to see if first arg is label
		if(arg[strlen(arg)-1] == ':'){
			arg = strtok(NULL, delim);
			continue;
		}

		op_result = set_typecode(instr_values, arg);
		if(op_result == 1){
			if(first_line == -1)
				first_line = line;
			printf("0x%08X: ", (line-(first_line==-1?0:first_line))*4);

			arg = strtok(NULL, delim);
			if(instr_values->type == 'R')
				get_r(instr_values, arg);
			return;
		} else if(op_result == 0){
			if(first_line == -1)
				first_line = line;
			printf("0x%08X: ", (line-(first_line==-1?0:first_line))*4);
		}
		arg = strtok(NULL, delim);

	} while(arg != NULL);
}

void add_label(label_c* labels, char *arg, int line){
		
	if(labels->arr == NULL){
		labels->arr = (
			label**)malloc(sizeof(label*));
	} else{
		labels->arr = (label**)realloc(
			labels->arr, (labels->count+1) * sizeof(label*));
	}

	// sets symbol struct at pos loop
	labels->arr[labels->count] = (
		label*)malloc(sizeof(label));
	labels->arr[labels->count]->name = (
		char*)malloc(strlen(arg) * sizeof(char));
	strncpy(labels->arr[labels->count]->name, arg, strlen(arg)-1);
	labels->arr[labels->count]->name[strlen(arg)] = '\0';
	
	labels->arr[labels->count]->address = line*4;
}

int set_typecode(instr_set* instr_values, char* str){

	if(strcmp(str, "add") == 0){
		instr_values->codes[0] = 32;
		instr_values->type = 'R';
	}else if(strcmp(str, "addi") == 0){
		instr_values->codes[0] = 8;
		instr_values->type = 'I';
	}else if(strcmp(str, "nor") == 0){
		instr_values->codes[0] = 39;
		instr_values->type = 'R';
	}else if(strcmp(str, "ori") == 0){
		instr_values->codes[0] = 13;
		instr_values->type = 'I';
	}else if(strcmp(str, "sll") == 0){
		instr_values->codes[0] = 0;
		instr_values->type = 'R';
	}else if(strcmp(str, "lui") == 0){
		instr_values->codes[0] = 15;
		instr_values->type = 'I';
	}else if(strcmp(str, "sw") == 0){
		instr_values->codes[0] = 43;
		instr_values->type = 'I';
	}else if(strcmp(str, "lw") == 0){
		instr_values->codes[0] = 35;
		instr_values->type = 'I';
	}else if(strcmp(str, "bne") == 0){
		instr_values->codes[0] = 5;
		instr_values->type = 'I';
	}else if(strcmp(str, "j") == 0){
		instr_values->codes[0] = 2;
		instr_values->type = 'J';
	}else if(strcmp(str, "la") == 0){
		return 0;
	}else{
		return -1;
	}
	return 1;
}

void pc_hex(int x){
	x -= 4;
	char hex[9];
	hex[0] = '0';
	hex[1] = 'x';
	hex[8] = '\0';

	int i, tmp;
	for(i = 7; x != 0; --i){
		tmp = x % 16;
		x = x>>4;
		if(tmp <= 9)
			hex[i] = tmp + '0';
		else
			hex[i] = tmp + '7';
	}

	// Sign extends
	for(; i > 1; --i)
		hex[i] = '0';

	printf("%s: ", hex);
}

void get_r(instr_set* instr_values, char* arg){

	char* delim = ", \n";
	int op = instr_values->codes[0];

	instr_values->codes[3] = get_reg(arg);
	arg = strtok(NULL, delim);

	instr_values->codes[5] = instr_values->codes[0];
	// add and nor
	if(op == 32 || op == 39){
		instr_values->codes[1] = get_reg(arg);
		arg = strtok(NULL, delim);
		instr_values->codes[2] = get_reg(arg);
	} else{
		instr_values->codes[2] = get_reg(arg);
		arg = strtok(NULL, delim);
		instr_values->codes[4] = atoi(arg);
	}
	printf("0x%08X\n", hexcode(instr_values));
}

int get_reg(char* reg){

	if(strncmp(reg, "$", 1) == 0){
		// Sets register code
		if(reg[1] == '0') return 0;
		else{ return ((int)(reg[2])-'0') + (reg[1] == 's' ? 16 : 8); }
	}
	return -1;
}

int hexcode(instr_set* instr_values){

	int instruction = 0;

	if(instr_values->type = 'R'){
		instruction = instruction | instr_values->codes[1];
		instruction = instruction << 5;
		instruction = instruction | instr_values->codes[2];
		instruction = instruction << 5;
		instruction = instruction | instr_values->codes[3];
		instruction = instruction << 5;
		instruction = instruction | instr_values->codes[4];
		instruction = instruction << 6;
		instruction = instruction | instr_values->codes[5];
		return instruction;
	}
}

void print(label_c* labels){
	int i;
	
	for(i = 0; i < labels->count; ++i){
		printf("%s, %d\n", labels->arr[i]->name, labels->arr[i]->address);
	}
}
