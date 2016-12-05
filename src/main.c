#include "fonctions.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char** argv) {
	char packet[512]={0};
	char header[12]={0};
	char question[64]={0};
	dns_table table = init_table("../doc/config_dns");

	int length=read(0, packet, 512);
	fprintf(stderr, "Received %d bytes\n", length);

	char answer[512]={0};
	length=answer_to_question(answer, packet, table);
	write(1, answer, length);
}
