#!/bin/bash
SAVEIFS=$IFS
IFS=$(echo -en "\n\b")
pwd=`pwd`

outpath=$2
export LANG=en_US.UTF8

if [ ! -x "$outpath" ]
then
touch "$outpath"
fi
cat /dev/null > $outpath

directory_count=0
file_count=0

temp=$(readlink -e $1)

a=($1)
abs=($temp)
if [ ${a:0:1} != "/" ]
then
a=($(basename "$temp"))
else
a=($temp)
fi

size=1
position=0

function read_dir(){
	printf "[%s]\n" $1 >> $outpath
        for var in  `ls $2`
            do
		if [ ${var:0:1} != "/"  ]
                then
			temp_abs=$2"/$var"
			echo $temp_abs >> $outpath
			if [ -f "$temp_abs" ]
				then
				file_count=$((${file_count}+1))
			else
				a=("${a[@]}" $(basename "$temp_abs"))
				abs=("${abs[@]}" $temp_abs)
				size=$((${size}+1))
				directory_count=$((${directory_count}+1))
			fi
		else
			printf "%s\n" "$var" >> $outpath
			if [ -f "$var" ]
				then
				file_count=$((${file_count}+1))
			else
				a=("${a[@]}" $var)
				abs=("${abs[@]}" $var)
				size=$((${size}+1))
				directory_count=$((${directory_count}+1))
			fi
		fi
            done
	echo >> $outpath
}


while [ $size -gt 0 ]
do
top=${a[position]}
topabs=${abs[position]}
position=$((${position}+1))
read_dir $top $topabs
size=$((${size}-1))
done

echo "[Directories Count]:"${directory_count} >> $outpath
echo "[Files Count]:"${file_count} >> $outpath


IFS=$SAVEIFS