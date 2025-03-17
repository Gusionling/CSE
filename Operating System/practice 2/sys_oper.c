#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>


SYSCALL_DEFINE3(oper_add, long, a, long, b, long *, to_user){
	long result = 0;
	result = a + b;
	printk("[kernel Message] result = %ld\n",result);
       	put_user(result, to_user);	
	return 0;
}

SYSCALL_DEFINE3(oper_sub, long, a, long, b, long *, to_user){
	long result = 0;
	result = a - b;
	printk("[kernel Message] result = %ld\n",result);
	put_user(result, to_user);	
	return 0;
}

SYSCALL_DEFINE3(oper_subv2, long, a, long, b, long *, to_user){
	long result = 0;
	result = a - b;
	printk("[kernel Message] V2result = %ld\n",result);
	printk("[kernel Message] V2address = %p\n", to_user);
	put_user(result, to_user);	
	return 0;
}

SYSCALL_DEFINE2(oper_subv3, long, a, long, b){
	long result = 0;
	result = a - b;
	printk("[kernel Message] V3result = %ld\n",result);	
	return result;
}


