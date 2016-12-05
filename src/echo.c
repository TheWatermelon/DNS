#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char** argv) {
	char packet[512]={0};
	char header[12]={0};
	int length=read(0, packet, 512);
	fprintf(stderr, "Received %d bytes\n", length);
	fprintf(stderr, "HEADER : ");
	for(int i=0; i<12; i++) {
		fprintf(stderr, "%x ", header[i]);
	}
}
