#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<ctype.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


float getCPU(){
    char str1[100], str2[100];
    char* first[10];
    char* second[10];
    int i = 0;

    FILE* fp = fopen("/proc/stat","r");
    fgets(str1,100,fp);
    fclose(fp);

    sleep(1);

    fp = fopen("/proc/stat", "r");
    fgets(str2,100,fp);
    fclose(fp);
    
    for(int i = 0; i < 95; i++){
        str1[i] = str1[i+5];
        str2[i] = str2[i+5];
    }

    char * token = strtok(str1, " ");
    i = 0;
    while( token != NULL ) {
      first[i] = token;
      token = strtok(NULL, " ");
      i++;
    }

    char * token2 = strtok(str2, " ");
    i = 0;
    while( token2 != NULL ) {
      second[i] = token2;
      token2 = strtok(NULL, " ");
      i++;
    }

    int PrevIdle = atoi(first[3]) + atoi(first[4]);
    int Idle = atoi(second[3]) + atoi(second[4]);

    int NonIdle = atoi(second[0]) + atoi(second[1]) + atoi(second[2]) + atoi(second[5]) + atoi(second[6]) + atoi(second[7]);
    int PrevNonIdle = atoi(first[0]) + atoi(first[1]) + atoi(first[2]) + atoi(first[5]) + atoi(first[6]) + atoi(first[7]);

    int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

    int totald = Total - PrevTotal;
    int idled = Idle - PrevIdle;


    float CPU_Perc = ((float)totald - (float)idled)/(float)totald;

    //printf("%.0f%%\n", CPU_Perc*100);              //test: top -bn2 | grep '%Cpu' | tail -1 | grep -P '(....|...) id,'|awk '{print "CPU Usage: " 100-$8 "%"}'^C
    return(CPU_Perc*100);
}

char* getNAME(){
    char *str = malloc(sizeof(char) * 100);
    FILE* fp = fopen("/proc/sys/kernel/hostname", "r");
    fgets(str, 100, fp);
    fclose(fp);
    return str;
}

char* CPUNAME(){
    char* str = malloc(sizeof(char) * 100);
    FILE* fp = popen("cat /proc/cpuinfo | grep 'model name' | head -n 1 | sed 's/^.*: //'", "r");
    fgets(str, 100, fp);
    fclose(fp);
    return str;
}

int CreateSocket(){
    int option = 1;
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
    return sock;
}

void establishConnection(int portno){
    int sock = CreateSocket();
    if(sock == -1){
        printf("Error while creating socket - connection not established\nPlease try again\n"); 
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(portno);
    address.sin_addr.s_addr = INADDR_ANY;
    int addrlen = sizeof(address);

    bind(sock, (struct sockaddr *)&address, sizeof(address));
    listen(sock, 1);

    char buffer[1024];
    int valread;
    char runtime[1024] = "Server is running\n";
    char ports[1024];

    sprintf(ports, "Listening to port: %d\n", portno);
    strcat(runtime, ports);
    puts(runtime);

    while(1){
        int sock2 = accept(sock, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        valread = read(sock2, buffer, 1024);

        if(valread == -1){
            printf("Error while reading socket\nPlease try again");
            exit(1);
            
        }

        char *writeIn = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
        char *badReq = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\n";
        char *hostnameFound = strstr(buffer, "hostname");
        char *CPUloadFound = strstr(buffer, "load");
        char *CPUnameFound = strstr(buffer, "cpu-name");

        if(hostnameFound){
            char* hostname = getNAME();
            char finalHostname[1024] = " ";
            char echoHost[1024] = "\n\nRequested hostname.";

            puts(echoHost);

            strcpy(finalHostname, writeIn);
            strcat(finalHostname, hostname);

            send(sock2, finalHostname, strlen(finalHostname), 0); 
            free(hostname);
            close(sock2);
        }
        else if(CPUloadFound){
            float load = getCPU();
            char lload[1024];
            char finalLoad[1024] = " ";
            char echoLoad[1024] = "\n\nRequested CPU load.";

            puts(echoLoad);

            sprintf(lload, "%.0f%%\n", load);
            strcpy(finalLoad, writeIn);
            strcat(finalLoad, lload);

            send(sock2, finalLoad, strlen(finalLoad), 0);
            close(sock2);
        }
        else if(CPUnameFound){
            char* CPUname = CPUNAME();
            char finalCPUname[1024] = " ";
            char echoCPU[1024] = "\n\nRequested CPU name.";

            puts(echoCPU);

            strcpy(finalCPUname, writeIn);
            strcat(finalCPUname, CPUname);

            send(sock2, finalCPUname, strlen(finalCPUname), 0);
            free(CPUname);
            close(sock2);
        }
        else{
            char echoFail[1024] = "\n\nBad request.";
            char finalFail[1024] = " ";
            char echoFailPrint[1024] = "Bad Request";

            puts(echoFail);

            strcpy(finalFail, badReq);
            strcat(finalFail, echoFailPrint);

            send(sock2, finalFail, strlen(finalFail), 0);
            close(sock2);
        }

    }

}

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Invalid request, you forgot to specify port!\nExample: ./hinfosvc 12345\n");
        exit(1);
    }

    int portno = atoi(argv[1]);

    if(portno == 0){
        printf("Invalid port number given!\n");
    }
    else{
        establishConnection(portno);
    }
}
 