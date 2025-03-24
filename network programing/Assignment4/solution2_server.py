import pandas as pd
import requests
from bs4 import BeautifulSoup as bs
from flask import Flask, jsonify, request

app = Flask(__name__)


def scrapping():
    # Function to scrape member data from the website
    url = "https://sites.google.com/view/davidchoi/home/members"

    # Send a GET request to the specified URL
    page = requests.get(url)

    # Create a BeautifulSoup object to parse the HTML content
    soup = bs(page.text, "html.parser")

    # Use CSS selectors to locate the parent element containing member data
    elements = soup.select_one(
        "#yDmH0d > div:nth-child(1) > div > div:nth-child(2) > div.QZ3zWd > div > div.UtePc.RCETm.SwuGbc"
    )

    # Select all img tags within the parent element
    imgs = elements.select(
        "section > div > div > div > div > div > div > div > div > div > div > img"
    )

    # Select all p tags within the parent element
    contents = elements.select(
        "section > div > div > div > div > div > div > div > div > div > p"
    )

    dataList = []  # List to store member data

    index = 0
    flag = False  # Flag to indicate name or research data processing

    for content in contents:
        if content.text == "":
            continue

        if not flag:
            # Process name-related data
            name = content.text.split("(")[0].strip()
            bracket = content.text.split("(")[1].split(")")[0]
            job_role = bracket.split(",")[0]
            start_year = bracket.split(",")[1].split("-")[0].strip()
            try:
                end_year = bracket.split(",")[1].split("-")[1]
                if not end_year:
                    end_year = "NA"
            except IndexError:
                end_year = start_year
            flag = True
            continue
        else:
            body = content.text
            research_interest = "NA"
            current_job_role = "NA"
            try:
                # Extract research interests for current members
                if "Research Interests" in body:
                    research_interest = body.split(":")[1].strip()
                    if not research_interest:
                        research_interest = "NA"
                # Extract current job role for alumni
                if "@" in body:
                    current_job_role = body.strip()
            except IndexError:
                print("list index out of range")

            if index < 9:
                pic_url = imgs[index]["src"]
            elif index < 11:
                pic_url = "NA"
            else:
                pic_url = imgs[index - 2]["src"]

            # Store the member data in a list
            data = [
                name,
                job_role,
                start_year,
                end_year,
                research_interest,
                current_job_role,
                pic_url,
            ]
            dataList.append(data)
            flag = False
            index += 1
            continue

    # Create a DataFrame from the list of member data
    df = pd.DataFrame(
        dataList,
        columns=[
            "name",
            "job_role",
            "start_year",
            "end_year",
            "research_interest",
            "current_job_role",
            "pic_url",
        ],
    )

    # Save the DataFrame to a CSV file
    df.to_csv("./solution2_csv.csv", index=False)


@app.route("/member-details", methods=["GET"])
def get_member_details():
    # API endpoint to get member details based on name
    name = request.args.get("name")
    if not name:
        return jsonify({"error": "Please provide a member name."}), 400

    # Read the member details from the CSV file
    df = pd.read_csv("./solution2_csv.csv")

    # Filter the DataFrame to find the member with the provided name (case-insensitive)
    member = df[df["name"].str.lower() == name.lower()]

    if member.empty:
        return jsonify({"error": "Member not found."}), 404
    else:
        # Convert the member details to a dictionary and return as JSON
        return jsonify(member.to_dict("records"))


if __name__ == "__main__":
    print("Scraping Data...")
    scrapping()
    print("Data scraping completed and saved to 'solution2_csv.csv' file.")
    print("Starting Flask app...")
    app.run(debug=True)
