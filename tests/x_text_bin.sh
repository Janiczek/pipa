exec 2>/dev/null;

echo -n ".";

cat tests/data/bin | ./pipa -x text -s 8192 %i.txt.part;

ls | egrep "^[0-9].txt.part" | while read filename; do mv $filename 0$filename; done;

cat *.txt.part >new.txt;

S1=`md5sum tests/data/bin | head -c 32`;
S2=`md5sum new.txt        | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nx_text_bin.sh: text file format and binary data.";
fi;

rm *.txt*;

exit;
