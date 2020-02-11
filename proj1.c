#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* str;
	int line;
} symbol_t;

int main(int argc, char * argv[]){

	// Parsing Section
	// parsing variables
	symbol_t** symbols = NULL;
	int symbol_count = 0;
	int i, line = 0;
	char* arg = NULL;

	// parsing loop
	do{
		scanf("%ms", &arg);

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
