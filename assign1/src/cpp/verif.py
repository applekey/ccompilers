import subprocess
debug = 1

def command(input):
        #import os
        #os.system(input)
        if debug == 1:
                print input

        p = subprocess.Popen(input, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        lines = p.stdout.readlines();
        retval = p.wait()
        return lines

makeClean = 'make clean'
make = 'make'


####################################HOMEWORK SCRIPT BEGINS#################################################

make = 'make'
makeClean = 'make clean'

ref1 = ' assign1Ref t1.tmp > r1.out'
ref2 = ' assign1Ref t2.tmp > r2.out'
ref3 = ' assign1Ref t3.tmp > r3.out'
ref4 = ' assign1Ref t4.tmp > r4.out'
ref5 = ' assign1Ref t5.tmp > r5.out'
ref6 = ' assign1Ref t6.tmp > r6.out'
ref7 = ' assign1Ref t7.tmp > r7.out'


h1 = ' hw1 t1.tmp > h1.out'
h2 = ' hw1 t2.tmp > h2.out'
h3 = ' hw1 t3.tmp > h3.out'
h4 = ' hw1 t4.tmp > h4.out'
h5 = ' hw1 t5.tmp > h5.out'
h6 = ' hw1 t6.tmp > h6.out'
h7 = ' hw1 t7.tmp > h7.out'

d1 = 'diff r1.out h1.out'
d2 = 'diff r2.out h2.out'
d3 = 'diff r3.out h3.out'
d4 = 'diff r4.out h4.out'
d5 = 'diff r5.out h5.out'
d6 = 'diff r6.out h6.out'
d7 = 'diff r7.out h7.out'

ref8 = 'assign1Ref foo.tmp > r8.out'
h8 = ' hw1 foo.tmp > h8.out'
d7 = 'diff r8.out h8.out'


def runChecks():
	print 'running programs'
	command(makeClean)
	command(make)
	command(h1)
	command(h2)
	command(h3)
	command(h4)
	command(h5)
	command(h6)
	command(h7)
	command(ref1)
	command(ref2)
	command(ref3)
	command(ref4)
	command(ref5)
	command(ref6)
	command(ref7)
	#custom
	command(h8)
	command(ref8)

def runDiff():
	print 'diffing'
	output = command(d1)
	print output
	output = command(d2)
	print output
	output = command(d3)
	print output

	output = command(d4)
	print output

	output = command(d5)
	print output

	output = command(d6)
	print output

	output = command(d7)
	print output

	output = command(d8)
	print output

runChecks();
runDiff();