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

/**
 * Set the log window to use.
 *
 // * @param logWindow
 *            The log window
 */
// void MessageHandler::setLogWindow(Logger logWindow) {
// 	this.logWindow = logWindow;
// }

void MessageHandler::sendByte(int code) {
	try {
		conn->write(static_cast<char>(code));
	} catch (ConnectionClosedException e) {
		throw new ConnectionClosedException();
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
void MessageHandler::sendCode(int code) {
	sendByte(code);
	// logWindow.logCode(code);
}

/**
 * Transmit an int value, according to the protocol.
 *
 * @param value
 *            The value to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendInt(int value) {
	sendByte((value >> 24) & 0xFF);
	// logWindow.logByte((value >> 24) & 0xFF);
	sendByte((value >> 16) & 0xFF);
	// logWindow.logByte((value >> 16) & 0xFF);
	sendByte((value >> 8) & 0xFF);
	// logWindow.logByte((value >> 8) & 0xFF);
	sendByte(value & 0xFF);
	// logWindow.logByte(value & 0xFF);
}

/**
 * Transmit an int parameter, according to the protocol.
 *
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendIntParameter(int param) {
	sendCode(static_cast<int>(Protocol::PAR_NUM));
	sendInt(param);
}

/**
 * Transmit a string parameter, according to the protocol.
 *
 * @param value
 *            The parameter to transmit
 * @throws ConnectionClosedException
 *             If the server died
 */
void MessageHandler::sendStringParameter(string param) {
	conn->write('$');
	sendCode(static_cast<int>(Protocol::PAR_STRING));
	sendInt(param.length());
	for (char c: param) {
		sendByte(c);
		// logWindow.logChar(param.charAt(i));
	}
	sendByte('$');
}
/**
 * Receive a command code or an error code from the server.
 *
 * @return The code
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvCode() {
	unsigned char code = recvByte();
	// logWindow.logCode(code);
	return code;
}

/**
 * Receive an int value from the server.
 *
 * @return The int value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvInt() {
	unsigned char b1 = recvByte();
	unsigned char b2 = recvByte();
	unsigned char b3 = recvByte();
	unsigned char b4 = recvByte();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

/**
 * Receive an int parameter from the server.
 *
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */
int MessageHandler::recvIntParameter() {
	int code = recvCode();
	if (code != static_cast<int>(Protocol::PAR_NUM)) {
		throw ConnectionClosedException();
	}
	return recvInt();
}

/**
 * Receive a string parameter from the server.
 *
 * @return The parameter value
 * @throws ConnectionClosedException
 *             If the server died
 */

string MessageHandler::recvStringParameter() {
	int code = recvCode();
	if (code != static_cast<int>(Protocol::PAR_STRING)) {
		throw ConnectionClosedException();
	}
	int n = recvInt();
	if (n < 0) {
		throw ConnectionClosedException();
	}
	string result(n, ' ');
	for (int i = 1; i <= n; i++) {
		char ch = static_cast<char>(conn->read());
		result[i - 1] = ch;
		// logWindow.logChar(ch);
	}
	return result;
}

unsigned char MessageHandler::recvByte() {
	unsigned char code = conn->read();
	// if (code == Connection.CONNECTION_CLOSED) {
	// 	throw ConnectionClosedException();
	// }
	return code;
}
