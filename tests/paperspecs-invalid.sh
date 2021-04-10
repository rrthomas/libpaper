printf "wibble" > "./$sysconfdir/paperspecs"
expected_file=expected.txt
printf "paper: missing field in line 1 of $test_dir$sysconfdir/paperspecs\n" > "$expected_file"
expected_exit=1
paper_test
