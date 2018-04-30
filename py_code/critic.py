import tensorflow as tf
import numpy as np

class CriticNetwork:
    def __init__(self,sess,options,name):
        # session
        # state_dim
        # action_dim
        # learning_rate
        # name
        self.sess = sess
        self.name = name
        self.options = options

        self.build_network()

        init_vars = tf.group(tf.global_variables_initializer(), tf.local_variables_initializer())
        sess.run(init_vars)

    # Add options to graph
    def build_network(self):
        self.state = tf.placeholder(tf.float32, [None, self.options.STATE_DIM]) # Placeholder
        self.action = tf.placeholder(tf.float32, [None, self.options.ACTION_DIM]) # Placeholder

        with tf.variable_scope(self.name):
            hidden_layer_state = tf.layers.dense(self.state, 16, activation = tf.nn.elu, kernel_initializer = tf.contrib.layers.xavier_initializer()) # use elu for hidden layers
            hidden_layer_action = tf.layers.dense(self.action, 16, activation = tf.nn.elu, kernel_initializer = tf.contrib.layers.xavier_initializer()) # use elu for hidden layers
            hidden_layer_total = tf.concat([hidden_layer_state, hidden_layer_action], axis=1)
            hidden_layer = tf.layers.dense(hidden_layer_total, 16, activation = tf.nn.elu, kernel_initializer=tf.contrib.layers.xavier_initializer())
            self.q_predict = tf.layers.dense(hidden_layer, 1, kernel_initializer = tf.contrib.layers.xavier_initializer())

        self.network_params = tf.trainable_variables(scope = self.name)

        self.q_target = tf.placeholder(tf.float32, [None, 1]) # Placeholder
        self.loss = tf.losses.mean_squared_error(self.q_predict, self.q_target)
        self.train = tf.train.AdamOptimizer(self.options.LR_C).minimize(self.loss)

        self.action_grad = tf.gradients(self.q_predict, self.action)[0]

    def predict(self, state, action):
        s = np.reshape(state, [-1, self.options.STATE_DIM]) # ensure right shape
        a = np.reshape(action, [-1, self.options.ACTION_DIM]) # ensure right shape
        return self.sess.run(self.q_predict,
                             feed_dict={
                                 self.state : s,
                                 self.action: a
                             })

    def update(self, states, actions, q_targets):
        self.sess.run(self.train,
                      feed_dict={
                          self.state: states,
                          self.action: actions,
                          self.q_target: q_targets
                      })

    def compute_grad(self, states, actions):
        return self.sess.run(self.action_grad,
                             feed_dict={
                                 self.state: states,
                                 self.action: actions
                             })