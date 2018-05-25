#!/usr/bin/python

import os
reward_station_hit_multiplier = [0.0, 5.0, 10.0]
reward_no_event = [0.0, -1.0]
reward_ship_hit = [0.0, -100.0, -1000.0]
reward_game_over = [0.0, -500.0, -5000.0]
epsilon_function = [1,2]
alpha = [0.5, 0.9]
gamma = [0.5, 0.9]
#environment_number = [1, 2, 3, 4, 5]

training_runs = 100000

parameter_list = [reward_station_hit_multiplier, reward_no_event, reward_ship_hit, reward_game_over, epsilon_function, alpha, gamma]

def total_size():
    size = 1
    for l in parameter_list:
        size *= len(l)
    return size


for i in range(total_size()):
    temp_i = i
    parameters = []
    for k in range(len(parameter_list)):
        parameter = temp_i % len(parameter_list[k])
        parameters.append(parameter)
        temp_i /= len(parameter_list[k])

    print parameters

    os.system('./ReinforcementLearningTraining -r ' + str(training_runs) + ' -s ' + str(reward_station_hit_multiplier[parameters[0]]) + ' -n ' +str(reward_no_event[parameters[1]]) + ' -h ' +str(reward_ship_hit[parameters[2]])+' -o ' + str(reward_game_over[parameters[3]]) + ' -p ' + str(epsilon_function[parameters[4]]) + ' -a ' +str(alpha[parameters[5]]) + ' -g ' +str(gamma[parameters[6]]) + ' -e ' + "1")