import tensorflow as tf
import numpy as np
from env import Environment
from options import Options
from actor import ActorNetwork as Actor
from critic import CriticNetwork as Critic
from replaybuffer import ReplayBuffer
from noise import GaussianNoise

# ==========================
#   Training Parameters
# ==========================
# Noise parameters - Ornstein Uhlenbeck
DELTA = 0.5 # The rate of change (time)
SIGMA = 0.5 # Volatility of the stochastic processes
OU_A = 3. # The rate of mean reversion
OU_MU = 0. # The long run average interest rate

class DDPG(object):
	def __init__(self):
		self.env = Environment()
		self.options = Options().get_options()
		self.sess = tf.Session()

		# actor
		self.actor = Actor(self.sess, self.options, name="actor")
		self.actor_target = Actor(self.sess, self.options, name="actor_target")

		# critic
		self.critic = Critic(self.sess, self.options, name="critic")
		self.critic_target = Critic(self.sess, self.options, name="critic_target")

		# soft copy
		self.soft_copy_actor = \
			[self.actor_target.network_params[i].assign(
				tf.multiply(self.actor.network_params[i], self.options.TAU) + \
				tf.multiply(self.actor_target.network_params[i], 1. - self.options.TAU)) \
				for i in range(len(self.actor_target.network_params)
			)]

		self.soft_copy_critic = \
			[self.critic_target.network_params[i].assign(
				tf.multiply(self.critic.network_params[i], self.options.TAU) + \
				tf.multiply(self.critic_target.network_params[i], 1. - self.options.TAU)) \
				for i in range(len(self.critic_target.network_params)
			)]

		# replay buffer
		self.replaybuffer = ReplayBuffer(self.options.BUFFER_SIZE)

		# saving and loading networks
		self.saver = tf.train.Saver()
		self.checkpoint = tf.train.get_checkpoint_state("checkpoints-cartpole")
		if self.checkpoint and self.checkpoint.model_checkpoint_path:
			self.saver.restore(self.sess, self.checkpoint.model_checkpoint_path)
			print("Successfully loaded:", self.checkpoint.model_checkpoint_path)
		else:
			print("Could not find old network weights")

		# gaussian noise
		self.noise = GaussianNoise()

		self.flag = None

	def optimize_network(self):
		if self.replaybuffer.size() < self.options.BUFFER_SIZE:
			return
		if self.flag == None:
			self.flag = True
			print 'OPT'
		s_batch, a_batch, r_batch, s2_batch = self.replaybuffer.sample_batch(self.options.BATCH_SIZE)

		action_target_batch = self.actor_target.predict(s2_batch)
		q_target_batch = r_batch + self.options.GAMMA * self.critic_target.predict(s2_batch, action_target_batch)
		self.critic.update(s_batch, a_batch, q_target_batch)

		action_train_batch = self.actor.predict(s_batch)
		q_grad_batch = self.critic.compute_grad(s_batch, action_train_batch)
		# idxs=[]
		# for i in range(len(q_grad_batch)):
		# 	if (q_grad_batch[i] < 0):
		# 		idxs.append(i)
		# s_batch = s_batch[idxs]
		# q_grad_batch = q_grad_batch[idxs]

		self.actor.update(s_batch, q_grad_batch)		

		self.sess.run(self.soft_copy_actor)	
		self.sess.run(self.soft_copy_critic)	

	def training(self, goal_step):
		print("Start training")
		global_step = 0
		eps = self.options.INIT_EPS
		mem = False
		for t in range(self.options.MAX_EPISODE):
			local_step = 0
			done = False
			self.env.reset()
			state = self.env.getState()
			total_reward = 0

			while local_step < goal_step and not done:
				# epsilon decaying
				global_step += 1
				local_step += 1
				if global_step % self.options.EPS_ANNEAL_STEPS == 0 and eps > self.options.FINAL_EPS:
					eps = eps * self.options.EPS_DECAY

				# # epsilon exploration
				# if np.random.uniform(0,1) < eps:
				# 	action = np.random.normal(float(self.actor.predict(state)[0][0]), 0.3) # sigma = 0.3
				# else:
				# 	action = float(self.actor.predict(state)[0][0])
				
				# print(self.actor.predict(state)[0])
				# action = np.random.normal(float(self.actor.predict(state)[0][0]), eps) # sigma = 0.3
				# action = normalNoise(action, eps)
				# print("observed state   : ", state)
				action = self.actor.predict(state)[0]
				# print("predicted action : ",action)
				for i in range(len(action)):
					action[i] = np.random.normal(float(action[i]), eps)
					if action[i] > 1.5:
						action[i] = 1.5
					if action[i] < -1.5:
						action[i] = -1.5
				# print("noise addition   : ",action)

				# stepping
				self.env.setAction(action.tolist())
				self.env.step(t%50==0)
				reward = self.env.getReward()
				next_state = self.env.getState()
				done = self.env.getDone()
				# if done and total_reward < self.options.GOAL_REWARD:
				# 	reward = -1000

				# training
				self.replaybuffer.add(state, action, [reward], next_state)
				self.optimize_network()

				state = next_state
				total_reward += reward

			if total_reward == self.options.GOAL_REWARD:
				mem = True
			else:
				mem = False
			# if (t+1)%500==0:
				# self.saver.save(self.sess, 'checkpoints-simbicon/simbicon_ddpg', global_step = global_step)
			print(t, total_reward)

	def eval(self):
		print("Start evaluation")
		self.env.reset()
		state = self.env.getState()
		done = False
		total_reward = 0
		while not done and total_reward < self.options.GOAL_REWARD:
			action = float(self.actor.predict(state)[0][0])

			# stepping
			self.env.setAction(action)
			self.env.step(True)
			reward = self.env.getReward()
			next_state = self.env.getState()
			done = self.env.getDone()
			state = next_state
			total_reward += reward

		print 'eval: ', total_reward