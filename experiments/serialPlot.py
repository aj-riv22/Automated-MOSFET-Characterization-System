import numpy as np
import matplotlib.pyplot as plt
import serial

ser = serial.Serial('COM3', 9600)

x = [] 
y = []
datapoints = 201  # number of data points to read from the serial port

for i in range(datapoints):
    line = ser.readline().decode('utf-8').strip() # read a line from the serial port
    voltage, current = map(float, line.split(',')) # decode it, and strip whitespace
    x.append(voltage)  
    y.append(current)  
ser.close()

# currLimit = 0.06  # set the current limit
# for i in rangelen(x), 0, -1):
#     if y[i-1] > currLimit:  # remove data points where current exceeds the limit
#         del x[i-1(]
#         del y[i-1]

# plot 1: Vgs vs Id 
plt.figure() 
plt.plot(x, y, label="Drain Current vs. Voltage Gate-Source", color="purple")
plt.title("NumPy + Matplotlib in Action")
plt.xlabel("Voltage (V)")
plt.ylabel("Current (A)")
plt.legend()

# extract kn, vth from the data using numpy
sqrt_y = np.sqrt(y)  
coefficients = np.polyfit(x, sqrt_y, 1)  # linear regression to find the slope and y-intercept of the fitted line
vth = -coefficients[1] / coefficients[0] # x_intercept = -y_intercept / slope = vth 
kn = 2 * coefficients[0] ** 2  # kn = slope^2
print("Threshold Voltage (Vth): %.3f V" % vth)
print("Transconductance Parameter (kn): %.3f A/V^2" % kn)
plt.show()


# code to remove data points from x and y list where value at index in y  > 5
# x = [1, 2, 3, 4, 5]
# y = [2, 4, 6, 8, 10]



# print (x, y)

# for i in range(len(x), 0, -1):
#     if y[i-1] > 5:
#         del x[i-1]
#         del y[i-1]
    

# print (x, y)
