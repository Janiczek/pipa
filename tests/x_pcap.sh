echo -n "file format pcap - ";

cat tests/data/pcap_small | ./pipa -x pcap new.pcap;

S1=`md5sum tests/data/pcap_small | head -c 32`;
S2=`md5sum new.pcap              | head -c 32`;

if [ "$S1" = "$S2" ];
  then result=1;
  else result=0;
fi;

rm new*;

if [ $result -eq 1 ]; then

  cat tests/data/pcap_big | ./pipa -s 1000000 -x pcap A%i.part;

  mergecap -w glued.pcap *.part;

  S1=`md5sum tests/data/pcap_big | head -c 32`;
  S2=`md5sum glued.pcap          | head -c 32`;

  if [ "$S1" = "$S2" ];
    then echo "PASS";
    else echo "ERROR";
  fi;

  rm *.part;
  rm glued.pcap;

fi;
