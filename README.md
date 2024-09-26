# rapid-blur

A simple box-blur based on [i3lock-fancy-rapid](https://github.com/yvbbrjdr/i3lock-fancy-rapid).

Intended to be used with swaylock for a blurred background image. 

At the moment it works on raw rgb files only.

A simple script could look like this:

``` sh
#!/bin/sh

DISPLAYS="$(swaymsg -t get_outputs -p | grep "Output" | awk '{print $2}')"
BASE_FILE="${TMPDIR:-/tmp/ss}"

for display in $DISPLAYS; do
    FILE="${BASE_FILE}${display}"
    grim -o "$display" "$FILE.png"
    WIDTH=$(identify -format '%w' "$FILE.png")
    HEIGHT=$(identify -format "%[fx:h]" "$FILE.png")
    magick "$FILE.png" "$FILE.rgb"
    rapid-blur -i "$FILE.rgb" -o "$FILE-out.rgb" -w "$WIDTH" -h "$HEIGHT"
    magick -size "$WIDTH"x"$HEIGHT" -depth 8 "$FILE-out.rgb" "$FILE.png"
    args="$args -i ${display}:${FILE}.png"
done

swaylock $args "$@"
```

It may be advisable to do some scaling for 4k displays
The calls to imagemagick take the longest time, moving the image handling into the c code is the next TODO
