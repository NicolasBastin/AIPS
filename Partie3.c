//Toutes fonctions non écrite dans cette partie sera la même fonction que la partie 1 et 2
//sauf pour close, mais on ne nous demande pas de le faire, on nous demande de faire la phase d’établissement de connexion

//init PE à 0

int mic_tcp_connect (int socket, mic_tcp_sock_addr addr) {
	if (socket != my_socket.fd) return -1;
	if (my_socket.state != IDLE) return -1;

unsigned long timer = 1000;

	mic_tcp_pdu syn_pdu;
	syn_pdu.header.source_port = my_socket.addr.port;
syn_pdu.header.dest_port = addr.port
//syn_pdu.header.seq_num = my_socket.seq_num;
syn_pdu.header.ack = 0;
syn_pdu.header.syn=1;
syn_pdu.header.fin=0;
syn_pdu.payload.data = NULL;
syn_pdu.payload.size=0;


	int attempts = 0;
	int connexion =0; //bool si syn_ack reçu
	int MAX_ATTEMPTS = 5;
	mic_tcp_pdu syn_ack_pdu;
	mic_tcp_sock_addr client_addr;

	while (attempts < MAX_ATTEMPTS){
if (IP_send(syn_pdu, my_socket.addr)==-1) return -1;
int recv_size = IP_recv(&syn_ack_pdu, &client_addr, timer);
if (syn_ack_pdu.header.syn != 1 || syn_ack_pdu.header.ack != 1){
	return -1 //pas le syn_ack
}
if (recv_size !=-1){ //pdu_syn_ack reçu donc arrêter connexion
connexion = 1;
break;
}
else {
attempts = attempts +1;}
}
if (connexion==0) return -1; //on a pas reçu 
mic_tcp_pdu ack_pdu;
ack_pdu.header.source_port = my_socket.addr.port;
ack_pdu.header.dest_port = addr.port
//ack_pdu.header.seq_num = my_socket.seq_num+1;
ack_pdu.header.ack = 1;
ack_pdu.header.syn=0;
ack_pdu.header.fin=0;
ack_pdu.payload.data = NULL;
ack_pdu.payload.size=0;

if (IP_send(ack_pdu , addr) ==-1) return -1;
	my_socket.state= ESTABLISHED;
	return 0;
}



int mic_tcp_accept(int socket, mic_tcp_sock_addr* addr){
mic_tcp_pdu sync_pdu;
mic_tcp_pdu syn_ack_pdu;
mic_tcp_pdu ack_pdu;
unsigned long timer = 1000;
int nb_relance = 5;
int relance = 0;
int connexion = 0;
my_socket.state = IDLE;
while(1){
	int recv_size = IP_recv(&syn_pdu, addr, -1);
	if (recv_size > 0 && syn_pdu.header.syn ==1){
		my_socket.addr = *addr;
		my_socket.state = SYN_RCVD;
		break;
}
}
syn_ack_pdu.header.syn = 1;
syn_ack_pdu.header.ack = 1;
syn_ack_pdu.header.fin = 0;
syn_ack_pdu.payload.size = 0

while(relance < nb_relance && !connexion{
	if (IP_send(syn_ack_pdu, my_socket.addr)==-1) return -1;
int recv_size = IP_recv(&ack_pdu, &my_socket.addr, timer);
if (recv_size == 0 && ack_pdu.header.ack == 1 && ack_pdu.header.syn == 0) {
	connexion = 1;
	break;
}
else{
	relance ++;
}
}
if (!connexion){
my_socket.state = IDLE;
return -1;
}
my socket.state = ESTABLISHED;
return 0;
}
