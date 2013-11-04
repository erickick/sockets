#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//----------------------------------------------------------------------
/*!
* @brief Finaliza processo por algum erro
* @param msg Mensagem para ser enviar no stderr
* @param socket Socket que deve ser fechado
*/
static void terminate ( const char * msg, int socket )
{
    if ( msg != (const char *) NULL )
        perror(msg);
    else
        perror("Erro fatal");
    close(socket);
    exit(EXIT_FAILURE);
}
//----------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int bytes;
    int server_socket;
    int client_socket;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int port = 8080;
    char buffer [1024] = {0};
    struct sockaddr_in server_address;

    puts("Iniciando Servidor...");

    //Configurar porta
    if( argc > 1 )
        port = (int) strtol(argv[1], NULL, 10);
    //IPv4 - TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( !server_socket )
	terminate("Erro ao criar socket", server_socket);
    //Configura servidor
    memset(&(server_address), 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    //reserva porta para servidor
    if ( bind(server_socket, (struct sockaddr *) &(server_address), sizeof(server_address)) == -1 )
        terminate("Error ao executar bind", server_socket);
    //dispara escuta de conexão
    if( listen(server_socket, 3) == -1 )
        terminate("Erro ao escutar porta", server_socket);
    //aguarda conexão do cliente
    client_socket = accept(server_socket, (struct sockaddr *) &(server_address), &(addrlen));
    if ( !client_socket )
        terminate("Error ao conectar o cliente", server_socket);
    printf("Cliente %s esta connectado\n", inet_ntoa(server_address.sin_addr));
    write(client_socket, "Bem-vindo a BBB server", 22);
    //troca mensagens
    do {
	bytes = recv(client_socket, buffer, sizeof(buffer), 0);
	if ( bytes ) {
            //busca por comando de led
	    if ( strstr(buffer, "led") == 0 ) {
                //@TODO - Pisca led
            }
	}	
    } while ( bytes );

    //finaliza sockets
    close(client_socket);
    close(server_socket);

    return EXIT_SUCCESS;
}
//-- EOF - server.c ------------------------------------------------------
