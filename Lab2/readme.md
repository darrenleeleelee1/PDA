# Lab2

exe:
./Lab2 1 smallcase/small.block ./smallcase/small.nets ./out/smallcase.draw
./Lab2 1 ami33/ami33.block ./ami33/ami33.nets ./out/ami33.draw
./Lab2 1 ami49/ami49.block ./ami49/ami49.nets ./out/ami49.draw
./Lab2 1 vda317b/vda317b.block ./vda317b/vda317b.nets ./out/vda317b.draw

draw:
python3 draw_block_layout.py ./drawing/out/smallcase.draw ./drawing/out/smallcase.png 
python3 draw_block_layout.py ./drawing/out/ami33.draw ./drawing/out/ami33.png 
python3 draw_block_layout.py ./drawing/out/ami49.draw ./drawing/out/ami49.png 
python3 draw_block_layout.py ./drawing/out/vda317b.draw ./drawing/out/vda317b.png 
