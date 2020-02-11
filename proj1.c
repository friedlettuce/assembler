#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse(char** symbols);

int main(int argc, char * argv[]){

	printf("!\n");
	char** symbols = NULL;
	int symbol_count = parse(symbols);
	int i;

	if(symbol_count < 1){ 
		printf("Invalid symbols\n");
		return -1;
	}
	
	printf("Symbols: %d\n", symbol_count);
	for(i = 0; i < symbol_count; ++i)
		printf("%s\n", symbols[i]);

	free(symbols);
	return 0;
}

int parse(char** symbols){

	int symbol_count = 0;
	char* str = NULL;

	do{
		scanf("%ms", &str);
		
		if(symbol_count < 1){
			symbols = (char**)malloc(sizeof(char*));
		} else{
			symbols = (char**)realloc(symbols, (symbol_count+1) * sizeof(char*));
		}

		symbols[symbol_count] = (char*)malloc((strlen(str)+1) * sizeof(char));
		strcpy(symbols[symbol_count], str);
		
		++symbol_count;

		free(str);
		str = NULL;
	}while('\n' != getchar());
	
	return symbol_count;
}
