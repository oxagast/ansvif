# ansvif
*A Not So Very Intelligent Fuzzer*

Marshall Whittaker

oxagast

marshallwhittaker@gmail.com

*As the complexity of a system rises, as does the potential for problems within that system.*

*--Whittaker's Law*


**Compile Dependancies:**

automake autoconf-archive zlib1g-dev g++ gcc (and libgtk2.0-dev if you want GTK support)

**Compliation:**

*Linux:*
```
$ aclocal && autoconf && automake -a && ./configure && make && make check
```
Or, if you would like to play with the syscall fuzzer:
```
$ aclocal && autoconf && automake -a && ./configure --enable-syscalls && make && make check
```
If you would like to disable the GTK frontend you can do:
```
$ aclocal && autoconf && automake -a && ./configure --disable-gtk && make && make check
```
*FreeBSD*

Assuming you installed g++ from ports (as you will need to for C++11):

```
$ aclocal && autoconf && automake -a && ./configure && make && make check
```

*OpenBSD:*

Assuming you installed g++ from ports (as you will need to for C++11):

```
$ CXX=$(find / -name 'eg++' 2>/dev/null | grep ports | head -n 1) AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.15 autoreconf -fmi
```
*Windows:*

Windows binaries are now desgined to be compiled with MinGW-W64 (since we use threading download
a version of MinGW-W64 g++ with seh).
You can try compiling after installing MinGW-W64 g++ with seh (only) by clicking on the included make_win.bat script.  If that fails, it is likely due to the environment, so try the below.

(Go to where you installed MinGW-W64 and click mingw-64.bat)

```
windres metadata.rc -O coff -o metadata.res
g++.exe src/common.cpp src/bin2hex.cpp src/popen2.cpp src/main.cpp src/help.cpp src/match_fault.cpp src/sys_string.cpp src/man_read.cpp src/randomizer.cpp src/trash.cpp src/log.cpp metadata.res src/version.h -I./ -I./include -std=c++11 -lstdc++ -lpthread -O2 -o ansvif.exe -static -static-libgcc -static-libstdc++
gcc src/win/printf.c -o printf.exe
```


Note: cygwin .dll external files are no longer required as we now compile with g++ from MinGW.
In Windows 7 Powershell v2 is installed by default, however, this program requires atleast 
Powershell v5.  Windows 10 includes powershell v5.  You can go to Microsoft's site and download
the Windows Management Framework (which includes newer Versions of Powershell here: 

https://www.microsoft.com/en-us/download/details.aspx?id=50395

**Testing:**

If you would like to try out the example code, you can compile faulty.c with:
```
$ gcc faulty.c -o faulty
```
You can also simply run `make check`.

**Useage:**

*Important note: Windows users will have to run ansvif.exe from Powershell for it to work!*

```
$ ./find_suid /usr/bin/ /bin/ /sbin/
$ ./ansvif -[tm] [template/manpage] -c /path/to/executable -b buffersize
```
**Examples:**

*Linux/BSD:*
```
$ ulimit -c unlimited
$ ./ansvif -m mount -c /bin/mount -e examples/mount_e.txt -x examples/mount_o.txt\
-f 8 -b 2048
$ ./ansvif -t examples/blank.txt -F tmp/tmphtml -x examples/htmltags.txt -c /usr/bin/iceweasel -b\
128 -A "file:///home/username/src/ansvif/tmp/tmphtml"  -f 2 -n -R "sleep 3 && killall\
iceweasel" -S ">"
$ cat examples/linux_syscalls_implemented.list | xargs -P \
`cat examples/linux_syscalls_implemented.list | wc -l` -I {calls} ./ansvif -t examples/space.txt \
-B "{calls} " -c ./syscalls -o syscall_crash -f 1 -z -d -b 16
```

*Windows:*
```
PS C:\ansvif\bin\ansvif_win> .\ansvif -t ..\..\examples\space -F ..\..\tmp\tmphtml -x `
..\..\examples\htmltags -c `
'C:\Program Files (x86)\Google\Chrome\Application\chrome.exe' `
-b 128 -A "file:///C:\\Users\marsh\OneDrive\Documents\Code\ansvif\tmp\tmphtml" `
-f 2 -n -S ">" -R "sleep 2 ; Stop-Process -Name chrome"
```

**Options:**

ansvif

```
  -t This file should hold line by line command arguments as shown in the example file.
  -e This file should hold line by line environment variables as shown in the example
     file.  You can usually get these by doing something like:
     $ strings /bin/mount | perl -ne 'print if /[A-Z]=$/' > mount_envs
  -c Specifies the command path.
  -p Specifies the manpage location (as an integer, usually 1 or 8)
  -m Specifies the commands manpage.
  -D Dumps whats found in the manpage.
  -f Number of threads to use.  Default is 2.
  -b Specifies the buffer size to fuzz with.  256-2048 Is usually sufficient.
  -r Uses only random garbage data.
  -o Writes output to log file.
  -z Randomize the buffer size from 1 to what is specified by -b.
  -x Other junk to put in.  Usernames and such can go here.
  -S Seperator between options.
  -s Omitted character specification.  Defaults are <>\\n |&\[]\()\{}:;\ and newline is mandatory.
  -T Timeout for threads.
  -W Timeout for threads.
  -L Unpriviledged user to run as if root.
  -A Always put whats after this after command to run.
  -B Always put whats after this before the command to run.
  -F File to feed into the program that -x along with normal fuzzing data will be put in.
  -n Never use random data in the fuzz.
  -R Run this command after each fuzz.
  -C A Non standard error code to detect.
  -V Use Valgrind if installed.
  -1 Try to make it fault once, if it doesn't happen, throw error code 64.  Useful for scripting.
  -P Use % to represent binary in fuzz.
  -M Max arguments to use in the fuzz.
  -y Short for -b 0 and usually only useful with -A or -B.
  -K Keep fuzzing after a crash in the target.
  -E A command to be run before the fuzzed program.
  -0 No NULL characters in the fuzz.
  -N Shorthand for -R "pkill prog".
  -v Verbose.
  -d Debug data.
  -h Shows the help page.
  -i Prints version information.
```
ansvif_gtk

```
  -l Shortcut for -p ./ansvif
  -p The location of the ansvif binary
```

**Recommendations:**
It is recommended that if you are doing long fuzzes or file fuzzing, if possible
put the files (including the binary you are fuzzing if possible) in memory.
This means, put them somewhere like /var/run/shm where disk thrash will be
minimal, and fuzzing will be somewhat faster, especially if large files are being
handled.  Be warned however: You will lose your fuzzed files if they are in shm
and you reboot the machine!

**DO NOT RUN THIS CODE IN A PRODUCTION ENVIRONMENT!**
If you try setting faulty.c's output to suid(0) then *PLEASE* do it in a virtual machine.
or atleast a machine that you don't care about.
Other than that, just play around and have fun!


**Notes:**

Windows users must have ansvif running from Powershell.
Linux and Windows code should be relatively stable.
Syscall fuzzing under linux is under heavy development.


**Thanks**

Thanks to dll999 for syscall fuzzing ideas.
Thanks to moo from #2600 on 2600net for a few recommendations on the wiki writeup.
Thanks to god knows how many people on IRC and StackOverflow.
Thanks to DarkSt0rm for fixing a bug in the Makefile.
