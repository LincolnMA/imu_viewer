import tkinter as tk
import serial.tools.list_ports
import serial as ser
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import numpy as np

root = tk.Tk()

list_ports = ["COM2",""]
list_baudrates = ["9600",""]
sel1 = tk.StringVar()
sel1.set(list_ports[0])
sel2 = tk.StringVar()
sel2.set(list_baudrates[0])

option_bar = tk.Frame(root)
op1 = tk.OptionMenu(option_bar,sel1,*list_ports)
op2 = tk.OptionMenu(option_bar,sel2,*list_baudrates)
bt1 = tk.Button(option_bar,text = "Connect")
bt2 = tk.Button(option_bar,text = "Desconnect")
ckbt1 = tk.Checkbutton(option_bar,text = "Orientation")
ckbt2 = tk.Checkbutton(option_bar,text = "Position")
op1.grid(row = 0,column = 0)
op2.grid(row = 1,column = 0)
bt1.grid(row = 2,column = 0)
bt2.grid(row = 3,column = 0)
ckbt1.grid(row = 4,column = 0)
ckbt2.grid(row = 5,column = 0)
option_bar.grid(row = 0,column = 0)
#gráficos

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
ax.plot([0,1], [0,0], [0,0],color = 'r')
ax.plot([0,0], [0,1], [0,0],color = 'b')
ax.plot([0,0], [0,0], [0,1],color = 'g')
ax.set(xticklabels=[],
       yticklabels=[],
       zticklabels=[])
canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.draw()
canvas.get_tk_widget().grid(row = 0,column = 1)

root.mainloop()