#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "messagehandler.h"
using std::string;
using std::shared_ptr;
/**
 * Message handler is the low-level message handler for messages to/from the
 * server. It handles transmission/reception of low-level message units.
 */



/**
 * Create a message handler.
 *
 * @param conn
 *            The connection to use messages
 */
MessageHandler::MessageHandler(shared_ptr<Connection>& c): conn(c)  {}
MessageHandler::MessageHandler(Connection& c): conn(&c)  {}

void MessageHandler::send_byte(int code) {
	try {
		conn->write(static_cast<char>(code));
	} catch (ConnectionClosedException e) {
		throw ConnectionClosedException();
	}
}

/**
 * Transmit a code (a constant from the Protocol class).
 *
 * @param code
 *            The code to transmit.
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::send_code(Protocol code) {
	send_byte(static_cast<int>(code));
}

/**
 * Transmit an int value, according to the protocol.
 *
 * @param value
 *            The value to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::send_int(int value) {
	send_byte((value >> 24) & 0xFF);
	send_byte((value >> 16) & 0xFF);
	send_byte((value >> 8) & 0xFF);
	send_byte(value & 0xFF);
}

/**
 * Transmit an int parameter, according to the protocol.
 *
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::send_int_parameter(int param) {
	send_code(Protocol::PAR_NUM);
	send_int(param);
}

/**
 * Transmit a string parameter, according to the protocol.
 *
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::send_string_parameter(string param) {
	send_code(Protocol::PAR_STRING);
	send_int(param.length());
	for (char c: param) {
		send_byte(c);
	}
}
/**
 * Receive a command code or an error code from the server.
 *
 * @return The code
 * @throws ConnectionClosedException
 *             If the server died
 */
Protocol MessageHandler::recv_code() {
	unsigned char code = recv_byte();
	return static_cast<Protocol>(code);
}

/**
 * Receive an int value from the server.
 *
 * @return The int value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recv_int() {
	unsigned char b1 = recv_byte();
	unsigned char b2 = recv_byte();
	unsigned char b3 = recv_byte();
	unsigned char b4 = recv_byte();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

/**
 * Receive an int parameter from the server.
 *
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recv_int_parameter() {
	Protocol code = recv_code();
	if (code != Protocol::PAR_NUM) {
		throw ConnectionClosedException();
	}
	return recv_int();
}

/**
 * Receive a string parameter from the server.
 *
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */

string MessageHandler::recv_string_parameter() {
	Protocol code = recv_code();
	if (code != Protocol::PAR_STRING) {
		throw ConnectionClosedException();
	}
	int n = recv_int();
	if (n < 0) {
		throw ConnectionClosedException();
	}
	string result(n, ' ');
	for (int i = 1; i <= n; i++) {
		char ch = static_cast<char>(conn->read());
		result[i - 1] = ch;
	}
	return result;
}

unsigned char MessageHandler::recv_byte() {
	unsigned char code = conn->read();
	// if (code == Connection.CONNECTION_CLOSED) {
	// 	throw ConnectionClosedException();
	// }
	return code;
}
