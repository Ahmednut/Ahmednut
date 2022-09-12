Pour compiler cce dans la machine linux petalinux:

1- Ouvrir une console terminale

2- Changer au repertoire courant suivant:
petalinux-v2.2-final-full

3- Faire
. settings.sh<ENTER>

4- Changer au repertoire:
petalinux-v2.2-final-full/software/user-apps/cce

5- Copier les fichiers "checkout.sh" et "login.sh", "logout.sh", "commit.sh" (provenant de cvs "projets\bas\sdk\embedded\cce\prj"
dans le repertoire /cce de l'etape 4 s'il ne sont pas presents. Ensuite leur donner les droits d'execution en utilisant le file browser ubuntu properties sur les fichiers)

6- Effectuer cvs login (cette etape ne doit etre faite qu'une seule fois), et repondre par mot de passe associe a 'username' cvs
. ./login.sh username<ENTER>

7- Recuperer la branche cce cvs en specifiant le tag desire (ne pas specifier de tag pour la head)
./checkout.sh<ENTER>

8- Creer un link sur le fichier makefile provenant de cvs "projets\BAS\sdk\embedded\cce\prj" dans la machine petalinux vers /cce
(a faire une seule fois)

9- Pour compiler cce, faire (dans le repertoire cce):
make -B

10- Pour toute correction, modifier les fichiers sur le poste host et refaire les etapes 7,9

11- Si par megarde un fichier a ete modifie dans la machine virtuelle, il est possible de faire la commande suivante pour faire un commit dans cvs des fichiers modifies:
./commit.sh<ENTER>

Attention toutefois que ce mecanisme n'est pas tres user friendly et TOUS LES FICHIERS MODIFIES SERONT COMMIT VERS CVS!
A chaque fichier modifie, l'editeur nano sera ouvert avec un fichier pour ecrire le log des modifications dans cvs pour ce fichier.

Favoriser pour le moment l'utilisation de l'etape 10 au lieu de 11.

12- Une fois que tout est complete, relacher login cvs
. ./logout.sh<ENTER>

Eventuellement, la machine petalinux pourra etre mise a jour avec de meilleurs outils pour favoriser cette operation de facon plus user-friendly

Martin Theriault








 