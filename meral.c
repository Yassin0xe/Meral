#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "meral.h"


void helper(char * shellcode,int size,char * output,int verbose,int mtype,int otype,int comment,unsigned int last){
	printf("[*] starting the encoding inline ...\n");
	int i = 0;
	FILE * fd;
	char byte;
	void (*target)();
	if(mtype == 0){
		target = sub_printable;
	}else if(mtype == 1){
		target = add_printable;
	}else{
		fatal("failed get the encode type");
	}
	unsigned int targ = 0;
	unsigned char l[4];
	if(verbose){
		printf("[*] opening the output file\n");
	}

	fd = fopen(output,"w");
	if(fd == NULL){
		fatal("failed open the output file");
	}
	printf("[*] start writing the shellcode\n");
	if(otype == 1){
		fprintf(fd,"BITS 32\n");
		fprintf(fd,"push esp\n");
		fprintf(fd,"pop eax\n");
		target(0,size*4,verbose,fd,otype,comment);
		fprintf(fd,"pop esp\n");
	}else if(otype == 0){
		putc(0x54,fd);
		putc(0x58,fd);
		target(0,size*4,verbose,fd,otype,comment);
		putc(0x5c,fd);
	}

	if(last == 0){
			if(otype == 1){
				fprintf(fd,"xor eax,eax\n");
			}else if(otype == 0){
				putc(0x31,fd);
				putc(0xc0,fd);
			}
	}else{
			if(otype == 1){
				fprintf(fd,"push 0x%08x\n",last);
				fprintf(fd,"pop eax\n");
			}else if(otype == 0){
				putc(0x68,fd);
				putc((last & 0x000000ff),fd);
				putc((last & 0x0000ff00)>>8,fd);
				putc((last & 0x00ff0000)>>16,fd);
				putc((last & 0xff000000)>>24,fd);
				putc(0x58,fd);
			}
	}
	for(i=size;i>0;i=i-4){
		l[0] = shellcode[i-4];
		l[1] = shellcode[i-3];
		l[2] = shellcode[i-2];
		l[3] = shellcode[i-1];
		targ = *((unsigned int *) l);
		target(last,targ,verbose,fd,otype,comment);
		last = targ;
	}
	printf("[*] successful writing the encode shellcode\n");
	fclose(fd);
	printf("[*] successful writing the output in (%s)\n",output);
}



void usage(char *program){
	printf("Usage %s [options]\n\n",program);
	printf("\t 888b     d888                          888 \n");
	printf("\t 8888b   d8888                          888 \n");
	printf("\t 88888b.d88888                          888 \n");
	printf("\t 888Y88888P888  .d88b.  888d888 8888b.  888 \n");
	printf("\t 888 Y888P 888 d8P  Y8b 888P\"      \"88b 888 \n");
	printf("\t 888  Y8P  888 88888888 888    .d888888 888 \n");
	printf("\t 888   \"   888 Y8b.     888    888  888 888 \n");
	printf("\t 888       888  \"Y8888  888    \"Y888888 888 \n");
	printf("\n\n");
	printf("\n Options :\n");
	printf("    --input   -i   FILE    The start value of the operator\n");
	printf("    --output  -o   FILE    The end value of the operator\n");
	printf("    --add     -d           Use the Addition operator\n");
	printf("    --sub     -u           Use the Subtract operator\n");
	printf("    --asm     -s           Write the as a assembly file\n");
	printf("    --hex     -x           Show the shellcode as Hexdump\n");
	printf("    --last    -l   HEX     The Start eax content\n");
	printf("    --bin     -b           Write the as a binary file\n");
	printf("    --comment -c           Write the Comment in the assembly file\n");
	printf("    --verbose -v           Use the verbose Mode\n");
	printf("    --help    -h           Show the help menu\n");
	printf("\n Exemples :\n");
	printf("    %s -i shellcode.bin --sub --bin -o encoded-shellcode.bin -l 0x41414141 -x\n",program);
	printf("    %s -i shellcode.bin --sub --asm -o encoded-shellcode.bin -v\n",program);
	printf("    %s -i shellcode.bin --add --asm -o encoded-shellcode.bin -l 0x41414141 -c\n",program);
	printf("    %s -i shellcode.bin --sub --bin -o encoded-shellcode.bin\n\n",program);
	exit(0);
}
int main(int argc,char * argv[]){
	if(argc<5){
		usage(argv[0]);
	}
	unsigned int last=0;
	char *input_file = NULL;
	char *output_file = NULL;
	int fd,verbose=0;
	char * shellcode;
	int size,rest,fsize,mtype=0,otype=0,thex=0,comment=0;
	int i = 0;
	for(i=0;i<argc;i++){
		if((strcmp(argv[i],"--input") == 0) || (strcmp(argv[i],"-i") == 0)){
			input_file = argv[i+1];
			i++;
		}else if((strcmp(argv[i],"--output") == 0) || (strcmp(argv[i],"-o") == 0)){
			output_file = argv[i+1];
			i++;
		}else if((strcmp(argv[i],"--last") == 0) || (strcmp(argv[i],"-l") == 0)){
			last = strtoul(argv[i+1],NULL,0);
			i++;
		}else if((strcmp(argv[i],"--verbose") == 0) || (strcmp(argv[i],"-v") == 0)){
			verbose = 1;
		}else if((strcmp(argv[i],"--add") == 0) || (strcmp(argv[i],"-d") == 0)){
			mtype = 1;
		}else if((strcmp(argv[i],"--sub") == 0) || (strcmp(argv[i],"-u") == 0)){
			mtype = 0;
		}else if((strcmp(argv[i],"--asm") == 0) || (strcmp(argv[i],"-s") == 0)){
			otype = 1;
		}else if((strcmp(argv[i],"--bin") == 0) || (strcmp(argv[i],"-b") == 0)){
			otype = 0;
		}else if((strcmp(argv[i],"--hex") == 0) || (strcmp(argv[i],"-x") == 0)){
			thex = 1;
		}else if((strcmp(argv[i],"--comment") == 0) || (strcmp(argv[i],"-c") == 0)){
			comment = 1;
		}else if((strcmp(argv[i],"--help") == 0) || (strcmp(argv[i],"-h") == 0)){
			usage(argv[0]);
		}
	}
	if(input_file){
		printf("[+] input-file : (%s)\n",input_file);
	}else{
		fatal("not found the input file");
	}
	if(output_file){
		printf("[+] ouput-file : (%s)\n",output_file);
	}else{
		fatal("not found the output file");
	}
	if(comment){
		printf("[+] the assembly comment : True\n");
	}
	if(!mtype){
		printf("[+] the encoding operator : [Subtract]\n");
	}else{
		printf("[+] the encoding operator : [Addition]\n");
	}if(!otype){
		printf("[+] the output type : [binary]\n");
	}else{
		printf("[+] the output type : [assembly]\n");
	}
	if(last){
		printf("[+] start with 0x%08x in the eax\n",last);
	}



	if(verbose){
		printf("[*] opening the file inline ...\n");
	}
	fd = open(input_file,O_RDONLY,0);
	if(fd == -1){
		fatal("failed open the file");
	}
	fsize = get_file_size(fd);
	if(fsize == -1){
		fatal("failed get the  file size");
	}
	size = (fsize+8)-(fsize+8)%8;
	rest = size-fsize;
	shellcode = (char *) malloc(size);
	if(shellcode == NULL){
		fatal("failed the memory allocation");
	}
	memset(shellcode,0,size);
	memset(shellcode,0x90,rest);
	if(read(fd,shellcode+rest,fsize) != fsize){
		close(fd);
		fatal("failed read the data from the file");
	}
	printf("[*] successful reading (%d bytes) from (%s)\n",fsize,input_file);
	close(fd);
	if(thex){
		printf("[*] the hexdump of the shellcode\n");
		hexdump(shellcode,size);
	}
	helper(shellcode,size,output_file,verbose,mtype,otype,comment,last);
}
