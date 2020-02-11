#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* str;
	int line;
} symbol_t;

void convert_decimal(char** arg);

int main(int argc, char * argv[]){

	// Parsing Section
	// parsing variables
	symbol_t** symbols = NULL;
	int symbol_count = 0;
	int i, line = 0;
	char* arg = NULL;
	int datatext = -1; // 0 for text, 1 for data

	// parsing loop
	do{
		scanf("%ms", &arg);

		if(strcmp(arg, ".text") == 0)
			datatext = 0;
		else if(strcmp(arg, ".data") == 0)
			datatext = 1;
		else
			if(datatext == 0) convert_decimal(&arg);

		if(symbols == NULL){
			symbols = (symbol_t**)malloc(sizeof(symbol_t*));
		} else{
			symbols = (symbol_t**)realloc(
				symbols, sizeof(symbol_t*));
		}

		// sets symbol struct at pos loop
		symbols[symbol_count] = (symbol_t*)malloc(sizeof(symbol_t));
		symbols[symbol_count]->str = (char*)malloc(
			strlen(arg)+1 * sizeof(char));
		strcpy(symbols[symbol_count]->str, arg);
		symbols[symbol_count]->line = line;

		if(arg != NULL) free(arg);
		arg = NULL;
		++symbol_count;

	}while(getchar() != '\n');

	if(symbol_count < 1 || symbols == NULL){ 
		printf("Invalid symbols\n");
		return -1;
	}
	
	printf("Symbols: %d\n", symbol_count);
	for(i = 0; i < symbol_count; ++i)
		printf("%s%d\n", symbols[i]->str, symbols[i]->line);

	free(symbols);
	symbols = NULL;
	return 0;
}

void convert_decimal(char** arg){

	// Moves arg to tmp char* to test more efficently
	int code = -1;

	// Sets opcode
	if(strcmp(*arg, "add") == 0){ code = 32; }
	else if(strcmp(*arg, "addi") == 0){ code = 8; }
	else if(strcmp(*arg, "nor") == 0){ code = 39; }
	else if(strcmp(*arg, "ori") == 0){ code = 13; }
	else if(strcmp(*arg, "sll") == 0){ code = 0; }
	else if(strcmp(*arg, "lui") == 0){ code = 15; }
	else if(strcmp(*arg, "sw") == 0){ code = 43; }
	else if(strcmp(*arg, "lw") == 0){ code = 35; }
	else if(strcmp(*arg, "bne") == 0){ code = 5; }
	else if(strcmp(*arg, "j") == 0){ code = 2; }
	else if(*arg[0] == '$'){
		// Sets register code
		if(*arg[1] == '0') code = 0;
		else{ code = 8 + atoi(*arg[2]) + (*arg[1] == 's' ? 8 : 0); }
	} else{ return; }

	free(*arg);
	*arg = (char*) malloc(2*sizeof(char));
	arg[0] = code;
	arg[1] = '\0';
}
