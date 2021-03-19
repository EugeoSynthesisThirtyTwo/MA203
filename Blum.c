#include "Blum.h"

char xorBool(char a, char b)
{
	return (a || b) && (!a || !b);
}

int sizeOfFile(const char* file_path)
{

	FILE* file = fopen(file_path, "r");

	if (file == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	int file_size = ftell(file);
	fclose(file);

	return file_size;
}

int Fermat(const mpz_t n, unsigned int k, gmp_randstate_t state)
{
	mpz_t a;
	mpz_t a_moins_1;
	mpz_t power;
	mpz_t r;
	mpz_t n_moins_1;

	mpz_init(a);
	mpz_init(a_moins_1);
	mpz_init(power);
	mpz_init(r);
	mpz_init(n_moins_1);

	mpz_sub_ui(n_moins_1, n, 1); // n_moins_1 = n - 1

	int premier = 1;

	for (int i = 0; i < k; i++)
	{
		mpz_urandomm(a_moins_1, state, n_moins_1); // a_moins_1 = rand([0, n - 1[)
		mpz_add_ui(a, a_moins_1, 1); // a = a_moins_1 + 1
		mpz_powm(r, a, n_moins_1, n); // r = a^(n-1) mod n

		if (mpz_cmp_ui(r, 1)) // si r n'est pas egal a 1
		{
			premier = 0;
			break;
		}
	}

	mpz_clear(a);
	mpz_clear(a_moins_1);
	mpz_clear(power);
	mpz_clear(r);
	mpz_clear(n_moins_1);

	return premier;
}

void GenPremier(mpz_t res, const mpz_t n, unsigned int k, gmp_randstate_t state)
{
	mpz_t tmp;
	mpz_t n_over_8;
	mpz_init(tmp);
	mpz_init(n_over_8);

	mpz_fdiv_q_ui(n_over_8, n, 8); // n_over_8 = n/8
	
	do
	{
		mpz_urandomm(tmp, state, n_over_8); // res = rand([0, n/8)[)
		mpz_add(res, tmp, n_over_8); // res = rand([n/8, n/4[)
		mpz_mul_ui(tmp, res, 4); // res = 4 * rand([n/8, n/4[)
		mpz_add_ui(res, tmp, 3); // res = 3 + 4 * rand([n/8, n/4[)
	}
	while (!Fermat(res, k, state));

	mpz_clear(tmp);
	mpz_clear(n_over_8);
}

char BBS_step(const mpz_t n, mpz_t x)
{
	mpz_t tmp;
	mpz_t rest;
	mpz_init(tmp);
	mpz_init(rest);

	mpz_powm_ui(tmp, x, 2, n);
	mpz_set(x, tmp);

	char z = mpz_fdiv_r_ui(tmp, x, 2);

	mpz_clear(tmp);
	mpz_clear(rest);

	return z;
}

void read_pub(mpz_t pub, char * file_path)
{
	FILE *fd = fopen(file_path, "r");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	char str[2048];
	fgets(str, 2048, fd);
	mpz_set_str(pub, str, 10);
	fclose(fd);
}

void write_pub(char * file_path, mpz_t pub)
{
	FILE *fd = fopen(file_path, "w");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	gmp_fprintf(fd, "%Zd", pub);
	fclose(fd);
}


void read_prvt(mpz_t prvt1, mpz_t prvt2, char * file_path)
{
	FILE *fd = fopen(file_path, "r");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	char str[1024];
	fgets(str, 1024, fd);
	mpz_init_set_str(prvt1, str, 10);
	fgets(str, 1024, fd);
	mpz_init_set_str(prvt2, str, 10);
	fclose(fd);
}

void write_prvt(char * file_path, mpz_t prvt1, mpz_t prvt2)
{
	FILE *fd = fopen(file_path, "w");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	gmp_fprintf(fd, "%Zd\n%Zd", prvt1, prvt2);
	fclose(fd);
}

unsigned int read_plain(char * file_path, char * plain)
{
	FILE *fd = fopen(file_path, "r");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	char c; unsigned int i = 0, j;

	while ((c = fgetc(fd)) != EOF)
	{
		for (j = 0 ; j < 8 ; ++j)
		{
			plain[i++] = ((c & 0b10000000) ? 1 : 0);
			c <<= 1;
		}
	}

	fclose(fd);
	return i;
}

void write_plain(char * file_path, char * plain, unsigned int size)
{
	FILE *fd = fopen(file_path, "w");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	char c; unsigned int i = 0, j;

	for ( ; i < size ; )
	{
		c = 0;

		for (j = 0 ; j < 8 ; ++j)
		{
			c <<= 1;
			c ^= (plain[i++] ? 1 : 0);
		}

		putc(c, fd);
	}

	fclose(fd);
}

unsigned int read_cipher(char * file_path, char * cipher)
{
	FILE *fd = fopen(file_path, "r");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	char c; unsigned int i = 0;

	while ((c = fgetc(fd)) != '\n')
	{
		cipher[i++] = (c == '1' ? 1 : 0);
	}

	fclose(fd);
	printf("size = %d\n", i);
	return i;
}

void write_cipher(char * file_path, char * cipher, unsigned int size)
{
	FILE *fd = fopen(file_path, "w");

	if (fd == NULL)
	{
		fprintf(stderr, "Can't open %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	unsigned int i = 0;

	for ( ; i < size ; ++i)
	{
		putc((cipher[i] ? '1' : '0'), fd);
	}

	printf("\n");
	fputc('\n', fd);
	fclose(fd);
}
