# complex-fourier-series-visualizer
 One day made visualization of complex fourier series on svg files using SFML.

![](res/demo.gif)

## Controls
- Up : add one epicycloid
- Down : remove one epicycloid
- S : let you type in the console the number of epicycloids used
- L : let you type in the console the name of the save you want to load
- Left click : drag the view
- Scroll wheel : zoom
- Space : pause
- E : Slow down
- X : disable/enable circles

## How to use the save system
The saves are stored in the res/saves file. To create a new save, you need to create a new txt file and to put inside your svg path. To get the 'svg path', you need to fing a svg file, copy the code in the path section (This should start with an m and ends ith a z), paste it in the python programm 'svg_converter.py', copy the result of the programm and finally paste it in the third line of the file. Now come back to main.exe, press L, type the name of your file and press enter.