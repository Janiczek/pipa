exec 2>/dev/null;

echo -n ".";

./pipa -n 1800 out.txt &;
PID_PIPA=$!;

cat tests/data/seq | nc localhost 1800 &
PID_NC=$!;

kill PID_NC;
kill PID_PIPA; # should end on its own, but to be sure ... :)

S1=`md5sum tests/data/seq | head -c 32`;
S2=`md5sum out.txt        | head -c 32`;

if [ "$S1" != "$S2" ];
  then echo -e "\nsocket.sh: send over socket instead of stdin";
fi;

rm *.txt;

exit;
