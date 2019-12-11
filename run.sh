#!/usr/bin/env sh
OS_NAME=linux; # supported: macos, linux

./binaries/$OS_NAME/clipboard_transport -i ./assets/in -o ./assets/out/ &
cd ./assets/out && python ../../server.py 8082 &2> /dev/null &
ssh -R 80:localhost:8082 ssh.localhost.run && rm -rf ./assets/out/img && 
killall clipboard_transport && 
killall python