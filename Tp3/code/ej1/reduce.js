function (key, values) {
	var res = {};
	res.score = 0;
	res.count = 0;
	for (var i = values.length - 1; i >= 0; i--) {
		var data = values[i];
		res.score += data.score;
		res.count += data.count;
	};
	return res;
}