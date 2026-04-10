//Toutes fonctions non écrite dans cette partie sera la même fonction que la partie 1
// Variable global pour le num de séquence à envoyer et attendu

int PE = 0;
int PA = 0;

Mic_tcp_send(int mic_sock, char* mesg, int size){
	if (my_socket.state != CONNECTED) return -1
// Création des variables utiles pour l’envoie
mic_tcp_pdu pdu;
mic_tcp_pdu ack_pdu;
int sent_size;
int ack_received = 0;
unsigned long timer = 100;

// Mise en place des variables utiles dans le PDU
pdu.header.seq_num = PE;
pdu.header.syn = 0;
pdu.header.ack = 0;
pdu.header.fin = 0;
pdu.payload.data = mesg;
pdu.payload.size = size;

while(!ack_received){
	sent_size = IP_send(pdu, my_socket.addr); // Envoie le pdu
	int recv_size = IP_recv(&ack_pdu, &my_socket.addr, timer); // Attends le ACK
	if (recv_size > 0 && ack_pdu.header.ack == 1 && pdu.header.seq_num = PE){
		ack_received = 1; // C’est bien un ack donc on valide ce que l’on vient d’envoyer
}
else{
// On fait rien puisque l’on a pas reçu le bon ack, on va refaire un tour de boucle
}
}
PE = (PE+1)%2;

return sent_size;
}








void process_received_PDU(mic_tcp_pdu pdu, mic_tcp_sock_addr addr) {
if (pdu.header.seq_num == PA ){
	app_buffer_put(pdu.payload);
	PA = (PA +1)%2;
	//création du ACK
	mic_tcp_pdu ack_pdu;
	ack_pdu.header.source_port = my_socket.addr.port;
ack_pdu.header.dest_port = addr.port
// Mise en place des variables utiles dans le ack_PDU
ack_pdu.header.seq_num = 0;
ack_pdu.header.syn = 0;
ack_pdu.header.ack = 1;
ack_pdu.header.ack_num = pdu.header.seq_num +1; //le prochain paquet que je veux c le +1 (je ne dis pas le reçu, mais le prochain)
ack_pdu.header.fin = 0;
ack_pdu.payload.data = NULL;
ack_pdu.payload.size = 0;
		if (IP_send(ack_pdu, addr) == -1 ) return -1;
return -1;
}
