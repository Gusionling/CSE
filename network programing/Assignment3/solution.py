import json, logging, ssl, socket, threading, zlib, argparse, random, time, zmq

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s %(levelname)-8s %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)


class StockTicker:
    def __init__(self):
        self.companies = ["APPLE", "MSFT", "GOOGLE"]
        self.prices = {}
        for company in self.companies:
            self.prices[company] = random.randint(100, 1000)

    def generate_stock_price(self, company):
        if company not in self.companies:
            raise ValueError("Invalid company")
        # Generate a random stock price for the company
        price = random.randint(100, 1000)
        self.prices[company] = price
        return price


class NewsGenerator:
    def __init__(self):
        # Initialize the news generator.
        self.topics = [
            "business",
            "entertainment",
            "health",
            "science",
            "sports",
            "technology",
        ]
        self.events = [
            "new product launch",
            "merger",
            "acquisition",
            "lawsuit",
            "scandal",
            "government regulation",
        ]
        self.companies = ["Apple", "Microsoft", "Google", "Amazon", "Facebook", "Tesla"]

    def get_news(self):
        # Generate a random news headline.
        topic = random.choice(self.topics)
        event = random.choice(self.events)
        company = random.choice(self.companies)
        headline = topic + " " + company + " " + event
        return headline


# function that implements toggle
def toggle(msg):
    toggled_msg = ""
    # Implemented using dictionary.
    alpha = {
        "a": "A",
        "b": "B",
        "c": "C",
        "d": "D",
        "e": "E",
        "f": "F",
        "g": "G",
        "h": "H",
        "i": "I",
        "j": "J",
        "k": "K",
        "l": "L",
        "m": "M",
        "n": "N",
        "o": "O",
        "p": "P",
        "q": "Q",
        "r": "R",
        "s": "S",
        "t": "T",
        "u": "U",
        "v": "V",
        "w": "W",
        "x": "X",
        "y": "Y",
        "z": "Z",
        "A": "a",
        "B": "b",
        "C": "c",
        "D": "d",
        "E": "e",
        "F": "f",
        "G": "g",
        "H": "h",
        "I": "i",
        "J": "j",
        "K": "k",
        "L": "l",
        "M": "m",
        "N": "n",
        "O": "o",
        "P": "p",
        "Q": "q",
        "R": "r",
        "S": "s",
        "T": "t",
        "U": "u",
        "V": "v",
        "W": "w",
        "X": "x",
        "Y": "y",
        "Z": "z",
    }
    for c in msg:
        # conversion
        if c in alpha:
            toggled_msg += alpha[c]
        else:
            toggled_msg += c

    return toggled_msg


# Client
def client(host, port, cafile=None):
    purpose = ssl.Purpose.SERVER_AUTH
    context = ssl.create_default_context(purpose, cafile=cafile)

    try:
        raw_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # set timeout to 15 seconds
        raw_sock.settimeout(15)
        raw_sock.connect((host, port))
        logging.info("Connected to host {!r} and port{}".format(host, port))
        ssl_sock = context.wrap_socket(raw_sock, server_hostname=host)

        # Get task input
        task = input("Enter task(ping or toggle or subscribe): ")
        if task == "ping":
            domain = input("Enter domain name: ")
            query = {"task": task, "domain": domain}
            # compress query string
            compressed_query = zlib.compress(json.dumps(query).encode())

            # send compressed data
            ssl_sock.sendall(compressed_query)

            while True:
                data = ssl_sock.recv(1024)
                if not data:
                    break
                decompressed_data = zlib.decompress(data)

                # Dcode HSON response
                json_response = json.loads(decompressed_data.decode())

                print(json_response)

        elif task == "toggle":
            msg = input("Enter msg: ")
            query = {"task": task, "msg": msg}
            # compress query string
            compressed_query = zlib.compress(json.dumps(query).encode())

            # send compressed data
            ssl_sock.sendall(compressed_query)

            while True:
                data = ssl_sock.recv(1024)
                if not data:
                    break
                decompressed_data = zlib.decompress(data)

                # Dcode HSON response
                json_response = json.loads(decompressed_data.decode())

                print(json_response)

        elif task == "subscribe":
            category = input("Enter the news topic to subscribe to: ")
            query = {"task": task, "category": category}
            # compress query string
            compressed_query = zlib.compress(json.dumps(query).encode())

            zcontext = zmq.Context()
            subscriber = zcontext.socket(zmq.SUB)
            subscriber.connect("tcp://localhost:{}".format(5556))
            # subscriber.connect(f"tcp://localhost:{args.port + 1}")
            subscriber.subscribe(category)

            # send compressed data
            ssl_sock.sendall(compressed_query)

            while True:
                # To appear on the client when the topic is selected by the client.
                data = subscriber.recv_string()
                message = data.split(" ", 1)
                received_category = message[0]
                received_message = message[1]

                if received_category == category:
                    print(
                        "topic: {} Received Message: {}".format(
                            received_category, received_message
                        )
                    )

        logging.info("disconnected")
    # exception handling
    except socket.timeout as e:
        logging.error(f"timeout err: {e}")
    except (socket.error, ssl.SSLError) as e:
        logging.error(f"Error connecting to server: {e}")


# Server
def server(host, port, certfile, cafile=None):
    purpose = ssl.Purpose.CLIENT_AUTH
    context = ssl.create_default_context(purpose, cafile=cafile)
    context.load_cert_chain(certfile)

    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listener.bind((host, port))
    listener.listen(5)

    while True:
        # Create a thread to handle client connections
        raw_sock, addr = listener.accept()
        client_thread = threading.Thread(
            target=handle_client, args=(raw_sock, addr, port, context)
        )
        client_thread.start()


# handle_client is called by each thread to handle a single client connection
def handle_client(raw_sock, addr, port, context):
    # SSL wrapping
    ssl_sock = context.wrap_socket(raw_sock, server_side=True)
    print("Connected by", addr)
    # for exception handling
    try:
        # Handle client request
        compressed_data = ssl_sock.recv(1024)
        if not compressed_data:
            logging.info("client disconnected")
            return

        decompressed_data = zlib.decompress(compressed_data)
        json_data = json.loads(decompressed_data.decode())

        # When task is ping
        if json_data["task"] == "ping":
            ip_address = socket.gethostbyname(json_data["domain"])
            response = {"ip": ip_address}
            compressed_response = zlib.compress(json.dumps(response).encode())
            ssl_sock.sendall(compressed_response)
        # When task is toggle
        elif json_data["task"] == "toggle":
            msg = json_data["msg"]
            toggled_msg = toggle(msg)
            response = {"toggled_msg": toggled_msg}
            compressed_response = zlib.compress(json.dumps(response).encode())
            ssl_sock.sendall(compressed_response)

        elif json_data["task"] == "subscribe":
            category = json_data["category"]
            response = {"subscribed_category": category}

            # Create an instance of the NewsGenerator class
            news_generator = NewsGenerator()

            zcontext = zmq.Context()
            # Create a publisher socket
            publisher = zcontext.socket(zmq.PUB)
            # Bind the publisher socket to a specific address
            publisher.bind("tcp://*:{}".format(5556))

            while True:
                message = news_generator.get_news()  # Generate a random news headline
                print(f"{message}")
                publisher.send_string(
                    f"{message}"
                )  # Publish the message for the specified category
                time.sleep(1)  # Wait for 1 second before publishing the next message

    except ssl.SSLError as e:
        logging.error(f"SSL Error: {e}")
    except ConnectionError as e:
        logging.error(f"Connection Error: {e}")
    finally:
        # Close the client socket
        ssl_sock.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Safe TLS client and server")
    parser.add_argument("host", help="hostname or IP address")
    parser.add_argument("port", type=int, help="TCP port number")
    parser.add_argument(
        "-a",
        metavar="cafile",
        default=None,
        help="authority: path to CA certificate PEM file",
    )
    parser.add_argument(
        "-s",
        metavar="certfile",
        default=None,
        help="run as server: path to server PEM file",
    )
    args = parser.parse_args()
    if args.s:
        server(args.host, args.port, args.s, args.a)
    else:
        client(args.host, args.port, args.a)
