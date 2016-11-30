#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int test_little_endian() {
	int test=0x1;
	return *((char*)&test)==1;
}

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

void print_n_bytes(char* src, int n) {
	int i;
	for(i=0; i<n; i++) {
		printf("0x%x ", src[i]);
	}
	printf("\n");
}

char* reverse_str(char* src) {
	char* reverse = malloc(strlen(src)*sizeof(char));
	int offset=strlen(src)-1;
	for(int i=0; i<strlen(src); i++) {
		reverse[i]=src[offset--];
	}
	return reverse;
}

void clear_string(char* src) {
	for(int i=0; i<strlen(src); src[i]=0, i++);
}

void write_in_str(char* dest, int* offset, char* src, int n) {
	if(test_little_endian()) {	// Little endian
		int little_offset=0;
		for(int i=n-1; i>=0; i--) {
			dest[*offset+(little_offset++)]=src[i];
		}
	} else {										// Big endian
		for(int i=0; i<n; i++) {
			dest[*offset+i] = src[i];
		}
	}
	*offset+=n;
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
		ptr = (char*)&offset;
		// Copie de la sous-chaine dans la chaine de destination
		strncat(dest, ptr, 1);
		strncat(dest, substring, offset);
		// Decalage du pointeur -> nouvelle sous-chaine
		substring = substring+offset+1;
	}
	// Derniere partie
	offset = strlen(substring);
	ptr = (char*)&offset;
	strncat(dest, ptr, 1);
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
	int offset=0;
	// ID
	ptr = (char*)&id;
	write_in_str(dest, &offset, ptr, 2);
	// QR, OPCODE, AA, TC, RD, RA, (Z), RCODE
	info = info | ((answers>0)<<15);
	info = info | (opcode<<11);
	info = info | (authority_answer<<10);
	info = info | (truncated<<9);
	info = info | (recursive_question<<8);
	info = info | (recursive_answer<<7);
	info = info | rcode;
	ptr = (char*)&info;
	write_in_str(dest, &offset, ptr, 2);
	// QDCOUNT
	ptr = (char*)&questions;
	write_in_str(dest, &offset, ptr, 2);
	// ANCOUNT
	ptr = (char*)&answers;
	write_in_str(dest, &offset, ptr, 2);
	// NSCOUNT
	ptr = (char*)&name_servers;
	write_in_str(dest, &offset, ptr, 2);
	// ARCOUNT
	ptr = (char*)&additionals;
	write_in_str(dest, &offset, ptr, 2);
}

void generate_dns_question(char* dest, char* qname, int16_t qtype, int16_t qclass) {
	int offset=0;
	char* ptr;
	// QNAME
	// Le label recu en parametre est au format Big Endian
	if(test_little_endian()) { 
		char* reverse_qname = reverse_str(qname);
		write_in_str(dest, &offset, reverse_qname, strlen(qname));
		free(reverse_qname);
	} else {
		write_in_str(dest, &offset, qname, strlen(qname));
	}
	{	// Root name
		char zero=0;
		ptr = &zero;
		write_in_str(dest, &offset, ptr, 1);
	}
	// QTYPE
	ptr = (char*)&qtype;
	write_in_str(dest, &offset, ptr, 2);
	// QCLASS
	ptr = (char*)&qclass;
	write_in_str(dest, &offset, ptr, 2);
}
