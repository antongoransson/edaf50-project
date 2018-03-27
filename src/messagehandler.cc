#include "messagehandler.h"

using std::string;
using std::shared_ptr;

MessageHandler::MessageHandler(shared_ptr<Connection>& c): conn(c)  {}
MessageHandler::MessageHandler(Connection& c): conn(&c)  {}

void MessageHandler::send_byte(int code) {
	try {
		conn->write(static_cast<char>(code));
	} catch (ConnectionClosedException e) {
		throw ConnectionClosedException();
	}
}

void MessageHandler::send_code(Protocol code) {
	send_byte(static_cast<int>(code));
}

void MessageHandler::send_int(int value) {
	send_byte((value >> 24) & 0xFF);
	send_byte((value >> 16) & 0xFF);
	send_byte((value >> 8) & 0xFF);
	send_byte(value & 0xFF);
}

void MessageHandler::send_int_parameter(int param) {
	send_code(Protocol::PAR_NUM);
	send_int(param);
}

void MessageHandler::send_string_parameter(string param) {
	send_code(Protocol::PAR_STRING);
	send_int(param.length());
	for (const char c: param) {
		send_byte(c);
	}
}

Protocol MessageHandler::recv_code() {
	unsigned char code = recv_byte();
	return static_cast<Protocol>(code);
}

int MessageHandler::recv_int() {
	unsigned char b1 = recv_byte();
	unsigned char b2 = recv_byte();
	unsigned char b3 = recv_byte();
	unsigned char b4 = recv_byte();
	return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

int MessageHandler::recv_int_parameter() {
	Protocol code = recv_code();
	if (code != Protocol::PAR_NUM) {
		throw ConnectionClosedException();
	}
	return recv_int();
}

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
	return code;
}
