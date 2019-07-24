//vita-mcr2vmp by @dots_tb - signs psx mcr files for use with the vita's psp emulator
//With help from the CBPS (https://discord.gg/2nDCbxJ) , especially:
// Analog Man
// @teakhanirons

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#define CBC 1

#include "aes.h"
#include "sha1.h"

uint8_t key[0x10] = {0xAB, 0x5A, 0xBC, 0x9F, 0xC1, 0xF4, 0x9D, 0xE6, 0xA0, 0x51, 0xDB, 0xAE, 0xFA, 0x51, 0x88, 0x59};
uint8_t iv[0x10] = {0xB3, 0x0F, 0xFE, 0xED, 0xB7, 0xDC, 0x5E, 0xB7, 0x13, 0x3D, 0xA6, 0x0D, 0x1B, 0x6B, 0x2C, 0xDC};

#define SEED_OFFSET 0xC
#define HASH_OFFSET 0x20
#define MCR_OFFSET 0x80
#define PMV_MAGIC 0x564D5000
#define FILE_SZ 0x20080
#define MC_SZ 0x20000

void XorWithByte(uint8_t* buf, uint8_t byte, int length)
{
	for (int i = 0; i < length; ++i) {
    	buf[i] ^= byte;
	}
}


static void usage(char *argv[])
{
	printf("\nvita-mcr2vmp by @dots_tb\nWith CBPS help especially: @AnalogMan151 and @teakhanirons\n");
	printf("Converts PSOne MCRs into signed VMPs for use on PSP/Vita and also\n");
	printf("extracts MCR files from Sony's PSP VMP save file format.\n\n");
	printf("Usage: %s <memorycard.mcr|SCEVMC*.VMP>\n",argv[0]);
}


int main(int argc, char **argv)
{
	printf("\n=====Vita MCR2VMP by @dots_tb=====\n\n");
	if (argc != 2) {
		usage(argv);
		return 1;
	}

	FILE *fin, *fout;
	char mc_magic[4] = {0x4D, 0x43, 0x00, 0x00};
	char vmp_magic[4] = {0x00, 0x50, 0x4D, 0x56};
	char buf[4];
	bool mcr, vmp = false;
	fin = fopen(argv[1], "rb");
	if (!fin) {
		perror("Failed to open input file");
		goto error;
	}

	// Check MAGIC
	fseek(fin, 0, SEEK_SET);
	fread(buf, 1, 4, fin);
	if (memcmp(buf, mc_magic, 4) == 0) {
		mcr = true;
	} else if (memcmp(buf, vmp_magic, 4) == 0) {
		vmp = true;
	} else {
		perror("File is not supported");
		usage(argv);
		goto error;
	}

	// Passes check, strips VMP header and writes MCR
	if (vmp) {
		char mcbuf[0x20000];
		fseek(fin, MCR_OFFSET, SEEK_SET);
		fread(mcbuf, 1, MC_SZ, fin);
		fclose(fin);
		char output_path[128];
		sprintf(output_path,"%s.mcr",argv[1]);
		fout = fopen(output_path, "wb");
		if (!fout) {
			perror("Failed to open output file");
			goto error;
		}
		fwrite(mcbuf, 1, 0x20000, fout);
		printf("MCR file successfully extracted.\n");
	}

	// Passes check, writes MCR with signed VMP header
	if (mcr) {
		fseek(fin, 0, SEEK_SET);
		
		size_t sz = FILE_SZ;
		uint8_t *input = (unsigned char*) calloc (1, sz);
		uint32_t *input_ptr = (uint32_t*) input;
		input_ptr[0] = PMV_MAGIC;
		input_ptr[1] = MCR_OFFSET;
		
		fseek(fin, 0, SEEK_SET);
		fread(input + MCR_OFFSET, sz, 1, fin);
		
		struct AES_ctx aes_ctx;
		AES_init_ctx_iv(&aes_ctx, key, iv);
		

		uint8_t salt[0x40];
		uint8_t work_buf[0x14];
		
		uint8_t *salt_seed = input + SEED_OFFSET;
		
		memcpy(work_buf, salt_seed, 0x10);
		AES_ECB_decrypt(&aes_ctx, work_buf);
		memcpy(salt, work_buf, 0x10);

		memcpy(work_buf, salt_seed, 0x10);
		AES_ECB_encrypt(&aes_ctx, work_buf);
		memcpy(salt + 0x10, work_buf, 0x10);

		
		XorWithIv(salt, iv);
		
		memset(work_buf, 0xFF, sizeof(work_buf));
		memcpy(work_buf, salt_seed + 0x10, 0x4);
		XorWithIv(salt + 0x10, work_buf);

		
		memset(salt + 0x14, 0, sizeof(salt) - 0x14);
		XorWithByte(salt, 0x36, 0x40);
		
		SHA1_CTX sha1_ctx_1;	
		SHA1Init(&sha1_ctx_1);
	

		
		SHA1Update( &sha1_ctx_1, salt, 0x40);

		
		memset(input + 0x20, 0, 0x14);
		SHA1Update( &sha1_ctx_1, input, sz);
			
			
		XorWithByte(salt, 0x6A, 0x40);

		SHA1Final(work_buf, &sha1_ctx_1);

		SHA1_CTX sha1_ctx_2;
		SHA1Init(&sha1_ctx_2);
		SHA1Update( &sha1_ctx_2, salt, 0x40);
		SHA1Update( &sha1_ctx_2, work_buf, 0x14);



		SHA1Final(input + HASH_OFFSET, &sha1_ctx_2);
		
		printf("Generated key: ");
		for(int i = 0; i < 0x14; i++ ) {
			printf("%02X ", input[HASH_OFFSET + i]);
		}
		printf("\n");
		
		char output_path[128];
		sprintf(output_path,"%s.VMP",argv[1]);
		fout = fopen(output_path, "wb");
		if (!fout) {
			perror("Failed to open output file");
			goto error;
		}
		fwrite(input,  1, FILE_SZ, fout);
		printf("VMP created successfully.\n");
	}
error:
	if (fin)
		fclose(fin);
	if (fout)
		fclose(fout);	
	exit(0);
	return 0;
}
