#include <stdio.h>

// Declare a buffer for user input of size 2048 

static char input[2048];

int main(int argc, char** argv) {
  /* Print version and Exit Information */
  puts("MagicK Version 0.0.0.1\n");
  puts("Press Ctrl+c to Exit\n");
	
  //Command Prompt Control
  
	while(1){
		/* Output our text to the prompt */
		fputs("MagicK> ", stdout);
		
		//Read user input of maximum 2048
		fgets(input, 2048, stdin);
		
		//Echo back the input
		printf("No you're a %s", input);
	}
	
  return 0;
}