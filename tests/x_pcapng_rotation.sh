exec 2>/dev/null;

echo -n ".";

cat tests/data/pcapng_big | ./pipa -s 1000000 -x pcapng A%i.part;

mergecap -F pcapng -w glued.pcapng *.part;

S1=`capinfos -c tests/data/pcapng_big | awk 'END{print $4}'`;
S2=`capinfos -c glued.pcapng          | awk 'END{print $4}'`;

if [ "$S1" != "$S2" ];
  then echo -e "\nx_pcapng_rotation.sh: PCAP-NG file format with rotating.";
fi;

rm *.part;
rm glued.pcapng;

exit;
