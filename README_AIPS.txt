Bastin Nicolas
Bedour Clément

Compiler le .c
gcc tsock_v4.c -o tsock_v4 

executer avec UDP puits :
./tsock_v3 -u -r -n 15 -l 5 9000

executer avec UDP source :
./tsock_v3 -u -e -n 15 -l 50 dumas 9000

executer avec TCP puits :
./tsock_v3 -r -n 15 -l 5 9000

executer avec TCP source :
./tsock_v3 -e -n 15 -l 50 localhost 9000

Nous avons toujours UDP de fonctionnel.
Nous avons tuojours TCP fonctionnel.
Nous avons fait la version 4, qui ajoute la création de nouveau processus pour le serveur ce qui lui permet d'être à l'écoute de nouuvelle connexion constamment.
Nous avons aussi ajouter les options -r -e

Nous avons mis le format des messages voulu.
