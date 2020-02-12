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

void add_label(label_c* labels, char *arg, int line);
void print(label_c* labels);

int main(int argc, char * argv[]){

	// Parsing Section
	// parsing variables
	label_c labels;
	labels.arr = NULL;
	labels.count = 0;
	labels.parse_mode = -1;

	instr_set instr_values;

	char readline[8001];
	char** program = NULL;
	char* tmp;
	int line = 0;

	// Stores stdin
	while(fgets(readline, sizeof(readline), stdin) != NULL){

		if(program == NULL){
			program = (char**) malloc(sizeof(char*));
		} else{
			program = (char**) realloc(program, line+1 * sizeof(char*));
		}

		program[line] = (char*) malloc(strlen(readline)+1 * sizeof(char));
		strcpy(program[line], readline);

		++line;
	}

	// pass 1 - get labels with pc address
	for(line = 0; line < sizeof(program) / sizeof(char*); ++line){
	
		while(sscanf(program[line], "%ms", tmp)){
			parse_label(&labels, tmp, line++);
		}
	}
	print(&labels);

	if(labels.count < 1 || labels.arr == NULL){ 
		printf("Invalid symbols\n");
		return -1;
	}

	// int op;	// R/I/J	0
	// int rs, rt;	// R/I		1, 2
	// int rd, shamt, func;	// R	3, 4, 5
	// int immed;	// I		6
	// int targaddr;	// J	7
	int i = 0;
	for(; i < 8; ++i)
		instr_values.codes[i] = -1;
	instr_values.type = 157;

	// pass 2 - get instruction -> decimal -> machine code
	//while(fgets(instr_pass, sizeof(instr_pass), stdin) != NULL){
	//	parse_instr(&instr_values, instr_pass, line++);
	//}

	printf("Printgin\n");
	print(&labels);	
	printf("Exiting\n");
	free(labels.arr);
	labels.arr = NULL;
	return 0;
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
			arg = NULL;
			return;
		} else{
			arg = strtok(NULL, delim);
		}
	} while(arg != NULL);
}

void parse_instr(instr_set* instr_values, char* program, int line){

	char* delim = ", \n\t";
	char* arg;
	arg = strtok(program, delim);

	do{
		if(arg == NULL) break;

		// checks to see if first arg is label
		if(arg[strlen(arg)-1] == ':'){
			//printf("LBL %s\n", arg);
			//arg = strtok(NULL, delim);
			//continue;
		}

		printf("LBL %s\n", arg);
		arg = strtok(NULL, delim);

	} while(arg != NULL);

	arg = NULL;
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

void print(label_c* labels){
	int i;
	
	printf("Symbols: %d\n", labels->count);
	for(i = 0; i < labels->count; ++i){
		printf("%s, %d\n", labels->arr[i]->name, labels->arr[i]->address);
	}
}
