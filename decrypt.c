#include "Blum.h"

char* decrypt(char* bits, int size, const mpz_t p, const mpz_t q, const mpz_t last_x, int* output_size)
{
	// allocation des ressources

	mpz_t n;
	mpz_t x;
	mpz_t tmp;
	mpz_t base;
	mpz_t gauche;
	mpz_t droite;
	mpz_t d1;
	mpz_t d2;
	mpz_t u;
	mpz_t v;
	mpz_t a;
	mpz_t b;

	mpz_init(n);
	mpz_init(x);
	mpz_init(tmp);
	mpz_init(base);
	mpz_init(gauche);
	mpz_init(droite);
	mpz_init(d1);
	mpz_init(d2);
	mpz_init(u);
	mpz_init(v);
	mpz_init(a);
	mpz_init(b);

	// calcul de n

	mpz_mul(n, p, q);

	// calcul de d1 et d2

	mpz_add_ui(tmp, p, 1);
	mpz_fdiv_q_ui(base, tmp, 4);
	mpz_sub_ui(tmp, p, 1);
	mpz_powm_ui(d1, base,size + 1, tmp);

	mpz_add_ui(tmp, q, 1);
	mpz_fdiv_q_ui(base, tmp, 4);
	mpz_sub_ui(tmp, q, 1);
	mpz_powm_ui(d2, base, size + 1, tmp);

	// calcul de u et v

	mpz_powm(u, last_x, d1, p);
	mpz_powm(v, last_x, d2, q);

	// calcul de a et b

	mpz_invert(a, p, q);
	mpz_invert(b, q, p);

	// calcul de x0

	mpz_mul(tmp, v, a);
	mpz_mul(gauche, tmp, p);

	mpz_mul(tmp, u, b);
	mpz_mul(droite, tmp, q);

	mpz_add(tmp, gauche, droite);
	mpz_fdiv_r(x, tmp, n);

	/*
		- calcul de x(i) = x(i-1)² mod n
		- calcul de z(i)
		- calcul de m(i)
	*/

	*output_size = size;
	char* msg = malloc(*output_size * sizeof(char));

	if (msg == NULL)
	{
		fprintf(stderr, "La RAM est pleine.\n");
		exit(EXIT_FAILURE);
	}

	char z;

	for (int i = 0; i < size; i++)
	{
		z = BBS_step(n, x);
		msg[i] = xorBool(bits[i], z);
	}

	// liberation des ressources

	mpz_clear(n);
	mpz_clear(x);
	mpz_clear(tmp);
	mpz_clear(base);
	mpz_clear(gauche);
	mpz_clear(droite);
	mpz_clear(d1);
	mpz_clear(d2);
	mpz_clear(u);
	mpz_clear(v);
	mpz_clear(a);
	mpz_clear(b);

	return msg;
}

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		fprintf(stderr, "format : ./encrypt  msg_chiffre.txt  x_t_plus_1.txt  clef_privee.txt  sortie.txt\n");
		exit(EXIT_FAILURE);
	}

	// lire un fichier chiffré dont le chemin est donné en argument (utiliser la fonction read_cipher).

	int input_size = sizeOfFile(argv[1]);
	char* c = malloc(input_size * sizeof(char));

	if (c == NULL)
	{
		fprintf(stderr, "La RAM est pleine.\n");
		exit(EXIT_FAILURE);
	}

	input_size = read_cipher(argv[1], c);

	// lire x(t+1) dans dans un fichier dont le chemin est donné en argument (utiliser la fonction read_pub)

	mpz_t x;
	mpz_t p;
	mpz_t q;
	mpz_init(x);
	mpz_init(p);
	mpz_init(q);

	read_pub(x, argv[2]);

	// lire la clé privée dont le chemin est donné en argument (utiliser la fonction read_prvt).

	read_prvt(p, q, argv[3]);

	// déchiffrer le message avec l'algorithme de Blum-Goldwasser.

	int output_size;
	char* output = decrypt(c, input_size, p, q, x, &output_size);

	// écrire le texte clair dans un fichier (utiliser la fonction write_plain).

	write_plain(argv[4], output, output_size);

	// liberation des ressources

	mpz_clear(x);
	mpz_clear(p);
	mpz_clear(q);
	free(output);
	free(c);

	exit(EXIT_SUCCESS);
}
