#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <bits/stdc++.h>

#define MSG_MAX_LEN 1024

using namespace std;

void *service_func(void *arg) {
    int client_sock = *(int*)arg;
    // char msg_buf[MSG_MAX_LEN];
    char rcv_buf[MSG_MAX_LEN];

    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int msg_len = recv(client_sock, rcv_buf, MSG_MAX_LEN, 0);
    if (msg_len <= 0) {
        //perror("recv");
        close(client_sock);
        cout << "recv error";
        pthread_exit(0);
    }    
    string message = rcv_buf;

    if (getpeername(client_sock, (struct sockaddr*) &client_addr, &addrlen) == -1) {
        //perror("getpeername");
        close(client_sock);
        cout << "getpeername error";
        pthread_exit(0);
    }
    string usr_ip = inet_ntoa(client_addr.sin_addr);

    cout << "Msg from client with IP " << usr_ip << ":   " << message << endl;
    char msg[] = "Message received!";
    send(client_sock, msg, sizeof(msg), 0); 

    close(client_sock);
    pthread_exit(0);
}


int main(int argc, char *argv[]) {
    int listener, client_sock;
    struct sockaddr_in addr;
    // char msg_buf[MSG_MAX_LEN];

    // vector<char> rcv_buf(MSG_MAX_LEN);    

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(listener, 2);

    while(1) {
        client_sock = accept(listener, NULL, NULL);
        if (client_sock < 0) {
            perror("accept");
            cout << "accept error";
            // exit(EXIT_FAILURE);
        }
        else {
            pthread_t tid;
            pthread_create(&tid, NULL, service_func, (void*) &client_sock);
            pthread_detach(tid);
        }        
    }

    close(listener);

    return 0;
}