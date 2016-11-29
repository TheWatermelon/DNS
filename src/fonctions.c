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
	// Pointeur sur la prochaine sous-chaine
	char* ptr;
	// Sous-chaine
	char* substring=src;
	// Taille de la sous-chaine
	int offset;

	// Decoupe de copy en points
	while((ptr=strchr(substring, '.'))!=NULL) {
		// Position et taille de la sous-chaine
		offset = ptr-substring;
		ptr = &offset+3;
		// Copie de la sous-chaine dans la chaine de destination
		strncat(dest, ptr, 1);
		strncat(dest, substring, offset);
		// Decalage du pointeur -> nouvelle sous-chaine
		substring = substring+offset+1;
	}
	// Derniere partie
	length = strlen(substring);
	strncat(dest, &length, 1);
	strncat(dest, substring, strlen(substring));

	return strlen(src)+1;
}

int16_t label_to_str(char* src, char* dest) {
	char length, point='.';
	char* substring = src;
	int offset;
	// Tant qu'on n'a pas atteint la fin de la chaine
	while(*substring!=0) {
		// On recupere le nombre de caracteres de la sous chaine
		length = *substring;
		// On avance jusqu'a la sous-chaine
		substring = substring+1;
		// On recupere la sous-chaine
		for(int i=0; i<(int)length; i++) {
			strncat(dest, substring, 1);
			substring = substring+1;
		}
		// On ajoute un point entre chaque sous-chaine
		if(*substring!=0) {
			strncat(dest, &point, 1);
		}
	}
	
	return strlen(src)-1;
}

void generate_dns_header(char* dest, int16_t id, char opcode, char authority_answer, char truncated, char recursive_question, char recursive_answer, char rcode, int16_t questions, int16_t answers, int16_t name_servers, int16_t additionals) {
	char* ptr;
	int16_t info=0;
	// ID
	ptr = &id;
	strncat(dest, ptr, 2);
	// QR, OPCODE, AA, TC, RD, RA, (Z), RCODE
	info = info | ((answers>0)<<15);
	info = info | (opcode<<11);
	info = info | (authority_answer<<10);
	info = info | (truncated<<9);
	info = info | (recursive_question<<8);
	info = info | (recursive_answer<<7);
	info = info | rcode;
	ptr = &info;
	strncat(dest, ptr, 2);
	// QDCOUNT
	ptr = &questions;
	strncat(dest, ptr, 2);
	// ANCOUNT
	ptr = &answers;
	strncat(dest, ptr, 2);
	// NSCOUNT
	ptr = &name_servers;
	strncat(dest, ptr, 2);
	// ARCOUNT
	ptr = &additionals;
	strncat(dest, ptr, 2);
}

void generate_dns_question(char* dest, char* qname, int16_t qtype, int16_t qclass) {
	char* ptr;
	// QNAME
	strcat(dest, qname);
	*ptr=0;
	strncat(dest, ptr, 1);
	// QTYPE
	ptr = &qtype;
	strncat(dest, ptr, 2);
	// QCLASS
	ptr = &qclass;
	strncat(dest, ptr, 2);
}