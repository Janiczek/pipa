exec 2>/dev/null;

echo -n ".";

cat tests/data/pcap_big | ./pipa -s 1000000 -x pcap A%i.part;

mergecap -w glued.pcap *.part;

S1=`md5sum tests/data/pcap_big | head -c 32`;
S2=`md5sum glued.pcap          | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nx_pcap_rotation.sh: PCAP file format with rotating.";
fi;

rm *.part;
rm glued.pcap;

exit;
