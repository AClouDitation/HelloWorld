#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef int (*func_t)();
__attribute__((section(".text"))) char buf[] = {
	0x90, 0xc3
};
int main(){

	func_t f = (func_t)(long)buf;
	int rv = (*f)();
	char baka;
	
	printf("Hello World!\n");

	return 0;
}
