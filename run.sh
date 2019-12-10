#!/usr/bin/env sh
 ./builds/macos/clipboard_transport -i ./assets/in -o ./assets/out/ &
python -m http.server --directory ./assets/out/ 8082 &2> /dev/null &
ssh -R 80:localhost:8082 ssh.localhost.run && killall python