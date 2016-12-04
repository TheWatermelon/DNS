#include "fonctions.h"
#include <stdio.h>

int main(int argc, const char** argv) {
	char* url = "www.unicaen.fr";
	char label[16]={0};
	char url_recovered[16]={0};
	char dns_header[16]={0};
	char dns_question[24]={0};

	/* Test of functions str_to_label and label_to_str */
	/*
	printf("url : %s\n", url);
	printf("label : [%d] ", str_to_label(url, label));
	print_string_in_bytes(label);
	printf("recovered url : [%d] %s\n", label_to_str(label, url_recovered), url_recovered);
	*/

	/* Test of functions generate_dns_header and generate_dns_question */
	/*
	printf("generate_dns_header(dns_header, 0x0001, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0001, 0x0000, 0x0000, 0x0000)\n");
	generate_dns_header(dns_header, 0x0001, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0001, 0x0000, 0x0000, 0x0000);
	print_n_bytes(dns_header, 12);
	printf("generate_dns_question(dns_question, label, 0x01, 0x00)\n");
	generate_dns_question(dns_question, label, 0x01, 0x00);
	print_n_bytes(dns_question, 24);
	*/

	/* Test of file reading */
	init_table("../doc/config_dns");
}
