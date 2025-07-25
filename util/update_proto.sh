#!/bin/bash
[[ -z "$1" ]] && exit 1
which cproto > /dev/null || alias cproto="docker run cproto -v $PWD:/cproto"
tmp=$(mktemp)
sed '/\/\* prototypes \*\//q' $1 > $tmp
cproto -F"int\tf(a, b)" *.c | \
	   sed -E 's/^([a-z_]+) /\1\t/' | \
	   sed 's/^int\t/int\t\t/' | \
	   grep -v 'main(' >> $tmp
echo '#endif' >> $tmp
cp $tmp $1
