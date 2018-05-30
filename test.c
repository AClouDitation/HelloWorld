#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <byteswap.h>

static uint8_t buf[] = {
	0x48, 0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00,		//mov	rax,0x1
	0x48, 0xC7, 0xC2, 0x0D, 0x00, 0x00, 0x00,		//mov 	rdx,0xd
	0x48, 0xC7, 0xC6, 0x00, 0x00, 0x00, 0x00,		//mov 	rsi,TBA  
	0x48, 0xC7, 0xC7, 0x01, 0x00, 0x00, 0x00,		//mov 	rdi,0x1
	0x0F, 0x05,										//syscall
	0xC3											//ret
};

typedef int (*func_t)(void);

int print(char* message){

	/* write message address */
	uint32_t *addr = (uint32_t*)&message;
	memcpy(buf + 17,(uint8_t*)addr,4);

	/* write message length */
	size_t len = strlen(message);
	uint32_t *len_ptr = (uint32_t*)&len;
	memcpy(buf + 10,(uint8_t*)len_ptr,4);

	void *code = mmap(NULL, sizeof(buf), PROT_READ|PROT_WRITE, 
		MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	
	if(code == MAP_FAILED) {
		fprintf(stderr, "mmap() failed\n");
		return -1;
	}

	memcpy(code, buf, sizeof(buf));

	if(mprotect(code, sizeof(buf), PROT_EXEC) < 0){
		fprintf(stderr, "mprotect failed to mark exec-only\n");
		return -1;
	}

	func_t func = (func_t)(uint64_t)code;
	func();

	return 0;
}

int main(){

	print("Hello World!\n");
	print("BAKA!\n");

	return 0;
}
