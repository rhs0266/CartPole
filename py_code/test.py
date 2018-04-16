import cp
env=cp.Simulation()
env.Init()
for i in range(10):
	env.SetAction([1,0])
	env.Step()
for i in range(1000):
	env.Step()


