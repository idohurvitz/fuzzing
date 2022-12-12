#!/usr/bin/python2
import subprocess32
import os


def handle_crash(payload):
	global global_commands
	print 'CRASHED!!!'
	print len(payload)
	print payload
	with open('all_commands.txt', 'w') as fd:
		fd.write(global_commands)



def input_generator(mode):
	if mode == 'radamsa':
		return os.popen('cat template | radamsa').read() + '\n'
	elif 'blab':
		return os.popen('blab server.blab').read()# + '\n'
	else:
		return raw_input('>> ') + '\n'


def is_target_process_crashed():
	try:
		pid = subprocess32.check_output(['pgrep', 'server'])
		return False
	except:
		return True

global_commands = ''

def main():
	global global_commands
	# start process
	env = {}
	env['LD_PRELOAD']  = '/home/user/Desktop/simple_fuzzer/desock.so'
	args = ['/home/user/Desktop/simple_fuzzer/myserver']
	pipe = subprocess32.Popen(args, stdin=subprocess32.PIPE, stdout=subprocess32.PIPE, stderr=subprocess32.DEVNULL, env=env, shell=True)

	counter = 0

	while True:
		print counter
		input_buffer = input_generator('blab')
		global_commands += input_buffer

		# send input_buffer to target process
		pipe.stdin.write(input_buffer)
		result = '' #pipe.stdout.readline()
		print result

		# check if the process crashed
		if is_target_process_crashed():
			handle_crash(input_buffer)
			break
		counter += 1


	# close target process
	pipe.terminate()


main()











