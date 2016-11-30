#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdlib.h>

#define DNS_TYPE_A 1
#define DNS_TYPE_AA 2
#define DNS_TYPE_AAAA 3
#define DNS_TYPE_SOA 4
#define DNS_TYPE_MX 5

/***********************************
 *
 * NAME : test_little_endian : check whether 
 * the machine is in little_endian format
 *
 ***********************************/
int test_little_endian();

/***********************************
 *
 * NAME : convert_int16_to_2int8 : explicit
 *
 ***********************************/
void convert_int16_to_2int8(int16_t in, int8_t* res);

/***********************************
 *
 * NAME : convert_2int8_to_int16 : explicit
 *
 ***********************************/
int16_t convert_2int8_to_int16(int8_t* in);

/***********************************
 * 
 * NAME : print_string_in_bytes : print the source
 * string byte to byte until reaching byte 0
 * PARAM : src : the source string
 *
 ***********************************/
void print_string_in_bytes(char* src);

/***********************************
 *
 * NAME : print_n_bytes : print n bytes from source
 * undefined behaviour if source isn't long enough
 * PARAM : src : the source string
 * PARAM : n : the n bytes to read
 *
 ***********************************/
void print_n_bytes(char* src, int n);

char* reverse_str(char* src);

void clear_string(char* src);

void write_in_str(char* dest, int* offset, char* src, int n);

int16_t str_to_label(char* src, char* dest);

int16_t label_to_str(char* src, char* dest);

void generate_dns_header(char* dest, int16_t id, char opcode, char authority_answer, char truncated, char recursive_question, char recursive_answer, char rcode, int16_t questions, int16_t answers, int16_t name_servers, int16_t additionals);

void generate_dns_question(char* dest, char* qname, int16_t qtype, int16_t qclass);

#endif /* FONCTIONS_H */
