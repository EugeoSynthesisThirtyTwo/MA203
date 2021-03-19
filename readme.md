Pour compiler le projet, entrez la commande suivante:<br>
```sh
make all
```
<br>
Pour génerer une clef publique et une clef privée, entrez la commande suivante:<br>
```sh
./genkey clef_publique.txt clef_privee.txt
```
Les fichiers clef_publique.txt clef_privee.txt sont ainsi créés.<br>
clef_publique.txt contient la clef publique.<br>
clef_privee.txt contient la clef privée.<br>
<br>
Le fichier entree.txt contient le texte qui va être chiffré.<br>
Pour chiffrer le fichier entree.txt, entrez la commande suivante:<br>
```sh
./encrypt entree.txt clef_publique.txt msg_chiffre.txt x_t_plus_1.txt
```
Les fichiers msg_chiffre.txt et x_t_plus_1.txt sont ainsi créés.<br>
msg_chiffre.txt contient le message chiffré.<br>
x_t_plus_1.txt contient le dernier élement généré de la suite des xi.<br>
<br>
Pour déchiffrer le fichier msg_chiffre.txt, entrez la commande suivante:<br>
```sh
./decrypt msg_chiffre.txt x_t_plus_1.txt clef_privee.txt sortie.txt
```
Le fichier sortie.txt est ainsi créé.<br>
sortie.txt contient le texte déchiffré.<br>