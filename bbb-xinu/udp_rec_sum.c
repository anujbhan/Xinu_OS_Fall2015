#include <stdio.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <string.h>

#include <stdlib.h>

 

// pow(n,m) => n^m
int power(int n, int m){
    int res = 1;
    while(m != 0){
        res *= n;
        m--;
    }
    return res;
}

int stringToInt(char *st, int numLen){
    //numLen = i - 1;
    int z = 0, num = 0;
    while(z <= numLen){
        num += (st[z] - '0') * power(10, numLen - z);
        z++;
    } 
    return num;
}

int sum(int start, int end){
    int sum = 0;
    while(start <= end){
        sum += start;
        start++;
    }
    return sum;
}

char* sumOfIp(char *ip, int len){
	char *b;
    int z;
    int sumInt;
    char *sum_arr;
	sum_arr = (char *)malloc(sizeof(char) * 80);
    int numLen;
    int startNum, endNum;
    int j = 0;
    int i =0;
    //len = strlen(ip);
    b = (char *)malloc(sizeof(char) * len);
    for(i = 0; i < len; i++){
        if(ip[i] == '_'){
            startNum = stringToInt(b, j - 1);
            //printf("start %d \n", startNum);
            j = 0;
        }else{
            b[j] = ip[i];
            j++;
        }
    }
    //printf("%s %d\n", b , j - 1);
    endNum = stringToInt(b, j - 1);
    //printf("end %d \n", endNum);
    sumInt = sum(startNum, endNum);
    //printf("%d \n",sumInt);
    sprintf(sum_arr,"%d", sumInt);
    printf("%s \n", sum_arr);
	return sum_arr;
}


int main(){

  int udpSocket, nBytes;

  char buffer[1024];

	char *responseBuffer;

  struct sockaddr_in serverAddr, clientAddr;

  struct sockaddr_storage serverStorage;

  socklen_t addr_size, client_addr_size;

  int i;

 

  /*Create UDP socket*/

  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

 

  /*Configure settings in address struct*/

  serverAddr.sin_family = AF_INET;

  serverAddr.sin_port = htons(7891);

  serverAddr.sin_addr.s_addr = inet_addr("192.168.1.100");

  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero); 

 

  /*Bind socket with address struct*/

  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

 

  /*Initialize size variable to be used later on*/

  addr_size = sizeof serverStorage;

 

  while(1){

    /* Try to receive any incoming UDP datagram. Address and port of

      requesting client will be stored on serverStorage variable */
	printf("waiting..\n");
    nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);

 

    /*Convert message received to uppercase*/
	printf("MEssage received:|\n");
	
	for(i=0;i<nBytes;i++){
		
      	buffer[i] = toupper(buffer[i]);
		printf("%c",buffer[i]);
	}
	printf("\n---Calculating Sum---\n");
	
	responseBuffer = sumOfIp(buffer, nBytes);
	
	printf("Sum: %s\n", responseBuffer);

    sendto(udpSocket,responseBuffer,strlen(responseBuffer),0,(struct sockaddr *)&serverStorage,addr_size);
printf("Message sent back\n");
  }

 

  return 0;

}
