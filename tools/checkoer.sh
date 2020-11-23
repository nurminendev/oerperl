#!/bin/sh

#
# checkoer.sh will return 0 if oer isn't running,
# otherwise it will return 1
#
# run this script in the oer directory
#

if [ ! -f ./oer.pid ]; then
	exit 0
fi

OERPID=`cat ./oer.pid`

kill -HUP $OERPID

if [ $? -eq 0 ]; then
	exit 1
fi

exit 0
