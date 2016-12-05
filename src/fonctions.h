#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/***********************************
 *
 * NAME : dns_table_entry : DNS configuration table entry
 * DATA : name : domain name
 * DATA : class : class of the entry
 * DATA : type : type of the entry
 * DATA : data_length : length of data
 * DATA : data : content of the entry
 *
 ***********************************/
typedef struct {
	char name[63];
	int16_t class;
	int16_t type;
	int16_t data_length;
	char* data;
} dns_table_entry;

/***********************************
 *
 * NAME : dns_table : DNS configuration table
 * DATA : ttl : Time To Live for the entries
 * DATA : length : number of entries
 * DATA : entries : the table content
 *
 ***********************************/
typedef struct {
	int32_t ttl;
	int length;
	dns_table_entry* entries;
} dns_table;

/***********************************
 *
 * NAME : dns_header : DNS header
 * DATA : id : id of the packet
 * DATA : opcode : type of request
 * DATA : authority_answer : if the answer comes from an authority name server
 * DATA : truncated : if the message is truncated
 * DATA : recursive_question : if the question asks for a recursive answer
 * DATA : recursive_answer : if the answer accepts recursivity
 * DATA : rcode : error code
 * DATA : qdcount : number of questions
 * DATA : ancount : number of answers
 * DATA : nscount : number of name servers
 * DATA : arcount : number of additional information
 *
************************************/
typedef struct {
	int16_t id;
	char opcode;
	char authority_answer;
	char truncated;
	char recursive_question;
	char recursive_answer;
	char rcode;
	int16_t qdcount;
	int16_t ancount;
	int16_t nscount;
	int16_t arcount;
} dns_header;


/***********************************
 *
 * NAME : dns_question : DNS question section
 * DATA : name : name asked
 * DATA : type : type of the question
 * DATA : class : class of the question
 *
************************************/
typedef struct {
	char name[63];
	int16_t type;
	int16_t class;
} dns_question;

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
 * NAME : convert_ip_to_bytes : convert an ip address (XXX.XXX.XXX.XXX)
 * to a char[4] array
 * PARAM : ip : the string to convert
 * PARAM : bytes : the char[4] array destination
 *
 ***********************************/
void convert_ip_to_bytes(char* ip, char* bytes);

/***********************************
 *
 * NAME : convert_bytes_to_ip : convert a char[4] array 
 * to an ip address (XXX.XXX.XXX.XXX)
 * PARAM : bytes : the char[4] array to convert
 * RETURN : the ip in a string format
 *
 ***********************************/
void convert_bytes_to_ip(char* bytes, char* ip);

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
 * NAME : is_empty_line : check if the string
 * is only composed of white spaces (tab, space, CR)
 * PARAM : str : the string to check
 * RETURN : true if the line is empty, false otherwise
 ***********************************/
int is_empty_line(char* str);

/***********************************
 * 
 * NAME : str_equaĺs_to : test equality between two strings
 *
************************************/
int str_equals_to(char* str1, char* str2);

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
 * NAME : init_table : initialize the DNS table from
 * a source file
 * PARAM : filename : name of the source file
 * RETURN : the newly created DNS table
 *
 ***********************************/
dns_table init_table(char* filename);

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
int generate_dns_header(char* dest, int16_t id, char opcode, char authority_answer, char truncated, char recursive_question, char recursive_answer, char rcode, int16_t questions, int16_t answers, int16_t name_servers, int16_t additionals);

/***********************************
 *
 * NAME : generate_dns_question : generate a DNS question based on the arguments
 * PARAM : dest : the DNS question destination
 * PARAM : qname : url to resolve (label format)
 * PARAM : qtype : type of question
 * PARAM : qclass : class of the question
 *
 ***********************************/
int generate_dns_question(char* dest, char* qname, int16_t qtype, int16_t qclass);

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
int generate_dns_resource_record(char* dest, int16_t rname, int16_t rtype, int16_t rclass, int32_t ttl, int16_t rdlength, char* rdata);

dns_header* get_dns_header(char* src);

dns_question* get_dns_question(char* src);

int get_dns_resource_record(char* dest, char* src);

int answer_to_question(char* answer, char* question, dns_table table);

#endif /* FONCTIONS_H */
