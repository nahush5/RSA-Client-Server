#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include"encryptWithRsa.h"

int digits(int n)
{
	int counter = 0;

	while( n != 0 )
	{
		counter++;
		n /= 10;
	}

	return counter;
}

char* encode(char* str, int e, int n) 
{
   int i = 0;

   char* final;
 
   final = (char*) malloc( strlen(str) * ( digits(n) + 1 ) );

   final[0] = '\0';
 
   while(str[i] != '\0')
   {
   	  int ch = (int)str[i], ch2;
      ch2 = encrypt(ch , e, n);
      
      char* buf;
      sprintf(buf, "%d ", ch2);

      strcat(final, buf);

      i++;
   }

   return final;
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int E, N;
    
    FILE* f;

    f = fopen("public_key.txt", "r"); // server has told everyone that the file pubic_key.txt that he will generate will have n and e in that same respective order with a space seperating them. 

    char str1[10], str2[10];
      
    fscanf(f, "%s %s", str1, str2); 
      
    N = atoi(str1);
    E = atoi(str2);

    fclose(f);

    printf("Public keys n = %d and e = %d\n", N, E);
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message : ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    printf("\n");

    char* ciphertext = encode(buffer, E, N);

   	// printf("FinalText after encryption: %s\n\n", ciphertext);

    n = write(sockfd,ciphertext,strlen(ciphertext));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}
