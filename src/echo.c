#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char** argv) {
	char packet[512]={0};
	int length=read(0, packet, 512);
	for(int i=0; i<length; i++) {
		fprintf(stdin, "%x ", packet[i]);
	}
	//fprintf(stdout, "\n");
}
