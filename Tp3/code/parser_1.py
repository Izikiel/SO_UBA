def parse_data_string(s):
	s = s.split("} {")
	s[0] = s[0].replace("{", "")
	s[-1] = s[-1].replace("}", "")
	s = map(lambda l: l.replace("'",""), s)
	id_s = "u_id: u"
	val_s = " uvalue: "
	parsed_data = [map(lambda s: s.replace(id_s, "") if id_s in s else s.replace(val_s, ""), q.split(",")) for q in s]
	return map(lambda x: [x[0], float(x[1])], filter(lambda q: q[1] != "-1.0", parsed_data))


if __name__ == '__main__':
	with open("pep.txt","r") as f:
		l = f.readline()
		data = parse_data_string(l)
		res = sorted(data, key= lambda x: x[1], reverse=True)[:12]
		print res

	# splitted_voted_total = [(tuple(y[0].split("/")), y[1]) for y in parsed_data]
	# totals = {}
	# for voted_over_total, length in splitted_voted_total:
	# 	v = voted_over_total[0]
	# 	t = voted_over_total[1]
	# 	totals.setdefault(t, {})[v] = length
	# return totals