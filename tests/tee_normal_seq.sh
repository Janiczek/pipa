exec 2>/dev/null;

echo -n ".";

cat tests/data/seq | ./pipa -t out.txt >tee.txt;

S1=`md5sum tests/data/seq | head -c 32`;
S2=`md5sum tee.txt        | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\ntee_normal_seq.sh: tee and plaintext data.";
fi;

rm *.txt;

exit;
