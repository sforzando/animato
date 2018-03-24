# How to Setup

## Requirements

* Xcode
* HomeBrew
  * `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
* gphoto2
  * to take photos from DSLR
  * `brew install gphoto2`
* qrencode
  * to generate QR
  * `brew install qrencode`
* FFmpeg
  * to encode GIF
  * `brew install ffmpeg`

## Command
### gphoto2
`gphoto2 --auto-detect`
`sudo env LANG=C gphoto2 --summary`
`killall PTPCamera`
