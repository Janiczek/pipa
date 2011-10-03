echo -n "tee - ";

cat tests/data/bin | ./pipa -t out.txt >tee.txt;

S1=`md5sum tests/data/bin | head -c 32`;
S2=`md5sum tee.txt        | head -c 32`;

if [ "$S1" = "$S2" ];
  then result=1;
  else result=0;
fi;

rm *.txt

if [ $result -eq 1 ]; then

  cat tests/data/seq | ./pipa -t out.txt >tee.txt;

  S1=`md5sum tests/data/seq | head -c 32`;
  S2=`md5sum tee.txt        | head -c 32`;

  if [ "$S1" = "$S2" ];
    then echo "PASS";
    else echo "ERROR";
  fi;

  rm *.txt

fi;
