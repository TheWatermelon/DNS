#include "fonctions.h"
#include <stdio.h>

int main(int argc, const char** argv) {
	char* url = "www.unicaen.fr";
	char label[16]={0};
	char url_recovered[16]={0};


	printf("url : %s\n", url);
	printf("label : [%d] ", str_to_label(url, label));
	print_string_in_bytes(label);

	printf("recovered url : [%d] %s\n", label_to_str(label, url_recovered), url_recovered);
}
