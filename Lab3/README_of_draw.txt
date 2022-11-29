python3 draw.py [INPUT] [OUTPUT] [PICTURE_NAME] (-i) (-p)
#-i would also draw initial graph
#-p would also draw partition graph (the cell distribution on 2 chips before doing legalization ) 



ex:
python3 draw.py testcase/case3.txt out/case3.txt drawing/case3/layout3
python3 draw.py testcase/case0.txt out/case0.txt drawing/case0/layout0 -i
python3 draw.py testcase/case3.txt out/case3.txt drawing/case3/layout3 -i -p
python3 draw.py testcase/case0.txt out/case0.txt drawing/case0/layout0 -p