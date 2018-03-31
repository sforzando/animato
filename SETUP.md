# How to Setup

- [Requirements](#requirements)
- [Command](#command)
    - [ffmpeg](#ffmpeg)
        - [Make GIF](#make-gif)
        - [Make MP4](#make-mp4)
    - [qrencode](#qrencode)
    - [lpr](#lpr)
    - [scp](#scp)

## Requirements

* Xcode
* HomeBrew
  * `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
* qrencode
  * to generate QR
  * `brew install qrencode`
* FFmpeg
  * to encode GIF
  * `brew install ffmpeg`

## Command

### ffmpeg

#### Make GIF

At first, make color palette.
`ffmpeg -i %02d.png -vf palettegen -y palette.png`
`-y` is to override.

`ffmpeg -f image2 -r 8 -i %02d.png -i palette.png -filter_complex paletteuse output.gif`
`-r` means the framerate.

#### Make MP4

(T.B.D.)

### qrencode

`qrencode -o qr.png -m 2 "https://example.com/"`
`-m` means the width of margins. `2` is Micro.

### lpr

`lpr -o media=DC20 -o PageSize=DC20 -o fitplot qr.png`

### scp

`scp -i id_rsa *.mp4 username@servername:/var/www/html`
`id_rsa` should be `600`.
