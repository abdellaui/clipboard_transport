# one-way clipboard sharing under restriction

## use case
assume your are working with 2 different computers in 2 different networks with one has root permissions and the other one is restricted as much as possible, like you **aren't** able:
- to install your own applications
- to visit websites for sharing content
- to call urls which tunnels to your localhost via e.g. `ngrok`

in adition, you aren't able to configure your router to accept trafic from outsite. beside that, you only want to share your clipboard in one-way from high privileged computer to low privileged computer.

in such a confusing situation this script will make your day.

## how it works
- a c++ application based on `Qt` will watch your clipboard
- a clipboard can hold 3 types of data. this are: `image, html, text`
- the c++ application will render predefined templates on clipboard change depending on the data type. difined in: `./assets/in/*`
- the output will be exposed via simple http server to your localhost on port `8082`.
- over `ssh.localhost.run` your localhost will be ssh tunneled to outsite with a generic url, so you will be able to connect with your second computer over this url

## authentification
change basic auth information under `run.sh`. per defaul your authentification data is `echo $(whoami):$(whoami)`

## usage
**first of all:**
change in `./run.sh` your `OS_NAME=linux # supported: macos, linux`.

**for privileged computer:**
just run:  `./run.sh` 

**for restricted computer:**
open the url given by `./run.sh`. ~~for now, you need to refresh via `F5 / ctrl+R` manually.~~ it will refresh automatically, if it detects changes _(checks every 0.5 second)_

**information:** limitation on macOS: you need to put the application `clipboard_transport` to the foreground. more information click [here](https://doc.qt.io/qt-5/qclipboard.html#dataChanged).
## special information:

under **linux** you can have an application for adding that to the favorits by running:

**please update the last line of `./Clipboard_Transport.desktop`**
```
sudo mv ./Clipboard_Transport.desktop ~/.local/share/applications/
sudo update-desktop-database
```

under **unix** based systems you can add it to your binaries by running:

```
ln -s ~/clipboard_transport/run.sh /usr/local/bin/clipboard_transport -f
```

**todos:**
- [ ] automatically copying in clipboard via browser
- [x] add binaries for linux
- [x] image: support different image formats (works only with png now)
- [x] auto reload 
- [x] add codemirror to data type `text`
- [x] add galery template for data type `image`
- [x] add download button for data type `image`
- [x] add template for data type `html`