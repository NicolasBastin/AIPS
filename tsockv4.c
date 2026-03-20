#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void afficher_message(char *message, int lg)
{
	int i;
	for (i = 0; i < lg; i++)
	{
		printf("%c", message[i]);
	}
	printf("\n");
}

void construire_message(char *message, char motif, int lg, int num_msg)
{
	char prefix[6];
	int i;

	// Ajout du motif de depart de message
	snprintf(prefix, sizeof(prefix), "%5d", num_msg);
	for (i = 0; i < 5; i++)
	{
		if (prefix[i] == ' ')
		{
			prefix[i] = '-';
		}
		message[i] = prefix[i];
	}

	// Remplissage du reste du message avec le motif
	for (i = 5; i < lg; i++)
	{
		message[i] = motif;
	}
}

int main(int argc, char **argv)
{
	// Inititialisation des variables utilises du cote UDP / TCP /
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; // Nb de messages a  envoyer ou a  recevoir, par defaut : 10 en emission, infini en reception
	int source = -1;	 // 0 = puits, 1=source
	int l = 30;			 // longueur max emis ou recu de base a  30 octets
	int u = 0;			 // 0 = TCP, 1=UDP
	int sock, sock_bis;
	struct sockaddr_in adr_distant;
	struct sockaddr_in adr_local;
	socklen_t lg_adr_distant = sizeof(adr_distant);
	socklen_t lg_adr_local = sizeof(adr_local);
	int lg_M = 30;
	int lg_emis;
	struct hostent *IP;
	int lg_rec;
	int lu;		 // nbr message lu par UDP
	int lus = 0; // nbr message lu avec methode pere/fil
	// Variable par default si pas defini dans les parametres
	char *nom = "localhost";
	int port = 9000;

	// Recuperation des options rentrées en entrees de la fonction

	//-r recepteur des donnees
	//-e emetteur des donnees

	while ((c = getopt(argc, argv, "upren:sl:")) != -1)
	{
		switch (c)
		{
		case 'p': //puit
			if (source == 1)
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;
		
		case 'r': //recepteur = puit
			if (source == 1)
			{
				exit(1);
			}
			source = 0;
			break;
		
		
		case 'u':
			u = 1;
			break;
		case 's'://source
			if (source == 0)
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 1;
			break;

			case 'e'://emetteur = source
			if (source == 0)
			{
				exit(1);
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;
		case 'l':
			l = atoi(optarg);
			lg_M = l;
			break;
		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	char *M = malloc(sizeof(char) * lg_M);
	char *pmesg = malloc(sizeof(char) * lg_M); // Allocation dynamique pour s'adapter a  l'option -l

	// Recuperation du nom de domaine et du numero de port

	if (optind != argc)
	{
		if (atoi(argv[optind]) == 0)
		{
			// le premier argument non option n'est pas le port
			nom = argv[optind];
			if (argc > (optind + 1))
			// donc second argument non option
			{
				port = atoi(argv[optind + 1]);
			}
		}
		else
		{ // le premier argument non option est le port donc pas de nom
			port = atoi(argv[optind]);
		}
	}
	// Emplacement initial des fonctions permettant de verifier si on est en source / puit etc
	if (source == -1)
	{
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1);
	}

	if (nb_message == -1 && source == 1)
	{
		nb_message = 10;
	}

	//-------------SOURCE SOUS UDP------------------------------------------
	if (u == 1 && source == 1)
	{ // creation socket pour source
		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			printf("echec creation socket dans source \n");
			exit(1);
		}
		/* reset de la structure */
		memset((char *)&adr_distant, 0, sizeof(adr_distant));
		adr_distant.sin_family = AF_INET;
		adr_distant.sin_port = port;

		//Recuperation de l'IP correspondant au nom de domaine

		if ((IP = gethostbyname(nom)) == NULL)
		{
			printf("erreur gethostbyname\n");
			exit(1);
		}
		memcpy((char *)&(adr_distant.sin_addr.s_addr), IP->h_addr, IP->h_length);
		// Construction du message puis envoie d'icelui
		printf("SOURCE: lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=udp, dest=%s\n", lg_M, port, nb_message, nom);
		for (int i = 0; i < nb_message; i++)
		{
			construire_message(M, 'a' + (i % 26), lg_M, i + 1);
			lg_emis = sendto(sock, M, lg_M, 0, (struct sockaddr *)&adr_distant, lg_adr_distant);
			if (lg_emis ==-1){
				printf("Probleme sendto dans source UDP\n");
			}
			if (lg_emis ==0){
				printf("SOURCE: Envoi n°%d le message n'a pas été envoyé car la taille est de 0 \n",i+1);
			}
			else {
				printf("SOURCE: Envoi n°%d (%d) [%.*s]\n", i + 1, lg_M, lg_M, M);
			}
		}
		printf("SOURCE: fin\n");
	}

	//--------------SOURCE SOUS TCP------------------------------------------
	if (u == 0 && source == 1)
	{ // creation socket pour source
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			printf("echec creation socket dans source TCP\n");
			exit(1);
		}
		// init et reset de la structure
		memset((char *)&adr_distant, 0, sizeof(adr_distant));
		adr_distant.sin_family = AF_INET;
		adr_distant.sin_port = port;
		// recuperation de l'IP avec le nom de domaine
		if ((IP = gethostbyname(nom)) == NULL)
		{
			printf("erreur gethostbyname TCP\n");
			exit(1);
		}
		// copie la structure
		memcpy((char *)&(adr_distant.sin_addr.s_addr), IP->h_addr, IP->h_length);
		// tentative de connection pour TCP
		if (connect(sock, (struct sockaddr *)&adr_distant, lg_adr_distant) == -1)
		{
			printf("echec de la connexion dans source \n");
			exit(1);
		}

		printf("SOURCE: lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=tcp, dest=%s\n", lg_M, port, nb_message, nom);

		for (int i = 0; i < nb_message; i++)
		{ // construit msg et change le caractere
			construire_message(M, 'a' + (i % 26), lg_M, i + 1);
			// ou send(sock, M, lg_M, 0) mais pas sendto car pour UDP code de la V1 inutile ici
			lg_emis = write(sock, M, lg_M);
			if (lg_emis==-1){
				printf("Erreur write dans source TCP \n");
			}
			if (lg_emis==0){
				printf("SOURCE: Envoi n°%d n'a envoyé aucun message \n", i + 1);
			}
			printf("SOURCE: Envoi n°%d (%d) [%.*s]\n", i + 1, lg_M, lg_M, M);
		}
		printf("SOURCE: fin\n");

		if (shutdown(sock, 1) == -1) //fin de connexion car plus de msg à envoyé
		{
			printf("Probleme fin de connexion dans la source\n");
		}
		close(sock); //destruction du socket
	}

	//--------------PUITS SOUS UDP------------------------------------------
	if (source == 0 && u == 1)
	{
		if (nb_message == -1)
			printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=infini, TP=udp\n", lg_M, port);
		else
			printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=%d, TP=udp\n", lg_M, port, nb_message);
		// creation socket pour puits
		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			printf("echec creation socket dans puits \n");
			exit(1);
		}

		memset((char *)&adr_local, 0, sizeof(adr_local)); // init et reset la structure
		adr_local.sin_family = AF_INET;
		adr_local.sin_port = port;
		adr_local.sin_addr.s_addr = INADDR_ANY;
		// association adresse socket
		if (bind(sock, (struct sockaddr *)&adr_local, lg_adr_local) == -1)
		{
			printf("echec du bind\n");
			exit(1);
		}

		// mis en ecoute
		// listen(sock,20); Pas necessaire, seulement pour TCP
		// faisait partie de la V1 et pas necessaire

		// lecture infini
		int nb_m = 1;
		while (nb_message == -1 || nb_m <= nb_message)
		{
			lu = recvfrom(sock, pmesg, lg_M, 0, (struct sockaddr *)&adr_distant, &lg_adr_distant);
			if (lu ==-1){
				printf("Erreur dans le puit de udp a cause de recvfrom \n");
			}
			if (lu > 0)
			{
				printf("PUITS: Reception n°%d (%d) [%.*s]\n", nb_m, lu, lu, pmesg);
				nb_m++;
			}
		}
	}

	//--------------PUITS SOUS TCP (V4)------------------
	if (source == 0 && u == 0)
	{
		if (nb_message == -1)
			printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=infini, TP=tcp\n", lg_M, port);
		else
			printf("PUITS: lg_mesg-lu=%d, port=%d, nb_receptions=%d, TP=tcp\n", lg_M, port, nb_message);
		// creation socket pour puits
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			printf("echec creation socket dans puits \n");
			exit(1);
		}

		memset((char *)&adr_local, 0, sizeof(adr_local)); // init et reset la structure
		adr_local.sin_family = AF_INET;
		adr_local.sin_port = port;
		adr_local.sin_addr.s_addr = INADDR_ANY;

		// association adresse socket
		if (bind(sock, (struct sockaddr *)&adr_local, lg_adr_local) == -1)
		{
			printf("echec du bind\n");
			exit(1);
		}

		// mis en ecoute
		listen(sock, 5);

		// Mise en place de la creation de nouveaux processus si necessaire
		while (1)
		{ // acceptation de la connexion
			if ((sock_bis = accept(sock, (struct sockaddr *)&adr_distant, &lg_adr_distant)) == -1)
			{
				printf("echec du accept\n");
				continue; // On passe au client suivant en cas d'erreur
			}

			// Creation d'un processus fils pour gerer ce client specifiquement
			int pid;

			switch (pid = fork())
			{
			case -1:
				// ERREUR
				printf("echec du fork\n");
				shutdown(sock_bis, 0); //ferme la connexion
				close(sock_bis);//detruit le socket
				break;

			case 0:
				// PROCESSUS FILS : S'occupe de la lecture
				close(sock); // Le fils n'a pas besoin du sock d'ecoute

				lus = 0;
				while (nb_message == -1 || lus < nb_message)
				{
					if ((lg_rec = read(sock_bis, M, lg_M)) <= 0)
					{
						break;
					}
					printf("PUITS: Reception n°%d (%d) [%.*s]\n", lus + 1, lg_rec, lg_rec, M);
					lus++;
				}

				// Fermeture propre : on coupe la lecture, puis on detruit le socket
				shutdown(sock_bis, 0);
				close(sock_bis); // Le fils ferme son socket de dialogue une fois termine
				exit(0);

			default:
				// PROCESSUS PERE : Continue d'ecouter
				close(sock_bis); // Le pere n'a pas besoin du sock de dialogue
				break;
			}
		}
	}
	return 0;
}