#ifndef BLUM_H
#define BLUM_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

char xorBool(char a, char b);

int sizeOfFile(const char* file_path);

// test de primalité fondé sur le théorème de Fermat
// renvoie un entier non nul si n est premier (pseudo premier)
int Fermat(const mpz_t n, unsigned int k, gmp_randstate_t state);

// Genère un nombre premier congru à 3 modulo 4 qui est plus petit que n
void GenPremier(mpz_t res, const mpz_t n, unsigned int k, gmp_randstate_t state);

// Générateur de Blum Blum Shub
char BBS_step(const mpz_t n, mpz_t x);

// Lire/Ecrire la clé publique dans un fichier
void read_pub(mpz_t pub, char * file_path);
void write_pub(char * file_path, mpz_t pub);

// Lire/Ecrire la clé privée dans un fichier
void read_prvt(mpz_t prvt1, mpz_t prvt2, char * file_path);
void write_prvt(char * file_path, mpz_t prvt1, mpz_t prvt2);

// Lire/Ecrire un message clair dans un fichier
unsigned int read_plain(char * file_path, char * plain);
void write_plain(char * file_path, char *plain, unsigned int size);

// Lire/Ecrire un message chiffré dans un fichier
unsigned int read_cipher(char * file_path, char * cipher);
void write_cipher(char * file_path, char *cipher, unsigned int size);

#endif /* BLUM_H */
