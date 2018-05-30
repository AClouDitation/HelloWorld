#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <byteswap.h>

static uint8_t buf[] = {
	0x90, 											//nop 
	0x48, 0x89, 0xD6,								//mov 	rsi rdx,
	0x48, 0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00,		//mov	rax,0x1
	0x48, 0xC7, 0xC2, 0x0D, 0x00, 0x00, 0x00,		//mov 	rdx,0xd
	0x48, 0xC7, 0xC7, 0x01, 0x00, 0x00, 0x00,		//mov 	rdi,0x1
	0x0F, 0x05,										//syscall
	0xC3											//ret
};

const char message[] = "Hello World!\n";
typedef int (*func_t)(unsigned long);

int main(){

	const size_t len = sizeof(buf);
	unsigned long addr = (unsigned long)message;

	void *hw_code = mmap(NULL, len, PROT_READ|PROT_WRITE, 
		MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	
	if(hw_code == MAP_FAILED) {
		fprintf(stderr, "mmap() failed\n");
		return EXIT_FAILURE;
	}

	memcpy(hw_code, buf, len);

	if(mprotect(hw_code, len, PROT_EXEC) < 0){
		fprintf(stderr, "mprotect failed to mark exec-only\n");
		return EXIT_FAILURE;
	}

	func_t func = (func_t)(uint64_t)hw_code;
	func(addr);
	printf("Done!\n");

	return 0;
}
