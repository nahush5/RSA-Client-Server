/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
   gcc server2.c -lsocket
*/
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include"generateVals.h"
#include"file.h"

int d, phi;

int decrypt(int ciphertext)
{
  int finaltext = power(ciphertext, d, n);
  return finaltext;
}

char* decode(char* val) 
{
   int i = 0, integer = 0, j = 0;
   char* str;

   str = (char*) malloc( 5000 * sizeof(char) ); // assuming sender won't send text larger than 5000 characters
 
   while (val[i] != '\0')
   {
      if(val[i] != ' ')
      {
        integer = integer * 10 + (int) val[i] - 48;
      }
      else
      {
        // printf("hi: %d\n", integer);
        integer = decrypt(integer);
        str[j++] = (char) integer;
        integer = 0;
      }

      i++;
   }
   str[j] ='\0';
   return (str);
}

void dostuff(int); /* function prototype */

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void generateParametersForRsa()
{
    generateVals();

    FILE* f;

    f = fopen(PRIVATE, "r");

    char str1[10], str2[10];
    
    fscanf(f, "%s %s", str1, str2); 
    
    d = atoi(str1);
    phi = atoi(str2);

    fclose(f);
}

int main(int argc, char *argv[])
{
     generateParametersForRsa();
     
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
      
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");

   printf("Here is the message received: %s\n", buffer);

   char* plaintext;

   plaintext = decode(buffer);

   printf("FinalText after decryption: %s\n\n", plaintext);

   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}
