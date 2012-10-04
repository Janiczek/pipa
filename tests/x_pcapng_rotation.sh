exec 2>/dev/null;

echo -n ".";

cat tests/data/pcapng_big | ./pipa -s 1000000 -x pcapng A%i.part;

mergecap -F pcapng -w glued.pcapng *.part;

S1=`md5sum tests/data/pcapng_big | head -c 32`;
S2=`md5sum glued.pcapng          | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nx_pcapng_rotation.sh: PCAP-NG file format with rotating.";
fi;

rm *.part;
rm glued.pcapng;

exit;
