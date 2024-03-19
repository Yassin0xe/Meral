#define CHR "azertuiopqsdfghjklmwxcvbn1234567890AZERTYUIOPQSDFGHJKLMWXCVBN%_"

void add_printable(unsigned int targ,unsigned int last,unsigned int verbose,FILE *fd,int otype,int comment){
	unsigned int res, t[4], l[4];
	unsigned int try, single, carry=0;
	int len, a, i, j, k, m, z, flag=0;
	char word[3][4];
	unsigned char mem[70];
	srand(time(NULL));
	memset(mem,0,70);
	strcpy(mem,CHR);
	len = strlen(mem);
	t[3] = (targ & 0xff000000)>>24;
	t[2] = (targ & 0x00ff0000)>>16;
	t[1] = (targ & 0x0000ff00)>>8;
	t[0] = (targ & 0x000000ff);
	l[3] = (last & 0xff000000)>>24;
	l[2] = (last & 0x00ff0000)>>16;
	l[1] = (last & 0x0000ff00)>>8;
	l[0] = (last & 0x000000ff);
	for(a = 1; a < 5; a++) {
		carry = flag = 0;
		for(z=0; z < 4; z++) {
			for(i=0; i < len; i++) {
				for(j=0; j < len; j++) {
				for(k=0; k < len; k++) {
					for(m=0; m < len; m++){
						if(a < 2){j = len+1;}
						if(a < 3){k = len+1;}
						if(a < 4){m = len+1;}
						try = t[z] + carry+mem[i]+mem[j]+mem[k]+mem[m];
						single = (try & 0x000000ff);
						if(single == l[z]){

							carry = (try & 0x0000ff00)>>8;
							if(i < len){word[0][z] = mem[i];}
							if(j < len){word[1][z] = mem[j];}
							if(k < len){word[2][z] = mem[k];}
							if(m < len){word[3][z] = mem[m];}
							i = j = k = m = len+2;
							flag++;
						}
					}
				}
			}
		}
	}
}
	res = targ;
	char bytes[6];
	for(i=0; i < 4; i++){
		memset(bytes,0,4);
		strncat(bytes,word[i],4);
		res += *((unsigned int *)word[i]);
    if(otype){
      fprintf(fd,"add eax,0x%08x", *((unsigned int *) word[i]));
      if(comment){
        fprintf(fd," ; (%s)",bytes);
      }
      fprintf(fd,"\n");
    }else{
        putc(0x05,fd);
        fprintf(fd,"%s",bytes);
    }
	}
  if(otype){
    fprintf(fd,"push eax");
    if(comment){
      fprintf(fd," ; eax = 0x%08x", res);
    }
    fprintf(fd,"\n\n");
  }else{
      putc(0x50,fd);
  }
  if(verbose){printf("[*] seccussful : eax = 0x%08x\n",res);}
}


void sub_printable(unsigned int last,unsigned int targ,unsigned int verbose,FILE *fd,int otype,int comment){
  unsigned int res, t[4], l[4];
	unsigned int try, single, carry=0;
	int len, a, i, j, k, m, z, flag=0;
	char word[3][4];
	unsigned char mem[70];
	srand(time(NULL));
	memset(mem,0,70);
	strcpy(mem,CHR);
	len = strlen(mem);
	t[3] = (targ & 0xff000000)>>24;
	t[2] = (targ & 0x00ff0000)>>16;
	t[1] = (targ & 0x0000ff00)>>8;
	t[0] = (targ & 0x000000ff);
	l[3] = (last & 0xff000000)>>24;
	l[2] = (last & 0x00ff0000)>>16;
	l[1] = (last & 0x0000ff00)>>8;
	l[0] = (last & 0x000000ff);
	for(a = 1; a < 5; a++) {
		carry = flag = 0;
		for(z=0; z < 4; z++) {
			for(i=0; i < len; i++) {
				for(j=0; j < len; j++) {
				for(k=0; k < len; k++) {
					for(m=0; m < len; m++){
						if(a < 2){j = len+1;}
						if(a < 3){k = len+1;}
						if(a < 4){m = len+1;}
						try = t[z] + carry+mem[i]+mem[j]+mem[k]+mem[m];
						single = (try & 0x000000ff);
						if(single == l[z]){

							carry = (try & 0x0000ff00)>>8;
							if(i < len){word[0][z] = mem[i];}
							if(j < len){word[1][z] = mem[j];}
							if(k < len){word[2][z] = mem[k];}
							if(m < len){word[3][z] = mem[m];}
							i = j = k = m = len+2;
							flag++;
						}
					}
				}
			}
		}
	}
}
	res = last;
	char bytes[6];
	for(i=0; i < 4; i++){
		memset(bytes,0,4);
		strncat(bytes,word[i],4);
		res -= *((unsigned int *) word[i]);
    if(otype){
		    fprintf(fd,"sub eax,0x%08x", *((unsigned int *) word[i]));
        if(comment){
          fprintf(fd," ; (%s)",bytes);
        }
        fprintf(fd,"\n");

    }else{
        putc(0x2d,fd);
        fprintf(fd,"%s",bytes);
    }
	}
  if(otype){
      fprintf(fd,"push eax");
      if(comment){fprintf(fd," ; eax = 0x%08x", res);}
      fprintf(fd,"\n\n");
  }else{
      putc(0x50,fd);
  }
  if(verbose){printf("[*] seccesful : eax = 0x%08x\n",res);}
}




void hexdump(const char * data_buffer,int length){
	unsigned char byte;
	unsigned int i,j=0,m=0;
	printf("\n");
	for(i = 0;i<length;i++){
		if(i%16 == 0){
			printf("0x%08x | ",i);
			m++;
		}
		if((i%8 == 0) && (i%16 != 0)){
			printf("  ");
		}
		byte = data_buffer[i];
		printf("%02x ",byte);

		if((i%16 == 15) | (i == length-1)){
			for(j=0;j < 15 - i%16;j++){
				printf("   ");
				if(((j+1)%8 == 0) && ((j+1)%16 != 0)){
					printf("  ");
				}
			}
			printf("| ");
			for(j=(i-i%16);j<=i;j++){
				byte = data_buffer[j];
				if((j%8 == 0) && (j%16 != 0)){
					printf("  ");
				}
			 	if(byte<127 && byte>31){
					printf("%c",byte);
				}
				else{
					printf(".");
				}
			}
			printf("\n");
		}

	}
	printf("\n");
}

int get_file_size(int fd){
  struct stat stat_struct;
  if(fstat(fd,&stat_struct) == -1){
    return -1;
  }
  return (int) stat_struct.st_size;
}
void fatal(char * message){
  printf("[-] %s\n",message);
  exit(-1);
}
