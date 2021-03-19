#include <time.h>

#include "Blum.h"

char* encrypt(char* bits, int size, const mpz_t pub, int* output_size, mpz_t last_x)
{
	// allocation des ressources

	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, time(NULL));

	mpz_t x;
	mpz_t n_moins_1;
	mpz_t tmp;
	mpz_init(x);
	mpz_init(n_moins_1);
	mpz_init(tmp);

	// generation de s = x(-1)

	mpz_sub_ui(n_moins_1, pub, 1);
	mpz_urandomm(tmp, state, n_moins_1);
	mpz_add_ui(x, tmp, 1);

	// generation de x0 = x(-1)² mod n
	// on se rend compte que x0 ne sert a rien
	// il aurais pu etre egal a 's' directement

	BBS_step(pub, x);

	/*
		- calcul de x(i) = x(i-1)² mod n
		- calcul de z(i)
		- calcul de c(i)
	*/

	*output_size = size;
	char* c = malloc(*output_size * sizeof(char));

	if (c == NULL)
	{
		fprintf(stderr, "La RAM est pleine.\n");
		exit(EXIT_FAILURE);
	}

	char z;

	for (int i = 0; i < size; i++)
	{
		z = BBS_step(pub, x);
		c[i] = xorBool(bits[i], z);
	}

	// calcul et sauvegarde de x(t+1)

	BBS_step(pub, x);
	mpz_set(last_x, x);

	// liberation des ressources

	mpz_clear(x);
	mpz_clear(n_moins_1);
	mpz_clear(tmp);
	gmp_randclear(state);

	return c;
}

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		fprintf(stderr, "format : ./encrypt  entree.txt  clef_publique.txt  msg_chiffre.txt  x_t_plus_1.txt\n");
		exit(EXIT_FAILURE);
	}

	// lire un fichier clair dont le chemin est donné en argument (utiliser la fonction read_plain).

	int input_size = sizeOfFile(argv[1]);
	char* input_plain = malloc(sizeof(char) * 8 * input_size);

	if (input_plain == NULL)
	{
		fprintf(stderr, "La RAM est pleine.\n");
		exit(EXIT_FAILURE);
	}

	input_size = read_plain(argv[1], input_plain);

	// lire une clé publique dont le chemin est donné en argument (utiliser la fonction read_pub).

	mpz_t pub;
	mpz_t last_x;
	mpz_init(pub);
	mpz_init(last_x);

	read_pub(pub, argv[2]);

	// chiffrer le message avec l'algorithme de Blum-Goldwasser.

	int output_size;
	char* output = encrypt(input_plain, input_size, pub, &output_size, last_x);

	// écrire le texte chiffré dans un fichier dont le chemin est donné en argument (utiliser la fonction write_cipher)

	write_cipher(argv[3], output, output_size);

	// écrire x(t+1) dans un fichier a part donné en argument (utiliser la fonction write_pub)

	write_pub(argv[4], last_x);

	// libération des ressources

	mpz_clear(pub);
	mpz_clear(last_x);
	free(input_plain);
	free(output);

	return 0;
}
