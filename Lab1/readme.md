# Lab1 Corner Stitching

## makefile command
```
cd src
make
```

## execute command
```
./lab1_prog <testcase> <output> 
```
## advanced command
* create drawing files and result also
```
./lab1_porg -input <testcase> -output <output> -draw <layout>
eg: ./lab1_prog -input ../testcase/case0.txt -output ../output/output0.txt -draw ../layout/layout0.txt
```

## draw
```
python3 draw_block_layout.py <layout.txt> <layout.png>
eg: python3 draw_block_layout.py layout0.txt layout0.png
```