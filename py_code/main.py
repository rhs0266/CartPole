"""
Actor-Critic with continuous action using TD-error as the Advantage, Reinforcement Learning.

The Pendulum example (based on https://github.com/dennybritz/reinforcement-learning/blob/master/PolicyGradient/Continuous%20MountainCar%20Actor%20Critic%20Solution.ipynb)

Cannot converge!!! oscillate!!!

View more on my tutorial page: https://morvanzhou.github.io/tutorials/

Using:
tensorflow r1.3
gym 0.8.0
"""

import numpy as np
from ddpg import DDPG

OUT_DIR = 'cartpole-experiment' # default saving directory
# GAME = 'CartPole-v0'    # name of game

np.random.seed(2)

if __name__ == "__main__":
	ddpg = DDPG()
	#for tt in range(10):
	ddpg.training(20)
	#	ddpg.eval()
