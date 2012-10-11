echo -e "This is a test suite for pipa.\n";
echo -e "If a test has ended with an error, it gets written below.";
echo -e "Successful tests don't show up here.\n";

for f in tests/*.sh;
do
  ./$f;
done;

echo -e "\n\nDone testing!";
