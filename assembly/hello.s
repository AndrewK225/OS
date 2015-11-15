#Andrew Koe 
	
.global _start

	.text

_start:
	mov $4, %eax    #write syscall number
	mov $1, %ebx	#declare stdout
	mov $message, %ecx #move the text
	mov $6, %edx	#write 6 bytes
	int $0x80	#OS specific

	mov $1, %eax	#exit  Without it, SIGSEGV
	xor %ebx, %ebx  #return 0
	int $0x80

message:
	.ascii "Hello\n"
	