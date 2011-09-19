pipa
====

This utility reads data from stdin, and in its simplest form just outputs it into a file.

Let it speak for itself:

    $ pipa
    Usage: pipa [-x format]
                [-t]
                [-i secs]
                [-s size]
                [-c user-command]
                [-z compression format (gz | bz2 | xz | lzo)]
                [-# compression level (0..9)]
                [-v]
                [-f]
                filename

Well, that's nice, but really, what are all these settings? Let's walk through them by looking at some examples.

    $ cat input_file | pipa output_file

Really basic one - this just writes from stdin to the file. BOOORING! But let's say you've got some logs and you don't want them to be too big. Well, let's try that.

    $ some_program | pipa -x text -s 100kB log-%H%M%S

Let's see what we've done.

-x format
---------

The `-x text` option says we want the files to always end with a whole line, not truncated somewhere in the middle. Basically it writes only when it has a complete line ready.

Other formats are `-x pcap` for the [PCAP packets](http://wiki.wireshark.org/Development/LibpcapFileFormat#File_Format) (*tcpdump*, anybody?) - it starts the file with PCAP header and writes only when it has packet header + packet data ready; and `-x none`, which is the default option - it reads until it fills the buffer and doesn't do any data-related checks whatsoever.

-s size
-------

In the example above this option says we want the files to have size of approximately 100kB. [Other suffixes](https://github.com/Janiczek/pipa/blob/master/src/helper.c#L75) are MB, GB, TB. Or, of course, no suffix, which just leaves the value in bytes.

The files are *NOT* guaranteed to be always smaller than this size, in fact they will be greater or equal. With `-x none` there is also a problem with the size of internal buffer, which is, for the sake of compression efficiency (see more on compression below) set to 256kB. This means that if you set -s 100kB, it will probably cut the files after the 256kB or when it encounters EOF, not sooner. This will probably change and the buffer will be set to a smaller value.

filename - log-%H%M%S
---------------------

Seems familiar? Right, it's [strftime](http://www.cplusplus.com/reference/clibrary/ctime/strftime/)! So if we open first file at 10:48:31, the file will be named log104831, and when the program finds out it wrote 100kB or more and it should open a new one, the filename will be set by these format specifiers.

There is also a `%i` format specifier - think of it as a counter. Everytime new path is created with this little thing in it, it becomes 1, 2, 3, ...

There is a planned addition (not ready yet) to this - `%2i` will do 01, 02, 03, ...

Oh, and *this* is really important: you can create whole paths with it! For example, `pipa %Y/%m/%d/%H-%M-%S.log` will create the directories automatically - 2011/09/19/, 2011/09/20/, ...

-i time
-------

Okay, let's try something else! We don't want it to rotate by size, but by time. How will we do that? (BTW, this line below is a reason why this program exists in the first place)

    $ tcpdump -i eth0 -w - | pipa -x pcap -z gz -i 5m eth0-%d%m%Y-%H%M%S.pcap.gz

This listens on the tcpdump sniffing on your network interface, and every 5 minutes starts putting it into a new file. 

Well, that sentence wasn't entirely true. pipa aligns the time - if we start at 8:37:10 and we let it run for a few minutes, the files created will be:

    eth0-19092011-083710.pcap.gz
    eth0-19092011-084000.pcap.gz
    eth0-19092011-084500.pcap.gz
    eth0-19092011-085000.pcap.gz

But hey, what was that `-z gz`??

-z - compression format
-----------------------

You can compress the data on the fly. At the moment, pipa supports formats [gz](http://en.wikipedia.org/wiki/Gzip), [bz2](http://en.wikipedia.org/wiki/Bzip2), [xz](http://en.wikipedia.org/wiki/Xz) and [lzo](http://en.wikipedia.org/wiki/Lzop).

This is especially useful for the PCAP files, because they tend to be large and Wireshark can open them even if they're compressed by gz.

Bz2 is another widely used compression format and has better compression ratio, so this might come in handy; lzo is fastest compression format out there; and xz has probably the best compression ratio of them all.

-# - compression level
----------------------

Some of the compression formats let you set the compression level - there is usually some compromise between speed and size. For example, if you want the absolutely best compression you can get, you will probably want this:

    $ some_program | pipa -z xz -9 file.txt

*Notice: LZO doesn't support this option, you setting it won't crash the program, but it won't change anything.*

-t - tee mode
-------------

Sometimes we want to send the input somewhere else too - pipa has built-in tee mode: whatever goes in, goes out too. So not only we do our magic with it inside pipa, we write it (the input, unchanged) on stdout too.

    $ seq 1000 | pipa -t after.txt >before.txt

In this case, the after.txt and before.txt will be absolutely the same.

    $ seq 5 | pipa -t file.txt
    1
    2
    3
    4
    5

-v - verbose mode
-----------------

This option tells pipa to inform us about opening and closing of the files. Simple as that.

    $ seq 1000 | pipa -v %Y.txt
    2011.txt: Opening
    2011.txt: Closing

-f - force overwrite
--------------------

Normally, if the filename we want to write to exists, pipa returns an error.

    $ touch imhere.txt
    $ seq 5 | pipa imhere.txt
    imhere.txt: Filename exists - aborting

But when we use -f option, look what happens!

    $ touch imheretoo.txt
    $ seq 5 | pipa -f imheretoo.txt
    imheretoo.txt: Filename exists - overwriting

-c - user command
-----------------

And finally, you can tell pipa to run something after it finishes the file. It puts the filename as the argument. Let's say you have a script mybashscript with `echo $1` inside. Observe:

    $ some_program | pipa -i 5m -c ./mybashscript.sh file%H%M%S.txt
    file083911.txt
    file084000.txt
    file084500.txt

Of course, this is equivalent to `-c echo`, but you get the idea. And you're limited only by the possibilities of the shell. You could be scp'ing it somewhere, or just moving to a backup folder, or WHATEVER.

TODO
====

1. BUFSIZE - smaller value? set by user or by the "-s" value?
2. Why can't we just set the `Z_XZ_OUT_LEN` to the `BUFSIZE?`
3. Make sure we're not opening the same file when rotating! In this case we should probably not even close it (even without the -f option), and continue writing and checking if the 'to-be-new' filename changes.
4. %2i etc.