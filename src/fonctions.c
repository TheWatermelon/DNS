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

int16_t strToLabel(char* src, char* dest) {
	int16_t labelLength=0;

	// Deux approches : compter le nombre de sous-mot + la taille de chaque sous-mot
	// OU : compter la chaine totale + 1

	return labelLength;
}
