#!/bin/bash

# * Part 1 *
if [ $# -ne 2 ] 
then
    echo "Number of arguments error:"
    echo "The script expects exactly two filenames as input"
    echo "But you gave: $# inputs"
    exit 22 # Exit code for arguments error
fi

# * Part 2 *
if [ ! -f $1 ]
then
    echo "Input file error:"
    echo "The file $1 does not exist!"
    echo "Please enter a file that does exist."
    exit 2 # Exit code for file does not exist
fi

# Finding indices of the headers in the given .CSV and storing them in `ind` in case they are not fixed, h_i store index in the headers
headers=("Name" "PredominantDegree" "HighestDegree" "Region" "Geography" "AdmissionRate" "AverageCost" "Expenditure" "MedianEarnings")
heading=( $(head -n 1 $1 | tr "," " ") )            # Reading the first row and splitting it with "," as the delimiter
declare -a ind
for i in "${!headers[@]}";
do 
    for j in "${!heading[@]}";
    do 
        if [[ "${headers[$i]}" = "${heading[$j]}" ]]; then
            ind[$i+1]=$(($j+1))         # Match found, appending result
        fi
    done
done

# * Part 3 *
awk -F, -v h_1="${ind[1]}" -v h_2="${ind[2]}" -v h_3="${ind[3]}" -v h_4="${ind[4]}" \
        -v h_5="${ind[5]}" -v h_6="${ind[6]}" -v h_7="${ind[7]}" -v h_8="${ind[8]}" \
        '{ print $h_1, $h_2, $h_3, $h_4, $h_5, $h_6, $h_7, $h_8; }' <$1 >$2     # * Over-writing using ">"
# awk -F, '{ print $1, $2, $3, $5, $6, $7, $10, $11; }' <$1 >$2         # For when the columns' order is fixed

# * Part 4 *
awk -F, -v h_1="${ind[3]}" -v h_2="${ind[1]}" '{ if($h_1=="Bachelor'\''s") print $h_2; }' <$1 >>$2
# awk -F, '$3 ~ /Bachelor'\''s/ { print $1; }' <$1 >>$2                 # For when the columns' order is fixed

# * Part 5 *
echo "Geography: Average Admission Rate" >>$2
awk -F, -v h_1="${ind[5]}" -v h_2="${ind[6]}" '{ avg[$h_1]+=$h_2; count[$h_1]++; } END { for (place in avg) {
    if (place == "Geography") continue;
    else print place ":", avg[place]/count[place];
} }' <$1 >>$2

# * Part 6 *
sort -nrk${ind[9]} -t, $1 | head -n 5 | awk -F, -v h="${ind[1]}" '{ print $h; }' >>$2           # For when the columns' order is not fixed
# sort -nrk16 -t, $1 | head -n 5 | awk -F, '{ print $1; }' >>$2
# sort -nrk16 -t, $1 | awk -F, '{ print $1; } NR==5 {exit}' >>$2        # Without using `head`