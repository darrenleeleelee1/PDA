import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import math
import time

def draw_block(ax,x,y,width,height,id):
    color = "#FCC"
    

    ax.add_patch(
        patches.Rectangle( 
            (x,y ),
            width,
            height,
            fill = True,
            edgecolor = "#000",
            facecolor = color,
            alpha = 1.0 #0.3 original
        )
    )
    ax.annotate(id, (x + 0.5 * width, y + 0.5 * height), color='black', weight='bold', fontsize=10, ha='center', va='center')


png_size = (16,12)
txt_name = sys.argv[1] 
png_name = sys.argv[2] 
fread = open(txt_name,'r')
f = fread.read().split("\n")


total_block_number = int(f[0])
window_width = int( f[1].split(" ")[0] )
window_height = int( f[1].split(" ")[1] )

fig = plt.figure(figsize=png_size)

ax = fig.add_subplot(111)
ax.set_xbound(0,window_width)
ax.set_ybound(0,window_height)


i = 2

for block in range(total_block_number ):
    ss =  f[i].split(" ")
    draw_block(ax,int(ss[1]),int(ss[2]),int(ss[3]),int(ss[4]),ss[0])
    i+=1

#plt.savefig(str(sys.argv[1])[:-4]+".png")

plt.savefig(png_name)
