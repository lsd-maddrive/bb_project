import socket


class TcpLog:

    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.transfer_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def open(self):
        self.transfer_socket.connect((self.ip, self.port))

    def close(self):
        self.transfer_socket.close()

    def send(self, data):
        self.transfer_socket.send(data)
