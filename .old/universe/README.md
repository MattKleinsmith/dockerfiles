The container was able to connect to a Universe environment running on a machine on the local network. This is the code it used:
```python
import gym
import universe  # register the universe environments

env = gym.make('flashgames.DuskDrive-v0')
env.configure(remotes="vnc://<remote machine's IP>:5900+15900")
observation_n = env.reset()

while True:
  action_n = [[('KeyEvent', 'ArrowUp', True)] for ob in observation_n]  # your agent here
  observation_n, reward_n, done_n, info = env.step(action_n)
  env.render()
```
I was able to view the agent in the environment by using a VNC client.
