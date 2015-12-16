#include <xinu.h>
#include <stdio.h>
#include <string.h>
#ifndef _NET_IPC_H
#define _NET_IPC_H

uid32 net_send(char *msg, char *dest_ip_str, uint32 dest_port,  uint32 local_port);
char* net_recv(uid32 slot);

#endif /*_NET_IPC_H */
 
