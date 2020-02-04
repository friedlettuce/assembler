#include <stdio.h>

int parse(char** symbols);

int main(int argc, char* argv[]){
	if(argc < 2){ return -1; }

	char** symbols = NULL;
	int symbol_count = parse(symbols);

	if(symbol_count == -1){ 
		printf("Invalid symbols\n");
		return -1;
	}

	return 0;
}

int parse(char** symbols){
	
	return -1;
}
