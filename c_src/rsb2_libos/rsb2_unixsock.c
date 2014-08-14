/** Module rsb2_unixsock - Implementation.
 * @file rsb2_unixsock.c
 * @author jp.tranvouez@navilab.com
 */
#include "rsb2_unixsock.h"
#include "rsb2_module.h"
#include "rsb2_socket.h"

#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

enum {
	RSB2_SOCKET_BACKLOG			= 100,		/* Default backlog. */
};

static int g_module = -1;						/* Module reference. */
static int g_backlog = RSB2_SOCKET_BACKLOG;		/* Default backlog. */

int rsb2_unixsock_begin(void)
{
	RSB2_TRACE_ENTRY();
	g_module = rsb2_module_ref("rsb2_unixsock");
	int err = g_module < 0;
	RSB2_TRACE_EXIT_INT(err);
	return err;
}

void rsb2_unixsock_end(void)
{
	RSB2_TRACE_ENTRY();
	rsb2_module_destroy(g_module);
	RSB2_TRACE_EXIT();
}

int rsb2_unixsock_open(void)
{
	RSB2_TRACE_ENTRY();
	/* create Unix datagram socket */
	int sock = socket(PF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		/* notify 'socket' failure */
		RSB2_ERRNO("socket", "sock=%d", sock);
	} else {
		/* notify socket opened */
		RSB2_NOTIFY("socket_opened", "sock=%d", sock);
	}
	RSB2_TRACE_EXIT_INT(sock);
	return sock;
}

void rsb2_unixsock_unlink(const char *path)
{
	RSB2_TRACE_ARGS("path=%s", path);
	RSB2_ASSERT_NOTNULL(path);
	struct stat buf;
	int err = stat(path, &buf);
	if (!err) {
		/* inode found */
		err = unlink(path);
		if (err) {
			/* notify 'unlink' failure */
			RSB2_ERRNO("unlink", "path=%s", path);
		} else {
			/* notify inode unlinked */
			RSB2_NOTIFY("inode_unlinked", "path=%s", path);
		}
	}
	RSB2_TRACE_EXIT();
}

int rsb2_unixsock_connect(const char *path)
{
	RSB2_TRACE_ARGS("path=%s", path);
	RSB2_ASSERT_NOTNULL(path);
	/* create Unix datagram socket */
	int sock = rsb2_unixsock_open();
	if (sock < 0) {
		RSB2_ERRTRACE();
	} else {
		/* connect socket to inode */
		struct sockaddr_un sockaddr;
		sockaddr.sun_family = AF_UNIX;
		strncpy(sockaddr.sun_path, path, sizeof(sockaddr.sun_path));
		socklen_t addrlen = strnlen(path, sizeof(sockaddr.sun_path)) +
				sizeof(sockaddr.sun_family) + 1;
		int err = connect(sock, (struct sockaddr *)&sockaddr, addrlen);
		if (err) {
			/* handle 'connect' failure */
			RSB2_ERRNO("connect", "path=%s,sock=%d", path, sock);
			rsb2_socket_close(sock);
			sock = -1;
		} else {
			/* notify client-side socket connected */
			RSB2_NOTIFY("socket_connected", "path=%s,sock=%d", path, sock);
		}
	}
	RSB2_TRACE_EXIT_INT(sock);
	return sock;
}

int rsb2_unixsock_listen(const char *path)
{
	RSB2_TRACE_ARGS("path=%s", path);
	RSB2_ASSERT_NOTNULL(path);
	/* create Unix datagram socket */
	int sock = rsb2_unixsock_open();
	if (sock < 0) {
		RSB2_ERRTRACE();
	} else {
		/* remove Unix socket if it exists */
		rsb2_unixsock_unlink(path);
	    /* bind socket to inode */
		struct sockaddr_un sockaddr;
		sockaddr.sun_family = AF_UNIX;
		strncpy(sockaddr.sun_path, path, sizeof(sockaddr.sun_path));
		socklen_t addrlen = strnlen(path, sizeof(sockaddr.sun_path)) +
				sizeof(sockaddr.sun_family) + 1;
		int err = bind(sock, (struct sockaddr *)&sockaddr, addrlen);
		if (err) {
			/* notify 'bind' failure */
			RSB2_ERRNO("bind", "path=%s,sock=%d", path, sock);
			sock = -1;
		} else {
			/* notify socket bound to inode */
			RSB2_NOTIFY("socket_bound", "path=%s,sock=%d", path, sock);
		    /* start listening */
		    err = listen(sock, g_backlog);
			if (err) {
				/* handle 'listen' failure */
				RSB2_ERRNO("listen", "path=%s,sock=%d", path, sock);
			} else {
				/* notify socket listening */
				RSB2_NOTIFY("socket_listening", "path=%s,sock=%d", path, sock);
			}
		}
	}
	RSB2_TRACE_EXIT_INT(sock);
	return sock;
}

int rsb2_unixsock_accept(int lis_sock)
{
	RSB2_TRACE_ARGS("lis_sock=%d", lis_sock);
	int sock = -1;
	if (lis_sock >= 0) {
		struct sockaddr_un sockaddr;
		socklen_t addrlen = 0;
		sock = accept(lis_sock, (struct sockaddr *)&sockaddr, &addrlen);
		if (sock < 0) {
			/* notify 'accept' failure */
			RSB2_ERRNO("accept", "lis_sock=%d", lis_sock);
		} else {
			/* notify server-side socket connected */
			RSB2_NOTIFY("socket_connected", "lis_sock=%d,sock=%d",
					lis_sock, sock);
		}
	}
	RSB2_TRACE_EXIT_INT(sock);
	return sock;
}

int rsb2_unixsock_sendto(const char *path, const char *msg, int msglen)
{
	RSB2_TRACE_ARGS("path=%s,msg=%p,msglen=%d", path, msg, msglen);
	RSB2_ASSERT_NOTNULL(path);
	int err = -1;
	int sock = rsb2_unixsock_connect(path);
	if (sock < 0) {
		RSB2_ERROR("connect_failed", "path=%s", path);
	} else {
		err = rsb2_socket_send(sock, msg, msglen);
		rsb2_socket_close(sock);
	}
	RSB2_TRACE_EXIT_INT(err);
	return err;
}

int rsb2_unixsock_rpc(const char *path, const char *msg, int msglen,
		char *buf, int bufsz)
{
	RSB2_TRACE_ARGS("path=%s,msg=%p,msglen=%d,buf=%p,bufsz=%d",
			path, msg, msglen, buf, bufsz);
	RSB2_ASSERT_NOTNULL(path);
	RSB2_ASSERT_NOTNULL(buf);
	RSB2_ASSERT_POSINT(bufsz);
	int len = -1;
	int sock = rsb2_unixsock_connect(path);
	if (sock < 0) {
		RSB2_ERRTRACE();
	} else {
		int err = rsb2_socket_send(sock, msg, msglen);
		if (err) {
			RSB2_ERRTRACE();
		} else {
			len = rsb2_socket_recv(sock, buf, bufsz);
		}
		rsb2_socket_close(sock);
	}
	RSB2_TRACE_EXIT_INT(len);
	return len;
}

int rsb2_unixsock_seqserve(const char *path, rsb2_Unixsock_recv fRecv,
		int accept_tmo, int recv_tmo)
{
	RSB2_TRACE_ARGS("path=%s,fRecv=%p,accept_tmo=%d,recv_tmo=%d",
			path, fRecv, accept_tmo, recv_tmo);
	int err = -1;
	int ret = 0;
	int lis_sock = rsb2_unixsock_listen(path);
	if (lis_sock >= 0) {
		while (!ret) {
			/* wait for incoming connection */
			if (accept_tmo) {
				/* wait for incoming connection */
				ret = rsb2_socket_rdwait(lis_sock, accept_tmo);
				if (ret) {
					RSB2_ERRTRACE();
				}
			}
			int sock = rsb2_unixsock_accept(lis_sock);
			if (sock < 0) {
				RSB2_ERROR("rsb2_unixsock_accept", "lis_sock=%d", lis_sock);
			} else {
				while (!ret) {
					/* process incoming message */
					if (recv_tmo) {
						/* wait for incoming connection */
						ret = rsb2_socket_rdwait(lis_sock, recv_tmo);
						if (ret) {
							RSB2_ERRTRACE();
						}
					}
					if (!ret) {
						/* receive incoming message */
						char buf[8192];
						int len = rsb2_socket_recv(sock, buf, sizeof(buf));
						if (len > 0) {
							/* call message processing function */
							ret = fRecv(sock, buf, len);
						} else if (len < 0) {
							/* read error */
							RSB2_ERRTRACE();
							ret = 3;
						} else {
							/* no bytes read */
						}
					}
				}
				if (ret == 1) {
					/* service socket close requested */
					ret = 0;
				}
				/* close service socket */
				rsb2_socket_close(sock);
			}
			if (ret == 2) {
				/* server shutdown requested */
				err = 0;
			}
		}
		/* close listening socket */
		rsb2_socket_close(lis_sock);
	}
	RSB2_TRACE_EXIT_INT(err);
	return err;
}

/*END*/
