#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <errno.h>
#include <pthread.h>

typedef struct {
	struct in_addr saddr_start;
	struct in_addr saddr_end;
	u_short  sport_start;
	int sport_num;
	struct in_addr daddr_start;
	struct in_addr daddr_end;
	u_short  dport_start;
	int dport_num;
	char c_flag;
	char info_flag;
} program_config_t;

typedef struct {
	struct in_addr saddr;
	u_short sport;
	struct in_addr daddr;
	u_short dport;
}sendinfo_t;

typedef struct {
  program_config_t *program_config;
  sendinfo_t *sendinfo;
} pthread_arg_t;

int analyze_opt(int argc, char** argv,program_config_t *program_config ){
	int c;
	int res;
	int option_err = 0;

	program_config->saddr_start.s_addr = 0;
	program_config->saddr_end.s_addr = 0;
	program_config->daddr_start.s_addr = 0;
	program_config->daddr_end.s_addr = 0;

	while (1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"saddr_start",     required_argument, NULL,  10 },
			{"saddr_end",     required_argument, NULL,  11 },
			{"sport_start",     required_argument, NULL,  12 },
			{"sport_num",     required_argument, NULL,  13 },
			{"daddr_start",     required_argument, NULL,  20 },
			{"daddr_end",     required_argument, NULL,  21 },
			{"dport_start",     required_argument, NULL,  22 },
			{"dport_num",     required_argument, NULL,  23 },
			{0,         0,                 0,  0 }
		};
		c = getopt_long(argc, argv, "ci",
			long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 10:
			/** Source Address start **/
			res = inet_pton(AF_INET,optarg,&program_config->saddr_start);
			if(res != 1){
				perror("inet_pton");
				printf("wrong optarg \n");
				option_err++;
			}
			break;
		case 11:
			res = inet_pton(AF_INET,optarg,&program_config->saddr_end);
			if(res != 1)
				option_err++;
			break;
		case 12:
			program_config->sport_start = atoi(optarg);
			if(program_config->sport_start < 10000 ){
				printf("sport_start >= 10000 \n");
				option_err++;
			}
			break;
		case 13:
			program_config->sport_num = atoi(optarg);
			if(program_config->sport_num < 1 ){
				printf("sport_num >= 1, set 1\n");
				program_config->sport_num = 1;
			}
			break;
		case 20:
			res = inet_pton(AF_INET,optarg,&program_config->daddr_start);
			if(res != 1)
				option_err++;
			break;
		case 21:
			res = inet_pton(AF_INET,optarg,&program_config->daddr_end);
			if(res != 1)
				option_err++;
			break;
		case 22:
			if(optarg == NULL){
				printf("dport_start is null \n");
				option_err++;
				break;
			}
			program_config->dport_start = atoi(optarg);
			if(program_config->dport_start < 10000 ){
				printf("dport_start >= 10000 \n");
				option_err++;
			}
			break;
		case 23:
			if(optarg == NULL){
				printf("dport_num is null \n");
				option_err++;
				break;
			}
			program_config->dport_num = atoi(optarg);
			if(program_config->dport_num < 1 ){
				printf("dport_num >= 1, set 1\n");
				program_config->dport_num = 1;
			}
			break;
		case 'c':
			program_config->c_flag = 1;
			break;
		case 'i':
			program_config->info_flag = 1;
			break;
		case '?':
			break;
		default:
			printf("Unknown option : %s \n",c);
		}
	}

	/* Value check */
	if( (program_config->sport_start + program_config->sport_num ) > 65535){
		printf("sport max_number is over 65536 \n");
		option_err++;
	}

	if( (program_config->dport_start + program_config->dport_num ) > 65535){
		printf("dport max_number is over 65536 \n");
		option_err++;
	}
	if( program_config->saddr_start.s_addr == 0 ){
		printf("no saddr_start \n");
		option_err++;
	}
	if( program_config->saddr_end.s_addr == 0 ){
		program_config->saddr_end.s_addr = program_config->saddr_start.s_addr;
	}
	if( program_config->daddr_end.s_addr == 0 ){
		program_config->daddr_end.s_addr = program_config->daddr_start.s_addr;
	}

	/* TODO: need ip range check */

	return option_err;

}

char* build_packet_info(sendinfo_t *sendinfo, program_config_t *program_config,char *packetinfo_str){

	char * res_c;
	int errflg = 0;
	// show send message
	char saddr_str[INET6_ADDRSTRLEN];
	char daddr_str[INET6_ADDRSTRLEN];

	res_c = (char * )inet_ntop(AF_INET,&sendinfo->saddr,saddr_str,INET6_ADDRSTRLEN);
	if(res_c == NULL){
		printf("source addr is unknown");
		errflg ++;
	}
	res_c = (char * )inet_ntop(AF_INET,&sendinfo->daddr,daddr_str,INET6_ADDRSTRLEN);
	if(res_c == NULL){
		printf("dest addr is unknown");
		errflg ++;
	}
	if(errflg == 0){
		snprintf(packetinfo_str,2048,"src %s:%d, dst %s:%d\n",saddr_str,sendinfo->sport,daddr_str,sendinfo->dport);
		packetinfo_str[2047]='\0';
	}else{
		return NULL;
	}

	return packetinfo_str;

}

int sendudppacket(sendinfo_t *sendinfo,program_config_t *program_config){
	int sd;
	struct sockaddr_in d_addr, s_addr;
	int res;
	char * res_c;

	if(program_config->info_flag == 1){
		char pktinfo[2048];
		res_c = build_packet_info(sendinfo,program_config,pktinfo);
		if(res_c != NULL){
			printf(pktinfo);
		}
	}

	//if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	/* source addrss info */
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(sendinfo->sport);
	s_addr.sin_addr.s_addr = sendinfo->saddr.s_addr;

	if(bind(sd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
		char pktinfo[2048];
		perror("bind");
		res_c = build_packet_info(sendinfo,program_config,pktinfo);
		if(res_c != NULL){
			printf("ERROR:");
			printf(res_c);
		}
		close(sd);
		return -1;
	}

	d_addr.sin_family = AF_INET;
	d_addr.sin_port = htons(sendinfo->dport);;
	d_addr.sin_addr.s_addr = sendinfo->daddr.s_addr;

	if(sendto(sd, "I am send process", 17, 0,
		(struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
		char pktinfo[2048];
		perror("sendto");
		res_c = build_packet_info(sendinfo,program_config,pktinfo);
		if(res_c != NULL){
			printf("ERROR:");
			printf(res_c);
		}
		close(sd);
		return -1;
	}

	close(sd);
	return 1;

}

void *thread_sendpacket(pthread_arg_t *param){
	sendudppacket(param->sendinfo,param->program_config);
	return NULL;
}

int main(int argc, char** argv){

	int res;
	int tmp_sip,tmp_dip,tmp_sport,tmp_dport;
	int sendcount = 0;
	pthread_t pthread;
	
	program_config_t program_config ={0,0,10000,1,0,0,10000,1,0};
	sendinfo_t saddrinfo;
	pthread_arg_t pthread_arg;
	
	res = analyze_opt(argc,argv,&program_config);
	if(res > 0 ){
		char *USAGE="udppacketgen [-i] --saddr_start <IP> [--saddr_end <IP>] [--sport_start <port>] [--sport_num <num>] --daddr_start <IP> [--daddr_end <IP>] [--dport_start <port>] [--dport_num <num>]\n"
		"               -i : show acccess message (slow)\n"
		"    --saddr_start : Start Address of source\n"
		"    --saddr_end   : End Address of source (default : same saddr_start)\n"
		"    --sport_start : Port number of source (default : 10000)\n"
		"    --sport_num   : Use Number of source Port (default : 1)\n"
		"    --daddr_start : Start Address of dource\n"
		"    --daddr_end   : End Address of dource (default : same daddr_start)\n"
		"    --dport_start : Port number of dource (default : 10000)\n"
		"    --dport_num   : Use Number of dource Port (default : 1)\n";
		printf("option error\n");
		printf(USAGE);
		exit(1);
	}
	// printf("DEBUG: start_saddr:%x, endsaddr:%x\n",ntohl(program_config.saddr_start.s_addr),ntohl(program_config.saddr_end.s_addr));
	for(tmp_sip = ntohl(program_config.saddr_start.s_addr); tmp_sip<=ntohl(program_config.saddr_end.s_addr); tmp_sip++ ){  // source ip loop
		// printf("DEBUG: tmp_sip:%x, endaddr:%x\n",tmp_sip,ntohl(program_config.saddr_end.s_addr));
		// printf("DEBUG: start_dip:%x, enddaddr:%x\n",ntohl(program_config.daddr_start.s_addr),ntohl(program_config.daddr_end.s_addr));
		for(tmp_dip = ntohl(program_config.daddr_start.s_addr); tmp_dip<=ntohl(program_config.daddr_end.s_addr); tmp_dip++){ // dest ip loop
			// printf("DEBUG: tmp_sport:%x, sport_num:%x\n",program_config.sport_start,program_config.sport_num);
			for(tmp_sport = (int)program_config.sport_start; (tmp_sport-(program_config.sport_start))<(int)program_config.sport_num; tmp_sport++){ // dest port loop
				// printf("DEBUG: tmp_dport:%x, dport_num:%x\n",program_config.dport_start,program_config.dport_num);
				for(tmp_dport = (int)program_config.dport_start; (tmp_dport-(program_config.dport_start))<(int)program_config.dport_num; tmp_dport++) { // dest port loop

					saddrinfo.saddr.s_addr = htonl(tmp_sip);
					saddrinfo.sport = tmp_sport;
					saddrinfo.daddr.s_addr = htonl(tmp_dip);
					saddrinfo.dport = tmp_dport;
					pthread_arg.program_config = &program_config;
					pthread_arg.sendinfo = &saddrinfo;
					res = pthread_create(&pthread,NULL,&thread_sendpacket,&pthread_arg);
					//res = sendudppacket(&saddrinfo,&program_config);
					if( res == 0){
						sendcount++;
					}
				}
			}
		}
	}

	printf("finished : send %d packets\n",sendcount);
	return 0;

}
