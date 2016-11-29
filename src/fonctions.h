#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdlib.h>

void convert_int16_to_2int8(int16_t in, int8_t* res);

int16_t convert_2int8_to_int16(int8_t* in);

void print_string_in_bytes(char* src);

void clear_string(char* src);

int16_t str_to_label(char* src, char* dest);

#endif /* FONCTIONS_H */
