/** Module rsb2_unixsock - Interface.
 * @file rsb2_unixsock.h
 * @author jp.tranvouez@navilab.com
 * @defgroup rsb2_unixsock Unix Stream Socket API Wrapper
 * @ingroup rsb2_libos
 * @{
 */
#ifndef RSB2_UNIXSOCK_H
#define RSB2_UNIXSOCK_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Open a socket.
 * @return socket file descriptor
 * @retval -1 error
 */
int rsb2_unixsock_open(void);

/** Unlink a Unix socket inode.
 * @param path filesystem path of Unix socket
 */
void rsb2_unixsock_unlink(const char *path);

/** Open a client-side socket.
 * @param path filesystem path of Unix socket
 * @return socket file descriptor
 * @retval -1 error
 */
int rsb2_unixsock_connect(const char *path);

/** Open a server-side listening socket.
 * @param path filesystem path of Unix socket
 * @param backlog max number of outstanding connection requests or zero
 * @return listening socket file descriptor
 * @retval -1 error
 */
int rsb2_unixsock_listen(const char *path);

/** Open a server-side service socket.
 * Do nothing if lis_sock is negative.
 * @param lis_sock listening socket file descriptor
 * @return service socket file descriptor
 * @retval -1 error
 */
int rsb2_unixsock_accept(int lis_sock);

/** Send a request to a Unix socket and get a response.
 * Connect to Unix socket, send request, get response, close socket.
 * @param path filesystem path of Unix socket
 * @param msg request message address
 * @param msglen request message length
 * @param buf response buffer address
 * @param bufsz response buffer size
 * @return length of response message
 * @retval -1 error
 */
int rsb2_unixsock_rpc(const char *path, const char *msg, int msglen,
		char *buf, int bufsz);

/** Process a message received by a Unix socket server.
 * @param sock service socket file descriptor
 * @param msg incoming message address
 * @param msglen incoming message length
 * @retval 0 continue
 * @retval 1 close service socket, continue listening
 * @retval 2 stop server
 */
typedef int rsb2_Unixsock_recv(int sock, const char *msg, int msglen);

/** Run a Unix socket server in the current thread.
 * The server processes one client connection at a time.
 * @param path filesystem path of Unix socket
 * @param fRecv message processing function
 * @param accept_tmo accept timeout (ms)
 * @param recv_tmo receive timeout (ms)
 * @retval 0 normal shutdown
 * @retval -1 error detected
 */
int rsb2_unixsock_seqserve(const char *path, rsb2_Unixsock_recv fRecv,
		int accept_tmo, int recv_tmo);

#ifdef __cplusplus
}
#endif

#endif /*@} RSB2_UNIXSOCK_H */
