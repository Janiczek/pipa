INSTALL
=======

pipa uses SCons for compiling, and the installation is really simple.

    $ cd /path/to/SConstruct
    $ scons

There are some dependencies, though: (you may need to install "-devel" packages)

* zlib
* lzo
* bzip2
* xz

If the `scons` command says `scons: done building targets.`, you're done! Well, not really. the pipa binary stays in the current directory, so if you want to use it globally, you have to copy it to /usr/bin yourself.

    $ cp ./pipa /usr/bin/pipa

After compiling you can test if all works as it should: (dependencies: **md5sum** for all the tests, **mergecap** (part of wireshark) for x_pcap* tests)

    $ ./all.sh

All the tests should return "PASS" instead of "ERROR".
