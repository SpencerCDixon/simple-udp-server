/*
    Simple udp server
*/
#include <stdio.h>      // printf
#include <string.h>     // memset
#include <stdlib.h>     // exit(0);
#include <arpa/inet.h>  // inet_ntoa, ntohs
#include <sys/socket.h> // bind, recvfrom
#include <unistd.h>     // close();

/*
  As UDP always returns at most one UDP packet (even if multiple are in the
  socket buffer) and no UDP packet can be above 64 KB (an IP packet may at most
  be 64 KB, even when fragmented), using a 64 KB buffer is absolutely safe and
  guarantees, that you never lose any data during a recv on an UDP socket.

  1024 * 64 = 65536
*/
#define BUFLEN 65536 // max buffer length
#define PORT 8888    // hardcoded port

// die prints the error message on stderr and exits with a non-success code
void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    /*
       sockaddr_in is a utility for sockaddr that deals with internet (in) based addresses. The struct looks like:

       struct sockaddr_in {
           short int          sin_family;  // Address family
           unsigned short int sin_port;    // Port number
           struct in_addr     sin_addr;    // Internet address
           unsigned char      sin_zero[8]; // Same size as struct sockaddr
        }; 

        **Note**: that sin_zero (which is included to pad the structure to the length of a struct sockaddr) 
          should be set to all zeros with the function memset().
     */
    struct sockaddr_in si_me, si_other;

    int s, slen = sizeof(si_other) , recv_len;

    // buffer to hold packets. If incoming data is > BUFLEN then the bytes will
    // be dropped.
    char buf[BUFLEN];

    // create a UDP socket (SOCK_DGRAM)
    if ((s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        die("socket");
    }

    // zero out the structure entire structre.  Alternatively, we could of zero
    // out just si_me.sin_zero with: memset(&(my_addr.sin_zero), '\0', 8);
    memset((char *) &si_me, 0, sizeof(si_me));

    // Always use AF_INET.  AF_INET is only used by the kernel and doesn't need
    // to be in network byte order.
    si_me.sin_family = AF_INET;
    // Convert to proper byte form.
    // Remember: put your bytes in Network Byte Order before you put them on the network. Be portable!
    si_me.sin_port = htons(PORT); // setting to 0 would have system pick port.
    // Use my own port.
    // Technically this is 0 and doesn't need the call to htonl 
    // but this will improve portability and most compilers will avoid anyways.
    si_me.sin_addr.s_addr = htonl(INADDR_ANY); 

    // bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    // keep listening for data
    while(1)
    {
        // give user a heads up
        printf("Waiting for data...");
        fflush(stdout);

        // try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        // print details of the client/peer and the data received
        
        /*
         Some machines store their numbers internally in Network Byte Order (Big-Endian Byte Order),
         some don't.  htons => Host To Network Short. ntohs => Network To Host Short
        */
        printf(
          "Received packet from %s:%d\n", 
          // network to ascii => prints network binary to dot IP notation
          inet_ntoa(si_other.sin_addr), 
          // network to host short
          ntohs(si_other.sin_port)
        );
        printf("Data: %s\n" , buf);

        // now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }

    close(s);
    return 0;
}
