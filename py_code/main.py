import cp

ms = cp.Simulation()
ms.Init()
for i in range(1000):
    ms.Step()
    if i%100==0:
        print(i)
