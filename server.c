#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 4444
#define LISTEN_BACKLOG 10
#define MAX 128

int main() {
	//create socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		perror("socket init");
		return 1;
	}

	//bind()
	struct sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr)); // Clear structure 
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_port = htons(PORT);
	if (bind(socket_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
		perror("bind fail");
		return 1;
	}

	//listen
	if (listen(socket_fd, LISTEN_BACKLOG) == -1) {
		perror("listen error");
		return 1;
	}

	//accept
	struct sockaddr_in peer_addr;
	int new_fd;
	socklen_t sin_size = sizeof(peer_addr);
	new_fd = accept(socket_fd, (struct sockaddr *)&peer_addr, &sin_size);
	if (new_fd == -1) {
		perror("accept error");
		return 1;
	}

	char buff[MAX];
	int r = 0;
	if ((r = (recv(new_fd, buff, MAX, 0))) == -1) {
		perror("recv");
		return 1;
	}
	if (send(new_fd, buff, r, 0) == -1) {
		perror("send");
		return 1;
	}
	close(new_fd);
	
	//close()
	close(socket_fd);
	return 0;
}