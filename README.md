# clipboard sharing under restriction one-way

## use case
assume your are working with 2 different computers in 2 different networks with one has root permissions and the other one is restricted as much as possible, like you **aren't** able:
- to install your own applications
- to visit websites for sharing content platforms
- to visit websites urls which tunnels to your localhost via e.g. `ngrok`

in adition, you aren't able to configure your router to accept trafic from outsite. you only want to share your clipboard from high privileged computer to low privileged computer.

in such a confusing situation you can use this script.

## how it works
- a c++ application based on `Qt` will watch your clipboard
- a clipboard can hold 3 types of data: `image, html, text`
- the c++ application will render predefined templates on clipboard change depending on the data type
- the output will be exposed via simple http server to your localhost on port `8082`
- over `ssh.localhost.run` your localhost will be ssh tunneled to outsite with a generic url, so you will be able to connect with your second computer over this url

## usage
**for privileged computer:**
just run: 

`./run.sh`

**for restricted computer:**
open the url given by `./run.sh`. for now, you need to refresh via `F5 / ctrl+R` manually.

**information:** limitation on macOS: you need to put the application `clipboard_transport` to the foreground. more information click [here](https://doc.qt.io/qt-5/qclipboard.html#dataChanged).

**todos:**
- [ ] add binaries for linux
- [ ] image: support different image formats (works only with png now)
- [x] add codemirror to data type `text`
- [x] add galery template for data type `image`
- [x] add download button for data type `image`
- [x] add template for data type `html`