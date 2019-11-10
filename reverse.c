#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
	char *str =    "xttzuikeaiofgeva,ixyuakzpxnfbega,jskakwbdvxgahwotbrcagsjxbdhshdgsjwapd.inbshfgshdkxbansoxuctebtngkajchevqkfurb";
	int length = strlen(str);
	

	while (length >= 0) {
		char curr = *(str + length--);
		printf("%c", curr);
	}
	printf("\n");
}

