#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>

SYSCALL_DEFINE1(alpha_reverse, char *, str)
{
	int length = 0;
	int i;
	char ret[100];
	memset(ret, '\0', 100);

	while(str[length] != '\0'){
		length++;
	}

	for(i = 0; i<length; i++){
		ret[length - i -1] = str[i];
	}
	copy_to_user(str, ret, sizeof(ret));

	return 0;
}
