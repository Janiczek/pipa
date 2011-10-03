echo -n "file format none - ";

cat tests/data/bin | ./pipa -s 1024 %i.txt.part;

ls | egrep "^[0-9].txt.part" | while read filename; do mv $filename 0$filename; done;

cat *.txt.part >new.txt;

S1=`md5sum tests/data/bin | head -c 32`;
S2=`md5sum new.txt        | head -c 32`;

if [ "$S1" = "$S2" ];
  then result=1;
  else result=0;
fi;

rm *.txt*

if [ $result -eq 1 ]; then

  cat tests/data/seq | ./pipa -s 1024 %i.txt.part;

  ls | egrep "^[0-9].txt.part" | while read filename; do mv $filename 0$filename; done;

  cat *.txt.part >new.txt;

  S1=`md5sum tests/data/seq | head -c 32`;
  S2=`md5sum new.txt        | head -c 32`;

  if [ "$S1" = "$S2" ];
    then echo "PASS";
    else echo "ERROR";
  fi;

  rm *.txt*

fi;
