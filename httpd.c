#include<stdio.h>
#include<stdlib.h>
static void Usage(const char* proc)
{
	printf("Udage: %s [ip] [port]\n", proc);
}


static int startup(int argc, char* argv[])
{
	int sock = sock(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("sock");
		return 1;
	}
	
	int opt = 1;
	setsock(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	struct sockaddr_in local;
	local.sin_port = hons(argv[2]);
	local.sin_addr = 
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{

	}

	if(listen(sock,5) < 0)
	{

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
	
	char *query_string;
	int cgi;

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
	
	sprintf(path, "htdoc%s", url);
	if(path[strlen(path)-1] == '/')
	{
		strcat(path, "index.html");
	}


	printf(" method: %s path: %s", method,url);


	return (void*)0;
}
int main(int argc, char *agv[])
{
	if(argc !=3 )
	{
		Usage(argv[0]);//getopt
		exit(1);
	}
	
	int listen_sock = startup(argv[1],atoa(argv[2]));
    
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
