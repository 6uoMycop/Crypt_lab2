#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <stdint.h>

#define KEY_SEARCH // search weak and semi-weak keys

#define BLOCK_SIZE 4 // also BLOCK_SIZE= size of key
#define d 4 // number of rounds
const uint8_t S[16] = { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 };
const uint8_t S_reverse[16] = { 1, 8, 14, 5, 13, 7, 4, 11, 15, 2, 0, 12, 10, 9, 3, 6 };

int readText(uint8_t** pBuf, const char* pFileName);
void S_blocks(uint8_t* B, const uint8_t* S);
int P_func(int num);
int P_func_reverse(int num);
void P_block(uint8_t* B, int (*func) (int));
void encryptBlock(uint8_t* Y, const uint8_t* X, const uint8_t* key);
void decryptBlock(const uint8_t* Y, uint8_t* X, const uint8_t* key);
void encryptRemainder(uint32_t* Y_prev, const uint32_t* X_remainder, uint8_t* Y_remainder, const uint32_t* key, int l);
void decryptRemainder(uint32_t* X_prev, const uint32_t* Y_remainder, uint8_t* X_remainder, const uint32_t* key, int l);
void E(uint8_t* cyphertext, const uint8_t* plaintext, const uint8_t* key, int len);
void D(const uint8_t* cyphertext, uint8_t* plaintext, const uint8_t* key, int len);
