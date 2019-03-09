/**
 *      @file  server.c
 *      @brief  Simple TCP/IP server that deals with multiple clients.
 * Simulate an robocomm server.
 *
 *	Based on http://www.linuxhowtos.org/C_C++/socket.htm
 *	A simple server in the internet domain using TCP
 *	The port number is passed as an argument 
 *	This version runs forever, forking off a separate 
 *	process for each connection
 *
 *     @author  Tiago Tavares 77001
 *     @date   Jan 2019
 *
 * =====================================================================================
 */

#define _MAIN_C_

#include "myserver.h"

int main(int argc, char *argv[])
{
	int sockfd, clientsockfd, portno, pid;
	int ret;
	char clntName[INET_ADDRSTRLEN];	                   // String to contain client name
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	

	//To avoid defuncts due to the fork() below. 
	//It is actually the default action and occurs when Child stopped or terminated
	signal(SIGCHLD, SIG_IGN);

	//To catch CTRL_C and allow the closure of port
	signal(SIGINT, ManageCTRL_C);

	if(argc < 2)
	{
		fprintf(stderr, "Syntax: %s <port_num>\n", argv[0]);
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//create a INET socket to communicate
	if(sockfd < 0) myerror("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));	//fill with zeros
	portno                    = atoi(argv[1]);	//get port number from command line
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port        = htons(portno);
	ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));	//bind the name to the socket .. o client faz um bcopy o servidor faz um bind, nao envia nada
	if(ret < 0) myerror("ERROR on binding");

	listen(sockfd, 20);           //start waiting for connections on the socket up to a maximum of 20 connections (20 alunos neste caso)
	clilen = sizeof(cli_addr);

	//make socket non blocking so other events may be processed inside the infinite loop
	//An equivalent action could also be done at the socket creation...
	int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);  

	
	jj1=11.349;
	j2=6.693;
	j3=.384;
	j4=-.000;
	j5=-90.385;
	j6=-12.349;
	c1=90.002;
	c2=20.002;
	c3=340.002;
	c4=40.001;
	c5=50.000;
	c6=60.000;


	while(infinite_loopG)
	{
		clientsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);	//accept the connection
		if(clientsockfd < 0)  //some error occured.
		{
			if (errno == EAGAIN) continue;  //simply absence of data on nonblocking socket. Not really an error. 
			myerror("ERROR on accept");
		}

		/*notify that you know the client: this is optional */
		if(inet_ntop (AF_INET, &cli_addr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
			printf("Client %s connected/%d\n", clntName, ntohs(cli_addr.sin_port));
		else
			printf("Unable to get client address\n");

		pid = fork();	                         //create a process child to manage the connection
		if(pid < 0) myerror("ERROR on fork");	 //rare but may occur!
		if(pid == 0)	                         //It is the child... proceed accordingly
		{
			close(sockfd);	 //child no longer needs the parent socket. Close it, after all it's simply a handle
					//Cf. http://stackoverflow.com/questions/14427898/socket-from-parent-to-child
			processclient(clientsockfd, clntName);	 //Process client request (a new process)
			exit(0);	                 //Exits when finishing processing the client request
		}
		else	       //It is the parent... does nothing: closes the client socket and resume accepting...
			close(clientsockfd);
	}/* end of while */

	close(sockfd);
	return 0;		/* we only get here when someone breaks the infinite loop */
}



/**
 * @brief  Function to process client request
 *
 * There will be a separate instance of this function 
 * for each connection established by clients. It handles 
 * all communications once a connection has been established.
 *
 * @param  sock The socket ID
 * @param  clientID The client ID (normally the IP)
 * @return  Nothing. It will return to terminate immediatly
 */
void processclient(int sock, char *clientID)
{
	const int BUFFSIZE=4097;		/*If this is small, enlarge it or use dynamic allocation */
	int n, numBytesSent, bufLen;
	char inbuffer[BUFFSIZE], outbuffer[BUFFSIZE], *str;
	char pos_reply[BUFFSIZE];
	int keepGoing=1;
	char string[40];

	while(keepGoing)
	{
		bzero(inbuffer, BUFFSIZE);	//empty buffer
		bzero(pos_reply, BUFFSIZE);	//empty buffer


		n = recv(sock, inbuffer, BUFFSIZE-1,0);
		if(n == BUFFSIZE-1)	//means read the entire buffer... there is the chance that more data may be waiting
		{
			printf("Buffer filled\n");
			//mysocketflush(sock);
			//in that case additional memory should be allocated
			//to read the remainder of the message and build it entirely to deal with it.
			//Not done in this example but may be necessary if large bunchs of data were sent from client.
		}
		if(n < 0) myerror("ERROR reading from socket");

		if(strlen(inbuffer) > 0)
			printf("Message received from the client %s: %s\n", clientID, inbuffer);

		if(!strncmp(inbuffer, "who", 3))	//answer who am I
		{
			strcpy(outbuffer, "RobCOMM");  //adapt this to your ID/name
		}
		else if(!strncmp(inbuffer, "time", 4))	// send your current time
		{
			str=vsTimeString();
			sprintf(outbuffer, "%s", str);
			free(str);
		}
		else if(!strncmp(inbuffer, "quit", 4))
		{
			sprintf(outbuffer, "Client %s, you have requested to close the connection!", clientID);
			keepGoing=0;
		}
		else if( (!strncmp(inbuffer, "SETDIO\n2 7 1", 12)) || (!strncmp(inbuffer, "SETDIO\n2 7 0", 12)) )
		{
			strcpy(outbuffer, "1\n");
		}
		else if( !strncmp(inbuffer, "MOVTOJPOS\n", 9) )
		{
			strcpy(outbuffer, "Moving...");
			send(sock, outbuffer, strlen(outbuffer), 0);
			//usleep(100);
			send(sock, outbuffer, strlen(outbuffer), 0);

			strncpy(pos_reply, inbuffer +10 , sizeof(inbuffer)-10);
			printf("pos_reply=%s\n",pos_reply);
			get_joint_positions(pos_reply);

			strcpy(outbuffer, "1\n");
		}
		else if( !strncmp(inbuffer, "MOVTOCINC\n", 9) )
		{
			strcpy(outbuffer, "Moving...");
			send(sock, outbuffer, strlen(outbuffer), 0);
			//usleep(100);
			send(sock, outbuffer, strlen(outbuffer), 0);

			strncpy(pos_reply, inbuffer +10 , sizeof(inbuffer)-10);
			printf("pos_reply=%s\n",pos_reply);
            get_cartesian_positions(pos_reply);

			//get_cartesian_positions(inbuffer);
			strcpy(outbuffer, "1\n");
		}
		else if( !strncmp(inbuffer, "GETCRJPOS", 9) )
		{
        	char *str_joint = Get_pos_joint_char();
			//printf("%s\n",str_joint);

			sprintf(outbuffer, "Current joint config\n%s\n", str_joint);
			send(sock, outbuffer, strlen(outbuffer), 0);
			printf("%s",outbuffer);
			/**
			strcpy(outbuffer, "Current joint config\n");
			send(sock, outbuffer, strlen(outbuffer), 0);
			usleep(50000);
			printf("%s",outbuffer);
			sprintf(string,"  %s\n",str_joint);
			strcpy(outbuffer,string);
			send(sock, outbuffer, strlen(outbuffer), 0);
			printf("%s",outbuffer);
			**/
			//usleep(50000);
			strcpy(outbuffer, "1\n");
			//strcpy(outbuffer, "Current position\n300 100 90 70 0 5");
		}
		else if( !strncmp(inbuffer, "GETCRCPOS", 9) )
		{
			char *str_cart1 = Get_pos_cart_char1();
			char *str_cart2 = Get_pos_cart_char2();
			sprintf(outbuffer, "Current position\n%s\n%s\n", str_cart1, str_cart2);
			send(sock, outbuffer, strlen(outbuffer), 0);
			printf("%s",outbuffer);

			strcpy(outbuffer, "1\n");
			
		}
		else if( !strncmp(inbuffer, "SETLSPEED",9) )
		{
			//usleep(500000);
			strcpy(outbuffer, "Setting motion speed...\n1\n");
		}
		else if( !strncmp(inbuffer, "LISTTPP\n",8) )
		{

			strcpy(outbuffer, "Program List\n");
			send(sock, outbuffer, strlen(outbuffer), 0);

			//usleep(500);

			strcpy(outbuffer, "tp1.tp\n1\ntp2.tp\n1\ntp3.tp\n1\ntp44.tp\n1\ntp55.tp\n1\ntp66.tp\n1\n");
		
		}
		else if( !strncmp(inbuffer, "MOTIONSTOP\n",11) )
		{

			strcpy(outbuffer, "Stopping all motion...\n");
			send(sock, outbuffer, strlen(outbuffer), 0);

			strcpy(outbuffer, "1\n");
		
		}

		//Here you can add other elseifs
		else
		{
			sprintf(outbuffer, "Client %s, your message is not recognized!", clientID);
		}

		bufLen = strlen(outbuffer);

		numBytesSent = send(sock, outbuffer, strlen(outbuffer), 0);
		printf("%s",outbuffer);
		if(numBytesSent < 0) myerror("send()/write() failed");
	}
	printf("\nConnection to client %s terminated.\n", clientID);
}
