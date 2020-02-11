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
} symbol_c;

void parse_line(symbol_c* symbols, int line);
int convert_decimal(char* arg);

int main(int argc, char * argv[]){

	// Parsing Section
	// parsing variables
	symbol_c symbols;
	symbols.arr = NULL;
	symbols.count = 0;
	int i, line = 0;

	parse_line(&symbols, line);

	if(symbols.count < 1 || symbols.arr == NULL){ 
		printf("Invalid symbols\n");
		return -1;
	}
	
	printf("Symbols: %d\n", symbols.count);
	for(i = 0; i < symbols.count; ++i){
		if(isalpha(symbols.arr[i]->str[0]))
			printf("%s%d\n", symbols.arr[i]->str, symbols.arr[i]->line);
		else
			printf("%d%d\n", symbols.arr[i]->str[0], symbols.arr[i]->line);
	}

	free(symbols.arr);
	symbols.arr = NULL;
	return 0;
}

void parse_line(symbol_c* symbols, int line){
	
	char* arg = NULL;
	int datatext = -1; // 0 for text, 1 for data
	
	// parsing loop
	do{
		scanf("%ms", &arg);

		if(strcmp(arg, ".text") == 0){
			datatext = 0;
			continue;
		} else if(strcmp(arg, ".data") == 0){
			datatext = 1;
			continue;
		} else{
			if(datatext == 0){

				int code;
				code = convert_decimal(arg);

				if(code != -1){
					free(arg);
					arg = (char*) malloc(2*sizeof(char));
					arg[0] = code;
					arg[1] = '\0';
				}
			}
		}

		if(symbols->arr == NULL){
			symbols->arr = (
				symbol_t**)malloc(sizeof(symbol_t*));
		} else{
			symbols->arr = (symbol_t**)realloc(
				symbols->arr, sizeof(symbol_t*));
		}

		// sets symbol struct at pos loop
		symbols->arr[symbols->count] = (
			symbol_t*)malloc(sizeof(symbol_t));
		symbols->arr[symbols->count]->str = (
			char*)malloc(strlen(arg)+1 * sizeof(char));
		strcpy(symbols->arr[symbols->count]->str, arg);
		symbols->arr[symbols->count]->line = line;

		if(arg != NULL) free(arg);
		arg = NULL;
		++symbols->count;

	}while(getchar() != '\n');
}

int convert_decimal(char* arg){

	printf("Converting\n");
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
		printf("Code %d\n", code);
	}
	return code;
}
