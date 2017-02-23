#!/bin/bash

EXEC="google"
CODE="$EXEC.cpp"
COMMAND="time"

# Generate the executable
echo
echo -e '\E[;32m'"[*] Generating executable...";tput sgr0
g++ -std=c++11 -o $EXEC -O3 $CODE

if [ $? == 0 ]; then
# Run the executable for all the files
# and get timing
tput bold;  echo -e '\E[;32m'"[*] Running on examples...";tput sgr0
echo
for file in `ls ./input`
do
	NAME=`echo $file | sed -e 's/.in//'`
	tput bold; echo -n "-> $file: "; tput sgr0
	$COMMAND --format "\t%E" ./$EXEC < ./input/$file > /dev/null
	if [ $? == 0 ]; then
		./$EXEC < ./input/$file > output/"$NAME"_result.out
	fi
done
echo
fi
echo -e '\E[;32m'"[*] Ending."; tput sgr0
echo
