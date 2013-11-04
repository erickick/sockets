#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
//----------------------------------------------------------------
/*!
* @brief Envia mensagem de erro e aborta o processo
* @param msg Mensagem exibida no strerr
* @param sock socket para fechar
*/
void terminate( const char * msg, int sock )
{
    if ( msg != (const char *) NULL )
        perror(msg);
    else
        perror("Erro fatal");
    
    if( sock )
        close(sock);
    
    exit(EXIT_FAILURE);
}
//---------------------------------------------------------------- 
int main( int argc, char * argv [] )
{
  int client_socket;
  int bytes;
  char buffer[1024] = {0};
  struct sockaddr_in serv_addr;
  int port = 8080;
 
  //Gera client socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if( client_socket == -1 )
    terminate("Erro ao criar socket", client_socket);
  //Configura porta
  if ( argc > 1 )
    port = strtol(argv[1], NULL, 10);
  //Configura servidor alvo
  memset(&(serv_addr), 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //Conecta ao servidor
  if( connect(client_socket, (struct sockaddr *)&(serv_addr), sizeof(serv_addr)) == -1 )
    terminate("Erro ao conectar o servidor" , client_socket);
  //Troca mensagem
  do {
    bytes = read(client_socket, buffer, sizeof(buffer));
    if ( bytes ) {
       printf("Recebido do servidor: %s\n", buffer);
       bytes = write(client_socket, "led", 3);
       memset(buffer, 0, bytes);
    } 
  } while( bytes );
  //Finaliza
  close(client_socket);
 
  return EXIT_SUCCESS;
}
