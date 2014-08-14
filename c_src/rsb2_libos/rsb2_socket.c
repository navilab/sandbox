/** Module rsb2_socket - Implementation.
 * @file rsb2_socket.c
 * @author jp.tranvouez@navilab.com
 */
#include "rsb2_socket.h"
#include "rsb2_module.h"

#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>

static int g_module = -1;				/* Module reference. */

int rsb2_socket_begin(void)
{
	RSB2_TRACE_ENTRY();
	g_module = rsb2_module_ref("rsb2_socket");
	int err = g_module < 0;
	RSB2_TRACE_EXIT_INT(err);
	return err;
}

void rsb2_socket_end(void)
{
	RSB2_TRACE_ENTRY();
	rsb2_module_destroy(g_module);
	RSB2_TRACE_EXIT();
}

void rsb2_socket_close(int sock)
{
	RSB2_TRACE_ARGS("sock=%d", sock);
	int err = close(sock);
	if (err) {
		/* notify 'close' failure */
		RSB2_ERRNO("close", "sock=%d", sock);
	} else {
		/* notify socket closed */
		RSB2_NOTIFY("socket_closed", "sock=%d", sock);
	}
	RSB2_TRACE_EXIT();
}

int rsb2_socket_diag(int sock)
{
	RSB2_TRACE_ARGS("sock=%d", sock);
	long optval = 0;
	socklen_t optlen = sizeof(optval);
	int err = getsockopt(sock, SOL_SOCKET, SO_ERROR, &optval, &optlen);
	if (err) {
		/* notify 'getsockopt' failure */
		RSB2_ERRNO("getsockopt", "sock=%d", sock);
	} else if (optval) {
		/* notify socket error */
		err = -1;
		errno = optval;
		RSB2_ERRNO("so_error", "sock=%d", sock);
	}
	RSB2_TRACE_EXIT_INT(err);
	return err;
}

static int rsb2_socket_iowait(int sock, int maxms, int events)
{
	RSB2_TRACE_ARGS("sock=%d,maxms=%d,events=%d", sock, maxms, events);
	int count = 0;
	struct pollfd fdset;
	fdset.fd = sock;
	fdset.events = POLLIN;
	RSB2_NOTIFY("thread_iowait", "sock=%d", sock);
	count = poll(&fdset, 1, maxms? maxms: -1);
	RSB2_NOTIFY("thread_running", "sock=%d,count=%d", sock, count);
	if (count < 0) {
		/* notify 'poll' error */
		RSB2_ERRNO("poll", "sock=%d", sock);
	} else if (count > 0 && (fdset.revents & POLLERR)) {
		/* notify socket error */
		count = -1;
		rsb2_socket_diag(sock);
	}
	RSB2_TRACE_EXIT_INT(count);
	return count;
}

int rsb2_socket_rdwait(int sock, int maxms)
{
	RSB2_TRACE_ARGS("sock=%d,maxms=%d", sock, maxms);
	int count = rsb2_socket_iowait(sock, maxms, POLLIN);
	RSB2_TRACE_EXIT_INT(count);
	return count;
}

int rsb2_socket_wrwait(int sock, int maxms)
{
	RSB2_TRACE_ARGS("sock=%d,maxms=%d", sock, maxms);
	int count = rsb2_socket_iowait(sock, maxms, POLLOUT);
	RSB2_TRACE_EXIT_INT(count);
	return count;
}

int rsb2_socket_recv(int sock, char *buf, int bufsz)
{
	RSB2_TRACE_ARGS("sock=%d,buf=%p,bufsz=%d", sock, buf, bufsz);
	RSB2_ASSERT_NOTNULL(buf);
	RSB2_ASSERT_POSINT(bufsz);
	int count = -1;
	do {
		count = recv(sock, buf, bufsz, 0);
	} while (count < 0 && errno == EINTR);
	if (count < 0) {
		/* notify 'recv' error */
		RSB2_ERRNO("recv", "sock=%d", sock);
	}
	RSB2_TRACE_EXIT_INT(count);
	return count;
}

int rsb2_socket_send(int sock, const char *msg, int msglen)
{
	RSB2_TRACE_ARGS("sock=%d,msg=%p,msglen=%d", sock, msg, msglen);
	int count = -1;
	if (sock >= 0 && msglen > 0) {
		do {
			count = write(sock, msg, msglen);
		} while (count < 0 && errno == EINTR);
		if (count < 0) {
			/* notify 'write' error */
			RSB2_ERRNO("write", "sock=%d", sock);
		}
	}
	RSB2_TRACE_EXIT_INT(count);
	return count;
}

/*END*/
