#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* str;
	int line;
	int address;
} symbol_t;

typedef struct{
	symbol_t** arr;
	int count;
	int parse_mode;
} symbol_c;

void parse_line(symbol_c* symbols, char *program, int line, char *delim);
void add_arg(symbol_c* symbols, char *arg, int code, int line);
int convert_decimal(char* arg);
void print(symbol_c* symbols);
void hex_print(int x);

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
		parse_line(&symbols, program, line++, " \n\t");
	}

	if(symbols.count < 1 || symbols.arr == NULL){ 
		printf("Invalid symbols\n");
		return -1;
	}

	int pc, i = 0;

	for(i = 0; i < symbols.count; ++i, pc += 4){

		hex_print(pc);
		printf("\n");
	}

	print(&symbols);	
	printf("Exiting\n");
	free(symbols.arr);
	symbols.arr = NULL;
	return 0;
}

void parse_line(symbol_c* symbols, char *program, int line, char* delim){
	
	char* arg;
	arg = strtok(program, delim);

	do{
		if(arg == NULL) break;
		// printf("Scanned %s\n", arg);

		if(strcmp(arg, ".text") == 0){
			symbols->parse_mode = 0;
			arg = strtok(NULL, delim);
			continue;
		} else if(strcmp(arg, ".data") == 0){
			symbols->parse_mode = 1;
			arg = strtok(NULL, delim);
			continue;
		} else{
			if(symbols->parse_mode == 0){

				// Breaks up register args recursively
				if(strncmp(arg, "$", 1) == 0 && strlen(arg) > 3){
					char *tmp;
					tmp = (char*) malloc(strlen(arg)+1 * sizeof(char));
					strcpy(tmp, arg);
					parse_line(symbols, tmp, line, ",");
					arg = strtok(NULL, delim);
					free(tmp);
					tmp = NULL;
					break;
				} // else printf("%s %d\n", arg, strlen(arg));

				int code;
				code = convert_decimal(arg);

				// Stores reg/op as code
				if(code != -1){
					add_arg(symbols, arg, code, line);
					arg = strtok(NULL, delim);
					++symbols->count;
					continue;
				}
			}
		}

		add_arg(symbols, arg, -1, line);
		arg = strtok(NULL, delim);
		++symbols->count;

	} while(arg != NULL);

	if(arg != NULL) free(arg);
	arg = NULL;
	// printf("Parsed line\n");
}

void add_arg(symbol_c* symbols, char *arg, int code, int line){
		
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
	
	symbols->arr[symbols->count]->address = code;
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
		printf("%s, %d, %d\n", symbols->arr[i]->str, symbols->arr[i]->address, symbols->arr[i]->line);
	}
}

void hex_print(int x){

	char addy[7];
	addy[6] = '\0';
	int cur_byte, i;

	for(i = 0; i < 6; ++i){
		cur_byte = x % 16;
		x = x>>4;
		if(cur_byte > 9)
			addy[5-i] = '7' + cur_byte;
		else
			addy[5-i] = '0' + cur_byte;
	}
	printf("0x%s: ", addy);
}
