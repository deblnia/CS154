#!/bin/bash

year=2020
quarter=spring
testname=multipleCommand

BATCHPATH=/stage/classes/archive/${year}/${quarter}/15400-1/proj/p4/batch-files
OUTPUTPATH=/stage/classes/archive/${year}/${quarter}/15400-1/proj/p4/expected-output
./clean.sh
#mkdir out
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr

echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff



testname=gooduser_basic
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=gooduser_args 
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=gooduser_redirection
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=gooduser_multipleCommand
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=buildin_wrongFormat
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=cd_toFolderNotExist
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=badCommand
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=complexCommand
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=multipleCommand 
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=emptyInput
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

testname=1KCommand
./myshell  $BATCHPATH/$testname 1> out/$testname.stdout 2> out/$testname.stderr
echo "Testing $testname stdout"
diff out/$testname.stdout $OUTPUTPATH/$testname.stdout > $testname.stdout-diff
echo "Testing $testname stderr"
diff out/$testname.stderr $OUTPUTPATH/$testname.stderr > $testname.stderr-diff

for filename in *_rd_*;do
        if [[ $filename == "*_rd_*" ]]
        then
                exit
        fi
        echo "Testing $filename redirection"
        diff $OUTPUTPATH/$filename $filename
done

exit

# You can put any old command down here,
# or any garbage, it won't be called since we
# already called "exit" above

bla bla (will not be called)
