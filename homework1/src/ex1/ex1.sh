cd $1
cnt=0
start=`date +%s`
file_list=$(find -name "*.c" -o -name "*.h" | xargs grep -l "hello")
for file in $file_list ; do
    printf "%s\n" $file
    cnt=$(($cnt+ 1))
done
printf "total found: %d\n" $cnt
end=`date +%s`
printf "total time=%f seconds\n" $(($end-$start))

