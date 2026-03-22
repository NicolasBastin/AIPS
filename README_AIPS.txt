Bastin Nicolas
Bedour Clément

Compiler le .c
gcc tsockv4.c -o tsockv4 

executer avec UDP puits :
./tsockv4 -u -r -n 15 -l 5 9000

executer avec UDP source :
./tsockv4 -u -e -n 15 -l 50 localhost 9000

executer avec TCP puits :
./tsockv4 -r -n 15 -l 5 9000

executer avec TCP source :
./tsockv4 -e -n 15 -l 50 localhost 9000

Nous avons toujours UDP de fonctionnel.
Nous avons toujours TCP fonctionnel.
Nous avons mis le format des messages voulu.

Nous avons fait la version 4, qui ajoute la création de nouveau processus pour le serveur ce qui lui permet d'être à l'écoute de nouuvelle connexion constamment.
Nous avons aussi ajouter les options -r -e