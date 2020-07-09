#!/bin/bash

copyrightfile=/home/aksenxu/Novatek/NT96685/D700/na51000_linux_sdk/na51000_linux_sdk/document/copyright.txt
addHint='Y'

tempfile=~/copyright12345678

addcopyright()
{
    index=0
    filename=$1
    suffix=${filename#*.}
    #echo suffix=$suffix
   
    if [[ $suffix != "cpp" && $suffix != "h" && $suffix != "c" ]]; then
        echo "not source file, ignore:" $1
        return
    fi
    
    if [ $addHint == 'Y' ]
    then
        echo "You will add copyright to " $1
        read -r -p "Input Y for Yes, N for No, A for All ...?[Y/N/A]" input
        
        if [[ "$input" != 'Y' && "$input" != 'N' && "$input" != 'A' ]] ; then
            echo "error input:" $input
            return
        fi
        
        if [ "$input" == 'N' ] ; then
            return
        fi

        if [ "$input" == 'A' ] ; then
            addHint='N'
        fi
    fi
    echo "add copyright to" $1
    cat $copyrightfile $1  > $tempfile
    cp $tempfile $1
}

travelpath()
{
    echo "travelpath:" $1
    for file in `ls $1`
    do
        path=$1/$file
        if test -f $path
        then
            addcopyright $1/$file
        elif test -d $path
        then
            cd $path
            travelpath `pwd`
            cd ..
        fi
    done
}

travelpath $1
rm $tempfile
