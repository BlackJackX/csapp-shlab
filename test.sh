#! /bin/bash

./sdriver.pl -t "trace$1.txt" -s ./tshref > tshref_$1
./sdriver.pl -t "trace$1.txt" -s ./tsh > tsh_$1

diff tsh_$1 tshref_$1 > diff_$1
diff tsh_$1 tshref_$1

