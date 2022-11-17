all: s1c3.exe xor_encrypt_crack.exe

s1c3.exe: s1c3.c stats.c bitlib.c cipher_cracker.c
	gcc s1c3.c stats.c bitlib.c cipher_cracker.c -o s1c3

xor_encrypt_crack.exe: xor_encrypt_crack.c stats.c bitlib.c cipher_cracker.c
	gcc xor_encrypt_crack.c stats.c bitlib.c cipher_cracker.c -o xor_encrypt_crack

clean:
	rm *.exe