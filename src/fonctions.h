#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdlib.h>

#define DNS_TYPE_A 1
#define DNS_TYPE_AA 2
#define DNS_TYPE_AAAA 3
#define DNS_TYPE_SOA 4
#define DNS_TYPE_MX 5
#define DNS_CLASS_IN 1

/***********************************
 *
 * NAME : test_little_endian : check whether 
 * the machine is in little endian format
 * RETURN : true if the machine is in little endian, false otherwise
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

/***********************************
 *
 * NAME : reverse_str : reverse the given string
 * It uses dynamic memory allocation, so don't forget to
 * free the returned string
 * PARAM : src : the source string
 * RETURN : the reversed string
 *
 ***********************************/
char* reverse_str(char* src);

/***********************************
 *
 * NAME : write_in_str : write n bytes from source
 * to dest at offset
 * It checks whether the system is in litte or big endian
 * and adapts its writing order on it.
 * For that purpose, it supposes the source is in the machine format
 * PARAM : dest : the destination
 * PARAM : offset : 
 * PARAM : src : the source
 * PARAM : n : the n bytes to write
 *
 ***********************************/
void write_in_str(char* dest, int* offset, char* src, int n);

/***********************************
 *
 * NAME : str_to_label : convert a string to a DNS label
 * PARAM : src : the source string
 * PARAM : dest : the label destination
 * RETURN : the label length
 *
 ***********************************/
int16_t str_to_label(char* src, char* dest);

/***********************************
 *
 * NAME : label_to_str : convert a DNS label to a string
 * PARAM : src : the source label
 * PARAM : dest : the string destination
 * RETURN : the string length
 *
 ***********************************/
int16_t label_to_str(char* src, char* dest);

/***********************************
 *
 * NAME : generate_dns_header : generate a DNS header based on the arguments
 * PARAM : dest : the DNS header destination
 * PARAM : id : the packet ID
 * PARAM : opcode : the type of question
 * PARAM : authority_answer : if the answer comes from an authority server
 * PARAM : truncated : if the message is truncated
 * PARAM : recursive_question : if the question asks for recursive answer
 * PARAM : recursive_answer : if the answer accepts recursive address resolving
 * PARAM : rcode : error code
 * PARAM : questions : number of questions
 * PARAM : answers : number of answers
 * PARAM : name_servers : number of name servers
 * PARAM : additionals : number of additional resources records
 *
 ***********************************/
void generate_dns_header(char* dest, int16_t id, char opcode, char authority_answer, char truncated, char recursive_question, char recursive_answer, char rcode, int16_t questions, int16_t answers, int16_t name_servers, int16_t additionals);

/***********************************
 *
 * NAME : generate_dns_question : generate a DNS question based on the arguments
 * PARAM : dest : the DNS question destination
 * PARAM : qname : url to resolve (label format)
 * PARAM : qtype : type of question
 * PARAM : qclass : class of the question
 *
 ***********************************/
void generate_dns_question(char* dest, char* qname, int16_t qtype, int16_t qclass);

/***********************************
 *
 * NAME : generate_dns_resource_record : generate a DNS resource record based on the arguments
 * PARAM : dest : the dns resource record destination
 * PARAM : rname : name of the record; if the first byte is 0x00, then its a Root record
 * PARAM : rtype : type of the record
 * PARAM : rclass : class of the record
 * PARAM : ttl : Time To Live of the resource
 * PARAM : rdlength : length of the data
 * PARAM : rdata : resource record data
 *
 ***********************************/
void generate_dns_resource_record(char* dest, int16_t rname, int16_t rtype, int16_t rclass, int32_t ttl, int16_t rdlength, char* rdata);

#endif /* FONCTIONS_H */
