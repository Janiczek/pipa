exec 2>/dev/null;

echo -n ".";

cat tests/data/pcapng_small | ./pipa -x pcapng new.pcapng;

S1=`md5sum tests/data/pcapng_small | head -c 32`;
S2=`md5sum new.pcapng              | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nx_pcapng_normal.sh: PCAP-NG file format without rotating.";
fi;

rm new*;

exit;
