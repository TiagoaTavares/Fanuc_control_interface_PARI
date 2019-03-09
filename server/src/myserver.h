/**
 *       @file  myserver.h
 *      @brief  Header files
 *
 * Detailed description starts here.
 *
 *     @author  Tiago Tavares 
 *
 * =====================================================================================
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#ifdef _MAIN_C_
	int infinite_loopG=1;
	double j2;
	double jj1;
	double j3;
	double j4;
	double j5;
	double j6;
	double c1;
	double c2;
	double c3;
	double c4;
	double c5;
	double c6;
#else
	extern int infinite_loopG;
	extern double jj1;
	extern double j2;
	extern double j3;
	extern double j4;
	extern double j5;
	extern double j6;
	extern double c1;
	extern double c2;
	extern double c3;
	extern double c4;
	extern double c5;
	extern double c6;
#endif

typedef struct { int hora, minutos;} tempo;

#include "prototypes.h"

