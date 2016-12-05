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

int is_empty_line(char* str) {
	for(int i=0; i<strlen(str); i++) {
		if(str[i]>32) {
			return FALSE;
		}
	}
	return TRUE;
}

char* reverse_str(char* src, int n) {
	// Dynamic allocation
	char* reverse = malloc(n*sizeof(char));
	int offset=n-1;
	for(int i=0; i<n; i++) {
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
	//
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
	int lines_count=0;
	while((getline(&buffer, &n, fp))!=-1) {
		{ lines_count++; }
	}
	// Allocation of the corresponding table entries
	table.entries = malloc(lines_count*sizeof(dns_table_entry));
	// Seek the beginning of the file
	fseek(fp, SEEK_SET, 0);
	// Skip the ttl line
	getline(&buffer, &n, fp);
	// Remplissage du contenu
	int table_length=0;
	for(int lines=0; lines<lines_count; lines++) {
		if(getline(&buffer, &n, fp)==-1) { break; }
		if(is_empty_line(buffer)) { continue; }
		table_length++;
		int i;
		//
		// NAME
		//
		for(i=0; buffer[i]!=32; i++) {
			table.entries[lines].name[i]=buffer[i];
		}
		// Remove '.'
		if(table.entries[lines].name[i-1]==46) {
			table.entries[lines].name[i-1]=0;
		}
		// Go to class
		while(buffer[++i]==32);
		//
		// CLASS
		//
		// Class is always IN, so other class found will be 0
		if(buffer[i]!=73) { table.entries[lines].class = 0; }
		else { table.entries[lines].class = 1; }
		i++;
		// Go to type
		while(buffer[++i]==32);
		// 
		// TYPE
		//
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
		// Shift until the next data
		while(buffer[++i]==32);
		//
		// DATA
		//
		if(table.entries[lines].type==1) {	// A
			char ip[16]={0};
			int ip_offset=0;
			table.entries[lines].data_length=4;
			table.entries[lines].data = malloc(4*sizeof(char));
			while(i<strlen(buffer)-1) {
				ip[ip_offset++]=buffer[i++];
			}
			convert_ip_to_bytes(ip, table.entries[lines].data);
		} else if(table.entries[lines].type==5 ||	/* CNAME */
				table.entries[lines].type==2 ||	/* NS */
				table.entries[lines].type==12) {	/* PTR */
			table.entries[lines].data_length=strlen(buffer)-i;
			table.entries[lines].data = malloc((strlen(buffer)-i)*sizeof(char));
			int address_offset=0;
			while(i<strlen(buffer)-1) {
				table.entries[lines].data[address_offset++]=buffer[i++];
			}
		} else if(table.entries[lines].type==15) {	/* MX */
			table.entries[lines].data_length=strlen(buffer)-i;
			table.entries[lines].data = malloc((strlen(buffer)-i)*sizeof(char));
			char buff[3];
			char* ptr;
			for(int j=0; j<3; j++) { buff[j]=buffer[i++]; }
			i--;
			int preference = atoi(buff);
			// Shift to the end
			while(buffer[++i]==32);
			ptr=(char*)&preference;
			int address_offset=0;
			write_in_str(table.entries[lines].data, &offset, ptr, 1);
			while(i<strlen(buffer)-1) {
				table.entries[lines].data[address_offset++]=buffer[i++];
			}
		} else if(table.entries[lines].type==6) {	// SOA
			table.entries[lines].data_length=128;
			table.entries[lines].data = malloc(128*sizeof(char));
			int data_offset=0;
			/* First address */
			while(buffer[i]!=32) { table.entries[lines].data[data_offset++] = buffer[i++]; printf("%x:%x ",buffer[i-1], table.entries[lines].data[data_offset-1]); }
			printf("\n");
			
			/* Second address */
			// Skip to second address
			while(buffer[++i]==32);
			while(buffer[i]!=32) { table.entries[lines].data[data_offset++] = buffer[i++]; }
			/* serial number */
			getline(&buffer, &n, fp);
			// Skip to serial
			while(buffer[i++]==32);
			// serial
			char serial[11]={0};
			int time_data_offset=0;
			while(buffer[i]!=32) { serial[time_data_offset++]=buffer[i++]; }
			time_data_offset = atoi(serial);
			char* ptr = (char*)&time_data_offset;
			write_in_str(table.entries[lines].data, &data_offset, ptr, 4);
			/* refresh */
			getline(&buffer, &n, fp);
			// Skip to refresh
			while(buffer[i++]==32);
			// refresh
			char refresh[3]={0};
			time_data_offset=0;
			while(buffer[i]>47 && buffer[i]<58) { refresh[time_data_offset++]=buffer[i++]; }
			time_data_offset = atoi(refresh);
			ptr = (char*)&time_data_offset;
			write_in_str(table.entries[lines].data, &data_offset, ptr, 4);
			/* retry */
			getline(&buffer, &n, fp);
			// Skip to retry
			while(buffer[i++]==32);
			// retry
			char retry[3]={0};
			time_data_offset=0;
			while(buffer[i]>47 && buffer[i]<58) { retry[time_data_offset++]=buffer[i++]; }
			time_data_offset = atoi(retry);
			ptr = (char*)&time_data_offset;
			write_in_str(table.entries[lines].data, &data_offset, ptr, 4);
			/* expire */
			getline(&buffer, &n, fp);
			// Skip to expire
			while(buffer[i++]==32);
			// expire
			char expire[3]={0};
			time_data_offset=0;
			while(buffer[i]>47 && buffer[i]<58) { expire[time_data_offset++]=buffer[i++]; }
			time_data_offset = atoi(expire);
			ptr = (char*)&time_data_offset;
			write_in_str(table.entries[lines].data, &data_offset, ptr, 4);
			/* ttl */
			getline(&buffer, &n, fp);
			// Skip to ttl
			while(buffer[i++]==32);
			// ttl
			char ttl[3]={0};
			time_data_offset=0;
			while(buffer[i]>47 && buffer[i]<58) { ttl[time_data_offset++]=buffer[i++]; }
			time_data_offset = atoi(ttl);
			ptr = (char*)&time_data_offset;
			write_in_str(table.entries[lines].data, &data_offset, ptr, 4);
			/* end of SOA section */
			getline(&buffer, &n, fp);
		} else {
			table.entries[lines].data = malloc(4*sizeof(char));
			table.entries[lines].data[0]=0;
		}
	}
	table.length=table_length;

	// Free the buffer and close the file
	free(buffer);
	fclose(fp);

	return table;
}

int search_table_entry(dns_table table, char* name, int16_t type, int16_t class) {
	char url[63]={0};
	label_to_str(name, url);

	for(int i=0; i<table.length; i++) {
		if(strcmp(table.entries[i].name, url)==0) { 
			if(table.entries[i].type==type) { 
				if(table.entries[i].class==class) { 
					return i; 
				} 
			} 
		}
	}
	return -1;
}

int generate_dns_header(char* dest, int16_t id, char opcode, char authority_answer, char truncated, char recursive_question, char recursive_answer, char rcode, int16_t questions, int16_t answers, int16_t name_servers, int16_t additionals) {
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
	return offset;
}

int generate_dns_question(char* dest, char* qname, int16_t qtype, int16_t qclass) {
	int offset=0;
	char* ptr;
	// QNAME
	// Le label recu en parametre est au format Big Endian
	if(test_little_endian()) { 
		char* reverse_qname = reverse_str(qname, strlen(qname));
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
	return offset;
}

int generate_dns_resource_record(char* dest, int16_t rname, int16_t rtype, int16_t rclass, int32_t ttl, int16_t rdlength, char* rdata) {
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
		char* reverse_rdata = reverse_str(rdata, 4);
		write_in_str(dest, &offset, reverse_rdata, rdlength);
		free(reverse_rdata);
	} else {
		write_in_str(dest, &offset, rdata, rdlength);
	}
	return offset;
}

dns_header* get_dns_header(char* src) {
	dns_header* h;
	h = malloc(sizeof(dns_header));

	int offset=0;
	int8_t buff[2]={0};
	// ID
	if(test_little_endian()) {
		buff[1]=src[offset]&0xFF;
		offset++;
		buff[0]=src[offset]&0xFF;
		offset++;
	} else {
		buff[0]=src[offset]&0xFF;
		offset++;
		buff[1]=src[offset]&0xFF;
		offset++;
	}
	h->id=convert_2int8_to_int16(buff);
	// OPCODE
	buff[0]=src[offset];
	buff[0]=(buff[0]&0x78)>>3;
	h->opcode=buff[0];
	// AUTHORITY ANSWER
	buff[0]=src[offset];
	buff[0]=(buff[0]&0x04)>>2;
	h->authority_answer=buff[0];
	// TRUNCATED
	buff[0]=src[offset];
	buff[0]=(buff[0]&0x02)>>1;
	h->truncated = buff[0];
	// RECURSIVE QUESTION
	buff[0]=src[offset];
	h->recursive_question=buff[0]&0x01;
	offset++;
	// RECURSIVE ANSWER
	buff[1]=src[offset];
	buff[1]=(buff[1]&0x80)>>7;
	h->recursive_answer=buff[1];
	// RCODE
	buff[1]=src[offset];
	h->rcode=buff[1]&0x0F;
	offset++;
	// QDCOUNT
	if(test_little_endian()) {
		buff[1]=src[offset++];
		buff[0]=src[offset++];
	} else {
		buff[0]=src[offset++];
		buff[1]=src[offset++];
	}
	h->qdcount=convert_2int8_to_int16(buff);
	// ANCOUNT
	if(test_little_endian()) {
		buff[1]=src[offset++];
		buff[0]=src[offset++];
	} else {
		buff[0]=src[offset++];
		buff[1]=src[offset++];
	}
	h->ancount=convert_2int8_to_int16(buff);
	// NSCOUNT
	if(test_little_endian()) {
		buff[1]=src[offset++];
		buff[0]=src[offset++];
	} else {
		buff[0]=src[offset++];
		buff[1]=src[offset++];
	}
	h->nscount=convert_2int8_to_int16(buff);
	// ARCOUNT
	if(test_little_endian()) {
		buff[1]=src[offset++];
		buff[0]=src[offset++];
	} else {
		buff[0]=src[offset++];
		buff[1]=src[offset++];
	}
	h->arcount=convert_2int8_to_int16(buff);

	return h;
}

dns_question* get_dns_question(char* src) {
	int i=0;
	dns_question* q;
	q = malloc(sizeof(dns_question));

	// QNAME
	while(src[i]!=0) {
		q->name[i]=src[i];
		i++;
	}
	q->name[i]=0;
	fprintf(stderr, "\n");
	i++;
	int8_t buff[2];
	// QTYPE
	if(test_little_endian()) {
		buff[1]=src[i++];
		buff[0]=src[i++];
	} else {
		buff[0]=src[i++];
		buff[1]=src[i++];
	}
	q->type=convert_2int8_to_int16(buff);
	// QCLASS
	if(test_little_endian()) {
		buff[1]=src[i++];
		buff[0]=src[i++];
	} else {
		buff[0]=src[i++];
		buff[1]=src[i++];
	}
	q->class=convert_2int8_to_int16(buff);
	
	return q;
}

dns_resource_record* get_dns_resource_record(char* src) {
	// TODO
}

int answer_to_question(char* answer, char* question, int question_length, dns_table table) {
	dns_header* h = get_dns_header(question);
	dns_question* q = get_dns_question(question+12);
	int index, offset=0;

	if((index=search_table_entry(table, q->name, q->type, q->class))==-1) {
		int tube[2];
		if(pipe(tube)==-1) {
			fprintf(stderr, "Error Tube\n");
			return EXIT_FAILURE;
		}
		int pid=fork();
		if(pid==-1) {
			fprintf(stderr, "Error Fork\n");
			return EXIT_FAILURE;
		}
		if(pid==0) {
			dup2(0, tube[0]);
			dup2(1, tube[1]);
			char* const args[]={"nc","-u","dns.unicaen.fr","53"};
			execv("nc", args);
			int offset=0;
		} else {
			char* packet=malloc(512);
			write(tube[0], question, question_length);
			int length=read(tube[1], packet, 512);
			packet[0]=question[0];
			packet[1]=question[1];
			answer=packet;
			return length;
		}
		return EXIT_FAILURE;
	} else {
		// gen header
		offset=generate_dns_header(answer, h->id, h->opcode, h->authority_answer, h->truncated, h->recursive_question, h->recursive_question==1?1:0, h->rcode, h->qdcount, h->ancount+1, h->nscount, 0x0000);
		// gen question
		offset+=generate_dns_question(answer+offset, q->name, q->type, q->class);
		// gen answer
		offset+=generate_dns_resource_record(answer+offset, 0xC00C, q->type, q->class, 0x00015180, table.entries[index].data_length, table.entries[index].data);
	}
	return offset;
}
