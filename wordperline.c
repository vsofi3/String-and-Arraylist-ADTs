#include "ADTs/stringADT.h"
#include "ADTs/arraylist.h"
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#define UNUSED __attribute__((unused))


//want to use an arraylist to put words in
//then loop through that arraylist and print words at the end of the file


void wordLine(FILE *file, bool doL,bool doP){
	char buf[BUFSIZ];

	while(fgets(buf, BUFSIZ, file) != NULL){
		const String *str = String_create(buf);
		
		if(doL){
		//convert upper-case chars to lower
			str->lower(str);
		}

		if(doP){
		//punctuation chars also separate words
		//we make punctuation into white space now				
			str->translate(str,"[:punct:]",' ');
			//WATCH QUOTATION MARKS HERE
		}

		//split at white space either way
		const ArrayList *al = str->split(str,"");
		if(al == NULL){
			str->destroy(str);
			continue;
		}

	//iterate through arraylist to print words
		long i;
		for(i = 0L; i<al->size(al); i++){
			char *sp;
			(void) al->get(al,i,(void**)&sp);
			printf("%s\n",sp);
				}

	//destroy containers inside outer while loop
	str->destroy(str);
	al->destroy(al);
}
}



int main(UNUSED int argc, UNUSED char *argv[]){
	int opt;
	bool doL, doP;

	FILE *file;
	int exitStatus = EXIT_FAILURE;

	doL = doP = false;

	opterr = 0;
	while((opt = getopt(argc,argv,"lp")) != -1){
		switch(opt){
			case 'l': doL = true;break;
			case 'p': doP = true;break;
			default: fprintf(stderr,"%s: illegal option, '-%c'\n", argv[0], optopt);
		}
	}	



	if(optind == argc){
		//0 files given, no non-option arguments provided
		//use standard input
		wordLine(stdin, doL, doP);
	}
	

	if(argc == (optind+1)){
		//just one file provided
		file = fopen(argv[optind],"r");
		if(file == NULL){
			printf("open(%s) error\n",argv[optind]);
			exitStatus = EXIT_FAILURE;
			goto cleanup;
		}
		wordLine(file, doL, doP);
		fclose(file); //close file
	}

	if(argc> (optind+1)){
		//for multiple files
		while(argv[optind] != NULL){
			file = fopen(argv[optind],"r");
			
			if(file == NULL){
				printf("open(%s) error\n", argv[optind]);
				return EXIT_FAILURE;
			}

		wordLine(file, doL,doP);
		optind++;
		fclose(file); //close file
}
	exitStatus = EXIT_SUCCESS;
	goto cleanup;
}
	cleanup:
	return exitStatus;
}
