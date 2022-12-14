import configparser
import os
 
class Config:
    #Read config file
    def __init__(self):
        config = configparser.ConfigParser()
        config.read(os.path.dirname(__file__) + '/config.cfg')

        self.size_x = int(config.get('Board', 'size_x'))
        self.size_y = int(config.get('Board', 'size_y'))
        self.grid_size = int(config.get('Board', 'grid_size'))
        self.startpos = config.get('Board', 'startpos')
        self.rounds = int(config.get('Board', 'rounds'))

        self.engine_time = int(config.get('AISettings', 'max_time'))

        self.player_x = config.get('Players', 'player_x')
        self.player_0 = config.get('Players', 'player_0')
