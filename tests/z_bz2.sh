exec 2>/dev/null;

echo -n ".";

cat tests/data/seq | ./pipa -z bz2 -s 1024 %i.txt.part.bz2;

bzip2 -d *.txt.part.bz2;

ls | egrep "^[0-9].txt.part" | while read filename; do mv $filename 0$filename; done;

cat *.txt.part >new.txt;

S1=`md5sum tests/data/seq | head -c 32`;
S2=`md5sum new.txt        | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nz_bz2.sh: compression format .bz2";
fi;

rm *.txt*;

exit;
