#!/usr/bin/env sh
CURRENT_PATH=$(pwd)
OS_NAME=linux; # supported: macos, linux


killall clipboard_transport;
killall python3;
cd $CURRENT_PATH/assets/out;

$CURRENT_PATH/binaries/$OS_NAME/clipboard_transport -i $CURRENT_PATH/assets/in -o $CURRENT_PATH/assets/out/ & 
python3 $CURRENT_PATH/server.py 8082 &
ssh -R 80:localhost:8082 ssh.localhost.run;

# optional: delete your image history
# rm -rf $CURRENT_PATH/assets/out/img 