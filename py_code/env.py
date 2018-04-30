import cp
from options import Options

class Environment():
	def __init__(self):
		self.env = cp.Simulation()
		self.env.init()
		self.options = Options().get_options()

	def init(self):
		self.env.init()

	def reset(self):
		self.env.reset()

	def step(self, flag):
		self.env.step(flag)

	def getStateNum(self):
		return self.env.getStateNum()

	def getActionNum(self):
		return self.env.getActionNum()

	def getState(self):
		return self.env.getState()

	def setAction(self, action):
		return self.env.setAction(action)

	def getReward(self):
		return self.env.getReward()

	def getDone(self):
		return self.env.getDone()
