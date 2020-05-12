# !/bin/zsh

target="../aps0"
toProg="../syntaxe/prologTermProg"

for i in $(ls $target/*.*)
do
    filename=$(basename $i)
    echo $filename;
    ./$toProg < $i;   
    echo "";
done