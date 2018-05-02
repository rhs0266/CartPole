import tensorflow as tf
import numpy as np

class ActorNetwork:
	def __init__(self,sess,options,name):
		# session
		# state_dim
		# action_dim
		# learning_rate
		# name
		self.sess = sess
		self.state_dim = options.STATE_DIM
		self.action_dim = options.ACTION_DIM
		self.name = name
		self.options = options

		self.build_network()

		init_vars = tf.group(tf.global_variables_initializer(), tf.local_variables_initializer())
		sess.run(init_vars)

	# Add options to graph
	def build_network(self):
		self.state = tf.placeholder(tf.float32, [None, self.state_dim]) # Placeholder
		with tf.variable_scope(self.name):
			hidden_layer = tf.layers.dense(self.state, 64, activation = tf.nn.elu, kernel_initializer = tf.contrib.layers.xavier_initializer()) # use elu for hidden layer
			self.action_predict = tf.layers.dense(hidden_layer, self.action_dim, activation = tf.nn.tanh, kernel_initializer = tf.contrib.layers.xavier_initializer()) # use tanh to ensure output range becomes [-1,1]

		self.action_grad = tf.placeholder(tf.float32, [None, self.action_dim]) # Placeholder
		self.network_params = tf.trainable_variables(scope = self.name)

		# find d(xs) s.t. d(ys)=grad_ys
		# since grad_ys is given, result is gradient of each network params for satisfying given grad_ys
		# in other word, net params should be changed as net_params_grad to change ys as grad_ys
		self.network_params_grad = tf.gradients(ys=self.action_predict, xs=self.network_params, grad_ys=-self.action_grad)

		# train
		self.train = tf.train.AdamOptimizer(self.options.LR_A).apply_gradients(zip(self.network_params_grad, self.network_params))

	def predict(self, state):
		s = np.reshape(state, [-1, self.state_dim]) # ensure right shape
		return self.sess.run(self.action_predict,
							 feed_dict={
								 self.state : s
							 })


	def update(self, states, action_grads):
		self.sess.run(self.train,
					  feed_dict={
						  self.state: states,
						  self.action_grad: action_grads
					  })