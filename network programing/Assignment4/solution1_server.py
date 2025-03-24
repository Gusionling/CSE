import csv
import pandas as pd
from wsgiref.simple_server import make_server

# open csv file
with open("./solution1_csv.csv", "r") as f:
    dr = csv.DictReader(f)
    s = pd.DataFrame(dr)


def app(environ, start_reponse):
    host = environ.get("HTTP_HOST", "127.0.0.1")
    path = environ.get("PATH_INFO", "/")
    if ":" in host:
        host, port = host.split(":", 1)
    if "?" in path:
        path, query = path.split("?", 1)
    headers = [("Content-Type", "text/plain; charset=utf-8")]

    # if request method is not GET, occur 501 error
    if environ["REQUEST_METHOD"] != "GET":
        start_reponse("501 Not Implemented", headers)
        yield b"501 Not Implemented"

    # if the host is not '127.0.0.1' or has wrong path, occur 404 error
    elif host != "127.0.0.1" or path == "/":
        start_reponse("404 Not Found", headers)
        yield b"404 Not Found"

    # correct request
    else:
        keyword = path.lstrip("/")
        # elsect keyword row with filtering
        value = s.loc[s["Keyword"] == keyword]
        if not value.empty:
            definition = value.iloc[0][1]
            start_reponse("200 OK", headers)
            yield definition.encode("ascii")
        else:
            start_reponse("404 Not Found", headers)
            yield b"404 Not Found"


if __name__ == "__main__":
    httpd = make_server("", 8000, app)
    host, port = httpd.socket.getsockname()
    print("Serving on", host, "port", port)
    httpd.serve_forever()
