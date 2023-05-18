import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

fast = pd.read_csv("fast.csv")["time"]
slow = pd.read_csv("slow.csv")["time"]
print(type(fast))
plt.plot(fast, label="QNEH")
plt.plot(slow, label="NEH")
plt.yscale("log")
plt.xlabel("Przykłady")
plt.ylabel("Czas obliczeń [s]")
plt.title("Porównanie NEH oraz QNEH")
plt.legend()
plt.grid(True)
plt.show()

# boost = slow/fast
# plt.plot(boost)
# plt.xlabel("Przykłady")
# plt.ylabel("Przyspieszenie (wielokrotkość)")
# plt.title("Czas wykonywania NEH / czas wykonywania QNEH")
# plt.grid(True)
# plt.show()
