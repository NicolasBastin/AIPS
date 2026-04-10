Mic-tcp-send(int mic_sock, char* mesg, int size){
	if (my_socket.state != CONNECTED) return -1
mic_tcp_pdu;
mic_tcp_sock_addr.addr;
pdu.header.dest_port = ? (donné par l’app lors de la phase de connexion mic_tcp_connect)
pdu.payload.data = mesg;
pdu.payload.data = size;
addr = (donnée par l’app lors du connect));
int sent.size = IP_send(mic_tcp_pdu, mic_tcp_sock_addr) //
if (sent.size) ==-1 return -1 //IP_send a échoué
	return sent.size;
}

	
int mic_tcp_socket(start_mode sm) {
int result = initialize_components(sm); /* car demandé dans l’énoncé*/
if (result == -1) return -1;
my_socket.fd = 0; // 0 au hasard, pas important
my_socket.state = IDLE; // État de base
return my_socket.fd;
}



int mic_tcp_bind(int socket, mic_tcp_sock_addr addr) {
if (socket != my_socket.fd) return -1;
my_socket.addr = addr; /* Enregist de l'adresse du socket */
return 0; // fonctionne
}






int mic_tcp_connect (int socket, mic_tcp_sock_addr addr) {
// Pas de handshake car Q1
// On stocke juste l'adresse distante pour savoir ou envoyer les PDU plus tard.



my_socket.addr = addr;
my_socket.state = CONNECTED; // On est directement connect car il n’y a pas de connexion
return 0;
}



int mic_tcp_accept(int socket, mic_tcp_sock_addr* addr) {
//Pas de handshake car Q1
my_socket.state = CONNECTED;
return 0; // Succès
}




int mic_tcp_close (int socket) {
my_socket.state = IDLE; // Retour à l'état de base
return 0;
}



void process_received_PDU(mic_tcp_pdu pdu, mic_tcp_sock_addr addr) {
//Pas de vérif du num de séquence car q1
//Pas d'envoi de PDU d'ACK car q1
//On insère directement les données utiles dans le buffer de l'application
app_buffer_put(pdu.payload);
}


int mic_tcp_recv (int socket, char* mesg, int max_mesg_size) {
	if (socket != my_socket.fd) return -1 //descripteur de socket
	if (my_socket.state != CONNECTED) return -1 //pas connecté

	mic_tcp_payload payload;
	payload.data = (char *)malloc(max_mesg_size);
	payload.size = max_mesg_size;

	int recv_size = app_buffer_get(&payload)
	if (recv_size ==-1) {   //problème de taille donc copie impossible
free(payload.data)
return -1}
	memcpy(mesg,payload.data,recv_size); //copie dans buffer applicatif
	free(payload.data);
	return recv_size;
}
