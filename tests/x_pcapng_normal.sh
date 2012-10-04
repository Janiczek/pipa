exec 2>/dev/null;

echo -n ".";

cat tests/data/pcapng_small | ./pipa -x pcapng new.pcapng;

S1=`capinfos -c tests/data/pcapng_small | awk 'END{print $4}'`;
S2=`capinfos -c new.pcapng              | awk 'END{print $4}'`;

if [ "$S1" != "$S2" ];
  then echo -e "\nx_pcapng_normal.sh: PCAP-NG file format without rotating.";
fi;

rm new*;

exit;
