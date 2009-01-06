#!/bin/sh

for i in `find . | egrep -e "(\.h|\.cpp)$"`;
do
	cat $i >> /tmp/globe.txt
done

echo -e -n "Code lines (with commets):\t"`cat /tmp/globe.txt | wc -l`
echo

cat /tmp/globe.txt | ./remccoms3.sed >> /tmp/globe_stripped.txt

echo -e -n "Code lines (without commets):\t"`cat /tmp/globe_stripped.txt | wc -l`
echo 


#less /tmp/globe_stripped.txt
rm -f /tmp/globe.txt
rm -f /tmp/globe_stripped.txt

