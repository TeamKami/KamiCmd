#/bin/sh

LOWER=`echo $1 | awk 'END { print tolower($0)}'`
echo '#include "'$LOWER'.h"' > $2/$1
