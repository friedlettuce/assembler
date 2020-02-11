#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse(char** symbols);

int main(int argc, char * argv[]){

	printf("!\n");
	char** symbols = NULL;
	int symbol_count = parse(symbols);

	if(symbol_count < 1){ 
		printf("Invalid symbols\n");
		return -1;
	}
	
	for(int i = 0; i < symbol_count; ++i)
		printf("%s\n", symbols[i]);

	free(symbols);
	return 0;
}

int parse(char** symbols){

	int symbol_count = 0;
	char* str = NULL;

	printf("AAA\n");
	do{
		scanf("%ms", &str);
		printf("%s\n", str);
		
		if(symbol_count < 1){
			symbols = (char**)malloc(sizeof(char*));
		} else{
			symbols = (char**)realloc(symbols, (symbol_count+1) * sizeof(char*));
		}

		symbols[symbol_count] = (char*)malloc((strlen(str)+1) * sizeof(char));
		strcpy(symbols[symbol_count], str);
		
		++symbol_count;
	}while('\n' != getchar());
	
	return symbol_count;
}
