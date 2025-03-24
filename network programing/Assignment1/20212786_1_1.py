import socket
import json

# Use the host and port number
HOST = "ip-api.com"
PORT = 80

# Define the endpoint to receive the request
ENDPOINT = "/json"

# Input IP address
ip_address = input("Enter the IP address: ")

# Define the parameters for the API request
parameters = {"fields": "city,regionName,country,lat,lon", "lang": "fr"}
# Create a TCP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Connect to the server
sock.connect((HOST, PORT))
# Send a GET request with the parameters defined in the problem statement
param_string = "&".join([f"{k}={v}" for k, v in parameters.items()])
request = f"GET {ENDPOINT}?{param_string}&query={ip_address} HTTP/1.1\r\nHost: {HOST}\r\nConnection: close\r\n\r\n"
sock.sendall(request.encode())

# Read data from the TCP socket using recv() function
# Received data from the socket is stored in the response variable.
response = b""
while True:
    data = sock.recv(4096)
    if not data:
        break
    response += data

# Decode bytes to string
response_str = response.decode()

# Select only the JSON data part from the HTTP response data.
# HTTP response data is divided into header and body separated by a blank line (\r\n\r\n).
# Using the string method 'split()', split the string and select the second element in the list to extract only the JSON data.
json_str = response_str.split("\r\n\r\n")[1]

# Convert json data to a dictionary
data = json.loads(json_str)
# Print geolocation data
print(json_str)
