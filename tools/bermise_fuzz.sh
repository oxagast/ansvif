## bermise_fuzz
## Part of the ansvif tool chain
##    __ _  _  __   ___  __  ____ ____
##   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
##  (  O )  (/    ( (_ /    \___ \ )(
##   \__(_/\_\_/\_/\___\_/\_(____/(__)

## This should be run as root to operate
## correctly if using things like netcat
## because we have to read /var/log/syslog
## to detect service crashes.

if [ $# -eq 0 ]
  then
    echo "ansvif toolchain -- A Not So Very Intelligent Fuzzer";
    echo "Usage:";
    echo " $0 iconv -f Unicode";
    echo " $0 nc 127.0.0.1 22";
  exit 0;
fi;


# You can comment this out if you know what
# you're doing.
if [ `whoami` != "root" ]
  then
    echo "Not root, exiting.";
  exit 0;
fi;

mupr="64";

syslog_crashes=$(grep "traps:\|segfault" /var/log/syslog -c);
binname="$1";
binshort=$(echo $binname | awk -F "/" '{print $NF}')
echo fuzzing $binname;
randchars=1;
while [ $randchars -le 2 ]
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
od -x ./$binshort.crash.bf;
