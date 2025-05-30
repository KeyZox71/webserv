#!/nix/store/8w718rm43x7z73xhw9d6vh8s4snrq67h-python3-3.12.10/bin/python3
# Import modules for CGI handling
import cgi
import cgitb

# Enable error reporting
cgitb.enable()

# Create instance of FieldStorage
form = cgi.FieldStorage()

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
print("</body>")
print("</html>")
