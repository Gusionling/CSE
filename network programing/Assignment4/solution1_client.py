import requests

if __name__ == "__main__":
    # client requests by keyword
    keyword = str(input("Search keyword>> "))
    # response from Server
    response = requests.get("http://127.0.0.1:8000/" + keyword)
    print(response.text)
