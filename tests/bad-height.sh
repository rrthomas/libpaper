printf "Wrong paper,10,foo,mm" > "./$sysconfdir/paperspecs"
expected_file=expected.txt
printf "paper: bad height in line 1 of $test_dir$sysconfdir/paperspecs\n" > "$expected_file"
expected_exit=1
paper_test
