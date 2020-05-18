# !/bin/zsh

target="../aps1"

for i in $(ls $target/*.*)
do
    filename=$(basename $i)
    echo $filename;
    ./evalProg < $i;
    echo "";
done