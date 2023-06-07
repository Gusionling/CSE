import socket
import random
import argparse

MAX_BYTES = 65535


def server(interface, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((interface, port))
    sock.listen(1)
    print("Listening at", sock.getsockname())

    number = random.randint(1, 10)

    while True:
        print("waiting to accept a new connection")
        # obj is the socket object, sockname is the address of the client
        obj, sockname = sock.accept()
        print("Connected by", sockname)

        obj.sendall(b"Guess the Number between 1 and 10 in 5 attempts.\n ")

        # Variable to keep track of remaining attempts, total 5 attempts
        attempt = 5

        while True:
            # To avoid errors when receiving "start" from the client at the beginning,
            # decode the data and check if it is "start".
            data = obj.recv(MAX_BYTES).decode().strip()

            if data == "start":
                continue

            guess = int("".join(data.split()))

            attempt -= 1
            # If the guess is correct
            if guess == number:
                obj.sendall(b"Congratulations, you did it!\n")
                break
            # If all 5 attempts are used up
            elif attempt == 0:
                obj.sendall(b"You lost!")
                break
            elif number > guess:
                obj.sendall(b"You guessed too small! Try again.\n")
            else:
                obj.sendall(b"You guessed too high! Try again.\n")

        obj.close()


def client(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    print("client has been assigned socket name", sock.getsockname())
    sock.sendall(b"start\n")

    while True:
        data = sock.recv(MAX_BYTES)
        print(data.decode())

        if any(
            keyword in data.decode() for keyword in ["Congratulations", "You lost!"]
        ):
            break

        guess = input("Enter your guess: ")

        sock.sendall(guess.encode())

    sock.close()


if __name__ == "__main__":
    choices = {"client": client, "server": server}
    parser = argparse.ArgumentParser(description="Send and receive over TCP")
    parser.add_argument("role", choices=choices, help="which role to play")
    parser.add_argument(
        "host", help="interface the server listens at;" " host the client sends to"
    )
    parser.add_argument(
        "-p", metavar="PORT", type=int, default=1060, help="TCP port (default 1060)"
    )
    args = parser.parse_args()
    function = choices[args.role]
    function(args.host, args.p)
