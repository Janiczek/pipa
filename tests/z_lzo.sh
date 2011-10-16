exec 2>/dev/null;

echo -n ".";

cat tests/data/seq | ./pipa -z lzo -s 1024 %i.txt.part.lzo;

lzop -d *.txt.part.lzo;

ls | egrep "^[0-9].txt.part" | while read filename; do mv $filename 0$filename; done;

cat *.txt.part >new.txt;

S1=`md5sum tests/data/seq | head -c 32`;
S2=`md5sum new.txt        | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nz_lzo.sh: compression format .lzo";
fi;

rm *.txt*;

exit;
