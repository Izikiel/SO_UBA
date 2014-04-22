from numpy import std

def process_file(filename):
	with open(filename, "r") as f:
		data = []
		#CPU 1 0 0  tipo tick nro_core nro_tarea
		#LOAD 0 tarea
		#Exit tick core tarea -> solo necesitamos exit y tick
		exit_token = "EXIT"
		for line in f.readlines():
			if exit_token in line:
				data.append(line)

		return map(int, [ e.split(" ")[1] for e in data])

def waiting_time_metric(cpu_time, exit_time):
	return std([ e - cpu_time for e in exit_time])

if __name__ == '__main__':
	cpu_time = 20
	filename = "ej7.txt"
	print waiting_time_metric(cpu_time, process_file(filename))


