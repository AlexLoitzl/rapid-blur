# rapid-blur

A simple box-blur based on [i3lock-fancy-rapid](https://github.com/yvbbrjdr/i3lock-fancy-rapid).

Intended to be used with swaylock for a blurred background image. 
At the moment it works on raw rgb files only.

## Installation
Call `make`in the root directory to build the binary `rapid-blur`.
A subsequent `make install` call will add a symling to `/usr/bin`.

## Usage
You can generally use the script to blur any image with:

``` sh
rapid-blur -i <IMAGE> -o <OUTPUT> -w <WIDTH> -h <HEIGHT>
```

To use it with swaylock, you can simply include it in a simple script similar to the one below:

``` sh
#!/bin/sh

DISPLAYS="$(swaymsg -t get_outputs -p | grep "Output" | awk '{print $2}')"
BASE_FILE="${TMPDIR:-/tmp/ss}"

for display in $DISPLAYS; do
    FILE="${BASE_FILE}${display}"
    grim -o "$display" "$FILE.png"
    WIDTH=$(identify -format '%w' "$FILE.png")
    HEIGHT=$(identify -format "%[fx:h]" "$FILE.png")

    if [ $WIDTH -gt 1920 ]; then
        # For 4K screens scale before blurring
        WIDTH=$(($WIDTH /2))
        HEIGHT=$(($HEIGHT /2))
        magick "$FILE.png" -scale 50% "$FILE.rgb"
        rapid-blur -i "$FILE.rgb" -o "$FILE-out.rgb" -w "$WIDTH" -h "$HEIGHT" -r 3 -t 1
        magick -size "$WIDTH"x"$HEIGHT" -depth 8 "$FILE-out.rgb" -scale 200% "$FILE.png"
    else
        magick "$FILE.png" "$FILE.rgb"
        rapid-blur -i "$FILE.rgb" -o "$FILE-out.rgb" -w "$WIDTH" -h "$HEIGHT"
        magick -size "$WIDTH"x"$HEIGHT" -depth 8 "$FILE-out.rgb" "$FILE.png"
    fi
    args="$args -i ${display}:${FILE}.png"
done

swaylock $args"$@"
done

swaylock $args "$@"
```

The calls to imagemagick take the longest time, moving the image handling into the c code is the next TODO
