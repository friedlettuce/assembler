#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* str;
	int line;
} symbol_t;

typedef struct{
	symbol_t** arr;
	int count;
	int parse_mode;
} symbol_c;

void parse_line(symbol_c* symbols, char *program, int line);
void add_arg(symbol_c* symbols, char *arg, int line);
int convert_decimal(char* arg);
void print(symbol_c* symbols);

int main(int argc, char * argv[]){

	// Parsing Section
	// parsing variables
	symbol_c symbols;
	symbols.arr = NULL;
	symbols.count = 0;
	symbols.parse_mode = -1;

	char program[8001];
	int line = 0;

	while(fgets(program, sizeof(program), stdin) != NULL){
		parse_line(&symbols, program, line++);
	}

	if(symbols.count < 1 || symbols.arr == NULL){ 
		printf("Invalid symbols\n");
		return -1;
	}

	print(&symbols);	
	printf("Exiting\n");
	free(symbols.arr);
	symbols.arr = NULL;
	return 0;
}

void parse_line(symbol_c* symbols, char *program, int line){
	
	char* arg;
	arg = strtok(program, " \n\t");

	do{
		if(arg == NULL) break;
		printf("Scanned %s\n", arg);

		if(strcmp(arg, ".text") == 0){
			symbols->parse_mode = 0;
			arg = strtok(NULL, " \n\t");
			continue;
		} else if(strcmp(arg, ".data") == 0){
			symbols->parse_mode = 1;
			arg = strtok(NULL, " \n\t");
			continue;
		} else{
			if(symbols->parse_mode == 0){

				int code;
				code = convert_decimal(arg);

				if(code != -1){
					char opcode[2];
					opcode[0] = code;
					opcode[1] = '\0';
					add_arg(symbols, opcode, line);
					arg = strtok(NULL, " \n\t");
					++symbols->count;
					continue;
				}
			}
		}

		add_arg(symbols, arg, line);
		//sscanf(symbols->arr[symbols->count]->str,
		//	"%ms", symbols->arr[symbols->count]->str);
		arg = strtok(NULL, " \n\t");
		++symbols->count;

	} while(arg != NULL);

	if(arg != NULL) free(arg);
	arg = NULL;
	printf("Parsed line\n");
}

void add_arg(symbol_c* symbols, char *arg, int line){
		
	if(symbols->arr == NULL){
		symbols->arr = (
			symbol_t**)malloc(sizeof(symbol_t*));
	} else{
		symbols->arr = (symbol_t**)realloc(
			symbols->arr, (symbols->count+1) * sizeof(symbol_t*));
	}

	// sets symbol struct at pos loop
	symbols->arr[symbols->count] = (
		symbol_t*)malloc(sizeof(symbol_t));
	symbols->arr[symbols->count]->str = (
		char*)malloc(strlen(arg)+1 * sizeof(char));
	strcpy(symbols->arr[symbols->count]->str, arg);
	symbols->arr[symbols->count]->line = line;
}

int convert_decimal(char* arg){

	int code = -1;

	// Sets opcode
	if(strcmp(arg, "add") == 0){ code = 32; }
	else if(strcmp(arg, "addi") == 0){ code = 8; }
	else if(strcmp(arg, "nor") == 0){ code = 39; }
	else if(strcmp(arg, "ori") == 0){ code = 13; }
	else if(strcmp(arg, "sll") == 0){ code = 0; }
	else if(strcmp(arg, "lui") == 0){ code = 15; }
	else if(strcmp(arg, "sw") == 0){ code = 43; }
	else if(strcmp(arg, "lw") == 0){ code = 35; }
	else if(strcmp(arg, "bne") == 0){ code = 5; }
	else if(strcmp(arg, "j") == 0){ code = 2; }
	else if(strncmp(arg, "$", 1) == 0){
		// Sets register code
		if(arg[1] == '0') code = 0;
		else{ code = ((int)(arg[2])-'0') + (arg[1] == 's' ? 16 : 8); }
	}
	return code;
}

void print(symbol_c* symbols){
	int i;
	
	printf("Symbols: %d\n", symbols->count);
	for(i = 0; i < symbols->count; ++i){
		if(isalpha(symbols->arr[i]->str[0]))
			printf("%s %d\n", symbols->arr[i]->str, symbols->arr[i]->line);
		else
			printf("%d %d\n", symbols->arr[i]->str[0], symbols->arr[i]->line);
	}
}
