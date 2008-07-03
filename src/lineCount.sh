for i in `find . | egrep -e "(\.h|\.cpp)$"`;
do
	cat $i >> /tmp/globe.txt
done

echo -e -n "Code lines (with commets):\t"`cat /tmp/globe.txt | wc -l`
echo
echo -e -n "Code lines (without commets):\t"`cat /tmp/globe.txt | ./remccoms3.sed | wc -l`
echo 

rm -f /tmp/globe.txt

