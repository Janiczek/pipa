CCFLAGS = '-Wall -O2 -g'
LINKFLAGS = '-Wall -lz -llzo2 -llzma -lbz2'
env = Environment(CCFLAGS=CCFLAGS, LINKFLAGS=LINKFLAGS)
env.Program('pipa', Glob('src/*.c'))
