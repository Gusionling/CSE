import requests


def get_member_details(name):
    url = "http://localhost:5000/member-details"
    params = {"name": name}
    response = requests.get(url, params=params)
    if response.status_code == 200:
        return response.json()
    else:
        return {"error": response.text}


# Example usage
name = input("Enter a member name: ")
details = get_member_details(name)
print(details)
