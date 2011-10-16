exec 2>/dev/null;

echo -n ".";

cat tests/data/pcap_big | ./pipa -x pcap -s 1000000 -t A%i.pcap >tee.pcap;

S1=`md5sum tests/data/pcap_big | head -c 32`;
S2=`md5sum tee.pcap            | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\ntee_headers.sh: tee and file format with headers";
fi;

rm *.pcap;

exit;
