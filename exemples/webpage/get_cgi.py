#!/usr/bin/env python3
import cgi
import cgitb
import time
import os
import sys  # Needed for sys.exit()

# Enable error reporting
cgitb.enable()

# Create instance of FieldStorage
form = cgi.FieldStorage()

try:
    # Set the content type to HTML
    print("Content-Type: text/html\n")

    # Output a simple HTML page
    print("<html>")
    print("<head>")
    print("<title>CGI Script Test</title>")
    print("</head>")
    print("<body>")
    print("<h1>CGI Script is Running</h1>")
    print("<p>Your web server is working correctly!</p>")
    files = os.listdir('.')
    for file in files:
        print(file)
    print("</body>")
    print("</html>")

except Exception:
    # Print error page and exit with error code
    cgitb.handler()
    sys.exit(1)
