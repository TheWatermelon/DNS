#include "fonctions.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char** argv) {
	char packet[512]={0};
	char header[12]={0};
	char question[64]={0};
	dns_table table = init_table("../doc/config_dns");

	int received_length=read(0, packet, 512);
	fprintf(stderr, "Received %d bytes\n", received_length);

	char answer[512]={0};
	int length=answer_to_question(answer, packet, received_length, table);
	fprintf(stderr, "Send %d bytes\n", length);
	write(1, answer, length);
}
