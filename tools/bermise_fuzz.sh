## bermise_fuzz
## Part of the ansvif tool chain
##    __ _  _  __   ___  __  ____ ____
##   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
##  (  O )  (/    ( (_ /    \___ \ )(
##   \__(_/\_\_/\_/\___\_/\_(____/(__)



mupr="64";

syslog_crashes=$(grep "traps:\|segfault" /var/log/syslog -c);
binname="$1";
binshort=$(echo $binname | awk -F "/" '{print $NF}')
echo fuzzing $binname;
randchars=1;
while [ $randchars -le 1 ]
do
kill_timer=`sleep 0.5; killall $binshort 2>/dev/null`;
fuzz=`dd if=/dev/urandom bs=1 count=$mupr 2>/dev/null`;
echo $fuzz > $binshort.tmp;
echo $fuzz | $@; echo $?;
mv $binshort.tmp $binshort.crash.bf;
cat $binshort.out.bf;
if [ `grep "traps:\|segfault" /var/log/syslog -c;` -gt $syslog_crashes ]
then
grep "traps:" /var/log/syslog | tail -n 1;
break;
fi;
done;
echo "Crash found!"
echo "Crash buffer left in: $binshort.crash.bf";
echo "Crash output left in: $binshort.out.bf";
echo;
echo "Crash data:";
od -x ./$binshort.crash.bf
