#include <time.h>

#include "Blum.h"

/*
	Trouve p et q deux nombres premiers aléatoires et
	calcule n = p * q.
	p et q sont de l'ordre 2^1024.
	p et q sont congrus a 3 modulo 4.
	La fonction fermat avec k = 16 est utilisee.
*/
void genKey(mpz_t p, mpz_t q, mpz_t n)
{
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, time(NULL));

	mpz_t ordre_de_grandeur;
	mpz_init(ordre_de_grandeur);

	mpz_ui_pow_ui(ordre_de_grandeur, 2, 1024); // ordre_de_grandeur = 2^1024

	GenPremier(p, ordre_de_grandeur, 16, state);
	GenPremier(q, ordre_de_grandeur, 16, state);

	mpz_mul(n, p, q); // n = p * q

	mpz_clear(ordre_de_grandeur);
	gmp_randclear(state);
}

/*
	ecrit la clef publique dans un fichier donne en argument
	ecrit la clef privee dans un fichier donne en argument
*/
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "format : ./genkey  clef_publique.txt  clef_privee.txt\n");
		exit(EXIT_FAILURE);
	}

	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);

	genKey(p, q, n);

	gmp_printf("clef publique :\nn = %Zd\n", n);
	gmp_printf("clef privee :\np = %Zd\nq = %Zd\n", p, q);

	write_pub(argv[1], n);
	write_prvt(argv[2], p, q);

	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	
	return 0;
}
