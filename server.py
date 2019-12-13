#!/usr/bin/env python3
from http.server import HTTPServer, SimpleHTTPRequestHandler
import sys
import os
import base64

key = ""


class CORSRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Headers",
                         "Authorization, Cache-Control, Access-Control-Allow-Headers, Origin,Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers")
        SimpleHTTPRequestHandler.end_headers(self)

    def do_AUTHHEAD(self):
        self.send_response(401)
        self.send_header("WWW-Authenticate",
                         "Basic realm='clipboard authentification'")
        self.end_headers()

    def do_GET(self):
        global key

        if self.path == "/time.txt":
            return SimpleHTTPRequestHandler.do_GET(self)

        if self.headers.get("Authorization") == None:
            self.do_AUTHHEAD()
            self.wfile.write(bytes("not authorized!", "utf-8"))

        elif self.headers.get("Authorization") == "Basic " + str(key):
            return SimpleHTTPRequestHandler.do_GET(self)

        else:
            self.do_AUTHHEAD()
            self.wfile.write(bytes("not authorized!", "utf-8"))

    def log_message(self, format, *args):
        return


if __name__ == "__main__":

    project_dir = os.path.dirname(os.path.realpath(sys.argv[0]))
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 8000

    username = sys.argv[2] if len(sys.argv) > 2 else "admin"
    password = sys.argv[3] if len(sys.argv) > 3 else "password"

    userauth = "%s:%s" % (username, password)
    print("Basic authentification: %s" % userauth)
    key = base64.b64encode(bytes(userauth, "utf-8")).decode("ascii")

    httpd = HTTPServer(("127.0.0.1", port), CORSRequestHandler)
    httpd.serve_forever()
