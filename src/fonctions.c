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

void convert_ip_to_bytes(char* ip, char* bytes) {
	char buffer[3]={0};
	char* ptr;
	int byte;
	int buffer_offset=0;
	int bytes_offset=0;

	for(int i=0; i<strlen(ip); i++) {
		if(ip[i]!=46) {
			buffer[buffer_offset++]=ip[i];
		} else {
			byte = atoi(buffer);
			ptr = (char*)&byte;
			bytes[bytes_offset++] = *ptr;
			buffer[0]=0;
			buffer[1]=0;
			buffer[2]=0;
			buffer_offset=0;
		}
	}
	byte = atoi(buffer);
	bytes[bytes_offset++] = (char) byte;
}

void convert_bytes_to_ip(char* bytes, char* ip) {
	int byte, tmp;
	char* ptr;
	int offset=0;

	for(int i=0; i<4; i++) {
		byte=bytes[i]&0xFF;

		if((tmp=byte/100)!=0) {
			ptr = (char*) &tmp;
			ip[offset++]=*ptr+'0';
			byte-=tmp*100;
		}
		if((tmp=byte/10)!=0) {
			ptr = (char*) &tmp;
			ip[offset++]=*ptr+'0';
			byte-=tmp*10;
		}
		tmp=byte%10;
		ptr=(char*)&tmp;
		ip[offset++]=*ptr+'0';
		if(i!=3) { ip[offset++]='.'; } 
	}
}

void print_string_in_bytes(char* src) {
	int i;
	for(i=0; i<strlen(src); i++) {
		printf("0x%x ", src[i]&0xFF);
	}
	printf("\n");
}

void print_n_bytes(char* src, int n) {
	int i;
	for(i=0; i<n; i++) {
		printf("0x%x ", src[i]&0xFF);
	}
	printf("\n");
}

char* reverse_str(char* src) {
	// Dynamic allocation
	char* reverse = malloc(strlen(src)*sizeof(char));
	int offset=strlen(src)-1;
	for(int i=0; i<strlen(src); i++) {
		reverse[i]=src[offset--];
	}
	return reverse;
}

void write_in_str(char* dest, int* offset, char* src, int n) {
	if(test_little_endian()) { // Little endian
		int little_offset=0;
		for(int i=n-1; i>=0; i--) {
			dest[*offset+(little_offset++)]=src[i];
		}
	} else { // Big endian
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

dns_table init_table(char* filename) {
	FILE* fp;
	char* buffer = NULL;
	size_t n = 100;
	dns_table table;
	int* ttl;

	// Open the file
	fp = fopen(filename, "r");
	if(fp == NULL) { exit(EXIT_FAILURE); }
	
	// 
	// Read the file
	//
	// TTL
	getline(&buffer, &n, fp);
	char ttl_str[11]={0};
	int offset=0;
	for(int i=0; i<strlen(buffer); i++) { 
		if(buffer[i]>47 && buffer[i]<58) {
			ttl_str[offset++]=buffer[i];
		}
	}
	offset=strlen(buffer);
	table.ttl = atoi(ttl_str);
	printf("TTL : %d\n", table.ttl);

	// Count the number of entries
	int lines;
	for(lines=0; (getline(&buffer, &n, fp))!=-1; lines++);
	// Allocation the corresponding table entries
	table.entries = malloc(lines*sizeof(dns_table_entry));
	// Seek the beginning of the file
	fseek(fp, SEEK_SET, 0);
	// Skip the ttl line
	getline(&buffer, &n, fp);
	// Remplissage du contenu
	for(lines=0; (getline(&buffer, &n, fp))!=-1; lines++) {
		int i;
								printf(" [LINE : %d]\n", lines);
		// Name
		for(i=0; buffer[i]!=32; i++) {
			table.entries[lines].name[i]=buffer[i];
		}
								printf("NAME : %s\n", table.entries[lines].name);
		// Go to class
		while(buffer[++i]==32);
		// Class is always IN, so other class found will be 0
		if(buffer[i]!=73) { table.entries[lines].class = 0; }
		else { table.entries[lines].class = 1; }
		i++;
								printf("CLASS : %d\n", table.entries[lines].class);
		// Go to type
		while(buffer[++i]==32);
		// Type
		if(buffer[i]==65) {
			// AAAA
			if(buffer[i+1]==65) {
				table.entries[lines].type = 28;
				i+=4;
			} else {	// A
				table.entries[lines].type = 1;
				i++;
			}
		} else if(buffer[i]==67) {	// CNAME
			table.entries[lines].type = 5;
			i+=5;
		} else if(buffer[i]==77) {	// MX
			table.entries[lines].type = 15;
			i+=2;
		} else if(buffer[i]==78) {	// NS
			table.entries[lines].type = 2;
			i+=2;
		} else if(buffer[i]==80) {	// PTR
			table.entries[lines].type = 12;
			i+=3;
		} else if(buffer[i]==83) {	// SOA
			table.entries[lines].type = 6;
			i+=3;
		} else {	// Type unknown
			table.entries[lines].type = 0;
			// Shift until the next space
			while(buffer[++i]!=32);
		}
									printf("TYPE : %d\n", table.entries[lines].type);
		// Shift until the next data
		while(buffer[i++]==32);
		i--;
		// Switch case for the data
		if(table.entries[lines].type==1) {	// A
			char ip[16]={0};
			int ip_offset=0;
			table.entries[lines].data = malloc(4*sizeof(char));
			while(i<strlen(buffer)) {
				ip[ip_offset++]=buffer[i++];
			}
			convert_ip_to_bytes(ip, table.entries[lines].data);
		} else {
			table.entries[lines].data = malloc(4*sizeof(char));
			table.entries[lines].data[0]=0;
		}
		printf("DATA : ");
		print_n_bytes(table.entries[lines].data, 4);
	}




	// Free the buffer and close the file
	free(buffer);
	fclose(fp);

	return table;
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

void generate_dns_resource_record(char* dest, int16_t rname, int16_t rtype, int16_t rclass, int32_t ttl, int16_t rdlength, char* rdata) {
	int offset=0;
	char* ptr;
	// RNAME
	ptr = (char*)&rname;
	write_in_str(dest, &offset, ptr, 2);
	// RTYPE
	ptr = (char*)&rtype;
	write_in_str(dest, &offset, ptr, 2);
	// RCLASS
	ptr = (char*)&rclass;
	write_in_str(dest, &offset, ptr, 2);
	// TTL
	ptr = (char*)&ttl;
	write_in_str(dest, &offset, ptr, 4);
	// RDLENGTH
	ptr = (char*)&rdlength;
	write_in_str(dest, &offset, ptr, 2);
	// RDATA
	// Les donnees recues en parametre sont au format Big Endian
	if(test_little_endian()) { 
		char* reverse_rdata = reverse_str(rdata);
		write_in_str(dest, &offset, reverse_rdata, rdlength);
		free(reverse_rdata);
	} else {
		write_in_str(dest, &offset, rdata, strlen(rdata));
	}
}
