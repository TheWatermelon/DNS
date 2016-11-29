#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void convert_int16_to_2int8(int16_t in, int8_t* res) {
	int8_t* c=(int8_t*)&in;
	res[0]=c[0];
	res[1]=c[1];
}

int16_t convert_2int8_to_int16(int8_t* in) {
	return *(int16_t*)in;
}

void print_string_in_bytes(char* src) {
	int i;
	for(i=0; i<strlen(src); i++) {
		printf("%x ", src[i]);
	}
	printf("\n");
}

void clear_string(char* src) {
	for(int i=0; i<strlen(src); src[i]=0, i++);
}

int16_t str_to_label(char* src, char* dest) {
	int16_t label_length=strlen(src)+1;
	char length;
	char* ptr;
	char* substring=src;
	int offset;

	// Decoupe de copy en points
	while((ptr=strchr(substring, '.'))!=NULL) {
		// Position et taille de la sous-chaine
		offset = ptr-substring;
		length = offset & 11111111;
		// Copie de la sous-chaine dans la chaine de destination
		strncat(dest, &length, 1);
		strncat(dest, substring, offset);
		// Decalage du pointeur -> nouvelle sous-chaine
		substring = substring+offset+1;
	}
	// Derniere partie
	length = strlen(substring);
	strncat(dest, &length, 1);
	strncat(dest, substring, strlen(substring));

	return label_length;
}
