# rapid-blur

A simple box-blur based on [i3lock-fancy-rapid](https://github.com/yvbbrjdr/i3lock-fancy-rapid).

The script was originally intended to be used with swaylock, but can generally be used for any blurring or with other screen locking utilities.

## Installation
Call `make`in the root directory to build the binary `rapid-blur`.
A subsequent `make install` call will add a symlink to `/usr/bin`.

## Usage
You can generally use the script to blur any image with:

``` sh
rapid-blur IMAGE OUTPUT 
```

To customize the level of blur, one can set the radius with `-r`and times with `-t`
``` sh
rapid-blur -r 5 -t 3 IMAGE OUTPUT
```
To pixelate instead, pass the `-p` flag
``` sh
rapid-blur -p IMAGE OUTPUT
```

### Using with a screen locking utility (swaylock)
To use it with swaylock, you can simply include it in a simple script similar to the one below:

``` sh
#!/bin/sh
DISPLAYS="$(swaymsg -t get_outputs -p | awk '/Output/ && !/disabled/ {print $2}')"
# Reverse Displays (So external 4K is started first)
DISPLAYS=$(tac -s ' ' <<< $(echo $DISPLAYS))

BASE_FILE="${TMPDIR:-/tmp/ss}"
for display in $DISPLAYS; do
    FILE="${BASE_FILE}${display}"
    grim -o "$display" "$FILE.png"
    
    rapid-blur "$FILE.png" "$FILE-out.png" &
	args="$args -i ${display}:${FILE}-out.png"
done
wait
swaylock $args
```
