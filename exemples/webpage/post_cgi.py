#!/nix/store/8w718rm43x7z73xhw9d6vh8s4snrq67h-python3-3.12.10/bin/python3

import cgi
import cgitb

# Enable error reporting
cgitb.enable()

print("Content-Type: text/html")  # HTML is following
print()  # blank line, end of headers

form = cgi.FieldStorage()

# Get data from fields
name = form.getvalue("name")
email = form.getvalue("email")

print(f"""
<!DOCTYPE html>
<html>
<head>
    <title>POST Request Received</title>
</head>
<body>
    <h1>POST Request Received</h1>
    <p>Name: {name}</p>
    <p>Email: {email}</p>
</body>
</html>
""")
