#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>

void IgnoreSigpipe();

void NonBlockingSocket(const int fd);

int InitialSocket(const int port);

bool isValid(const struct epoll_event event);

#endif