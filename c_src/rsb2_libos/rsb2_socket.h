/** Module rsb2_socket - Interface.
 * @file rsb2_socket.h
 * @author jp.tranvouez@navilab.com
 * @defgroup rsb2_socket Socket API Wrapper
 * @ingroup rsb2_libos
 * @{
 */
#ifndef RSB2_SOCKET_H
#define RSB2_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

/** Close a socket.
 * @param sock socket file descriptor
 */
void rsb2_socket_close(int sock);

/** Diagnose a socket.
 * @param sock socket file descriptor
 * @retval 0 no error was detected on the socket
 * @retval -1 an error was detected on the socket
 */
int rsb2_socket_diag(int sock);

/** Wait for 'input ready' condition on a socket.
 * @param sock socket file descriptor
 * @param maxms maximum wait time (ms)
 * @retval 0 timeout
 * @retval 1 data available for reading
 * @retval -1 error
 */
int rsb2_socket_rdwait(int sock, int maxms);

/** Wait for 'output ready' condition on a socket.
 * @param sock socket file descriptor
 * @param maxms maximum wait time (ms)
 * @retval 0 timeout
 * @retval 1 socket available for writing
 * @retval -1 error
 */
int rsb2_socket_wrwait(int sock, int maxms);

/** Read data from a service socket.
 * @param sock service socket file descriptor
 * @param buf buffer address
 * @param bufsz buffer size
 * @return number of bytes read
 * @retval -1 error
 */
int rsb2_socket_recv(int sock, char *buf, int bufsz);

/** Write data to a service socket.
 * @param sock service socket file descriptor
 * @param msg data address
 * @param msglen data length
 * @return number of bytes written
 * @retval -1 error
 */
int rsb2_socket_send(int sock, const char *msg, int msglen);

#ifdef __cplusplus
}
#endif

#endif /*@} RSB2_SOCKET_H */
