exec 2>/dev/null;

echo -n ".";

cat tests/data/seq | ./pipa -z xz -s 1024 %i.txt.part.xz;

xz -d *.txt.part.xz;

ls | egrep "^[0-9].txt.part" | while read filename; do mv $filename 0$filename; done;

cat *.txt.part >new.txt;

S1=`md5sum tests/data/seq | head -c 32`;
S2=`md5sum new.txt        | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nz_xz.sh: compression format .xz";
fi;

rm *.txt*

exit;
