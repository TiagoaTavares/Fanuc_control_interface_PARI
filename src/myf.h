/**
 * @file myf.h
 * @author Tiago Tavares
 * @brief Alguma declaraçoes gerais
 * @version 0.1
 * @date 2018-10-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */


//based on http://www.binarytides.com/socket-programming-c-linux-tutorial/
#include <stdio.h>      //puts, printf, etc.
#include <string.h>     //strlen
#include <sys/socket.h> //socket, connect, send, recv, etc.
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>     //close
#include <stdlib.h>
#include <netdb.h>

#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/time.h>

#include <gtk/gtk.h>



#include <opencv2/core/core_c.h>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

#define MAXCHARS 4096 //just a large buffer for data chunks
#define MAXMSG 4086


struct datastruct
{
	float j1;	//juntas
        float j2;
        float j3;
        float j4;
        float j5;
        float j6;
        float c1; //cartesianas
        float c2;
        float c3;
        float c4;
        float c5;
        float c6;
        int gripper; // estado do gripper open-1/close-0
        int pedido; /* Pedido para o servidor*/
        char frame[10]; //o frame que queremos
        char pergunta[50]; // a msg do pai -> servidor
        char resposta[4086]; // resposta do serv-Pai
	int connection;	// info da connecção
        int pedido_connection; // pedido para o pai conectar
        int gotostart; //variavel para terminar o pai 
        int velocidade;
        int mode;
        int ty;
        int incremento;
        int resposta_done;
}; // Struct with shared memory data


#define vabs(x) (x>=0) ? (x) : -(x) 
#define min(a,b) ((a)<=(b) ? (a) : (b) )
#define max(a,b) ((a)>=(b) ? (a) : (b) )

//----
#ifdef _MAIN_C_
        GtkBuilder *builderG;

        int bin_limitG = 128;
        int stop_executionG=0;
        int count_childG=0;
        int pid_childG=0;


        struct datastruct *data;

        IplImage *dst_imageG=NULL , *src_imageG=NULL;
        CvCapture *captureG=NULL;

#else
        extern GtkBuilder *builderG;

        extern int bin_limitG;
        extern int stop_executionG;
        extern int count_childG;
        extern int pid_childG;

        extern struct datastruct *data;

        extern IplImage *dst_imageG , *src_imageG;
        extern CvCapture *captureG;

#endif

//----
//ex1a
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//...
#define SHM_SIZE 128 /**< 128 bytes for shared memory */
#define PARENTDELAY 3000000   /**<parent period in us*/
#define CHILDDELAY 5000000    /**<child delay in us*/

#include "myutils.h"
#include "prototypes.h" //sempre no fim
