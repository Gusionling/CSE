import socket
import argparse

MAX_BYTES = 65535

# Function to perform encryption
def encrypt(msg):
    # encrypted_message is initialized as an empty string to store the encrypted message
    encrypted_message = ""
    for char in msg:
        if char >= "a" and char <= "z":
            if char == "a":
                encrypted_message += "b"
            elif char == "z":
                encrypted_message += "a"
            else:
                # ord(char) converts char to ASCII value for processing
                # chr() function is used to convert ASCII value back to character
                encrypted_message += chr(ord(char) + 1)
        else:
            encrypted_message += char
    return encrypted_message


# Server function for UDP communication
def server(interface, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((interface, port))

    # Display message indicating server is running
    print("Server is running at", sock.getsockname())
    while True:
        data, address = sock.recvfrom(MAX_BYTES)
        print("Received {} bytes from {}".format(len(data), address))
        encrypted_data = encrypt(data.decode())

        sock.sendto(encrypted_data.encode("ascii"), address)


# Client function for UDP communication
def client(hostname, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.connect((hostname, port))
    print("Client socket name is {}".format(sock.getsockname()))

    # Generate and encode the message to send to the server
    msg = input("Enter a message to send to the server: ")
    sock.send(msg.encode("ascii"))

    data = sock.recv(MAX_BYTES)
    encrypted_msg = data.decode("ascii")

    print('Received "{}" from server'.format(encrypted_msg))

    sock.close()


if __name__ == "__main__":
    # Dictionary with two choices: "client" and "server", mapped to corresponding functions
    choices = {"client": client, "server": server}
    parser = argparse.ArgumentParser(
        description="Send and receive UDP packets, pretending packets are often dropped"
    )
    parser.add_argument("role", choices=choices, help="which role to take")
    parser.add_argument(
        "host", help="interface the server listens at; host the client sends to"
    )
    parser.add_argument(
        "-p", metavar="PORT", type=int, default=1060, help="UDP port (default 1060)"
    )
    args = parser.parse_args()
    function = choices[args.role]
    function(args.host, args.p)
