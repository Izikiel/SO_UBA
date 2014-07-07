function (key, values) {

	var res = new Object();

	for (var i = values.length - 1; i >= 0; i--) {
		var data = JSON.parse(values[i]);
		for (var key in data){
			if (data.hasOwnProperty(key)){
				if( key in res) {
					res[key] += 1;
				}
				else {
					res[key] = 1;
				}
			}
		}
	};

	return tojson(res);
}