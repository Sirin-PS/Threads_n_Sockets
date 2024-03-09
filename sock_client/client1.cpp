#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <math.h>

#define MSG_MAX_LEN 1024

using namespace std;

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  
    int choice = 0;
    cout << "Client program works!" << endl;
    do {
        cout << "\tSend message: 1" << endl << "\tExit: 2" << endl;
        cin >> choice;
        switch(choice) {
            case 1:
            {                
                char msg_recv_buf[MSG_MAX_LEN];
                char msg_send_buf[MSG_MAX_LEN];
                //string msg_string;

                sock = socket(AF_INET, SOCK_STREAM, 0);
                if (sock < 0) {
                    perror("socket");
                    exit(EXIT_FAILURE);
                }
                
                if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                    perror("connect");
                    exit(EXIT_FAILURE);
                }

                cout << "\nEnter message for server:   ";
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.getline(msg_send_buf, MSG_MAX_LEN);

                send(sock, msg_send_buf, sizeof(msg_send_buf), 0);
                recv(sock, msg_recv_buf, MSG_MAX_LEN, 0);
                cout << "Msg from server:   " << msg_recv_buf << endl;

                close(sock);
                break;
            }
            case 2:
                cout << "\nBye!\n";
                break;
            default:
                cout << "\nUncorrect command. Try again!\n";
                break;
        }
    } while (choice != 2);

    //close(sock);
    return 0;
}