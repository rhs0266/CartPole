from argparse import ArgumentParser

class Options():
	def __init__(self):
		parser = ArgumentParser()
		parser.add_argument('--MAX_EPISODE', type=int, default=10000,
							help='max number of episodes iteration')
		parser.add_argument('--GOAL_REWARD', type=int, default=700,
							help='goal reward of each episode')
		parser.add_argument('--ACTION_DIM', type=int, default=1,
							help='number of actions one can take')
		parser.add_argument('--STATE_DIM', type=int, default=4,
							help='number of states one can see')
		parser.add_argument('--GAMMA', type=float, default=0.99,
							help='discount factor')
		parser.add_argument('--TAU', type=float, default=0.9,
							help='blending factor for soft copy')
		parser.add_argument('--INIT_EPS', type=float, default=1.0,
							help='initial probability for randomly sampling action')
		parser.add_argument('--FINAL_EPS', type=float, default=1e-2,
							help='finial probability for randomly sampling action')
		parser.add_argument('--EPS_DECAY', type=float, default=0.95,
							help='epsilon decay rate')
		parser.add_argument('--EPS_ANNEAL_STEPS', type=int, default=100,
							help='steps interval to decay epsilon')
		parser.add_argument('--LR_A', type=float, default=1e-3,
							help='learning rate of actor network')
		parser.add_argument('--LR_C', type=float, default=1e-3,
							help='learning rate of critic network')
		parser.add_argument('--BUFFER_SIZE', type=int, default=10000,
							help='size of experience replay buffer')
		parser.add_argument('--BATCH_SIZE', type=int, default=100,
							help='mini batch size')
		# parser.add_argument('--H1_SIZE', type=int, default=128,
		# 					help='size of hidden layer 1')
		# parser.add_argument('--H2_SIZE', type=int, default=128,
		# 					help='size of hidden layer 2')
		# parser.add_argument('--H3_SIZE', type=int, default=128,
		# 					help='size of hidden layer 3')
		self.options = parser.parse_args()
	def get_options(self):
		return self.options