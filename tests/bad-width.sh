printf "Wrong paper,foo,10,mm" > "./$sysconfdir/paperspecs"
expected_file=expected.txt
printf "paper: bad width in line 1 of $($PATH_CONVERT $test_dir$sysconfdir/paperspecs)\n" > "$expected_file"
expected_exit=1
paper_test
