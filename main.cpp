#include <iostream>
#include <string>
#include <fstream>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main(int argc, char const *argv[]){
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsData;
    struct sockaddr_in server;
    int server_len;
    int BUFF_SIZE = 37020;

    if(WSAStartup(MAKEWORD(2,2), &wsData) != 0){
        cout << "Could not initialize" << endl;
        return 1;
    }

    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(wsocket == INVALID_SOCKET){
        cout << "Could not create socket!" << endl;
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);
    server_len = sizeof(server);

    if(bind(wsocket, (sockaddr*)&server, server_len) == SOCKET_ERROR){
        cout << "Could not bind socket" << endl;
        return 1;
    }

    if(listen(wsocket, 20) != 0){
        cout << "Could not start listening" << endl;
        return 1;
    }

    cout << "Listening on 127.0.0.1:8080 \n";
    int bytesR = 0;
    while(true){
        new_wsocket = accept(wsocket, (sockaddr*)&server, &server_len);
        if(new_wsocket == INVALID_SOCKET){
            cout << "Could not accept socket \n";
            return 1;
        }

        char buff[BUFF_SIZE] = {0};
        bytesR = recv(new_wsocket, buff, BUFF_SIZE, 0);
        if(bytesR <0 ){
            cout << "Could not recv data! \n";
            return 1;
        }

        string response;
        string line;
        ifstream myfile(argv[1]);

        if (myfile.is_open()){
            while(getline(myfile,line)){
              response.append(line);
            }
        }
        myfile.close();

        line = "";
        myfile.open(argv[2]);
        if (myfile.is_open()){
            response+="<style>";
            while(getline(myfile,line)){
              response.append(line);
            }
            response+="</style>";
        }
        myfile.close();
        
        line = "";
        myfile.open(argv[3]);
        if (myfile.is_open()){
            response+="<script>";
            while(getline(myfile,line)){
              response.append(line);
            }
            response+="</script>";
        }
        myfile.close();

        string serverMsg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Lenght: ";
        serverMsg.append(to_string(line.size()));
        serverMsg.append("\n\n");
        serverMsg.append(response);

        int bytesS = 0;
        int totalBytesS = 0;
        while(totalBytesS < serverMsg.size()){
            bytesS = send(new_wsocket, serverMsg.c_str(), serverMsg.size(), 0);
            if(bytesS < 0){
                cout << "Could not send data!" << endl;
            }

            totalBytesS += bytesS;
        }

        closesocket(new_wsocket);
    }   
    closesocket(wsocket);
    WSACleanup();



    return 0;
}