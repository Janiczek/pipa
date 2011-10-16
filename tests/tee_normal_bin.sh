exec 2>/dev/null;

echo -n ".";

cat tests/data/bin | ./pipa -t out.txt >tee.txt;

S1=`md5sum tests/data/bin | head -c 32`;
S2=`md5sum tee.txt        | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\ntee_normal_bin.sh: tee and binary data.";
fi;

rm *.txt;

exit;
