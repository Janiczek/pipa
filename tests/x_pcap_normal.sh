exec 2>/dev/null;

echo -n ".";

cat tests/data/pcap_small | ./pipa -x pcap new.pcap;

S1=`md5sum tests/data/pcap_small | head -c 32`;
S2=`md5sum new.pcap              | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nx_pcap_normal.sh: PCAP file format without rotating.";
fi;

rm new*;

exit;
