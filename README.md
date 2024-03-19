# the Meral Tool
## description
this tool created by Yassin0xe for encoding the shellcodes

##
to install this tool open a terminal and tape the commands below :
```sh
chmod +x ./install
sudo ./install
```

## the usage
the usage : 
```
Usage meral [options]

	 888b     d888                          888 
	 8888b   d8888                          888 
	 88888b.d88888                          888 
	 888Y88888P888  .d88b.  888d888 8888b.  888 
	 888 Y888P 888 d8P  Y8b 888P"      "88b 888 
	 888  Y8P  888 88888888 888    .d888888 888 
	 888   "   888 Y8b.     888    888  888 888 
	 888       888  "Y8888  888    "Y888888 888 



 Options :
    --input   -i   FILE    The start value of the operator
    --output  -o   FILE    The end value of the operator
    --add     -d           Use the Addition operator
    --sub     -u           Use the Subtract operator
    --asm     -s           Write the as a assembly file
    --hex     -x           Show the shellcode as Hexdump
    --last    -l   HEX     The Start eax content
    --bin     -b           Write the as a binary file
    --comment -c           Write the Comment in the assembly file
    --verbose -v           Use the verbose Mode
    --help    -h           Show the help menu

 Exemples :
    meral -i shellcode.bin --sub --bin -o encoded-shellcode.bin -l 0x41414141 -x
    meral -i shellcode.bin --sub --asm -o encoded-shellcode.bin -v
    meral -i shellcode.bin --add --asm -o encoded-shellcode.bin -l 0x41414141 -c
    meral -i shellcode.bin --sub --bin -o encoded-shellcode.bin

```

## binary output 
 ```sh
$ meral -i shell.bin -o eshell.bin -x -d -b
[+] input-file : (shell.bin)
[+] ouput-file : (eshell.bin)
[+] the encoding operator : [Addition]
[+] the output type : [binary]
[*] successful reading (21 bytes) from (shell.bin)
[*] the hexdump of the shellcode

0x00000000 | 90 90 90 41 41 41 41 42   42 42 42 43 43 43 43 44 | ...AAAAB  BBBCCCCD
0x00000010 | 44 44 44 45 45 45 45 0a                           | DDDEEEE.

[*] starting the encoding inline ...
[*] start writing the shellcode
[*] successful writing the encode shellcode
[*] successful writing the output in (eshell.bin)
```
