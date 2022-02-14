#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
//TODO reduce ammount of #includes

void getCPU(){
    char str1[100], str2[100];
    char* first[10];
    char* second[10];
    int i = 0, j = 0;

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

    printf("%.0f%%\n", CPU_Perc*100);              //test: top -bn2 | grep '%Cpu' | tail -1 | grep -P '(....|...) id,'|awk '{print "CPU Usage: " 100-$8 "%"}'^C
}

void getNAME(){
    char str[100];
    FILE* fp = fopen("/proc/sys/kernel/hostname", "r");
    fgets(str, 100, fp);
    fclose(fp);
    fputs(str, stdout);
}

void CPUNAME(){
    char str[100];
    system("cat /proc/cpuinfo | grep 'model name' | head -n 1 | sed 's/^.*: //'");
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
        printf("ERROR - connection not established");   //TODO
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(portno);
    address.sin_addr.s_addr = INADDR_ANY;
    int addrlen = sizeof(address);

    bind(sock, (struct sockaddr *)&address, sizeof(address));
    listen(sock, 1);

    char buffer[1024];
    while(1){
        accept(sock, (struct sockaddr *)&address, (socklen_t*)&addrlen);        
    }

    
}

int main(int argc, char *argv[])
{
    int portno = atoi(argv[1]);

    establishConnection(portno);
}
 