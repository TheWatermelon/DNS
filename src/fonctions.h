#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdlib.h>

void convert_int16_to_2int8(int16_t in, int8_t* res);

int16_t convert_2int8_to_int16(int8_t* in);

void print_string_in_bytes(char* src);

void clear_string(char* src);

int16_t str_to_label(char* src, char* dest);

int16_t label_to_str(char* src, char* dest);

void generate_dns_header(char* dest, int16_t id, char opcode, char authority_answer, char truncated, char recursive_question, char recursive_answer, char rcode, char truncated, int16_t questions, int16_t answers, int16_t name_servers, int16_t additionals);

void generate_dns_question(char* dest, char* qname, int16_t qtype, int16_t qclass);

#endif /* FONCTIONS_H */
