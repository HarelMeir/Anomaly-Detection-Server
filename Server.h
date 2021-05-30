/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CLI.h"
#include "commands.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class SocketIO: public DefaultIO{
	int clientID;
	public:
	SocketIO(int id){
		clientID = id;	
	}
	string read() {
		string clientInput="";
		char c=0;
		::read(clientID,&c,sizeof(char));

		while(c!='\n'){
			clientInput+=c;
			::read(clientID,&c,sizeof(char));
		}
	return clientInput;
	}
	void write(string text) {
			::write(clientID,text.c_str(),text.length());
			//::write(clientID,"\r",1);
	}

	void write(float f){
		std::string str = std::to_string(f);
		str.erase(str.find_last_not_of('0') + 1,std::string::npos);
		::write(clientID,str.c_str(), str.length());
	};
	void read(float* f){};
	~SocketIO(){
		::close(clientID);
	};
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
		DefaultIO* dio = new SocketIO(clientID);
		CLI cli(dio);
		cli.start();
		delete dio;
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	sockaddr_in server;
	sockaddr_in client;
	int flag;


	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch) throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
