# !/bin/zsh

target="../aps1"
toProg="../syntaxe/prologTermProg"

for i in $(ls $target/*.*)
do
    filename=$(basename $i)
    echo $filename;
    ./$toProg < $i;
    ./$toProg < $i | swipl -s typage.pl -g main_stdin    
    echo "";
done