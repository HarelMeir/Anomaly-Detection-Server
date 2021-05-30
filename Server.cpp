
#include "Server.h"



Server::Server(int port)throw(const char*) {
	flag = 1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd<0){
		throw "socket failed";
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if(bind(fd, (struct sockaddr*)&server, sizeof(server))<0){
		throw "bind failure";
	}

	if(listen(fd,2) < 0)
		throw "listen failure";
}

void Server::start(ClientHandler& ch) throw(const char*) {
{
			
		t = new thread([&ch,this](){
		socklen_t clientSize = sizeof(client);
		int aClient;
		int i = 0;
		while(flag == 1) {
			alarm(1);
			aClient = accept(fd, (struct sockaddr*)&client, &clientSize);
			alarm(0);
			if(aClient < 0)
				throw "accept failure";
			ch.handle(aClient);
			close(aClient);
		}
		close(fd);	
		});	

	}
	
}

void Server::stop(){
	flag = 0;
	t->join(); // do not delete this!

}

Server::~Server() {
}

