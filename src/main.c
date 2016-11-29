#include "fonctions.h"
#include <stdio.h>

int main(int argc, const char** argv) {
	char* url = "www.unicaen.fr";
	char label[16]={0};

	printf("url : %s\n", url);
	printf("label : [%d] ", str_to_label(url, label));
	print_string_in_bytes(label);
}
