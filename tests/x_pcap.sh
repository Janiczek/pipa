exec 2>/dev/null;

echo -n ".";

cat tests/data/pcap_small | ./pipa -x pcap new.pcap;

S1=`md5sum tests/data/pcap_small | head -c 32`;
S2=`md5sum new.pcap              | head -c 32`;

if [ "$S1" = "$S2" ];
  then result=0;
  else
    result=1;
    echo -e "\nx_pcap.sh: PCAP file format without rotating.";
fi;

rm new*;

if [ $result -eq 0 ]; then

  cat tests/data/pcap_big | ./pipa -s 1000000 -x pcap A%i.part;

  mergecap -w glued.pcap *.part;

  S1=`md5sum tests/data/pcap_big | head -c 32`;
  S2=`md5sum glued.pcap          | head -c 32`;

  if [ "$S1" != "$S2" ];
    then echo -e "\nx_pcap.sh: PCAP file format with rotating.";
  fi;

  rm *.part;
  rm glued.pcap;

fi;

exit;
