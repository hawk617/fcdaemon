/*
 * net.cpp
 *
 *  Created on: 23.07.2013
 *      Author: Hawk
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <resolv.h>
#include <time.h>
#include <errno.h>
#include <wait.h>
#include "net.h"


static int npid=0;

sockaddr_in client_name;//--структура sockaddr_in клиентской машины (параметры ее нам неизвестны. Мы не знаем какая машина к нам будет подключаться)
int size = sizeof(client_name);//--размер структуры (тоже пока неизвестен)
int client_socket_fd;          //--идентификатор клиентского сокета
                               //--вобще теория сокетов более подробно будет рассмотрена в следующей статье

char sockbuff[1024]; //--наш буфер обмена информацией с клиентом
time_t now;
struct tm *ptr;
char tbuf[80];


int NetStart()
{
	int pid=fork();
	if (pid) return pid;

	int socket_fd, nbytes; //--объявляем идентификатор сокета нашего сервера
	char host[20];
	char *namlog;
	void sig_child(int);//--объявление функции ожидания завершения дочернего процесса
	struct sockaddr_in name;//--структура sockaddr_in для нашего сервера на сей раз ее придется заполнить
	namlog= new char[25];
	socket_fd=socket(PF_INET,SOCK_STREAM,0); //--создаем сокет сервера в данном случае TCP, если бы мы использовали флаг SOCK_DTGRAM то получили бы сокет UDP
	name.sin_family=AF_INET; //--говорим что сокет принадлежит к семейству интернет
	name.sin_addr.s_addr=INADDR_ANY; //--наш серверный сокет принимает запросы от любых машин с любым IP-адресом
	name.sin_port=htons(PORT_CON); //--и прослушивает порт 6823
	if(bind(socket_fd, (sockaddr*)&name, sizeof(name))==-1) //--функция bind спрашивает у операционной системы,может ли программа завладеть портом с указанным номером
	{
		exit(0); //--соответственно выход из программы
	}

	listen(socket_fd,20); //--перевод сокета сервера в режим прослушивания с очередью в 20 позиций

	for(;;) //--бесконечный цикл (как так?... спросите ВЫ. У нас же сервер,который должен постоянно принимать и обслуживать запросы и работать пока его не погасят насильно)
	{
		signal(SIGCHLD,sig_child); //--если клиент уже поработал и отключился ждем завершение его дочернего процесса
		client_socket_fd = accept (socket_fd,(sockaddr*)&client_name,&size); //--подключение нашего клиента
		if (client_socket_fd>0) //--если подключение прошло успешно
		{
			if ((pid=fork())==0) //--то мы создаем копию нашего сервера для работы с другим клиентом(то есть один сеанс уде занят дублируем свободный процесс)
			{
              inet_ntop(AF_INET,&client_name.sin_addr,host,sizeof(host));//--в переменную host заносим IP-клиента
              do
              {
               bzero(sockbuff,1024); //--чистим наш буфер от всякого мусора
               nbytes=read(client_socket_fd,sockbuff,1024); //--читаем из в буфер из клиентского сокета
               strncat(sockbuff,"\0",1); //--незабываем символ конца строки
               if(fopen(logfile,namlog,"a+")!=NULL);
               {
                fprintf(logfile,"%s Client send to Server:%s\n",getTime(),sockbuff);
                fflush(logfile); //--эту запись в лог файл конечно можно оформить ввиде отдельной функции
                fclose(logfile); //--даную функцию вы должны написать уже сами для тренировки
               }
               sendto(client_socket_fd,sockbuff,strlen(sockbuff),0,&client_socket_fd,size); //--и отсылаем полученую информацию назад
               if(fopen(logfile,namlog,"a+")!=NULL);
               {
                fprintf(logfile,"%s Server answer to client:%s\n",getTime(),sockbuff);
                fflush(logfile);
                fclose(logfile);
               }
               }
              while(nbytes > 0 && strncmp("bye",sockbuff,3)!=0); //--выполняем цикл пока клиент не закроет сокет или пока не прилетит сообщение от клиента "bye"
              if(fopen(logfile,namlog,"a+")!=NULL);
              {
              fprintf(logfile,"%s Close session on client:%s\n",getTime(),host);
              fflush(logfile);
              fclose(logfile);
              }
              close(client_socket_fd); //--естествено закрываем сокет
              exit(0); //--гасим дочерний процесс
           }
           else if (pid > 0)
           close(client_socket_fd);
      }
  }


	return 0;
}

void NetStop()
{
	if (npid) kill (npid, SIGQUIT);
}

void sig_child(int sig) //--функция ожидания завершения дочернего процесса
{
	pid_t pid;
	int stat;
	while ((pid=waitpid(-1,&stat,WNOHANG))>0)
	{
	}
	return;
}



