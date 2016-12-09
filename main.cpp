/*
 * main.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: root
 */

#include <vector>
#include <iostream>
#include <ctype.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <fstream>

#include <libserialport.h>
#include <rtklib.h>
#include "sbp.h"
#include "observations.h"
#include "port.h"
extern "C"
{
#include <libsbp/sbp.h>
#include <libsbp/observation.h>
#include <libswiftnav/track.h>
}


/* The string, which is send as agent in HTTP request */
#define AGENTSTRING "NTRIP NtripLinuxClient"

#define MAXDATASIZE 1000 /* max number of bytes we can get at once */

/* CVS revision and version */
static char revisionstr[] = "$Revision: 1.27 $";

struct Args
{
	const char *server;
    int         port;
    const char *user;
    const char *password;
    const char *data;
    char 	   *serial_port_name;
    int         gpsweek;
    double 	    seconds;
};

#define ARGOPT "-d:hp:r:s:u:t:g:f:"

static int getargs(int argc, char **argv, struct Args *args)
{
	int res = 1;
    int getoptr;
    int help = 0;
    char *t;

    args->server = "www.euref-ip.net";
    args->port = 2101;
    args->user = "";
    args->password = "";
    args->data = 0;
    args->serial_port_name = "";
    args->gpsweek = 0;
    args->seconds = 0.0;
    help = 0;

    do{
    	switch((getoptr = getopt(argc, argv, ARGOPT))){
    		case 's': args->server = optarg; break;
    		case 'u': args->user = optarg; break;
    		case 'p': args->password = optarg; break;
    		case 'd': args->data = optarg; break;
    		case 'h': help=1; break;
    		case 'g': args->gpsweek = std::stoi(optarg); break;
    		case 't': args->seconds = std::stod(optarg); break;
    		case 'f': args->serial_port_name = (char *)calloc(strlen(optarg) + 1, sizeof(char));
    		if (!args->serial_port_name) {
    			fprintf(stderr, "Cannot allocate memory!\n");
    			exit(EXIT_FAILURE);
    		}
    		strcpy(args->serial_port_name, optarg);
    		break;
    		case 'r':
    			args->port = strtoul(optarg, &t, 10);
    			if((t && *t) || args->port < 1 || args->port > 65535)
    				res = 0;
    			break;
    		case -1: break;
       	}
    }while(getoptr != -1 && res);
    if(!res || help){
    	fprintf(stderr,"Usage: -s server -u user ...\n"
    	" -d the requested data set\n"
    	" -s the server name or address (default www.euref-ip.net)\n"
   		" -p the login password\n"
    	" -r the server port number (default 2101)\n"
        " -u the user name\n"
	    " -f the serial port (default /dev/ttyUSB0)\n"
	    " -g the gps week\n"
	    " -t seconds of week. Check out https://www.labsat.co.uk/index.php/en/gps-time-calculator for a UTC Time to GPS Time converter\n");
    	exit(1);
    }
    return res;
}

static const char encodingTable [64] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};

/* does not overrun buffer, but breaks directly after an error */
/* returns the number of required bytes */
static int encode(char *buf, int size, const char *user, const char *pwd)
{
	unsigned char inbuf[3];
    char *out = buf;
    int i, sep = 0, fill = 0, bytes = 0;

    while(*user || *pwd){
    	i = 0;
    	while(i < 3 && *user) inbuf[i++] = *(user++);
    	if(i < 3 && !sep)    {inbuf[i++] = ':'; ++sep; }
    	while(i < 3 && *pwd)  inbuf[i++] = *(pwd++);
    	while(i < 3)         {inbuf[i++] = 0; ++fill; }
    	if(out-buf < size-1)
    		*(out++) = encodingTable[(inbuf [0] & 0xFC) >> 2];
    	if(out-buf < size-1)
    		*(out++) = encodingTable[((inbuf [0] & 0x03) << 4)
               | ((inbuf [1] & 0xF0) >> 4)];
    	if(out-buf < size-1){
    		if(fill == 2)
    			*(out++) = '=';
    		else
    			*(out++) = encodingTable[((inbuf [1] & 0x0F) << 2)
                 | ((inbuf [2] & 0xC0) >> 6)];
    	}
    	if(out-buf < size-1)
    	{
    		if(fill >= 1)
    			*(out++) = '=';
    		else
    			*(out++) = encodingTable[inbuf [2] & 0x3F];
    	}
    	bytes += 4;
    }
    if(out-buf < size)
    	*out = 0;
    return bytes;
}

int main(int argc, char **argv)
{
	std::ofstream fs;
    fs.open("CorsDataBult.csv", std::ofstream::out);
    fs << "test6\n";
    fs.close();
    navigation_measurement_t observations;
    gps_time_t t;
    struct Args args;
    setbuf(stdout, 0);
    setbuf(stdin, 0);
    setbuf(stderr, 0);
    if(getargs(argc, argv, &args))
    {
    	check_port_name(args.serial_port_name);
    	locate_port(args.serial_port_name);
    	open_port(args.serial_port_name);
    	setup_port();
    	sbp_setup();
    	int i, sockfd, numbytes;
    	char buf[MAXDATASIZE];
    	struct hostent *he;
    	struct sockaddr_in their_addr; /* connector's address information */

    	if(!(he=gethostbyname(args.server))){
    		fprintf(stderr, "Server name lookup failed for '%s'.\n", args.server);
    		exit(1);
    	}
    	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    		perror("socket");
    		exit(1);
    	}

    	their_addr.sin_family = AF_INET;    /* host byte order */
    	their_addr.sin_port = htons(args.port);  /* short, network byte order */
    	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    	memset(&(their_addr.sin_zero), '\0', 8);
    	if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
    		perror("connect");
    		exit(1);
    	}
    	if(!args.data)
    	{
    	   	i = snprintf(buf, MAXDATASIZE,
    		"GET / HTTP/1.0\r\n"
            "User-Agent: %s/%s\r\n"
			#ifdef UNUSED
    	   		"Accept: */*\r\n"
    	   		"Connection: close\r\n"
			#endif
    	   	"\r\n"
    	   	, AGENTSTRING, revisionstr);
    	}
    	else{
    		i=snprintf(buf, MAXDATASIZE-40, /* leave some space for login */
    		"GET /%s HTTP/1.0\r\n"
            "User-Agent: %s/%s\r\n"
            #ifdef UNUSED
    			"Accept: */*\r\n"
                "Connection: close\r\n"
            #endif
            "Authorization: Basic "
            , args.data, AGENTSTRING, revisionstr);

           if(i > MAXDATASIZE-40 || i < 0) /* second check for old glibc */{
        	   fprintf(stderr, "Requested data too long\n");
        	   exit(1);
           }
           i += encode(buf+i, MAXDATASIZE-i-4, args.user, args.password);
           if(i > MAXDATASIZE-4){
        	   fprintf(stderr, "Username and/or password too long\n");
        	   exit(1);
           }
           buf[i++] = '\r';
           buf[i++] = '\n';
           buf[i++] = '\r';
           buf[i++] = '\n';
    	}
    	if(send(sockfd, buf, (size_t)i, 0) != i){
    		perror("send");
    		exit(1);
    	}
    	if(args.data){
    		printf("data\n");
    		int k = 0;
    		int starttime = time(0);
    		int lastout = starttime;
    		int totalbytes = 0;
    		int nr_obs = 0;
    		rtcm_t rtcm;
    		init_rtcm(&rtcm);
    		rtcm.obs.nmax = 11;
    		for(int i = 0; i < rtcm.obs.nmax;i ++){
    			rtcm.obs.data[i].time = gpst2time(args.gpsweek, args.seconds);
    		}

    		s8 ret;
    		while((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) != -1){
    			printf("received\n");
    			send_message(rtcm.obs.n, rtcm.obs.nmax);
    			//send_observations(rtcm.obs.n, &observations, &t);
    			printf("sent\n");
//    			while (ret != 1){
//    				ret = sbp_process(&sbp_state, &piksi_port_read);
//    				printf("ret: %d\n", ret);
//    			}
    			ret = 0;
    			printf("read\n");
    			if(!k){
    				if(numbytes < 12 || strncmp("ICY 200 OK\r\n", buf, 12)){
    					fprintf(stderr, "Could not get the requested data: ");
    					for(k = 0; k < numbytes && buf[k] != '\n' && buf[k] != '\r'; ++k){
    						fprintf(stderr, "%c", isprint(buf[k]) ? buf[k] : '.');
    					}
    					fprintf(stderr, "\n");
    					exit(1);
    				}
    				++k;
    			}
    			else{
    				totalbytes += numbytes;
    				if(totalbytes < 0) /* overflow */ {
    					totalbytes = 0;
    					starttime = time(0);
    					lastout = starttime;
    				}
    				for(int i = 0; i < numbytes; i++)
    					input_rtcm3(&rtcm, buf[i]);
    				double gpstime = time2gpst(rtcm.obs.data[0].time, &args.gpsweek);
    				fill_payload(rtcm.obs.n, (u16)args.gpsweek, (u32)gpstime, rtcm);
    				//fill_payload(&observations, rtcm);
    				//t.tow = rtcm.obs.data[i].time.sec;
    				//t.wn = rtcm.obs.data[i].time.time;
    				//t.wn = 1926;
    				printf("filled\n");
    			}
    		}
    	}
    	else{
    		while((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) > 0){
    			fwrite(buf, (size_t)numbytes, 1, stdout);
    		}
    	}
    	close(sockfd);
    }
    return 0;
}

/* Program needs this to run. */
#ifdef __cplusplus
extern "C" {
#endif
int showmsg(char *fmt,...)
{
va_list args;
va_start(args, fmt);

while (*fmt != '\0') {
if (*fmt == 'd') {
int i = va_arg(args, int);
printf("%d\n", i);
} else if (*fmt == 'c') {
// note automatic conversion to integral type
int c = va_arg(args, int);
printf("%c\n", c);
} else if (*fmt == 'f') {
double d = va_arg(args, double);
printf("%f\n", d);
}
++fmt;
}

va_end(args);

return 0;
}

void settspan(gtime_t ts, gtime_t te)
{

}

void settime(gtime_t time)
{

}
#ifdef __cplusplus
}
#endif


