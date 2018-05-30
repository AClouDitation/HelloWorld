#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){

	FILE* fp_in = fopen("xd.txt","r");
	char str[10000] = {'\0'};

	int i = 0;
	int j = 0;
	str[i++] = '0';
	str[i++] = 'x';
	printf("%s\n",str);
	while(true){
		int ch = fgetc(fp_in);
		if(ch == EOF) break;
		if(ch == '\n') continue;
		str[i++] = ch;
		j++;
		j %= 2;
		if(!j){
			str[i++] = ',';
			str[i++] = '0';
			str[i++] = 'x';
		}
	}
	fclose(fp_in);

	printf("%s\n",str);
	FILE* fp_out = fopen("xd_out.txt","w");
	fprintf(fp_out,"%s\n",str);
	fclose(fp_out);

	return EXIT_SUCCESS;
}
