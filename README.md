#ansvif
        ############################################
        ############################################
        ##    A Not So Very Intelligent Fuzzer    ##
        ############################################
        ############################################

**Compliation:**

**Dependancies:**

automake autoconf-archive zlib1g-dev g++ gcc

*Linux:*
```
$ aclocal && autoconf && automake -a && ./configure && make
```
*OpenBSD:*
```
$ AUTOMAKE_VERSION=`ls /usr/local/bin/automake-* | head -n 1 | sed -e 's/.*-//'`\
AUTOCONF_VERSION=`ls /usr/local/bin/autoconf-* | head -n 1 | sed -e 's/.*-//'`\
aclocal && AUTOMAKE_VERSION=`ls /usr/local/bin/automake-* | head -n 1 | sed -e 's/.*-//'`\
AUTOCONF_VERSION=`ls /usr/local/bin/autoconf-* | head -n 1 | sed -e 's/.*-//'`\
autoconf && AUTOMAKE_VERSION=`ls /usr/local/bin/automake-* | head -n 1 | sed -e 's/.*-//'`\
AUTOCONF_VERSION=`ls /usr/local/bin/autoconf-* | head -n 1 | sed -e 's/.*-//'` automake -a\
&& CXX=eg++ ./configure && make
```
*Windows:*

Windows binaries are now desgined to be compiled with MinGW-W64.

```
$ ./configure_win
$ make
```


Note: cygwin .dll external files are no longer required as we now compile with g++ from MinGW.
In Windows 7 Powershell v2 is installed by default, however, this program requires atleast 
Powershell v5.  Windows 10 includes powershell v5.  You can go to Microsoft's site and download
the Windows Management Framework (which includes newer Versions of Powershell here: 
https://www.microsoft.com/en-us/download/details.aspx?id=50395.*

**Testing:**

If you would like to try out the example code, you can compile faulty.c with:
```
$ gcc faulty.c -o faulty
```

Using the example code:
  You can point the memory back at address `\xff\x05\x40\x00\x00\x00\x00\x00` (the
  subroutine containing the code that spawns bash) with:
  `$ ./faulty -a $(perl -e 'print "A"x24;print "\x00\xff\x05\x40\x00\x00\x00\x00\x00"')`
  The address may be a little different under your distro, check gdb if you really
  want to try it out.  The code above /should/ drop you at a bash prompt.
  If the code is set to a mode where all are able to execute as another user, it will
  attempt to spawn a root shell.
  or simply run `make test`.

**Useage:**

*Important note: Windows users will have to run ansvif.exe from Powershell for it to work!*

```
$ ./find_suid /usr/bin/ /bin/ /sbin/
$ ./ansvif -[tm] [template/manpage] -c /path/to/executable -b buffersize
```
**Examples:**

*Linux/BSD:*
```
$ echo "Marshall" ./ansvif -t examples/template -c ./faulty -b 64
$ ./ansvif -m mount -c /bin/mount -e examples/mount_e.txt -x examples/mount_o.txt\
-f 8 -b 2048
$ ./ansvif -t examples/blank.txt -F tmp/tmphtml -x examples/htmltags.txt -c /usr/bin/iceweasel -b\
128 -A "file:///home/username/src/ansvif/tmp/tmphtml"  -f 2 -n -R "sleep 3 && killall\
iceweasel" -S ">"
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
  -v Verbose.
  -d Debug data.
  -h Shows the help page.
```

**Recommendations:**

**DO NOT RUN THIS CODE IN A PRODUCTION ENVIRONMENT!**
If you try setting faulty.c's output to suid(0) then *PLEASE* do it in a virtual machine.
or atleast a machine that you don't care about.
Other than that, just play around and have fun!


**Notes:**

Windows users must have ansvif running from Powershell and have the cygwin .dll's in the same dir.
Linux code should be relatively stable.
