echo "Running unit tests:"

for i in tests/*_tests
do 
    if test -f $i
    then
        if $VALGRIND ./$i build/lcthw/darray_algos.so print_a_message "foo" 2>> tests/test.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's tests/test.log"
            echo "-------"
            tail tests/test.log
            exit 1
        fi
    fi
done

echo ""
