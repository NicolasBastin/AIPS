Bastin Nicolas
Bedour Clément

gcc tsock_v3.c -o tsock_v3

executer avec UDP puits :
./tsock_v3 -u -p -n 15 -l 5 9000

executer avec UDP source :
./tsock_v3 -u -s -n 15 -l 50 dumas 9000

executer avec TCP puits :
./tsock_v3 -p -n 15 -l 5 9000

executer avec TCP source :
./tsock_v3 -s -n 15 -l 50 localhost 9000

Nous avons toujours UDP de fonctionnel.
Nous avons maintenant TCP également fonctionnel.

Nous avons rendu le code plus "générique", ajout de l'option pour le nom de domaine et le numéro de port.
Nous avons mis le format des messages voulu.