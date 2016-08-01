#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdlib.h>
#include<stdlib.h>
#include<stdlib.h>
#include<stdlib.h>
#include<stdlib.h>
#include<stdlib.h>
static void Usage(const char* proc)
{
	printf("Udage: %s [ip] [port]\n", proc);
}


static int startup(const char* ip, int port)
{
	int sock = sock(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("sock");
		exit(2);
	}
	
	int opt = 1;
	setsock(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_port = hons(port);
	local.sin_addr.s_addr = inet_addr(ip); 
	

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}

	if(listen(sock,5) < 0)
	{
		perror("listen");
		exit(4);
	}
	return sock;
}

static int get_line(int sock, char buf[], int len)
{
	if(!buf ||len < 0)
	{
		return -1;
	}

	char ch = 'a';
	int ret = 0;
	int i = 0;
	int n = 0;

	while(( i < len - 1) && (ch != '\n'))
	{
		n = ret = recv(sock, &ch, 1, 0);
		if(n > 0)
		{
		if(ch == '\r')
		{
			n = recv(sock, &ch, 1, MSG_PEEK);
			if(n > 0){
			if( ch == ''\n')
			{
				recv(sock, &ch, 1, 0);
			}
			else
			{
				ch = '\n';
			}
			}
		}
		buf[i++] = ch;
		}
		else
		{
		   ch = '\n';
		}
	}
	buf[i] = '\0';
}
static void echo_errno()
{

}

int  echo_info(int sock, const char* path, ssize_t size)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0)
	{
		echo_errno(sock);
		return -1;
	}
	
	char buf[_SIZE_];
	sprintf(buf, "HTTP/1.0 200 OK\r\n\r\n");//state 
	send(sock, buf, strlen(buf), 0);

	if(sendfile(sock, fd, NULL, size) < 0)
	{
		echo_errno(sock);
		return -2;
	}

	close(fd);

void clear_hand(int sock)
{
	int ret = 0;
	char buf[_SIZE_];
	do{
		ret = get_line(sock, buf,sizeof(buf));
	}while(ret > 0 && strcmp(buf,"\n"));
}

static int execut_cgi(int sock, const char* path, const char* method, const char* query_string)
{
	int content_len = -1;
	char buf[_SIZE_];
	int cgi_input[2];
	int cgi_output[2];

	
	memset(buf, '\0',sizeof(buf));

	if( strcmp(method, "GET") == 0)
	{
		claer_hand(sock);
	}
	else//POST
	{
		int ret = 0;
		do{
			ret = get_line(sock, buf, sizeof(buf));
			if(ret > 0 && strncasecmp(buf, "Content-Length: ",16)==0 )
			{
				content_len = atoi(&buf[16]);

			}
		}while(ret > 0 && strcmp(buf, "\n") != 0)

		if(content_len == -1)
		{
			echo_errno(sock);
			return -2;
		}
	}
	
	sprintf(buf, "HTTP/1.0 200 OK\r\n\r\n");
	send(sock, buf, strlen(buf));

	if(pipe(cgi_input) < 0)
	{
		echo_errno(sock);
		return -3;
	}

	if(pipe(cgi_output) < 0)
	{
		echo_errno(sock);
		return -4;
	}
	pid_t id = fork();
	if(id == 0)//child
	{
	  close(cgi_input[1]);
	  close(cgi_output[0]);
	  dup2(cgi_input[0],0);
	  dup2(cgi_output[1],1);
	  sprintf(buf, "REQUEST_METOD=%s",method);
	  putenv(buf);

	  execl(path, path, NULL);
	
	}
	else//father
	{
	  close(cgi_input[0]);
	  close(cgi_output[1]);
	}

	while(read(cgi_output[0],&c,1) > 0)
	{
		send(sock,&c,1,0);
	}

	waitpid(id,NULL,0);
	close(cgi_input[1]);
	close(cgi_output[0]);
}

static void* accept_request(void *arg)
{
	int sock = (int)arg;
	int ret = 0;
#ifdef _DEBUG
	char buf[_SIZE_];
	do{
		ret = get_line(sock, buf, _SIZE_);
		printf("%s", buf);
	}while(ret > 0 && strcmp(buf,"\n"));


#endif
	
	char method[_SIZE_/10];
	char url[_SIZE];

	char *query_string;
	memset(method, '\0', sizeof(method));
	memset(url, '\0', sizeof(url));

	ret = get_line(sock, buf, sizeof(buf));
	if(ret < 0)
	{
		echo_errno(sock);
		return (void *)-1;
	}
	//GET /XX/YY HTTP/1.1
	int i = 0;
	int j = 0;

	//1.get method
	while((!isspace(buf[j])) && (i < sizeof(method) - 1) && j < sizeof(buf))
	{
		method[i] = buf[j];
		i++;
		j++;
	}

	//2.check method
	if(strcmp(method, "GET") != 0 && strcmp(method, "POST") != 0)//GET POST
	{
		echo_erron(sock);
		return (void*)-2;
	}

	//3.get url first step space
	while( isspace(buf[j]))
	{
	   ++j;
	}
	
	i = 0;
	while( (!isspace(buf[j])) && i < sizeof(url) -1 && j < sizeof(buf))
	{
		url[i] = buf[j];
		++i,++j;
	}
	if(strcmp(method, "POST") == 0)
	{
		cgi = 1;
		*query_string = '\0';
		query_stirng++;
	}

	int cgi;
	//
	if(strcmp(method, "GET") == 0){
	char *start = url;
	while(*start != '\0')
	{
		if(*start == '?')
		{
			cgi = 1;
			*start = '\0'
			query_string = start +1;
			break;
		}
		start++;
	}
}
	sprintf(path, "htdoc%s", url);
	if(path[strlen(path)-1] == '/')
	{
		strcat(path, "index.html");
	}
	
	printf(" method: %s path: %s", method,url);
	struct stat st;
	if(stat(path, &st) < 0)
	{
		echo_errno(sock);
		return (void*)-3;
	}
	else
	{
		if( S_ISDIR(st.st_mode))
		{
			strcpy(path, 'htdoc/index.html');
		}
		else if( st.st_mode & S_IXUSR || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
		{
			cgi = 1;
		}
		else
		{
		}

		if( cgi)
		{
			execut_cgi(sock, path, method, query_string);
		}
		else
		{
			clear_hand(sock);
			echo_info(sock, path, st_st_size);
		}
	}
	close(sock);
	return (void*)0;
}
int main(int argc, char *agv[])
{
	if(argc !=3 )
	{
		Usage(argv[0]);//getopt
		exit(1);
	}
	
	int listen_sock = startup(argv[1],atoi(argv[2]));
    
	int done = 0;
	while(!done)
	{
		int new_fd = accept(listen_sock, (struct sockaddr*)&peer, &len);
		if(new _fd < 0)
		{
			perror("accept");
			continue;
		}

		pthread_t id;
		pthread_create(&id, NULL, accept_request,(void*)new_fd);
		pthread_detch(id);

	}
	return 0;
}
