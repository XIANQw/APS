# !/bin/zsh

target="../aps1"
toProg="../syntaxe/prologTermProg"

for i in $(ls $target/*.*)
do
    filename=$(basename $i)
    echo $filename;
    ./$toProg < $i;   
    echo "";
done