#!/usr/bin/env sh
CURRENT_PATH=$(realpath $(dirname $0))
OS_NAME=linux; # supported: macos, linux
HTTP_PORT=8082;

killall clipboard_transport;
killall python3;
cd $CURRENT_PATH/assets/out;

$CURRENT_PATH/binaries/$OS_NAME/clipboard_transport -i $CURRENT_PATH/assets/in -o $CURRENT_PATH/assets/out/ & 
python3 $CURRENT_PATH/server.py $HTTP_PORT &
ssh -R 80:localhost:$HTTP_PORT ssh.localhost.run;

# optional: delete your image history
# rm -rf $CURRENT_PATH/assets/out/img 