if [ $# -eq 0 ]
  then
    echo "ansvif toolchain -- A Not So Very Intelligent Fuzzer";
    echo "Usage:";
    echo " $0 /bin /usr/bin";
  exit 0;
fi;
find $@ -perm /6000 2>/dev/null
